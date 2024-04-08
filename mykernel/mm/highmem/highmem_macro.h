#ifndef _LINUX_HIGHMEM_MACRO_H_
#define _LINUX_HIGHMEM_MACRO_H_

	#define ENT_PER_TABLE		(PAGE_SIZE / sizeof(pgd_t))
	#define PFN_ALIGN(x)		(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
	#define PFN_UP(x)			(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
	#define PFN_DOWN(x)			((x) >> PAGE_SHIFT)
	#define PFN_PHYS(x)			((phys_addr_t)(x) << PAGE_SHIFT)
	#define PHYS_PFN(x)			((ulong)((x) >> PAGE_SHIFT))
	#define page_to_pfn(page)	((ulong)((page) - mem_map))
	#define pfn_to_page(pfn)	((pfn) + mem_map)
	#define nth_page(page,n)	pfn_to_page(page_to_pfn((page)) + (n))

	#ifndef page_to_phys
	#  define page_to_phys(x)	(page_to_pfn(x) << PAGE_SHIFT)
	#endif
	#ifndef page_to_virt
	#  define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
	#endif

	#define offset_in_page(p)	((unsigned long)(p) & ~PAGE_MASK)

	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)

#endif /* _LINUX_HIGHMEM_MACRO_H_ */