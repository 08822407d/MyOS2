#ifndef _MYOS_MM_H

#define _MYOS_MM_H

#include <linux/mm/mmzone.h>
#include <uapi/const.h>

#include <obsolete/block_dev.h>
#include "../arch/amd64/include/archconst.h"

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

	// slab consts
	#define SLAB_LEVEL			16
	#define SLAB_SIZE_BASE		32

	extern pglist_data_s 	pg_list;
	extern page_s *			mem_map;

	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)


#endif