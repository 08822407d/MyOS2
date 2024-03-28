// SPDX-License-Identifier: GPL-2.0
/*
 * Slab allocator functions that are independent of the allocator strategy
 *
 * (C) 2012 Christoph Lameter <cl@linux.com>
 */
#include <linux/kernel/mm.h>
#include <linux/kernel/lib.h>

#include "kmalloc.h"
#include "slub.h"


#include "../kmalloc_api.h"


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
		uint ralign;

		ralign = cache_line_size();
		while (size <= ralign / 2)
			ralign /= 2;
		align = max(align, ralign);
	}

	align = max(align, arch_slab_minalign());

	return ALIGN(align, sizeof(void *));
}

static kmem_cache_s
*create_cache(const char *name, uint object_size, uint align,
		slab_flags_t flags, kmem_cache_s *root_cache) {
	kmem_cache_s *s;
	int err;

	err = -ENOMEM;
	s = kmem_cache_zalloc(kmem_cache, GFP_KERNEL);
	if (!s)
		goto out;

	s->name = name;
	s->size = s->object_size = object_size;
	s->align = align;

	err = __kmem_cache_create(s, flags);
	if (err)
		goto out_free_cache;

	s->refcount = 1;
	list_header_push(&slab_caches, &s->list);
out:
	if (err)
		return ERR_PTR(err);
	return s;

out_free_cache:
	// kmem_cache_free(kmem_cache, s);
	goto out;
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
kmem_cache_create(const char *name, uint size, uint align,
		slab_flags_t flags, void (*ctor)(void *)) {

	kmem_cache_s *s = NULL;
	const char *cache_name;
	int err;

	// mutex_lock(&slab_mutex);

	/* Refuse requests with allocator specific flags */
	if (flags & ~SLAB_FLAGS_PERMITTED) {
		err = -EINVAL;
		goto out_unlock;
	}

	/*
	 * Some allocators will constraint the set of valid flags to a subset
	 * of all flags. We expect them to define CACHE_CREATE_MASK in this
	 * case, and we'll just provide them with a sanitized version of the
	 * passed flags.
	 */
	flags &= CACHE_CREATE_MASK;

	s = create_cache(cache_name, size,
			calculate_alignment(flags, align, size), flags, NULL);
	if (IS_ERR(s)) {
		err = PTR_ERR(s);
		// kfree_const(cache_name);
	}

out_unlock:
	// mutex_unlock(&slab_mutex);

	if (err) {
		if (flags & SLAB_PANIC)
			panic("%s: Failed to create slab '%s'. Error %d\n",
				__func__, name, err);
		else {
			pr_warn("%s(%s) failed with error %d\n",
				__func__, name, err);
			// dump_stack();
		}
		return NULL;
	}
	return s;
}
EXPORT_SYMBOL(kmem_cache_create);




/* Create a cache during boot when no slab services are available yet */
void __init create_boot_cache(kmem_cache_s *s,
		const char *name, uint size, slab_flags_t flags)
{
	int err;
	unsigned int align = ARCH_KMALLOC_MINALIGN;

	s->name = name;
	s->size = s->object_size = size;

	/*
	 * For power of two sizes, guarantee natural alignment for kmalloc
	 * caches, regardless of SL*B debugging options.
	 */
	if (is_power_of_2(size))
		align = max(align, size);
	s->align = calculate_alignment(flags, align, size);

	err = __kmem_cache_create(s, flags);

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
	list_header_push(&slab_caches, &s->list);
	s->refcount = 1;
	return s;
}

kmem_cache_s *
kmalloc_caches[KMALLOC_SHIFT_HIGH + 1] __ro_after_init =
{ /* initialization for https://bugs.llvm.org/show_bug.cgi?id=42570 */ };
EXPORT_SYMBOL(kmalloc_caches);


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



/*
 * To avoid unnecessary overhead, we pass through large allocation requests
 * directly to the page allocator. We use __GFP_COMP, because we will need to
 * know the allocation order to free the pages properly in kfree.
 */
void *kmalloc_large(size_t size, gfp_t flags) {
	page_s *page;
	void *ptr = NULL;
	unsigned int order = get_order(size);

	flags |= __GFP_COMP;
	page = alloc_pages(flags, order);
	if (page)
	{

		ptr = (void *)page_to_virt(page);
	}

	// ptr = kasan_kmalloc_large(ptr, size, flags);
	// /* As ptr might get tagged, call kmemleak hook after KASAN. */
	// kmemleak_alloc(ptr, size, 1, flags);
	// kmsan_kmalloc_large(ptr, size, flags);

	return ptr;
}
EXPORT_SYMBOL(kmalloc_large);


/*
 * To avoid unnecessary overhead, we pass through large allocation requests
 * directly to the page allocator. We use __GFP_COMP, because we will need to
 * know the allocation order to free the pages properly in kfree.
 */
void *kmalloc_order(size_t size, gfp_t flags, unsigned int order)
{
	void *ret = NULL;
	page_s *page;

	// if (unlikely(flags & GFP_SLAB_BUG_MASK))
	// 	flags = kmalloc_fix_flags(flags);

	flags |= __GFP_COMP;
	page = alloc_pages(flags, order);
	if (likely(page)) {
	// 	ret = page_address(page);
		ret = (void *)page_to_virt(page);
	// 	mod_lruvec_page_state(page, NR_SLAB_UNRECLAIMABLE_B,
	// 			      PAGE_SIZE << order);
	}
	// ret = kasan_kmalloc_large(ret, size, flags);
	// /* As ret might get tagged, call kmemleak hook after KASAN. */
	// kmemleak_alloc(ret, size, 1, flags);
	return ret;
}
