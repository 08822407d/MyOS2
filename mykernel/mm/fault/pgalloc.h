#ifndef _LINUX_PGALLOC_H_
#define _LINUX_PGALLOC_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		void
		*__myos_pgtable_alloc_one(mm_s *mm);


		extern p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong address);

		extern pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d_entp, ulong address);

		extern pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud_entp, ulong address);

		extern int
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp);
		extern pte_t
		*pgtble_alloc_map(mm_s *mm, pmd_t *pmd_entp, ulong vaddr);
		extern pte_t
		*pgtble_alloc_map_lock(mm_s *mm, pmd_t *pmd_entp,
				ulong vaddr, spinlock_t **ptlp);



		void
		pagetable_free(ptdesc_s *pt);
		ptdesc_s
		*virt_to_ptdesc(const void *x);

		void
		pte_free(mm_s *mm, page_s *pte_page);
		void
		pmd_free(mm_s *mm, pmd_t *pmd);
		void
		pud_free(mm_s *mm, pud_t *pud);


	#endif

	#include "pgalloc_macro.h"
	
	#if defined(PGALLOC_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		*__myos_pgtable_alloc_one(mm_s *mm) {
			page_s *page;
			gfp_t gfp = GFP_PGTABLE_USER;
			if (mm == &init_mm)
				gfp = GFP_PGTABLE_KERNEL;

			page = alloc_page(gfp);
			if (!page)
				return NULL;

			memset((void *)page_to_virt(page), 0, PAGE_SIZE);
			__SetPageTable(page);
			return (void *)page_to_virt(page);
		}




		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		}

		PREFIX_STATIC_INLINE
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d_entp, ulong vaddr) {
			return (p4d_ent_is_none(*p4d_entp)) && __pud_alloc(mm, p4d_entp) ?
						NULL : pud_entp_from_vaddr_and_p4d_entp(p4d_entp, vaddr);
		}

		PREFIX_STATIC_INLINE
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud_entp, ulong vaddr) {
			return (pud_ent_is_none(*pud_entp)) && __pmd_alloc(mm, pud_entp)?
						NULL: pmd_entp_from_vaddr_and_pud_entp(pud_entp, vaddr);
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



		/**
		 * pagetable_free - Free pagetables
		 * @pt:	The page table descriptor
		 *
		 * pagetable_free frees the memory of all page tables described by a page
		 * table descriptor and the memory for the descriptor itself.
		 */
		PREFIX_STATIC_INLINE
		void
		pagetable_free(ptdesc_s *pt) {
			page_s *page = ptdesc_page(pt);
			__free_pages(page, compound_order(page));
		}
		PREFIX_STATIC_INLINE
		ptdesc_s
		*virt_to_ptdesc(const void *x) {
			return page_ptdesc(virt_to_page(x));
		}

		/**
		 * pte_free - free PTE-level user page table memory
		 * @mm: the mm_struct of the current context
		 * @pte_page: the `struct page` referencing the ptdesc
		 */
		PREFIX_STATIC_INLINE
		void
		pte_free(mm_s *mm, page_s *pte_page) {
			ptdesc_s *ptdesc = page_ptdesc(pte_page);
			// pagetable_pte_dtor(ptdesc);
			pagetable_free(ptdesc);
		}
		PREFIX_STATIC_INLINE
		void
		pmd_free(mm_s *mm, pmd_t *pmd) {
			ptdesc_s *ptdesc = virt_to_ptdesc(pmd);
			BUG_ON((ulong)pmd & (PAGE_SIZE-1));
			// pagetable_pmd_dtor(ptdesc);
			pagetable_free(ptdesc);
		}
		PREFIX_STATIC_INLINE
		void
		pud_free(mm_s *mm, pud_t *pud) {
			ptdesc_s *ptdesc = virt_to_ptdesc(pud);
			BUG_ON((ulong)pud & (PAGE_SIZE-1));
			// pagetable_pud_dtor(ptdesc);
			pagetable_free(ptdesc);
		}

	#endif

#endif /* _LINUX_PGALLOC_H_ */