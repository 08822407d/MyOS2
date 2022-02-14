#ifndef _LINUX_MM_H_
#define _LINUX_MM_H_

#include <const.h>

#include <include/mm/mmzone.h>
#include <include/block_dev.h>

#include <arch/amd64/include/archconst.h>

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
	extern Page_s *			mem_map;

	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)

	#define PFN_ALIGN(x)	(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
	#define PFN_UP(x)		(((size_t)(x) + PAGE_SIZE-1) >> PAGE_SHIFT)
	#define PFN_DOWN(x)		((size_t)(x) >> PAGE_SHIFT)
	#define PFN_PHYS(x)		((phys_addr_t)(x) << PAGE_SHIFT)
	#define PHYS_PFN(x)		((unsigned long)((x) >> PAGE_SHIFT))

	static inline zone_s *page_zone(const Page_s * page)
	{
		unsigned long pfn = page_to_pfn(page);
		for (int i = 0 ; i < MAX_NR_ZONES; i ++)
		{
			zone_s * zone = &pg_list.node_zones[i];
			if (pfn >= zone->zone_start_pfn && 
				pfn < (zone->zone_start_pfn + zone->spanned_pages))
				return zone;
		}
	}
#endif /* _LINUX_MM_H_ */