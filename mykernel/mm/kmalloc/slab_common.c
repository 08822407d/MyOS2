// SPDX-License-Identifier: GPL-2.0
/*
 * Slab allocator functions that are independent of the allocator strategy
 *
 * (C) 2012 Christoph Lameter <cl@linux.com>
 */
#include <linux/kernel/mm.h>
#include <linux/kernel/lib.h>

#include "kmalloc.h"


#include "../kmalloc_api.h"


enum slab_state slab_state;
LIST_HDR_S(slab_caches);
// DEFINE_MUTEX(slab_mutex);
struct kmem_cache *kmem_cache;


/*
 * Figure out what the alignment of the objects will be given a set of
 * flags, a user specified alignment and the size of the objects.
 */
static unsigned int calculate_alignment(slab_flags_t flags,
		unsigned int align, unsigned int size)
{
	/*
	 * If the user wants hardware cache aligned objects then follow that
	 * suggestion if the object is sufficiently large.
	 *
	 * The hardware cache alignment cannot override the specified
	 * alignment though. If that is greater then use it.
	 */
	if (flags & SLAB_HWCACHE_ALIGN) {
		unsigned int ralign;

		ralign = cache_line_size();
		while (size <= ralign / 2)
			ralign /= 2;
		align = max(align, ralign);
	}

	align = max(align, arch_slab_minalign());

	return ALIGN(align, sizeof(void *));
}

static struct kmem_cache *create_cache(const char *name,
		unsigned int object_size, unsigned int align,
		slab_flags_t flags, unsigned int useroffset,
		unsigned int usersize, void (*ctor)(void *),
		struct kmem_cache *root_cache)
{
	struct kmem_cache *s;
	int err;

	if (WARN_ON(useroffset + usersize > object_size))
		useroffset = usersize = 0;

	err = -ENOMEM;
	// s = kmem_cache_zalloc(kmem_cache, GFP_KERNEL);
	// if (!s)
	// 	goto out;

	s->name = name;
	s->size = s->object_size = object_size;
	s->align = align;
	// s->ctor = ctor;

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
 * kmem_cache_create_usercopy - Create a cache with a region suitable
 * for copying to userspace
 * @name: A string which is used in /proc/slabinfo to identify this cache.
 * @size: The size of objects to be created in this cache.
 * @align: The required alignment for the objects.
 * @flags: SLAB flags
 * @useroffset: Usercopy region offset
 * @usersize: Usercopy region size
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
struct kmem_cache *
kmem_cache_create_usercopy(const char *name,
		  unsigned int size, unsigned int align,
		  slab_flags_t flags,
		  unsigned int useroffset, unsigned int usersize,
		  void (*ctor)(void *))
{
	struct kmem_cache *s = NULL;
	const char *cache_name;
	int err;

	// mutex_lock(&slab_mutex);

	// err = kmem_cache_sanity_check(name, size);
	// if (err) {
	// 	goto out_unlock;
	// }

	// /* Refuse requests with allocator specific flags */
	// if (flags & ~SLAB_FLAGS_PERMITTED) {
	// 	err = -EINVAL;
	// 	goto out_unlock;
	// }

	// /*
	//  * Some allocators will constraint the set of valid flags to a subset
	//  * of all flags. We expect them to define CACHE_CREATE_MASK in this
	//  * case, and we'll just provide them with a sanitized version of the
	//  * passed flags.
	//  */
	// flags &= CACHE_CREATE_MASK;

	// /* Fail closed on bad usersize of useroffset values. */
	// if (!IS_ENABLED(CONFIG_HARDENED_USERCOPY) ||
	// 	WARN_ON(!usersize && useroffset) ||
	// 	WARN_ON(size < usersize || size - usersize < useroffset))
	// 	usersize = useroffset = 0;

	// if (!usersize)
	// 	s = __kmem_cache_alias(name, size, align, flags, ctor);
	// if (s)
	// 	goto out_unlock;

	// cache_name = kstrdup_const(name, GFP_KERNEL);
	// if (!cache_name) {
	// 	err = -ENOMEM;
	// 	goto out_unlock;
	// }

	s = create_cache(cache_name, size,
			calculate_alignment(flags, align, size),
			flags, useroffset, usersize, ctor, NULL);
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
EXPORT_SYMBOL(kmem_cache_create_usercopy);

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
struct kmem_cache *
kmem_cache_create(const char *name, unsigned int size, unsigned int align,
		slab_flags_t flags, void (*ctor)(void *))
{
	return kmem_cache_create_usercopy(name, size, align, flags, 0, 0, ctor);
}
EXPORT_SYMBOL(kmem_cache_create);




/* Create a cache during boot when no slab services are available yet */
void __init create_boot_cache(struct kmem_cache *s, const char *name,
		unsigned int size, slab_flags_t flags,
		unsigned int useroffset, unsigned int usersize)
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
