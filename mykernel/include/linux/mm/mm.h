/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_H
#define _LINUX_MM_H

	#include <linux/lib/errno.h>
	#include <linux/mm/gfp.h>
	#include <linux/debug/bug.h>
	#include <linux/lib/list.h>
	#include <linux/mm/mmzone.h>
	#include <linux/mm/mm_types.h>
	#include <linux/kernel/resource.h>
	#include <linux/kernel/err.h>
	#include <linux/mm/page-flags.h>
	#include <linux/kernel/overflow.h>
	#include <linux/kernel/sizes.h>
	#include <linux/kernel/sched.h>
	#include <linux/mm/pgtable.h>
	#include <linux/kernel/slab.h>


	#include <mm/earlymem_api.h>
	#include <mm/page_alloc_api.h>
	#include <mm/kmalloc_api.h>
	#include <mm/memfault_api.h>
	#include <mm/vm_map_api.h>
	#include <mm/misc_api.h>


	extern void *high_memory;

	#include <asm/page.h>
	#include <asm/processor.h>

	extern int sysctl_max_map_count;

	static inline void
	set_compound_order(page_s *page, unsigned int order) {
		page[1].compound_order = order;
		page[1].compound_nr = 1U << order;
	}


	extern int set_mm_exe_file(mm_s *mm, file_s *new_exe_file);

	// from <linux/mm/internal.h>
	// {
		extern unsigned long highest_memmap_pfn;
	// }


	extern void __iomem
	*myos_ioremap(size_t paddr_start, unsigned long size);


#endif /* _LINUX_MM_H */