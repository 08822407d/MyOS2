/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PFN_H_
#define _LINUX_PFN_H_

	#ifndef __ASSEMBLY__

	#	include <linux/kernel/types.h>

	/*
	 * pfn_t: encapsulates a page-frame number that is optionally backed
	 * by memmap (page_s).  Whether a pfn_t has a 'page_s'
	 * backing is indicated by flags in the high bits of the value.
	 */
		typedef struct {
			u64 val;
		} pfn_t;

	#endif

	#define ENT_PER_TABLE	(PAGE_SIZE / sizeof(pgd_t))
	#define PFN_ALIGN(x)	(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
	#define PFN_UP(x)		(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
	#define PFN_DOWN(x)		((x) >> PAGE_SHIFT)
	#define PFN_PHYS(x)		((phys_addr_t)(x) << PAGE_SHIFT)
	#define PHYS_PFN(x)		((unsigned long)((x) >> PAGE_SHIFT))

#endif
