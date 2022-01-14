#ifndef _PAGE_H_
#define _PAGE_H_

#include <lib/utils.h>

#include <sys/types.h>

#include <include/mmzone.h>

	struct  slab;
	typedef struct slab slab_s;

	typedef struct Page
	{
		List_s *		free_list;
		zone_s *		zone;
		phys_addr_t		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	map_count;

		slab_s *		slab_ptr;
	} Page_s;

	void memblock_free_pages(Page_s * page, unsigned long pfn, unsigned int order);

#endif /* _PAGE_H_ */