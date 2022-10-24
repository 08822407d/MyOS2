#ifndef _MYOS_MM_H

#define _MYOS_MM_H

#include <linux/mm/mmzone.h>
#include <uapi/kernel/const.h>

#include <obsolete/block_dev.h>
#include <obsolete/archconst.h>

	// slab consts
	#define SLAB_LEVEL			16
	#define SLAB_SIZE_BASE		32

	extern pg_data_t 	pg_list;

	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)


#endif