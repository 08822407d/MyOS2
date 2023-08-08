/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGALLOC_H
#define _ASM_X86_PGALLOC_H

	// #include <linux/threads.h>
	#include <linux/mm/pagemap.h>

	#define __HAVE_ARCH_PTE_ALLOC_ONE
	#define __HAVE_ARCH_PGD_FREE
	#include <linux/kernel/asm-generic/pgalloc.h>

	// static inline int  __paravirt_pgd_alloc(mm_s *mm) { return 0; }

	// #ifdef CONFIG_PARAVIRT_XXL
	// #include <asm/paravirt.h>
	// #else
	// #define paravirt_pgd_alloc(mm)	__paravirt_pgd_alloc(mm)
	// static inline void paravirt_pgd_free(mm_s *mm, pgd_t *pgd) {}
	// static inline void paravirt_alloc_pte(mm_s *mm, unsigned long pfn)	{}
	// static inline void paravirt_alloc_pmd(mm_s *mm, unsigned long pfn)	{}
	// static inline void paravirt_alloc_pmd_clone(unsigned long pfn, unsigned long clonepfn,
	// 						unsigned long start, unsigned long count) {}
	// static inline void paravirt_alloc_pud(mm_s *mm, unsigned long pfn)	{}
	// static inline void paravirt_alloc_p4d(mm_s *mm, unsigned long pfn)	{}
	// static inline void paravirt_release_pte(unsigned long pfn) {}
	// static inline void paravirt_release_pmd(unsigned long pfn) {}
	// static inline void paravirt_release_pud(unsigned long pfn) {}
	// static inline void paravirt_release_p4d(unsigned long pfn) {}
	// #endif

	// /*
	// * Flags to use when allocating a user page table page.
	// */
	// extern gfp_t __userpte_alloc_gfp;

	// #ifdef CONFIG_PAGE_TABLE_ISOLATION
	/*
	 * Instead of one PGD, we acquire two PGDs.  Being order-1, it is
	 * both 8k in size and 8k-aligned.  That lets us just flip bit 12
	 * in a pointer to swap between the two 4k halves.
	 */
	// #define PGD_ALLOCATION_ORDER 1
	// #else
	#define PGD_ALLOCATION_ORDER 0
	// #endif

	/*
	 * Allocate and free page tables.
	 */
	extern pgd_t *pgd_alloc(mm_s *);
	extern void pgd_free(mm_s *mm, pgd_t *pgd);

	// extern pgtable_t pte_alloc_one(mm_s *);

	// extern void ___pte_free_tlb(struct mmu_gather *tlb, page_s *pte);

	// static inline void __pte_free_tlb(struct mmu_gather *tlb, page_s *pte,
	// 				unsigned long address)
	// {
	// 	___pte_free_tlb(tlb, pte);
	// }

	// static inline void pmd_populate_kernel(mm_s *mm,
	// 					pmd_t *pmd, pte_t *pte)
	// {
	// 	paravirt_alloc_pte(mm, __pa(pte) >> PAGE_SHIFT);
	// 	set_pmd(pmd, __pmd(__pa(pte) | _PAGE_TABLE));
	// }

	// static inline void pmd_populate_kernel_safe(mm_s *mm,
	// 					pmd_t *pmd, pte_t *pte)
	// {
	// 	paravirt_alloc_pte(mm, __pa(pte) >> PAGE_SHIFT);
	// 	set_pmd_safe(pmd, __pmd(__pa(pte) | _PAGE_TABLE));
	// }

	// static inline void arch_pmd_populate(mm_s *mm, pmd_t *pmd, pte_t *pte) {
	// 	set_pmd(pmd, arch_make_pmd(_PAGE_TABLE | __pa(pte)));
	// }

	// #if CONFIG_PGTABLE_LEVELS > 2
	// extern void ___pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd);

	// static inline void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd,
	// 				unsigned long address)
	// {
	// 	___pmd_free_tlb(tlb, pmd);
	// }

	// static inline void arch_pud_populate(mm_s *mm, pud_t *pud, pmd_t *pmd) {
	// 	set_pud(pud, arch_make_pud(_PAGE_TABLE | __pa(pmd)));
	// }

	// static inline void pud_populate_safe(mm_s *mm, pud_t *pud, pmd_t *pmd)
	// {
	// 	paravirt_alloc_pmd(mm, __pa(pmd) >> PAGE_SHIFT);
	// 	set_pud_safe(pud, __pud(_PAGE_TABLE | __pa(pmd)));
	// }
	// #endif	/* CONFIG_X86_PAE */

	// #if CONFIG_PGTABLE_LEVELS > 3
	// static inline void arch_p4d_populate(mm_s *mm, p4d_t *p4d, pud_t *pud) {
	// 	// paravirt_alloc_pud(mm, __pa(pud) >> PAGE_SHIFT);
	// 	set_p4d(p4d, arch_make_p4d(_PAGE_TABLE | __pa(pud)));
	// }

	// static inline void p4d_populate_safe(mm_s *mm, p4d_t *p4d, pud_t *pud)
	// {
	// 	paravirt_alloc_pud(mm, __pa(pud) >> PAGE_SHIFT);
	// 	set_p4d_safe(p4d, __p4d(_PAGE_TABLE | __pa(pud)));
	// }

	// extern void ___pud_free_tlb(struct mmu_gather *tlb, pud_t *pud);

	// static inline void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pud,
	// 				unsigned long address)
	// {
	// 	___pud_free_tlb(tlb, pud);
	// }

	// #if CONFIG_PGTABLE_LEVELS > 4
	// static inline void pgd_populate(mm_s *mm, pgd_t *pgd, p4d_t *p4d)
	// {
	// 	if (!pgtable_l5_enabled())
	// 		return;
	// 	paravirt_alloc_p4d(mm, __pa(p4d) >> PAGE_SHIFT);
	// 	set_pgd(pgd, __pgd(_PAGE_TABLE | __pa(p4d)));
	// }

	// static inline void pgd_populate_safe(mm_s *mm, pgd_t *pgd, p4d_t *p4d)
	// {
	// 	if (!pgtable_l5_enabled())
	// 		return;
	// 	paravirt_alloc_p4d(mm, __pa(p4d) >> PAGE_SHIFT);
	// 	set_pgd_safe(pgd, __pgd(_PAGE_TABLE | __pa(p4d)));
	// }

	// static inline p4d_t *p4d_alloc_one(mm_s *mm, unsigned long addr)
	// {
	// 	gfp_t gfp = GFP_KERNEL_ACCOUNT;

	// 	if (mm == &init_mm)
	// 		gfp &= ~__GFP_ACCOUNT;
	// 	return (p4d_t *)get_zeroed_page(gfp);
	// }

	// static inline void p4d_free(mm_s *mm, p4d_t *p4d)
	// {
	// 	if (!pgtable_l5_enabled())
	// 		return;

	// 	BUG_ON((unsigned long)p4d & (PAGE_SIZE-1));
	// 	free_page((unsigned long)p4d);
	// }

	// extern void ___p4d_free_tlb(struct mmu_gather *tlb, p4d_t *p4d);

	// static inline void __p4d_free_tlb(struct mmu_gather *tlb, p4d_t *p4d,
	// 				unsigned long address)
	// {
	// 	if (pgtable_l5_enabled())
	// 		___p4d_free_tlb(tlb, p4d);
	// }

	// #endif	/* CONFIG_PGTABLE_LEVELS > 4 */
	// #endif	/* CONFIG_PGTABLE_LEVELS > 3 */
	// #endif	/* CONFIG_PGTABLE_LEVELS > 2 */

#endif /* _ASM_X86_PGALLOC_H */