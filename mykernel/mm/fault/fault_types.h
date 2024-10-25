#ifndef _MMFAULT_TYPES_H_
#define _MMFAULT_TYPES_H_

	#include "../mm_type_declaration.h"

	#include "fault_const.h"


	/*
	 * vm_fault is filled by the pagefault handler and passed to the vma's
	 * ->fault function. The vma's ->fault is responsible for returning a bitmask
	 * of VM_FAULT_xxx flags that give details about how the fault was handled.
	 *
	 * MM layer fills up gfp_mask for page allocations but fault handler might
	 * alter it if its implementation requires a different allocation context.
	 *
	 * pgoff should be used in favour of virtual_address, if possible.
	 */
	struct vm_fault {
		const struct {
			vma_s		*vma;			/* Target VMA */
			gfp_t		gfp_mask;		/* gfp mask to be used for allocations */
			pgoff_t		pgoff;			/* Logical page offset based on vma */
			ulong		address;		/* Faulting virtual address */
		};
		union {
			enum fault_flag	flags;		/* FAULT_FLAG_xxx flags
										 * XXX: should really be 'const' */
			struct {
				unchar
					FF_WRITE			: 1,
					FF_MKWRITE			: 1,
					FF_ALLOW_RETRY		: 1,
					FF_RETRY_NOWAIT		: 1,
					FF_KILLABLE			: 1,
					FF_TRIED			: 1,
					FF_USER				: 1,
					FF_REMOTE			: 1,
					FF_INSTRUCTION		: 1,
					FF_INTERRUPTIBLE	: 1,
					FF_UNSHARE			: 1,
					FF_ORIG_PTE_VALID	: 1,
					FF_VMA_LOCK			: 1;
			} flag_defs;
		};

		p4d_t			*p4d_entp;
		pud_t			*pud_entp;		/* Pointer to pud entry matching
										 * the 'address'
										 */
		pmd_t			*pmd_entp;		/* Pointer to pmd entry matching
										 * the 'address' */
		pte_t			*pte_ptr;		/* Pointer to pte entry matching
										 * the 'address'. NULL if the page
										 * table hasn't been allocated.
										 */
		// union {
			pte_t		orig_pte;		/* Value of PTE at the time of fault */
		// 	pmd_t		orig_pmd;		/* Value of PMD at the time of fault,
		// 								 * used by PMD fault only.
		// 								 */
		// };

		page_s			*cow_page;		/* Page handler may use for COW fault */
		page_s			*page;			/* ->fault handlers should return a
										 * page here, unless VM_FAULT_NOPAGE
										 * is set (which is also implied by
										 * VM_FAULT_ERROR).
										 */
		/* These three entries are valid only while holding ptl lock */
		spinlock_t		*ptl;			/* Page table lock.
										 * Protects pte page table if 'pte'
										 * is not NULL, otherwise pmd.
										 */
		// pgtable_t		prealloc_pte;	/* Pre-allocated pte page table.
		// 								 * vm_ops->map_pages() sets up a page
		// 								 * table from atomic context.
		// 								 * do_fault_around() pre-allocates
		// 								 * page table to avoid allocation from
		// 								 * atomic context.
		// 								 */

		task_s			*curr_tsk;
	};

#endif /* _MMFAULT_TYPES_H_ */
