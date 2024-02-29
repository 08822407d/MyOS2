#ifndef _LINUX_MM_FAULT_H_
#define _LINUX_MM_FAULT_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/mm/mm.h>

	#include "memfault_types.h"

	struct vm_area_struct;
	typedef struct vm_area_struct vma_s;

	#define p4d_alloc(mm, pgd, addr)	p4d_ent_offset(pgd, addr)


	#ifdef DEBUG
	
		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address);

		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address);

		extern pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address);

		extern pte_t
		maybe_mkwrite(pte_t pte, vma_s *vma);

		extern bool
		is_cow_mapping(vm_flags_t flags);

	#endif
	
	#if defined(MMFAULT_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address) {
			return (arch_p4d_none(*p4d)) && __myos_pud_alloc(mm, p4d, address) ?
						NULL : pud_ent_offset(p4d, address);
		}

		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address) {
			return (arch_pud_none(*pud)) && __myos_pmd_alloc(mm, pud, address)?
						NULL: pmd_ent_offset(pud, address);
		}

		PREFIX_STATIC_INLINE
		pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address) {
			return (arch_pmd_none(*pmd)) && __myos_pte_alloc(mm, pmd, address) ?
						NULL : pte_ent_offset(pmd, address);
		}

		/*
		 * Do pte_mkwrite, but only if the vma says VM_WRITE.  We do this when
		 * servicing faults for write access.  In the normal case, do always want
		 * pte_mkwrite.  But get_user_pages can cause write faults for mappings
		 * that do not have writing enabled, when used by access_process_vm.
		 */
		PREFIX_STATIC_INLINE
		pte_t
		maybe_mkwrite(pte_t pte, vma_s *vma) {
			if (likely(vma->vm_flags & VM_WRITE))
				pte = pte_mkwrite(pte);
			return pte;
		}

		PREFIX_STATIC_INLINE
		bool
		is_cow_mapping(vm_flags_t flags) {
			return (flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;
		}

	#endif

#endif /* _LINUX_MM_FAULT_H_ */