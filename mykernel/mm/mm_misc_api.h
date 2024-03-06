#ifndef _LINUX_MM_MISC_API_H_
#define _LINUX_MM_MISC_API_H_

	extern void *high_memory;
	extern int sysctl_max_map_count;
	extern unsigned long highest_memmap_pfn;

	void mm_core_init(void);
	void setup_initial_init_mm(void *start_code,
			void *end_code, void *end_data, void *brk);
	extern int set_mm_exe_file(mm_s *mm, file_s *new_exe_file);
	extern void __iomem *myos_ioremap(size_t paddr_start,
			unsigned long size);

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
	void free_area_init(unsigned long *max_zone_pfn);
	extern void mem_init(void);

	/*
	 * Routines for handling mm_structs
	 */
	extern mm_s *mm_alloc(void);

	/* mmput gets rid of the mappings and all user-space */
	extern void mmput(mm_s *);

#endif /* _LINUX_MM_MISC_API_H_ */