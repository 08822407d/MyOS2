/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_PGALLOC_H
#define __ASM_GENERIC_PGALLOC_H


	#include <asm/pgtable_types.h>
	#include <linux/mm/mm.h>		/* for page_s */

	// #ifdef CONFIG_MMU

	#define GFP_PGTABLE_KERNEL	(GFP_KERNEL | __GFP_ZERO)
	#define GFP_PGTABLE_USER	(GFP_PGTABLE_KERNEL | __GFP_ACCOUNT)

	// /**
	//  * __pte_alloc_one_kernel - allocate a page for PTE-level kernel page table
	//  * @mm: the mm_struct of the current context
	//  *
	//  * This function is intended for architectures that need
	//  * anything beyond simple page allocation.
	//  *
	//  * Return: pointer to the allocated memory or %NULL on error
	//  */
	// static inline pte_t *__pte_alloc_one_kernel(mm_s *mm)
	// {
	// 	return (pte_t *)__get_free_page(GFP_PGTABLE_KERNEL);
	// }

	// #ifndef __HAVE_ARCH_PTE_ALLOC_ONE_KERNEL
	// /**
	//  * pte_alloc_one_kernel - allocate a page for PTE-level kernel page table
	//  * @mm: the mm_struct of the current context
	//  *
	//  * Return: pointer to the allocated memory or %NULL on error
	//  */
	// static inline pte_t *pte_alloc_one_kernel(mm_s *mm)
	// {
	// 	return __pte_alloc_one_kernel(mm);
	// }
	// #endif

	// /**
	//  * pte_free_kernel - free PTE-level kernel page table page
	//  * @mm: the mm_struct of the current context
	//  * @pte: pointer to the memory containing the page table
	//  */
	// static inline void pte_free_kernel(mm_s *mm, pte_t *pte)
	// {
	// 	free_page((unsigned long)pte);
	// }

	// /**
	//  * __pte_alloc_one - allocate a page for PTE-level user page table
	//  * @mm: the mm_struct of the current context
	//  * @gfp: GFP flags to use for the allocation
	//  *
	//  * Allocates a page and runs the pgtable_pte_page_ctor().
	//  *
	//  * This function is intended for architectures that need
	//  * anything beyond simple page allocation or must have custom GFP flags.
	//  *
	//  * Return: `page_s` initialized as page table or %NULL on error
	//  */
	// // static inline pgtable_t __pte_alloc_one(mm_s *mm, gfp_t gfp)
	// static inline pte_t *pte_alloc_one(mm_s *mm) {
	// 	page_s *page;
	// 	gfp_t gfp = GFP_PGTABLE_USER;
	// 	if (mm == &init_mm)
	// 		gfp = GFP_PGTABLE_KERNEL;

	// 	page = alloc_page(gfp);
	// 	if (!page)
	// 		return NULL;

	// 	memset((void *)page_to_virt(page), 0, PAGE_SIZE);
	// 	__SetPageTable(page);
	// 	__free_pages(page, 0);
	// 	return (pte_t *)page_to_virt(page);
	// }

	// #ifndef __HAVE_ARCH_PTE_ALLOC_ONE
	// /**
	//  * pte_alloc_one - allocate a page for PTE-level user page table
	//  * @mm: the mm_struct of the current context
	//  *
	//  * Allocates a page and runs the pgtable_pte_page_ctor().
	//  *
	//  * Return: `page_s` initialized as page table or %NULL on error
	//  */
	// static inline pgtable_t pte_alloc_one(mm_s *mm)
	// {
	// 	return __pte_alloc_one(mm, GFP_PGTABLE_USER);
	// }
	// #endif

	// /*
	// * Should really implement gc for free page table pages. This could be
	// * done with a reference count in page_s.
	// */

	// /**
	//  * pte_free - free PTE-level user page table page
	//  * @mm: the mm_struct of the current context
	//  * @pte_page: the `page_s` representing the page table
	//  */
	// static inline void pte_free(mm_s *mm, page_s *pte_page) {
	// 	pgtable_pte_page_dtor(pte_page);
	// 	free_page((unsigned long)pte_page);
	// }



	// #ifndef __HAVE_ARCH_PMD_ALLOC_ONE
	// /**
	//  * pmd_alloc_one - allocate a page for PMD-level page table
	//  * @mm: the mm_struct of the current context
	//  *
	//  * Allocates a page and runs the pgtable_pmd_page_ctor().
	//  * Allocations use %GFP_PGTABLE_USER in user context and
	//  * %GFP_PGTABLE_KERNEL in kernel context.
	//  *
	//  * Return: pointer to the allocated memory or %NULL on error
	//  */
	// 	static inline pmd_t
	// 	*pmd_alloc_one(mm_s *mm) {
	// 		page_s *page;
	// 		gfp_t gfp = GFP_PGTABLE_USER;
	// 		if (mm == &init_mm)
	// 			gfp = GFP_PGTABLE_KERNEL;

	// 		page = alloc_page(gfp);
	// 		if (!page)
	// 			return NULL;

	// 		memset((void *)page_to_virt(page), 0, PAGE_SIZE);
	// 		__SetPageTable(page);
	// 		__free_pages(page, 0);
	// 		return (pmd_t *)page_to_virt(page);
	// 	}
	// #endif

	// #ifndef __HAVE_ARCH_PMD_FREE
	// 	static inline void
	// 	pmd_free(mm_s *mm, pmd_t *pmd) {
	// 		// BUG_ON((unsigned long)pmd & (PAGE_SIZE-1));
	// 		// pgtable_pmd_page_dtor(virt_to_page(pmd));
	// 		free_page((unsigned long)pmd);
	// 	}
	// #endif

	// static inline pud_t
	// *__pud_alloc_one(mm_s *mm, unsigned long addr) {
	// static inline pud_t
	// *pud_alloc_one(mm_s *mm) {
	// 	page_s *page;
	// 	gfp_t gfp = GFP_PGTABLE_USER;
	// 	if (mm == &init_mm)
	// 		gfp = GFP_PGTABLE_KERNEL;

	// 	page = alloc_page(gfp);
	// 	if (!page)
	// 		return NULL;

	// 	memset((void *)page_to_virt(page), 0, PAGE_SIZE);
	// 	__SetPageTable(page);
	// 	__free_pages(page, 0);
	// 	return (pud_t *)page_to_virt(page);
	// }

	// #ifndef __HAVE_ARCH_PUD_ALLOC_ONE
	// /**
	//  * pud_alloc_one - allocate a page for PUD-level page table
	//  * @mm: the mm_struct of the current context
	//  *
	//  * Allocates a page using %GFP_PGTABLE_USER for user context and
	//  * %GFP_PGTABLE_KERNEL for kernel context.
	//  *
	//  * Return: pointer to the allocated memory or %NULL on error
	//  */
	// 	static inline pud_t
	// 	*pud_alloc_one(mm_s *mm, unsigned long addr) {
	// 		return __pud_alloc_one(mm, addr);
	// 	}
	// #endif

	// static inline void
	// __pud_free(mm_s *mm, pud_t *pud) {
	// 	BUG_ON((unsigned long)pud & (PAGE_SIZE-1));
	// 	free_page((unsigned long)pud);
	// }

	// #ifndef __HAVE_ARCH_PUD_FREE
	// 	static inline void
	// 	pud_free(mm_s *mm, pud_t *pud) {
	// 		__pud_free(mm, pud);
	// 	}
	// #endif

	// #ifndef __HAVE_ARCH_PGD_FREE
	// 	static inline void
	// 	pgd_free(mm_s *mm, pgd_t *pgd) {
	// 		free_page((unsigned long)pgd);
	// 	}
	// #endif


	static inline void *__myos_pgtable_alloc_one(mm_s *mm) {
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

	static inline void __myos_pgtable_free(void *address) {
		page_s *page = virt_to_page(address);
		__SetPageTable(page);
		__free_pages(page, 0);
	}	


	#define pte_alloc_one(mmp)	(pte_t *)__myos_pgtable_alloc_one(mmp)
	#define pmd_alloc_one(mmp)	(pmd_t *)__myos_pgtable_alloc_one(mmp)
	#define pud_alloc_one(mmp)	(pud_t *)__myos_pgtable_alloc_one(mmp)

	#define pte_free(ptep) __myos_pgtable_free((void *)(ptep))
	#define pmd_free(pmdp) __myos_pgtable_free((void *)(pmdp))
	#define pud_free(pudp) __myos_pgtable_free((void *)(pudp))
	// #define pgd_free(mmp, pgdp) free_page((unsigned long)(pgdp))

	// #endif /* CONFIG_MMU */

#endif /* __ASM_GENERIC_PGALLOC_H */
