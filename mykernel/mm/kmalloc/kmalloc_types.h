#ifndef _KMALLOC_TYPES_H_
#define _KMALLOC_TYPES_H_

	#include "kmalloc_const.h"

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

#endif /* _KMALLOC_TYPES_H_ */
