#ifndef _LINUX_MMZONE_CONST_H_
#define _LINUX_MMZONE_CONST_H_


	/* Free memory management - zoned buddy allocator.  */
	#ifndef CONFIG_FORCE_MAX_ZONEORDER
	#  define MAX_ORDER	11
	#else
	#  define MAX_ORDER	CONFIG_FORCE_MAX_ZONEORDER
	#endif
	#define MAX_ORDER_NR_PAGES	(1 << (MAX_ORDER - 1))

	/*
	 * PAGE_ALLOC_COSTLY_ORDER is the order at which allocations are deemed
	 * costly to service.  That is between allocation orders which should
	 * coalesce naturally under reasonable reclaim pressure and those which
	 * will not.
	 */
	#define PAGE_ALLOC_COSTLY_ORDER 3

#endif /* _LINUX_MMZONE_CONST_H_ */