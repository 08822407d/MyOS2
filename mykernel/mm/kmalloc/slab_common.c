// SPDX-License-Identifier: GPL-2.0
/*
 * Slab allocator functions that are independent of the allocator strategy
 *
 * (C) 2012 Christoph Lameter <cl@linux.com>
 */
#include "slub.h"


enum slab_state slab_state;
LIST_HDR_S(slab_caches);
// DEFINE_MUTEX(slab_mutex);
kmem_cache_s *kmem_cache;


/*
 * Figure out what the alignment of the objects will be given a set of
 * flags, a user specified alignment and the size of the objects.
 */
static uint
calculate_alignment(slab_flags_t flags, uint align, uint size) {
	/*
	 * If the user wants hardware cache aligned objects then follow that
	 * suggestion if the object is sufficiently large.
	 *
	 * The hardware cache alignment cannot override the specified
	 * alignment though. If that is greater then use it.
	 */
	if (flags & SLAB_HWCACHE_ALIGN) {
		uint ralign = cache_line_size();
		while (size <= ralign / 2)
			ralign /= 2;
		align = max(align, ralign);
	}
	return ALIGN(max(align, ARCH_SLAB_MINALIGN), sizeof(void *));
}

static kmem_cache_s
*create_cache(const char *name, uint object_size,
		uint align, slab_flags_t flags) {

	kmem_cache_s *s = kmem_cache_zalloc(kmem_cache, GFP_KERNEL);
	if (s == NULL)
		goto out;

	s->name = name;
	s->size = s->object_size = object_size;
	s->align = align;

	int err = __kmem_cache_create(s, flags);
	if (err) {
		kmem_cache_free(kmem_cache, s);
		s = ERR_PTR(err);
		goto out;
	}

	s->refcount = 1;
	list_header_add_to_head(&slab_caches, &s->list);

out:
	return s;
}

/**
 * kmem_cache_create - Create a cache.
 * @name: A string which is used in /proc/slabinfo to identify this cache.
 * @size: The size of objects to be created in this cache.
 * @align: The required alignment for the objects.
 * @flags: SLAB flags
 * @ctor: A constructor for the objects.
 *
 * Cannot be called within a interrupt, but can be interrupted.
 * The @ctor is run when new pages are allocated by the cache.
 *
 * The flags are
 *
 * %SLAB_POISON - Poison the slab with a known test pattern (a5a5a5a5)
 * to catch references to uninitialised memory.
 *
 * %SLAB_RED_ZONE - Insert `Red` zones around the allocated memory to check
 * for buffer overruns.
 *
 * %SLAB_HWCACHE_ALIGN - Align the objects in this cache to a hardware
 * cacheline.  This can be beneficial if you're counting cycles as closely
 * as davem.
 *
 * Return: a pointer to the cache on success, NULL on failure.
 */
kmem_cache_s *
kmem_cache_create(const char *name, uint size,
		uint align, slab_flags_t flags) {

	// mutex_lock(&slab_mutex);
	/*
	 * Some allocators will constraint the set of valid flags to a subset
	 * of all flags. We expect them to define CACHE_CREATE_MASK in this
	 * case, and we'll just provide them with a sanitized version of the
	 * passed flags.
	 */
	flags &= CACHE_CREATE_MASK;
	kmem_cache_s *s = create_cache(name, size,
			calculate_alignment(flags, align, size), flags);
	// mutex_unlock(&slab_mutex);

	if (IS_ERR(s)) {
		if (flags & SLAB_PANIC)
			panic("%s: Failed to create slab '%s'. Error %d\n",
					__func__, name, (int)PTR_ERR(s));
		else
			pr_warn("%s(%s) failed with error %d\n",
					__func__, name, (int)PTR_ERR(s));

		s = NULL;
	}
	return s;
}
EXPORT_SYMBOL(kmem_cache_create);


/* Create a cache during boot when no slab services are available yet */
void __init create_boot_cache(kmem_cache_s *s,
		const char *name, uint size, slab_flags_t flags)
{
	s->name = name;
	s->size = s->object_size = size;
	s->align = calculate_alignment(flags, ARCH_KMALLOC_MINALIGN, size);

	int err = __kmem_cache_create(s, flags);
	if (err)
		panic("Creation of kmalloc slab %s size=%u failed. Reason %d\n",
				name, size, err);

	s->refcount = -1;	/* Exempt from merging for now */
}

kmem_cache_s __init
*create_kmalloc_cache(const char *name, uint size, slab_flags_t flags)
{
	kmem_cache_s *s = kmem_cache_zalloc(kmem_cache, GFP_NOWAIT);

	if (!s)
		panic("Out of memory when creating slab %s\n", name);

	create_boot_cache(s, name, size, flags | SLAB_KMALLOC);
	list_header_add_to_head(&slab_caches, &s->list);
	s->refcount = 1;
	return s;
}


kmem_cache_s *
kmalloc_caches[KMALLOC_SHIFT_HIGH + 1] __ro_after_init =
{ /* initialization for https://bugs.llvm.org/show_bug.cgi?id=42570 */ };
EXPORT_SYMBOL(kmalloc_caches);

static inline u8
size_to_index(uint bytes) {
	while (bytes <= 0 || bytes > 192);
	
	switch ((bytes - 1) / 8) {
		case 0:			return 3;
		case 1:			return 4;
		case 2 ... 3:	return 5;
		case 4 ... 7:	return 6;
		case 8 ... 11:	return 1;
		case 12 ... 15:	return 7;
		case 16 ... 23:	return 2;
		default:		return (u8)-1;
	}
}

/*
 * Find the kmem_cache structure that serves a given size of
 * allocation
 */
kmem_cache_s *kmalloc_slab(size_t size, gfp_t flags)
{
	uint index;
	if (size == 0 || WARN_ON_ONCE(size > KMALLOC_MAX_CACHE_SIZE))
		return NULL;
	else if (size <= 192)
		index = size_to_index(size);
	else
		index = fls(size - 1);

	return kmalloc_caches[index];
}

#define INIT_KMALLOC_INFO(__size, __short_size) {	\
			.name	= "kmalloc-" #__short_size,		\
			.size	= __size,						\
		}
/*
 * kmalloc_info[] is to make slub_debug=,kmalloc-xx option work at boot time.
 * kmalloc_index() supports up to 2^21=2MB, so the final entry of the table is
 * kmalloc-2M.
 */
const kmalloc_info_s kmalloc_info[] __initconst = {

	INIT_KMALLOC_INFO(0,		0),
	INIT_KMALLOC_INFO(96,		96),
	INIT_KMALLOC_INFO(192,		192),
	INIT_KMALLOC_INFO(8,		8),
	INIT_KMALLOC_INFO(16,		16),
	INIT_KMALLOC_INFO(32,		32),
	INIT_KMALLOC_INFO(64,		64),
	INIT_KMALLOC_INFO(128,		128),
	INIT_KMALLOC_INFO(256,		256),
	INIT_KMALLOC_INFO(512,		512),
	INIT_KMALLOC_INFO(1024,		1k),
	INIT_KMALLOC_INFO(2048,		2k),
	INIT_KMALLOC_INFO(4096,		4k),
	INIT_KMALLOC_INFO(8192,		8k),
	INIT_KMALLOC_INFO(16384,	16k),
	INIT_KMALLOC_INFO(32768,	32k),
	INIT_KMALLOC_INFO(65536,	64k),
	INIT_KMALLOC_INFO(131072,	128k),
	INIT_KMALLOC_INFO(262144,	256k),
	INIT_KMALLOC_INFO(524288,	512k),
	INIT_KMALLOC_INFO(1048576,	1M),
	INIT_KMALLOC_INFO(2097152,	2M)
};

/*
 * Create the kmalloc array. Some of the regular kmalloc arrays
 * may already have been created because they were needed to
 * enable allocations for slab creation.
 */
void __init create_kmalloc_caches(slab_flags_t flags)
{
	/*
	 * Including KMALLOC_CGROUP if CONFIG_MEMCG_KMEM defined
	 */
	for (int idx = 1; idx <= KMALLOC_SHIFT_HIGH; idx++)
		if (!kmalloc_caches[idx])
			kmalloc_caches[idx] = create_kmalloc_cache(
					kmalloc_info[idx].name, kmalloc_info[idx].size, flags);

	/* Kmalloc array is now usable */
	slab_state = UP;
}


void *__kmalloc(size_t size, gfp_t flags)
{
	while (unlikely(size > KMALLOC_MAX_CACHE_SIZE));

	void *ret = NULL;
	kmem_cache_s *s = kmalloc_slab(size, flags);
	if (s != NULL)
		ret = kmem_cache_alloc(s, flags);

	// trace_kmalloc(caller, ret, size, s->size, flags, node);
	return ret;
}
EXPORT_SYMBOL(__kmalloc);

/*
 * To avoid unnecessary overhead, we pass through large allocation requests
 * directly to the page allocator. We use __GFP_COMP, because we will need to
 * know the allocation order to free the pages properly in kfree.
 */
void *kmalloc_large(size_t size, gfp_t flags)
{
	void *ptr = NULL;
	page_s *page = alloc_pages(flags | __GFP_COMP, get_order(size));
	if (page) {
		ptr = (void *)page_to_virt(page);
		if (flags & __GFP_ZERO)
			memset(ptr, 0, size);
	}
	return ptr;
}
EXPORT_SYMBOL(kmalloc_large);

void free_large_kmalloc(folio_s *folio, void *object)
{
	uint order = folio_order(folio);
	if (WARN_ON_ONCE(order == 0))
		pr_warn_once("object pointer: 0x%p\n", object);
	__free_pages(folio_page(folio, 0), order);
}
EXPORT_SYMBOL(free_large_kmalloc);


