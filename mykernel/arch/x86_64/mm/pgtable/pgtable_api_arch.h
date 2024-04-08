#ifndef _ASM_X86_PGTABLE_API_H_
#define _ASM_X86_PGTABLE_API_H_

	#include "pgtable_arch.h"


	extern spinlock_t	pgd_lock;
	extern List_hdr_s	pgd_list_hdr;
	extern pteval_t __supported_pte_mask;
	extern pteval_t __default_kernel_pte_mask;

	/*
	 * Allocate and free page tables.
	 */
	extern pgd_t *pgd_alloc(mm_s *);
	extern void pgd_free(mm_s *mm, pgd_t *pgd);

	void init_mem_mapping(void);
	void early_alloc_pgt_buf(void);
	extern void paging_init(void);

#endif /* _ASM_X86_PGTABLE_API_H_ */