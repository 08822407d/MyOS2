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
		*pgd_ent_ptr_in_mm(mm_s *mmp, ulong address);


		extern p4d_t
		p4dp_get_p4de(p4d_t *p4dp);
		extern ulong
		p4de_index_in_pgd(ulong address);
		extern int
		p4de_none_or_clear_bad(p4d_t *p4d);
		extern p4d_t
		*p4de_ptr_in_pgd(pgd_t *pgdp, ulong address);
		extern p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd, ulong address);


		extern pud_t
		pudp_get_pude(pud_t *pudp);
		extern ulong
		pude_index_in_p4d(ulong address);
		extern int
		pude_none_or_clear_bad(pud_t *pud);
		extern pud_t
		*pude_ptr_in_p4d(p4d_t *p4dp, ulong address);
		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, ulong address);


		extern pmd_t
		pmdp_get_pmde(pmd_t *pmdp);
		extern ulong
		pmde_index_in_pud(ulong address);
		extern int
		pmde_none_or_clear_bad(pmd_t *pmd);
		extern pmd_t
		*pmde_ptr_in_pud(pud_t *pud, ulong address);
		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, ulong address);


		extern pte_t
		ptep_get_pte(pte_t *ptep);
		extern ulong
		pte_index_in_pmd(ulong address);
		extern pte_t
		*pte_ptr_in_pmd(pmd_t *pmdp, ulong address);
		extern pte_t
		*ptd_alloc(mm_s *mm, pmd_t *pmd, ulong address);

		extern pte_t
		*__pte_map(pmd_t *pmdp, ulong address);
		extern void
		pte_unmap(pte_t *ptep);



		extern void
		ptlock_cache_init(void);
		extern bool
		ptlock_alloc(ptdesc_s *ptdesc);
		extern void
		ptlock_free(ptdesc_s *ptdesc);
		extern spinlock_t
		*ptlock_ptr(ptdesc_s *ptdesc);
		extern spinlock_t
		*pte_lockptr(mm_s *mm, pmd_t *pmde_ptr);

		extern page_s
		*pmd_pgtable_page(pmd_t *pmde_ptr);
		extern ptdesc_s
		*pmd_ptdesc(pmd_t *pmde_ptr);
		extern spinlock_t
		*pmd_lockptr(mm_s *mm, pmd_t *pmde_ptr);
		extern spinlock_t
		*pmd_lock(mm_s *mm, pmd_t *pmde_ptr);

		extern spinlock_t
		*pud_lockptr(mm_s *mm, pud_t *pude_ptr);
		extern spinlock_t
		*pud_lock(mm_s *mm, pud_t *pude_ptr);

		extern spinlock_t
		*p4d_lockptr(mm_s *mm, p4d_t *p4de_ptr);
		extern spinlock_t
		*p4d_lock(mm_s *mm, p4d_t *p4de_ptr);


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
		*pgd_ent_ptr_in_mm(mm_s *mmp, ulong address) {
			return (mmp->pgd + pgd_index(address));
		};


		PREFIX_STATIC_INLINE
		p4d_t
		p4dp_get_p4de(p4d_t *p4dp) {
			return READ_ONCE(*p4dp);
		}
		PREFIX_STATIC_INLINE
		ulong
		p4de_index_in_pgd(ulong address) {
			return (address >> P4D_SHIFT) & (PTRS_PER_P4D - 1);
		}
		PREFIX_STATIC_INLINE
		int
		p4de_none_or_clear_bad(p4d_t *p4d) {
			if (p4de_is_none(*p4d))
				return 1;
			if (p4de_is_bad(*p4d)) {
				p4d_ent_clear(p4d);
				return 1;
			}
			return 0;
		}
		PREFIX_STATIC_INLINE
		p4d_t
		*p4de_ptr_in_pgd(pgd_t *pgdp, ulong address) {
			return ((p4d_t *)pgdp);
		};
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgdp, ulong address) {
			return ((p4d_t *)pgdp);
		}


		PREFIX_STATIC_INLINE
		pud_t
		pudp_get_pude(pud_t *pudp) {
			return READ_ONCE(*pudp);
		}
		PREFIX_STATIC_INLINE
		ulong
		pude_index_in_p4d(ulong address) {
			return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
		PREFIX_STATIC_INLINE
		int
		pude_none_or_clear_bad(pud_t *pud) {
			if (pude_is_none(*pud))
				return 1;
			if (pude_is_bad(*pud)) {
				pud_ent_clear(pud);
				return 1;
			}
			return 0;
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pude_ptr_in_p4d(p4d_t *p4dp, ulong address) {
			return (pud_t *)p4de_pointed_page_vaddr(*p4dp) +
						pude_index_in_p4d(address);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d, ulong address) {
			return (p4de_is_none(*p4d)) && __pud_alloc(mm, p4d, address) ?
						NULL : pude_ptr_in_p4d(p4d, address);
		}


		PREFIX_STATIC_INLINE
		pmd_t
		pmdp_get_pmde(pmd_t *pmdp) {
			return READ_ONCE(*pmdp);
		}
		PREFIX_STATIC_INLINE
		ulong
		pmde_index_in_pud(ulong address) {
			return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
		PREFIX_STATIC_INLINE
		int
		pmde_none_or_clear_bad(pmd_t *pmd) {
			if (pmde_is_none(*pmd))
				return 1;
			if (pmde_is_bad(*pmd)) {
				pmd_ent_clear(pmd);
				return 1;
			}
			return 0;
		}
		/* Find an entry in the second-level page table.. */
		PREFIX_STATIC_INLINE
		pmd_t
		*pmde_ptr_in_pud(pud_t *pudp, ulong address) {
			return (pmd_t *)pude_pointed_page_vaddr(*pudp) +
						pmde_index_in_pud(address);
		}
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud, ulong address) {
			return (pude_is_none(*pud)) && __pmd_alloc(mm, pud, address)?
						NULL: pmde_ptr_in_pud(pud, address);
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
		ptep_get_pte(pte_t *ptep) {
			return READ_ONCE(*ptep);
		}
		PREFIX_STATIC_INLINE
		ulong
		pte_index_in_pmd(ulong address) {
			return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pte_ptr_in_pmd(pmd_t *pmdp, ulong address) {
			return (pte_t *)pmd_page_vaddr(*pmdp) +
						pte_index_in_pmd(address);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*ptd_alloc(mm_s *mm, pmd_t *pmd, ulong address) {
			return (pmd_none(*pmd)) && __ptd_alloc(mm, pmd, address) ?
						NULL : pte_ptr_in_pmd(pmd, address);
		}

		PREFIX_STATIC_INLINE
		pte_t
		*__pte_map(pmd_t *pmdp, ulong address) {
			return pte_offset_kernel(pmdp, address);
		}
		PREFIX_STATIC_INLINE
		void
		pte_unmap(pte_t *ptep) {
			// rcu_read_unlock();
		}


		PREFIX_STATIC_INLINE
		void
		ptlock_cache_init(void) {}
		PREFIX_STATIC_INLINE
		bool
		ptlock_alloc(ptdesc_s *ptdesc) {
			return true;
		}
		PREFIX_STATIC_INLINE
		void
		ptlock_free(ptdesc_s *ptdesc) {}
		PREFIX_STATIC_INLINE
		spinlock_t
		*ptlock_ptr(ptdesc_s *ptdesc) {
			return &ptdesc->ptl;
		}
		PREFIX_STATIC_INLINE
		spinlock_t
		*pte_lockptr(mm_s *mm, pmd_t *pmde_ptr) {
			return ptlock_ptr(page_ptdesc(pmd_page(*pmde_ptr)));
		}

		PREFIX_STATIC_INLINE
		page_s
		*pmd_pgtable_page(pmd_t *pmde_ptr) {
			ulong mask = ~(PTRS_PER_PMD * sizeof(pmd_t) - 1);
			return virt_to_page((void *)((ulong) pmde_ptr & mask));
		}
		PREFIX_STATIC_INLINE
		ptdesc_s
		*pmd_ptdesc(pmd_t *pmde_ptr) {
			return page_ptdesc(pmd_pgtable_page(pmde_ptr));
		}
		PREFIX_STATIC_INLINE
		spinlock_t
		*pmd_lockptr(mm_s *mm, pmd_t *pmde_ptr) {
			return ptlock_ptr(pmd_ptdesc(pmde_ptr));
		}
		PREFIX_STATIC_INLINE
		spinlock_t
		*pmd_lock(mm_s *mm, pmd_t *pmde_ptr) {
			spinlock_t *ptl = pmd_lockptr(mm, pmde_ptr);
			spin_lock(ptl);
			return ptl;
		}


		/*
		 * No scalability reason to split PUD locks yet, but follow the same pattern
		 * as the PMD locks to make it easier if we decide to.  The VM should not be
		 * considered ready to switch to split PUD locks yet; there may be places
		 * which need to be converted from page_table_lock.
		 */
		PREFIX_STATIC_INLINE
		spinlock_t
		*pud_lockptr(mm_s *mm, pud_t *pude_ptr) {
			return &mm->page_table_lock;
		}
		PREFIX_STATIC_INLINE
		spinlock_t
		*pud_lock(mm_s *mm, pud_t *pude_ptr) {
			spinlock_t *ptl = pud_lockptr(mm, pude_ptr);
			spin_lock(ptl);
			return ptl;
		}

		PREFIX_STATIC_INLINE
		spinlock_t
		*p4d_lockptr(mm_s *mm, p4d_t *p4de_ptr) {
			return &mm->page_table_lock;
		}
		PREFIX_STATIC_INLINE
		spinlock_t
		*p4d_lock(mm_s *mm, p4d_t *p4de_ptr) {
			spinlock_t *ptl = p4d_lockptr(mm, p4de_ptr);
			spin_lock(ptl);
			return ptl;
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