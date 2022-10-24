#ifndef _MYOS_MM_H

#define _MYOS_MM_H

#include <linux/mm/mmzone.h>
#include <uapi/kernel/const.h>

#include <obsolete/block_dev.h>
#include <obsolete/archconst.h>

	// ////struct page attribute (alloc_pages flags)
	// //
	// #define PG_Kernel_Init	(1 << 1)
	// //
	// #define PG_Referenced	(1 << 2)
	// //
	// #define PG_Dirty		(1 << 3)
	// //
	// #define PG_Active		(1 << 4)
	// //
	// #define PG_Up_To_Date	(1 << 5)
	// //
	// #define PG_Device		(1 << 6)
	// //
	// #define PG_Kernel		(1 << 7)
	// //
	// #define PG_K_Share_To_U	(1 << 8)

	// slab consts
	#define SLAB_LEVEL			16
	#define SLAB_SIZE_BASE		32

	extern pg_data_t 	pg_list;

	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)


#endif