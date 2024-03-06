#ifndef _LINUX_MMZONE_CONST_H_
#define _LINUX_MMZONE_CONST_H_

	/* Free memory management - zoned buddy allocator.  */
	#ifndef CONFIG_FORCE_MAX_ZONEORDER
	#  define MAX_ORDER	11
	#else
	#  define MAX_ORDER	CONFIG_FORCE_MAX_ZONEORDER
	#endif
	#define MAX_ORDER_NR_PAGES	(1 << (MAX_ORDER - 1))

#endif /* _LINUX_MMZONE_CONST_H_ */