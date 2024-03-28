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
kmem_cache_s *kmem_cache;


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

static kmem_cache_s *create_cache(const char *name,
		unsigned int object_size, unsigned int align,
		slab_flags_t flags, unsigned int useroffset,
		unsigned int usersize, void (*ctor)(void *),
		kmem_cache_s *root_cache)
{
	kmem_cache_s *s;
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
kmem_cache_s *
kmem_cache_create_usercopy(const char *name,
		  unsigned int size, unsigned int align,
		  slab_flags_t flags,
		  unsigned int useroffset, unsigned int usersize,
		  void (*ctor)(void *))
{
	kmem_cache_s *s = NULL;
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
kmem_cache_s *
kmem_cache_create(const char *name, unsigned int size, unsigned int align,
		slab_flags_t flags, void (*ctor)(void *))
{
	return kmem_cache_create_usercopy(name, size, align, flags, 0, 0, ctor);
}
EXPORT_SYMBOL(kmem_cache_create);




/* Create a cache during boot when no slab services are available yet */
void __init create_boot_cache(kmem_cache_s *s, const char *name,
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


// #define INIT_KMALLOC_INFO(__size, __short_size)			\
// {								\
// 	.name[KMALLOC_NORMAL]  = "kmalloc-" #__short_size,	\
// 	KMALLOC_RCL_NAME(__short_size)				\
// 	KMALLOC_CGROUP_NAME(__short_size)			\
// 	KMALLOC_DMA_NAME(__short_size)				\
// 	.size = __size,						\
// }

// /*
//  * kmalloc_info[] is to make slub_debug=,kmalloc-xx option work at boot time.
//  * kmalloc_index() supports up to 2^21=2MB, so the final entry of the table is
//  * kmalloc-2M.
//  */
// const struct kmalloc_info_struct kmalloc_info[] __initconst = {
// 	INIT_KMALLOC_INFO(0, 0),
// 	INIT_KMALLOC_INFO(96, 96),
// 	INIT_KMALLOC_INFO(192, 192),
// 	INIT_KMALLOC_INFO(8, 8),
// 	INIT_KMALLOC_INFO(16, 16),
// 	INIT_KMALLOC_INFO(32, 32),
// 	INIT_KMALLOC_INFO(64, 64),
// 	INIT_KMALLOC_INFO(128, 128),
// 	INIT_KMALLOC_INFO(256, 256),
// 	INIT_KMALLOC_INFO(512, 512),
// 	INIT_KMALLOC_INFO(1024, 1k),
// 	INIT_KMALLOC_INFO(2048, 2k),
// 	INIT_KMALLOC_INFO(4096, 4k),
// 	INIT_KMALLOC_INFO(8192, 8k),
// 	INIT_KMALLOC_INFO(16384, 16k),
// 	INIT_KMALLOC_INFO(32768, 32k),
// 	INIT_KMALLOC_INFO(65536, 64k),
// 	INIT_KMALLOC_INFO(131072, 128k),
// 	INIT_KMALLOC_INFO(262144, 256k),
// 	INIT_KMALLOC_INFO(524288, 512k),
// 	INIT_KMALLOC_INFO(1048576, 1M),
// 	INIT_KMALLOC_INFO(2097152, 2M)
// };

// /*
//  * Patch up the size_index table if we have strange large alignment
//  * requirements for the kmalloc array. This is only the case for
//  * MIPS it seems. The standard arches will not generate any code here.
//  *
//  * Largest permitted alignment is 256 bytes due to the way we
//  * handle the index determination for the smaller caches.
//  *
//  * Make sure that nothing crazy happens if someone starts tinkering
//  * around with ARCH_KMALLOC_MINALIGN
//  */
// void __init setup_kmalloc_cache_index_table(void)
// {
// 	unsigned int i;

// 	BUILD_BUG_ON(KMALLOC_MIN_SIZE > 256 ||
// 		!is_power_of_2(KMALLOC_MIN_SIZE));

// 	for (i = 8; i < KMALLOC_MIN_SIZE; i += 8) {
// 		unsigned int elem = size_index_elem(i);

// 		if (elem >= ARRAY_SIZE(size_index))
// 			break;
// 		size_index[elem] = KMALLOC_SHIFT_LOW;
// 	}

// 	if (KMALLOC_MIN_SIZE >= 64) {
// 		/*
// 		 * The 96 byte sized cache is not used if the alignment
// 		 * is 64 byte.
// 		 */
// 		for (i = 64 + 8; i <= 96; i += 8)
// 			size_index[size_index_elem(i)] = 7;

// 	}

// 	if (KMALLOC_MIN_SIZE >= 128) {
// 		/*
// 		 * The 192 byte sized cache is not used if the alignment
// 		 * is 128 byte. Redirect kmalloc to use the 256 byte cache
// 		 * instead.
// 		 */
// 		for (i = 128 + 8; i <= 192; i += 8)
// 			size_index[size_index_elem(i)] = 8;
// 	}
// }

// static void __init
// new_kmalloc_cache(int idx, enum kmalloc_cache_type type, slab_flags_t flags)
// {
// 	if ((KMALLOC_RECLAIM != KMALLOC_NORMAL) && (type == KMALLOC_RECLAIM)) {
// 		flags |= SLAB_RECLAIM_ACCOUNT;
// 	} else if (IS_ENABLED(CONFIG_MEMCG_KMEM) && (type == KMALLOC_CGROUP)) {
// 		if (mem_cgroup_kmem_disabled()) {
// 			kmalloc_caches[type][idx] = kmalloc_caches[KMALLOC_NORMAL][idx];
// 			return;
// 		}
// 		flags |= SLAB_ACCOUNT;
// 	} else if (IS_ENABLED(CONFIG_ZONE_DMA) && (type == KMALLOC_DMA)) {
// 		flags |= SLAB_CACHE_DMA;
// 	}

// 	kmalloc_caches[type][idx] = create_kmalloc_cache(
// 					kmalloc_info[idx].name[type],
// 					kmalloc_info[idx].size, flags, 0,
// 					kmalloc_info[idx].size);

// 	/*
// 	 * If CONFIG_MEMCG_KMEM is enabled, disable cache merging for
// 	 * KMALLOC_NORMAL caches.
// 	 */
// 	if (IS_ENABLED(CONFIG_MEMCG_KMEM) && (type == KMALLOC_NORMAL))
// 		kmalloc_caches[type][idx]->refcount = -1;
// }

// /*
//  * Create the kmalloc array. Some of the regular kmalloc arrays
//  * may already have been created because they were needed to
//  * enable allocations for slab creation.
//  */
// void __init create_kmalloc_caches(slab_flags_t flags)
// {
// 	int i;
// 	enum kmalloc_cache_type type;

// 	/*
// 	 * Including KMALLOC_CGROUP if CONFIG_MEMCG_KMEM defined
// 	 */
// 	for (type = KMALLOC_NORMAL; type < NR_KMALLOC_TYPES; type++) {
// 		for (i = KMALLOC_SHIFT_LOW; i <= KMALLOC_SHIFT_HIGH; i++) {
// 			if (!kmalloc_caches[type][i])
// 				new_kmalloc_cache(i, type, flags);

// 			/*
// 			 * Caches that are not of the two-to-the-power-of size.
// 			 * These have to be created immediately after the
// 			 * earlier power of two caches
// 			 */
// 			if (KMALLOC_MIN_SIZE <= 32 && i == 6 &&
// 					!kmalloc_caches[type][1])
// 				new_kmalloc_cache(1, type, flags);
// 			if (KMALLOC_MIN_SIZE <= 64 && i == 7 &&
// 					!kmalloc_caches[type][2])
// 				new_kmalloc_cache(2, type, flags);
// 		}
// 	}

// 	/* Kmalloc array is now usable */
// 	slab_state = UP;
// }




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
