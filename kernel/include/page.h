#ifndef _PAGE_H_
#define _PAGE_H_

#include <sys/types.h>

	struct  slab;
	typedef struct slab slab_s;

	typedef struct Page
	{
		struct MemZone *zone_belonged;
		phys_addr_t		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	map_count;
		unsigned long	age;

		slab_s *		slab_ptr;
	} Page_s;

#endif /* _PAGE_H_ */