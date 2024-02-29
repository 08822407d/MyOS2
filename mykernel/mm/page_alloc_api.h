#ifndef _LINUX_PAGE_ALLOC_API_H_
#define _LINUX_PAGE_ALLOC_API_H_

	#include "page_alloc/page_alloc_const.h"
	#include "page_alloc/page_alloc_types.h"
	#include "page_alloc/page_alloc.h"

	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)

	#ifndef page_to_phys
	#	define page_to_phys(x)	(page_to_pfn(x) << PAGE_SHIFT)
	#endif
	#ifndef page_to_virt
	#	define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
	#endif

	#define offset_in_page(p)		((unsigned long)(p) & ~PAGE_MASK)

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