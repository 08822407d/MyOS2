#ifndef _LINUX_PAGE_ALLOC_API_H_
#define _LINUX_PAGE_ALLOC_API_H_

	#include "page_alloc/page_alloc_const.h"
	#include "page_alloc/page_alloc_types.h"

	page_s *alloc_pages(gfp_t gfp, unsigned int order);
	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)

	page_s *__myos_alloc_pages(gfp_t gfp, unsigned int order);

	extern void __free_pages(page_s *page, unsigned int order);
	extern void free_pages(unsigned long addr, unsigned int order);

	extern unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order);
	extern unsigned long get_zeroed_page(gfp_t gfp_mask);

	extern void get_pfn_range( unsigned long *start_pfn, unsigned long *end_pfn);

	void memblock_free_pages(page_s *page,
			unsigned long pfn, unsigned int order);


	/* memory.c */
	page_s * paddr_to_page(phys_addr_t paddr);

#endif /* _LINUX_PAGE_ALLOC_API_H_ */