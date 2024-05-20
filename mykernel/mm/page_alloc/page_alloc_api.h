#ifndef _PAGE_ALLOC_API_H_
#define _PAGE_ALLOC_API_H_

	#include "buddy.h"
	#include "page-flags.h"

	/*
	 * The array of struct pages for flatmem.
	 * It must be declared for SPARSEMEM as well because there are configurations
	 * that rely on that.
	 */
	extern page_s	*mem_map;

	/*
	 * gfp_allowed_mask is set to GFP_BOOT_MASK during early boot to restrict what
	 * GFP flags are used before interrupts are enabled. Once interrupts are
	 * enabled, it is set to __GFP_BITS_MASK while the system is running. During
	 * hibernation, it is used by PM to avoid I/O during memory allocation while
	 * devices are suspended.
	 */
	extern gfp_t gfp_allowed_mask;


	extern void zone_sizes_init(void);



	page_s *alloc_pages(gfp_t gfp, uint order);

	page_s *__myos_alloc_pages(gfp_t gfp, uint order);

	extern void __free_pages(page_s *page, uint order);
	extern void free_pages(ulong addr, uint order);

	extern ulong __get_free_pages(gfp_t gfp_mask, uint order);
	extern ulong get_zeroed_page(gfp_t gfp_mask);

	extern void get_pfn_range(ulong *start_pfn, ulong *end_pfn);

	void memblock_free_pages(page_s *page, ulong pfn, uint order);

#endif /* _PAGE_ALLOC_API_H_ */