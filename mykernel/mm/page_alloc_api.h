#ifndef _LINUX_PAGE_ALLOC_API_H_
#define _LINUX_PAGE_ALLOC_API_H_

	#include <asm/mmzone_64.h>
	#include <asm/sparsemem.h>

	#include "page_alloc/page_alloc_types.h"
	#include "page_alloc/page-flags.h"
	#include "page_alloc/page_alloc.h"

	/*
	 * The array of struct pages for flatmem.
	 * It must be declared for SPARSEMEM as well because there are configurations
	 * that rely on that.
	 */
	extern page_s	*mem_map;

	#define ENT_PER_TABLE		(PAGE_SIZE / sizeof(pgd_t))
	#define PFN_ALIGN(x)		(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
	#define PFN_UP(x)			(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
	#define PFN_PHYS(x)			((phys_addr_t)(x) << PAGE_SHIFT)
	#define PHYS_PFN(x)			((unsigned long)((x) >> PAGE_SHIFT))
	#define page_to_pfn(page)	((unsigned long)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)

	#ifndef page_to_phys
	#  define page_to_phys(x)	(page_to_pfn(x) << PAGE_SHIFT)
	#endif
	#ifndef page_to_virt
	#  define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
	#endif

	#define offset_in_page(p)	((unsigned long)(p) & ~PAGE_MASK)


	/*
	 * gfp_allowed_mask is set to GFP_BOOT_MASK during early boot to restrict what
	 * GFP flags are used before interrupts are enabled. Once interrupts are
	 * enabled, it is set to __GFP_BITS_MASK while the system is running. During
	 * hibernation, it is used by PM to avoid I/O during memory allocation while
	 * devices are suspended.
	 */
	extern gfp_t gfp_allowed_mask;

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