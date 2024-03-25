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
		KMALLOC_NORMAL		= 0,
		KMALLOC_RECLAIM,
		KMALLOC_DMA,
		NR_KMALLOC_TYPES
	};

#endif /* _KMALLOC_TYPES_H_ */
