/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_INIT_H
#define _ASM_X86_INIT_H

#define __head	__section(".head.text")

#include <linux/kernel/types.h>

struct x86_mapping_info {
	void	*(*alloc_pgt_page)(void *);			/* allocate buf for page table */
	void	(*free_pgt_page)(void *, void *);	/* free buf for page table */
	void	*context;							/* context for alloc_pgt_page */
	ulong	page_flag;							/* page flag for PMD or PUD entry */
	ulong	offset;								/* ident mapping offset */
	bool	direct_gbpages;						/* PUD level 1GB page support */
	ulong	kernpg_flag;						/* kernel pagetable flag override */
};

// int kernel_ident_mapping_init(struct x86_mapping_info *info, pgd_t *pgd_page,
// 				unsigned long pstart, unsigned long pend);

// void kernel_ident_mapping_free(struct x86_mapping_info *info, pgd_t *pgd);

#endif /* _ASM_X86_INIT_H */
