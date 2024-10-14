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


		extern int
		p4d_ent_none_or_clear_bad(p4d_t *p4d_entp);
		extern p4d_t
		p4d_entp_get_ent(p4d_t *p4d_entp);
		extern ulong
		ent_index_in_p4d(ulong address);
		extern p4d_t
		*p4d_entp_from_vaddr_and_pgd_entp(pgd_t *pgd_entp, ulong address);
		extern p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong address);
		extern void
		fill_pud_to_p4d_ent(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p);
		extern void
		fill_pud_to_p4d_ent_safe(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p);


		extern int
		pud_ent_none_or_clear_bad(pud_t *pud_entp);
		extern pud_t
		pud_entp_get_ent(pud_t *pud_entp);
		extern ulong
		ent_index_in_pud(ulong address);
		extern pud_t
		*pud_entp_from_vaddr_and_p4d_entp(p4d_t *p4d_entp, ulong address);
		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d_entp, ulong address);
		extern void
		fill_pmd_to_pud_ent(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p);
		extern void
		fill_pmd_to_pud_ent_safe(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p);


		extern int
		pmde_none_or_clear_bad(pmd_t *pmd_entp);
		extern pmd_t
		pmd_entp_get_ent(pmd_t *pmd_entp);
		extern ulong
		ent_index_in_pmd(ulong address);
		extern pmd_t
		*pmd_entp_from_vaddr_and_pud_entp(pud_t *pud, ulong address);
		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud_entp, ulong address);
		extern void
		fill_pgtbl_to_pmd_ent(mm_s *mm, pmd_t *pmd_entp, pte_t *pgtbl_p);
		extern void
		fill_pgtbl_to_pmd_ent_safe(mm_s *mm, pmd_t *pmd_entp, pte_t *pgtbl_p);


		extern pte_t
		pgtbl_entp_get_ent(pte_t *pte_p);
		extern ulong
		ent_index_in_pgtbl(ulong address);
		extern pte_t
		*pgtbl_entp_from_vaddr_and_pmd_entp(pmd_t *pmd_entp, ulong address);
		extern int
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp);
		extern pte_t
		*pgtble_alloc_map(mm_s *mm, pmd_t *pmd_entp, ulong vaddr);
		extern pte_t *pgtble_alloc_map_lock(mm_s *mm,
				pmd_t *pmd_entp, ulong vaddr, spinlock_t **ptlp);

		extern pte_t
		*__pte_map(pmd_t *pmd_entp, ulong address);
		extern void
		pte_unmap(pte_t *pte_p);



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


		extern void
		mm_inc_nr_puds(mm_s *mm);
		extern void
		mm_dec_nr_puds(mm_s *mm);
		extern void
		mm_inc_nr_pmds(mm_s *mm);
		extern void
		mm_dec_nr_pmds(mm_s *mm);
		extern void
		mm_inc_nr_ptes(mm_s *mm);
		extern void
		mm_dec_nr_ptes(mm_s *mm);


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
		*pgd_ent_ptr_in_mm(mm_s *mmp, ulong vaddr) {
			return (mmp->pgd + pgd_index(vaddr));
		};


		PREFIX_STATIC_INLINE
		int
		p4d_ent_none_or_clear_bad(p4d_t *p4d_entp) {
			if (p4d_ent_is_none(*p4d_entp))
				return 1;
			if (p4d_ent_is_bad(*p4d_entp)) {
				p4d_ent_clear(p4d_entp);
				return 1;
			}
			return 0;
		}
		PREFIX_STATIC_INLINE
		p4d_t
		p4d_entp_get_ent(p4d_t *p4d_entp) {
			return READ_ONCE(*p4d_entp);
		}
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_p4d(ulong vaddr) {
			return (vaddr >> P4D_SHIFT) & (PTRS_PER_P4D - 1);
		}
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_entp_from_vaddr_and_pgd_entp(
				pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		};
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		}
		PREFIX_STATIC_INLINE
		void
		fill_pud_to_p4d_ent(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p) {
			// paravirt_alloc_pud(mm, __pa(pud_p) >> PAGE_SHIFT);
			set_p4d_ent(p4d_entp, __p4d(__pa(pud_p) | _PAGE_TABLE));
		}
		PREFIX_STATIC_INLINE
		void
		fill_pud_to_p4d_ent_safe(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p) {
			// paravirt_alloc_pud(mm, __pa(pud_p) >> PAGE_SHIFT);
			set_p4d_ent_safe(p4d_entp, __p4d(__pa(pud_p) | _PAGE_TABLE));
		}


		PREFIX_STATIC_INLINE
		int
		pud_ent_none_or_clear_bad(pud_t *pud_entp) {
			if (pud_ent_is_none(*pud_entp))
				return 1;
			if (pud_ent_is_bad(*pud_entp)) {
				pud_ent_clear(pud_entp);
				return 1;
			}
			return 0;
		}
		PREFIX_STATIC_INLINE
		pud_t
		pud_entp_get_ent(pud_t *pud_entp) {
			return READ_ONCE(*pud_entp);
		}
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pud(ulong vaddr) {
			return (vaddr >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_entp_from_vaddr_and_p4d_entp(p4d_t *p4d_entp, ulong vaddr) {
			return (pud_t *)p4de_pointed_page_vaddr(*p4d_entp) +
						ent_index_in_pud(vaddr);
		}
		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d_entp, ulong vaddr) {
			return (p4d_ent_is_none(*p4d_entp)) && __pud_alloc(mm, p4d_entp) ?
						NULL : pud_entp_from_vaddr_and_p4d_entp(p4d_entp, vaddr);
		}
		PREFIX_STATIC_INLINE
		void
		fill_pmd_to_pud_ent(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p) {
			// paravirt_alloc_pmd(mm, __pa(pmd_p) >> PAGE_SHIFT);
			set_pud_ent(pud_entp, __pud(__pa(pmd_p) | _PAGE_TABLE));
		}
		PREFIX_STATIC_INLINE
		void
		fill_pmd_to_pud_ent_safe(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p) {
			// paravirt_alloc_pmd(mm, __pa(pmd_p) >> PAGE_SHIFT);
			set_pud_ent_safe(pud_entp, __pud(__pa(pmd_p) | _PAGE_TABLE));
		}


		PREFIX_STATIC_INLINE
		int
		pmde_none_or_clear_bad(pmd_t *pmd_entp) {
			if (pmd_ent_is_none(*pmd_entp))
				return 1;
			if (pmd_ent_is_bad(*pmd_entp)) {
				pmd_ent_clear(pmd_entp);
				return 1;
			}
			return 0;
		}
		PREFIX_STATIC_INLINE
		pmd_t
		pmd_entp_get_ent(pmd_t *pmd_entp) {
			return READ_ONCE(*pmd_entp);
		}
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pmd(ulong vaddr) {
			return (vaddr >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
		/* Find an entry in the second-level page table.. */
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_entp_from_vaddr_and_pud_entp(pud_t *pud_entp, ulong vaddr) {
			return (pmd_t *)pude_pointed_page_vaddr(*pud_entp) +
						ent_index_in_pmd(vaddr);
		}
		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud_entp, ulong vaddr) {
			return (pud_ent_is_none(*pud_entp)) && __pmd_alloc(mm, pud_entp)?
						NULL: pmd_entp_from_vaddr_and_pud_entp(pud_entp, vaddr);
		}
		PREFIX_STATIC_INLINE
		void
		fill_pgtbl_to_pmd_ent(mm_s *mm, pmd_t *pmd_entp, pte_t *pgtbl_p) {
			// paravirt_alloc_pte(mm, __pa(pgtbl_p) >> PAGE_SHIFT);
			set_pmd(pmd_entp, __pmd(__pa(pgtbl_p) | _PAGE_TABLE));
		}
		PREFIX_STATIC_INLINE
		void
		fill_pgtbl_to_pmd_ent_safe(mm_s *mm, pmd_t *pmd_entp, pte_t *pgtbl_p) {
			// paravirt_alloc_pte(mm, __pa(pgtbl_p) >> PAGE_SHIFT);
			set_pmd_safe(pmd_entp, __pmd(__pa(pgtbl_p) | _PAGE_TABLE));
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
		pgtbl_entp_get_ent(pte_t *pte_p) {
			return READ_ONCE(*pte_p);
		}
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pgtbl(ulong vaddr) {
			return (vaddr >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pgtbl_entp_from_vaddr_and_pmd_entp(pmd_t *pmd_entp, ulong vaddr) {
			return (pte_t *)pmd_page_vaddr(*pmd_entp) +
						ent_index_in_pgtbl(vaddr);
		}
		PREFIX_STATIC_INLINE
		int
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp) {
			return (unlikely(pmd_none(*(pmd_entp))) && __pte_alloc(mm, pmd_entp));
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pgtble_alloc_map(mm_s *mm, pmd_t *pmd_entp, ulong vaddr) {
			return (pgtble_alloc(mm, pmd_entp) ?
						NULL : pgtbl_entp_from_vaddr_and_pmd_entp(pmd_entp, vaddr));
		}
		PREFIX_STATIC_INLINE
		pte_t
		*pgtble_alloc_map_lock(mm_s *mm, pmd_t *pmd_entp,
				ulong vaddr, spinlock_t **ptlp) {
			return (pgtble_alloc(mm, pmd_entp) ?
						NULL : pte_offset_map_lock(mm, pmd_entp, vaddr, ptlp));
		}

		PREFIX_STATIC_INLINE
		pte_t
		*__pte_map(pmd_t *pmd_entp, ulong address) {
			return pte_offset_kernel(pmd_entp, address);
		}
		PREFIX_STATIC_INLINE
		void
		pte_unmap(pte_t *pte_p) {
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

		PREFIX_STATIC_INLINE
		spinlock_t
		*pte_lockptr(mm_s *mm, pmd_t *pmde_ptr) {
			return ptlock_ptr(page_ptdesc(pmd_page(*pmde_ptr)));
		}



		PREFIX_STATIC_INLINE
		void
		mm_inc_nr_puds(mm_s *mm) {
			// if (mm_pud_folded(mm))
			// 	return;
			atomic_long_add(PTRS_PER_PUD * sizeof(pud_t),
					&mm->pgtables_bytes);
		}
		PREFIX_STATIC_INLINE
		void
		mm_dec_nr_puds(mm_s *mm) {
			// if (mm_pud_folded(mm))
			// 	return;
			atomic_long_sub(PTRS_PER_PUD * sizeof(pud_t),
					&mm->pgtables_bytes);
		}

		PREFIX_STATIC_INLINE
		void
		mm_inc_nr_pmds(mm_s *mm) {
			// if (mm_pmd_folded(mm))
			// 	return;
			atomic_long_add(PTRS_PER_PMD * sizeof(pmd_t),
					&mm->pgtables_bytes);
		}
		PREFIX_STATIC_INLINE
		void
		mm_dec_nr_pmds(mm_s *mm) {
			// if (mm_pmd_folded(mm))
			// 	return;
			atomic_long_sub(PTRS_PER_PMD * sizeof(pmd_t),
					&mm->pgtables_bytes);
		}

		PREFIX_STATIC_INLINE
		void
		mm_inc_nr_ptes(mm_s *mm) {
			atomic_long_add(PTRS_PER_PTE * sizeof(pte_t),
					&mm->pgtables_bytes);
		}
		PREFIX_STATIC_INLINE
		void
		mm_dec_nr_ptes(mm_s *mm) {
			atomic_long_sub(PTRS_PER_PTE * sizeof(pte_t),
					&mm->pgtables_bytes);
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