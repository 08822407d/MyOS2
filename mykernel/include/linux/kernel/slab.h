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

#endif /* _LINUX_SLAB_H */
