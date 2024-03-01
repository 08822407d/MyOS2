#ifndef _ASM_X86_PGTABLE_API_H_
#define _ASM_X86_PGTABLE_API_H_

	#include "pgtable/pgtable_const_arch.h"
	#include "pgtable/pgtable_types_arch.h"
	#include "pgtable/pgtable_arch.h"

	extern spinlock_t	pgd_lock;
	extern List_hdr_s	pgd_list_hdr;
	extern pteval_t __supported_pte_mask;
	extern pteval_t __default_kernel_pte_mask;

	/*
	 * We only update the dirty/accessed state if we set
	 * the dirty bit by hand in the kernel, since the hardware
	 * will do the accessed bit for us, and we don't want to
	 * race with other CPU's that might be updating the dirty
	 * bit at the same time.
	 */
	struct vm_area_struct;
	typedef struct vm_area_struct vma_s;
	struct mm_struct;
	typedef struct mm_struct mm_s;

	/*
	 * Allocate and free page tables.
	 */
	extern pgd_t *pgd_alloc(mm_s *);
	extern void pgd_free(mm_s *mm, pgd_t *pgd);

	void init_mem_mapping(void);
	void early_alloc_pgt_buf(void);
	extern void paging_init(void);

#endif /* _ASM_X86_PGTABLE_API_H_ */