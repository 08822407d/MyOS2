#ifndef _MM_MISC_API_H_
#define _MM_MISC_API_H_

	#include "mm_misc.h"

	extern mm_s		init_mm;
	extern void		*high_memory;
	extern int		sysctl_max_map_count;
	extern ulong	highest_memmap_pfn;

	void mm_core_init(void);
	void setup_initial_init_mm(void *start_code,
			void *end_code, void *end_data, void *brk);

	extern void mmdrop(mm_s *mm);
	extern int set_mm_exe_file(mm_s *mm, file_s *new_exe_file);
	extern void __iomem *myos_ioremap(size_t paddr_start, ulong size);

	/*
	 * Using memblock node mappings, an architecture may initialise its
	 * zones, allocate the backing mem_map and account for memory holes in an
	 * architecture independent manner.
	 *
	 * An architecture is expected to register range of page frames backed by
	 * physical memory with simple_mmblk_add[_node]() before calling
	 * free_area_init() passing in the PFN each zone ends at. At a basic
	 * usage, an architecture is expected to do something like
	 *
	 * unsigned long max_zone_pfns[MAX_NR_ZONES] = {max_dma, max_normal_pfn,
	 * 							 max_highmem_pfn};
	 * for_each_valid_physical_page_range()
	 *	memblock_add_node(base, size, nid, MEMBLOCK_NONE)
	 * free_area_init(max_zone_pfns);
	 */
	void free_area_init(ulong *max_zone_pfn);
	extern void mem_init(void);

	/*
	 * Routines for handling mm_structs
	 */
	extern mm_s *mm_alloc(void);

	/* mmput gets rid of the mappings and all user-space */
	extern void mmput(mm_s *);


	// gup.c
	long
	myos_get_user_pages(mm_s *mm, virt_addr_t start,
		ulong nr_pages, page_s **pages);

#endif /* _MM_MISC_API_H_ */