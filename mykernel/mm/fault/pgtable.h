#ifndef _LINUX_PGTABLE_H_
#define _LINUX_PGTABLE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern pte_t
		maybe_mkwrite(pte_t pte, vma_s *vma);

		extern bool
		is_cow_mapping(vm_flags_t flags);



		extern pgd_t
		pgdp_get(pgd_t *pgdp);
		extern ulong
		pgd_index(ulong address);
		extern pgd_t
		*pgd_ent_ptr(mm_s *mmp, ulong address);
		#define pgd_offset_pgd pgd_ent_ptr
		extern int
		pgd_none_or_clear_bad(pgd_t *pgd);


		extern p4d_t
		p4dp_get(p4d_t *p4dp);
		extern p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd, ulong address);
		extern p4d_t
		*p4d_ent_ptr(pgd_t *pgdp, ulong address);
		#define p4d_offset p4d_ent_ptr
		extern int
		p4d_none_or_clear_bad(p4d_t *p4d);


		extern pud_t
		pudp_get(pud_t *pudp);
		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, ulong address);
		extern ulong
		pud_index(ulong address);
		extern pud_t
		*pud_ent_ptr(p4d_t *p4dp, ulong address);
		#define pud_offset pud_ent_ptr
		extern int
		pud_none_or_clear_bad(pud_t *pud);


		extern pmd_t
		pmdp_get(pmd_t *pmdp);
		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, ulong address);
		extern ulong
		pmd_index(ulong address);
		extern pmd_t
		*pmd_ent_ptr(pud_t *pud, ulong address);
		#define pmd_offset pmd_ent_ptr
		extern int
		pmd_none_or_clear_bad(pmd_t *pmd);


		extern pte_t
		ptep_get(pte_t *ptep);
		extern pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, ulong address);
		extern ulong
		pte_index(ulong address);
		extern pte_t
		*pte_ent_ptr(pmd_t *pmdp, ulong address);
		#define pte_offset pte_ent_ptr



		extern int
		is_zero_pfn(ulong pfn);
		extern ulong
		my_zero_pfn(ulong addr);

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
			// must VM_MAYWRITE and not VM_SHARED
			return (flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;
		}



		PREFIX_STATIC_INLINE
		pgd_t
		pgdp_get(pgd_t *pgdp) {
			return READ_ONCE(*pgdp);
		}
		/* Must be a compile-time constant, so implement it as a macro */
		PREFIX_STATIC_INLINE
		ulong
		pgd_index(ulong address) {
			return (address >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1);
		}
		PREFIX_STATIC_INLINE
		pgd_t
		*pgd_ent_ptr(mm_s *mmp, ulong address) {
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
		p4d_t
		p4dp_get(p4d_t *p4dp) {
			return READ_ONCE(*p4dp);
		}
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgdp, ulong address) {
			return ((p4d_t *)pgdp);
		}
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_ent_ptr(pgd_t *pgdp, ulong address) {
			return ((p4d_t *)pgdp);
		};
		PREFIX_STATIC_INLINE
		int
		p4d_none_or_clear_bad(p4d_t *p4d) {
			if (p4d_none(*p4d))
				return 1;
			if (p4d_bad(*p4d)) {
				p4d_clear(p4d);
				return 1;
			}
			return 0;
		}


		PREFIX_STATIC_INLINE
		pud_t
		pudp_get(pud_t *pudp) {
			return READ_ONCE(*pudp);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, ulong address) {
			return (p4d_none(*p4d)) && __myos_pud_alloc(mm, p4d, address) ?
						NULL : pud_ent_ptr(p4d, address);
		}
		PREFIX_STATIC_INLINE
		ulong
		pud_index(ulong address) {
			return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_ent_ptr(p4d_t *p4dp, ulong address) {
			if (p4d_none_or_clear_bad(p4dp))
				return NULL;
			else
				return p4d_page_vaddr(*p4dp) + pud_index(address);
		}
		PREFIX_STATIC_INLINE
		int
		pud_none_or_clear_bad(pud_t *pud) {
			if (pud_none(*pud))
				return 1;
			if (pud_bad(*pud)) {
				pud_clear(pud);
				return 1;
			}
			return 0;
		}


		PREFIX_STATIC_INLINE
		pmd_t
		pmdp_get(pmd_t *pmdp) {
			return READ_ONCE(*pmdp);
		}
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, ulong address) {
			return (pud_none(*pud)) && __myos_pmd_alloc(mm, pud, address)?
						NULL: pmd_ent_ptr(pud, address);
		}
		PREFIX_STATIC_INLINE
		ulong
		pmd_index(ulong address) {
			return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
		/* Find an entry in the second-level page table.. */
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_ent_ptr(pud_t *pudp, ulong address) {
			if (pud_none_or_clear_bad(pudp))
				return NULL;
			else
				return pud_page_vaddr(*pudp) + pmd_index(address);
		}
		PREFIX_STATIC_INLINE
		int
		pmd_none_or_clear_bad(pmd_t *pmd) {
			if (pmd_none(*pmd))
				return 1;
			if (pmd_bad(*pmd)) {
				pmd_clear(pmd);
				return 1;
			}
			return 0;
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
		ptep_get(pte_t *ptep) {
			return READ_ONCE(*ptep);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pte_alloc(mm_s *mm, pmd_t *pmd, ulong address) {
			return (pmd_none(*pmd)) && __myos_pte_alloc(mm, pmd, address) ?
						NULL : pte_ent_ptr(pmd, address);
		}
		PREFIX_STATIC_INLINE
		ulong
		pte_index(ulong address) {
			return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pte_ent_ptr(pmd_t *pmdp, ulong address) {
			if (pmd_none_or_clear_bad(pmdp))
				return NULL;
			else
				return pmd_page_vaddr(*pmdp) + pte_index(address);
		}




		PREFIX_STATIC_INLINE
		int
		is_zero_pfn(ulong pfn) {
			extern ulong zero_pfn;
			return pfn == zero_pfn;
		}
		PREFIX_STATIC_INLINE
		ulong
		my_zero_pfn(ulong addr) {
			extern ulong zero_pfn;
			return zero_pfn;
		}

	#endif

#endif /* _LINUX_PGTABLE_H_ */