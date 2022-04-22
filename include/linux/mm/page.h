#ifndef _PAGE_H_
#define _PAGE_H_

#include <lib/utils.h>

#include <sys/types.h>


	struct zone;
	typedef struct zone zone_s;
	
	struct  slab;
	typedef struct slab slab_s;

	typedef struct page
	{
		List_s 			free_list;
		zone_s *		zone;
		phys_addr_t		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	map_count;

		unsigned long	buddy_order;

		slab_s *		slab_ptr;
	} page_s;

	void memblock_free_pages(page_s * page, unsigned long pfn, unsigned int order);
	void preinit_page(void);
	void init_page(void);

#endif /* _PAGE_H_ */