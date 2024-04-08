#ifndef _LINUX_PGTABLE_H_
#define _LINUX_PGTABLE_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern pte_t
		maybe_mkwrite(pte_t pte, vma_s *vma);

		extern bool
		is_cow_mapping(vm_flags_t flags);


		extern pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address);
		extern unsigned long
		pte_index(unsigned long address);
		extern pte_t
		*pte_ent_offset(pmd_t *pmdp, unsigned long address);
		#define pte_offset pte_ent_offset


		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address);
		extern unsigned long
		pmd_index(unsigned long address);
		extern pmd_t
		*pmd_ent_offset(pud_t *pud, unsigned long address);
		#define pmd_offset pmd_ent_offset
		extern int
		pmd_none_or_clear_bad(pmd_t *pmd);


		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address);
		extern unsigned long
		pud_index(unsigned long address);
		extern pud_t
		*pud_ent_offset(p4d_t *p4dp, unsigned long address);
		#define pud_offset pud_ent_offset
		extern int
		pud_none_or_clear_bad(pud_t *pud);


		extern unsigned long
		pgd_index(unsigned long address);
		extern pgd_t
		*pgd_ent_offset(mm_s *mmp, unsigned long address);
		#define pgd_offset_pgd pgd_ent_offset
		extern int
		pgd_none_or_clear_bad(pgd_t *pgd);

		extern int
		p4d_none_or_clear_bad(p4d_t *p4d);

	#endif

	#include "pgtable_macro.h"
	
	#if defined(PGTABLE_DEFINATION) || !(DEBUG)

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


		/*
		 * A page table page can be thought of an array like this: pXd_t[PTRS_PER_PxD]
		 *
		 * The pXx_index() functions return the index of the entry in the page
		 * table page which would control the given virtual address
		 *
		 * As these functions may be used by the same code for different levels of
		 * the page table folding, they are always available, regardless of
		 * CONFIG_PGTABLE_LEVELS value. For the folded levels they simply return 0
		 * because in such cases PTRS_PER_PxD equals 1.
		 */

		PREFIX_STATIC_INLINE
		pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address) {
			return (arch_pmd_none(*pmd)) && __myos_pte_alloc(mm, pmd, address) ?
						NULL : pte_ent_offset(pmd, address);
		}
		PREFIX_STATIC_INLINE
		unsigned long
		pte_index(unsigned long address) {
			return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pte_ent_offset(pmd_t *pmdp, unsigned long address) {
			return arch_pmd_pgtable(*pmdp) + pte_index(address);
		}


		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address) {
			return (arch_pud_none(*pud)) && __myos_pmd_alloc(mm, pud, address)?
						NULL: pmd_ent_offset(pud, address);
		}
		PREFIX_STATIC_INLINE
		unsigned long
		pmd_index(unsigned long address) {
			return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
		/* Find an entry in the second-level page table.. */
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_ent_offset(pud_t *pud, unsigned long address) {
			return arch_pud_pgtable(*pud) + pmd_index(address);
		}
		PREFIX_STATIC_INLINE
		int
		pmd_none_or_clear_bad(pmd_t *pmd) {
			if (arch_pmd_none(*pmd))
				return 1;
			if (arch_pmd_bad(*pmd)) {
				pmd_clear(pmd);
				return 1;
			}
			return 0;
		}


		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address) {
			return (arch_p4d_none(*p4d)) && __myos_pud_alloc(mm, p4d, address) ?
						NULL : pud_ent_offset(p4d, address);
		}
		PREFIX_STATIC_INLINE
		unsigned long
		pud_index(unsigned long address) {
			return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_ent_offset(p4d_t *p4dp, unsigned long address) {
			return arch_p4d_pgtable(*p4dp) + pud_index(address);
		}
		PREFIX_STATIC_INLINE
		int
		pud_none_or_clear_bad(pud_t *pud) {
			if (arch_pud_none(*pud))
				return 1;
			if (arch_pud_bad(*pud)) {
				pud_clear(pud);
				return 1;
			}
			return 0;
		}


		/* Must be a compile-time constant, so implement it as a macro */
		PREFIX_STATIC_INLINE
		unsigned long
		pgd_index(unsigned long address) {
			return (address >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1);
		}
		PREFIX_STATIC_INLINE
		pgd_t
		*pgd_ent_offset(mm_s *mmp, unsigned long address) {
			return (mmp->pgd + pgd_index(address));
		};
		PREFIX_STATIC_INLINE
		int
		pgd_none_or_clear_bad(pgd_t *pgd) {
			if (pgd_none(*pgd))
				return 1;
			if (pgd_bad(*pgd)) {
				pgd_clear(pgd);
				return 1;
			}
			return 0;
		}

		PREFIX_STATIC_INLINE
		int
		p4d_none_or_clear_bad(p4d_t *p4d) {
			if (arch_p4d_none(*p4d))
				return 1;
			if (arch_p4d_bad(*p4d)) {
				p4d_clear(p4d);
				return 1;
			}
			return 0;
		}

	#endif

#endif /* _LINUX_PGTABLE_H_ */