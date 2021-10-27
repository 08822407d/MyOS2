#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <const.h>

	////alloc_pages zone_select
	//
	#define ZONE_DMA		(1 << 0)
	//
	#define ZONE_NORMAL		(1 << 1)
	//
	#define ZONE_UNMAPED	(1 << 2)

	// ////struct page attribute
	// //	mapped=1 or un-mapped=0 
	// #define PG_PTable_Maped	(1 << 0)
	// //	init-code=1 or normal-code/data=0
	// #define PG_Kernel_Init	(1 << 1)
	// //	device=1 or memory=0
	// #define PG_Device	(1 << 2)
	// //	kernel=1 or user=0
	// #define PG_Kernel	(1 << 3)
	// //	shared=1 or single-use=0 
	// #define PG_Shared	(1 << 4)

	////struct page attribute (alloc_pages flags)
	//
	#define PG_PTable_Maped	(1 << 0)
	//
	#define PG_Kernel_Init	(1 << 1)
	//
	#define PG_Referenced	(1 << 2)
	//
	#define PG_Dirty		(1 << 3)
	//
	#define PG_Active		(1 << 4)
	//
	#define PG_Up_To_Date	(1 << 5)
	//
	#define PG_Device		(1 << 6)
	//
	#define PG_Kernel		(1 << 7)
	//
	#define PG_K_Share_To_U	(1 << 8)
	//
	#define PG_Slab			(1 << 9)

	// process and task consts
	#define TASK_KSTACK_SIZE	(32 * CONST_1K)

	// slab consts
	#define SLAB_LEVEL			16
	#define SLAB_SIZE_BASE		32

	// sysconfigs
	#define MAX_FILE_NR			32

#endif /* _MEMORY_H_ */