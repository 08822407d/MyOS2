/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Written by Mark Hemment, 1996 (markhe@nextd.demon.co.uk).
 *
 * (C) SGI 2006, Christoph Lameter
 * 	Cleaned up and restructured to ease the addition of alternative
 * 	implementations of SLAB allocators.
 * (C) Linux Foundation 2008-2013
 *      Unified interface for all slab allocators
 */

#ifndef _LINUX_SLAB_H
#define _LINUX_SLAB_H

	#include <linux/kernel/overflow.h>
	#include <linux/kernel/types.h>
	#include <linux/mm/gfp.h>

	#include <mm/kmalloc_api.h>


	#ifdef CONFIG_SLAB
		/*
		* The largest kmalloc size supported by the SLAB allocators is
		* 32 megabyte (2^25) or the maximum allocatable page order if that is
		* less than 32 MB.
		*
		* WARNING: Its not easy to increase this value since the allocators have
		* to do various tricks to work around compiler limitations in order to
		* ensure proper constant folding.
		*/
		#define KMALLOC_SHIFT_HIGH	((MAX_ORDER + PAGE_SHIFT - 1) <= 25 ? \
										(MAX_ORDER + PAGE_SHIFT - 1) : 25)
		#define KMALLOC_SHIFT_MAX	KMALLOC_SHIFT_HIGH
		#ifndef KMALLOC_SHIFT_LOW
			#define KMALLOC_SHIFT_LOW	5
		#endif
	#endif

	#ifdef CONFIG_SLUB
		/*
		 * SLUB directly allocates requests fitting in to an order-1 page
		 * (PAGE_SIZE*2).  Larger requests are passed to the page allocator.
		 */
	#	define KMALLOC_SHIFT_HIGH	(PAGE_SHIFT + 1)
	#	define KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT - 1)
	#	ifndef KMALLOC_SHIFT_LOW
	#		define KMALLOC_SHIFT_LOW	3
	#	endif
	#endif

	/* Maximum allocatable size */
	#define KMALLOC_MAX_SIZE		(1UL << KMALLOC_SHIFT_MAX)
	/* Maximum size for which we actually use a slab cache */
	// #define KMALLOC_MAX_CACHE_SIZE	(1UL << KMALLOC_SHIFT_HIGH)
	#define KMALLOC_MAX_CACHE_SIZE	PAGE_SIZE
	/* Maximum order allocatable via the slab allocator */
	#define KMALLOC_MAX_ORDER		(KMALLOC_SHIFT_MAX - PAGE_SHIFT)

	/*
	 * Kmalloc subsystem.
	 */
	#ifndef KMALLOC_MIN_SIZE
	#	define KMALLOC_MIN_SIZE		(1 << KMALLOC_SHIFT_LOW)
	#endif

	/*
	 * This restriction comes from byte sized index implementation.
	 * Page size is normally 2^12 bytes and, in this case, if we want to use
	 * byte sized index which can represent 2^8 entries, the size of the object
	 * should be equal or greater to 2^12 / 2^8 = 2^4 = 16.
	 * If minimum size of kmalloc is less than 16, we use it as minimum object
	 * size and give up to use byte sized index.
	 */
	#define SLAB_OBJ_MIN_SIZE		(KMALLOC_MIN_SIZE < 16 ? (KMALLOC_MIN_SIZE) : 16)

	/*
	 * Whenever changing this, take care of that kmalloc_type() and
	 * create_kmalloc_caches() still work as intended.
	 *
	 * KMALLOC_NORMAL can contain only unaccounted objects whereas KMALLOC_CGROUP
	 * is for accounted but unreclaimable and non-dma objects. All the other
	 * kmem caches can have both accounted and unaccounted objects.
	 */
	enum kmalloc_cache_type
	{
		KMALLOC_NORMAL = 0,
	#ifndef CONFIG_ZONE_DMA
		KMALLOC_DMA = KMALLOC_NORMAL,
	#endif
	#ifndef CONFIG_MEMCG_KMEM
		KMALLOC_CGROUP = KMALLOC_NORMAL,
	#else
		KMALLOC_CGROUP,
	#endif
		KMALLOC_RECLAIM,
	#ifdef CONFIG_ZONE_DMA
		KMALLOC_DMA,
	#endif
		NR_KMALLOC_TYPES
	};

	/*
	 * Figure out which kmalloc slab an allocation of a certain size
	 * belongs to.
	 * 0 = zero alloc
	 * 1 =  65 .. 96 bytes
	 * 2 = 129 .. 192 bytes
	 * n = 2^(n-1)+1 .. 2^n
	 *
	 * Note: __kmalloc_index() is compile-time optimized, and not runtime optimized;
	 * typical usage is via kmalloc_index() and therefore evaluated at compile-time.
	 * Callers where !size_is_constant should only be test modules, where runtime
	 * overheads of __kmalloc_index() can be tolerated.  Also see kmalloc_slab().
	 */
	static __always_inline unsigned int
	__kmalloc_index(size_t size, bool size_is_constant) {
		if (!size)
			return 0;

		if (size <= KMALLOC_MIN_SIZE)
			return KMALLOC_SHIFT_LOW;

		if (KMALLOC_MIN_SIZE <= 32 && size > 64 && size <= 96)
			return 1;
		if (KMALLOC_MIN_SIZE <= 64 && size > 128 && size <= 192)
			return 2;
		if (size <= 8)
			return 3;
		if (size <= 16)
			return 4;
		if (size <= 32)
			return 5;
		if (size <= 64)
			return 6;
		if (size <= 128)
			return 7;
		if (size <= 256)
			return 8;
		if (size <= 512)
			return 9;
		if (size <= 1024)
			return 10;
		if (size <= 2 * 1024)
			return 11;
		if (size <= 4 * 1024)
			return 12;
		if (size <= 8 * 1024)
			return 13;
		if (size <= 16 * 1024)
			return 14;
		if (size <= 32 * 1024)
			return 15;
		if (size <= 64 * 1024)
			return 16;
		if (size <= 128 * 1024)
			return 17;
		if (size <= 256 * 1024)
			return 18;
		if (size <= 512 * 1024)
			return 19;
		if (size <= 1024 * 1024)
			return 20;
		if (size <= 2 * 1024 * 1024)
			return 21;
		if (size <= 4 * 1024 * 1024)
			return 22;
		if (size <= 8 * 1024 * 1024)
			return 23;
		if (size <= 16 * 1024 * 1024)
			return 24;
		if (size <= 32 * 1024 * 1024)
			return 25;

		// if (!IS_ENABLED(CONFIG_PROFILE_ALL_BRANCHES) && size_is_constant)
		// 	BUILD_BUG_ON_MSG(1, "unexpected size in kmalloc_index()");
		// else
		// 	BUG();

		/* Will never be reached. Needed because the compiler may complain */
		return -1;
	}
	#define kmalloc_index(s) __kmalloc_index(s, true)

	/**
	 * kmalloc_array - allocate memory for an array.
	 * @n: number of elements.
	 * @size: element size.
	 * @flags: the type of memory to allocate (see kmalloc).
	 */
	// static inline __alloc_size(1, 2) void *kmalloc_array(size_t n, size_t size, gfp_t flags)
	// {
	// 	size_t bytes;

	// 	if (unlikely(check_mul_overflow(n, size, &bytes)))
	// 		return NULL;
	// 	if (__builtin_constant_p(n) && __builtin_constant_p(size))
	// 		return kmalloc(bytes, flags);
	// 	return __kmalloc(bytes, flags);
	// }
	static inline void
	*kmalloc_array(size_t n, size_t size, gfp_t flags) {
		size_t bytes;

		if (unlikely(check_mul_overflow(n, size, &bytes)))
			return NULL;
		if (__builtin_constant_p(n) && __builtin_constant_p(size))
			return kmalloc(bytes, flags);
		return __kmalloc(bytes, flags);
	}

	/**
	 * kcalloc - allocate memory for an array. The memory is set to zero.
	 * @n: number of elements.
	 * @size: element size.
	 * @flags: the type of memory to allocate (see kmalloc).
	 */
	// static inline __alloc_size(1, 2) void *kcalloc(size_t n, size_t size, gfp_t flags)
	// {
	// 	return kmalloc_array(n, size, flags | __GFP_ZERO);
	// }
	static inline void
	*kcalloc(size_t n, size_t size, gfp_t flags) {
		return kmalloc_array(n, size, flags | __GFP_ZERO);
	}


	/**
	 * kzalloc - allocate memory. The memory is set to zero.
	 * @size: how many bytes of memory are required.
	 * @flags: the type of memory to allocate (see kmalloc).
	 */
	static inline void *
	kzalloc(size_t size, gfp_t flags) {
		return kmalloc(size, flags | __GFP_ZERO);
	}

#endif /* _LINUX_SLAB_H */
