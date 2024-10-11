/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_MACRO_H_
#define _ASM_X86_PGTABLE_MACRO_H_

	#define KERNEL_PGD_BOUNDARY		pgd_index(PAGE_OFFSET)
	#define KERNEL_PGD_PTRS			(PTRS_PER_PGD - KERNEL_PGD_BOUNDARY)

	/*
	 * The "pgd_xxx()" functions here are trivial for a folded two-level
	 * setup: the p4d is never bad, and a p4d always exists (as it's folded
	 * into the pgd entry)
	 */
	static inline int pgd_none(pgd_t pgd)		{ return 0; }
	static inline int pgd_bad(pgd_t pgd)		{ return 0; }
	static inline int pgd_present(pgd_t pgd)	{ return 1; }
	#define pgd_pgtable			p4d_page_vaddr


	extern pgd_t init_top_pgt[];
	#define swapper_pg_dir init_top_pgt


	#define set_p4d_ent(p4dp, p4d)	WRITE_ONCE(*(p4dp), (p4d))
	#define set_p4d					set_p4d_ent
	#define set_pud_ent(pudp, pud)	WRITE_ONCE(*(pudp), (pud))
	#define set_pud					set_pud_ent
	#define set_pmd_ent(pmdp, pmd)	WRITE_ONCE(*(pmdp), (pmd))
	#define set_pmd					set_pmd_ent

	#define set_pte(ptep, pte)		WRITE_ONCE(*(ptep), (pte))	/*

	 * (p4ds are folded into pgds so this doesn't get actually called,
	 * but the define is needed for a generic inline function.)
	 */
	#define set_pgd_ent(pgdp, pgdv)	set_p4d_ent((p4d_t *)(pgdp), (p4d_t) { pgdv })
	#define set_pgd					set_pgd_ent


	#define pgd_clear(pgdp)
	#define p4d_clear(p4dp)		set_p4d((p4dp), arch_make_p4d_ent(0))
	#define pud_clear(pudp)		set_pud((pudp), arch_make_pud_ent(0))
	#define pmd_clear(pmdp)		set_pmd((pmdp), arch_make_pmd_ent(0))
	#define pte_clear(ptep)		set_pte((ptep), arch_make_pte(0))


	#define __pgd(x)			arch_make_pgd_ent(x)
	#define __p4d(x)			arch_make_p4d_ent(x)
	#define __pud(x)			arch_make_pud_ent(x)
	#define __pmd(x)			arch_make_pmd_ent(x)
	#define __pte(x)			arch_make_pte(x)


	#define pte_writable(ptep)	pte_write(ptep)

	#define pte_present			pte_is_present
	#define pte_none			pte_is_none


	/*
	 * Currently stuck as a macro due to indirect forward reference to
	 * linux/mmzone.h's __section_mem_map_addr() definition:
	 */
	#define p4d_page(p4d)		pfn_to_page(p4d_pfn(p4d))


	#define p4d_present			p4d_ent_is_present
	#define pud_present			pud_ent_is_present
	#define pmd_present			pmd_ent_is_present

	#define p4d_none			p4d_ent_is_none
	#define pud_none			pud_ent_is_none
	#define pmd_none			pmd_ent_is_none

	#define p4d_bad				p4d_ent_is_bad
	#define pud_bad				pud_ent_is_bad
	#define pmd_bad				pmd_ent_is_bad


	/*
	 * ZERO_PAGE is a global shared page that is always zero: used
	 * for zero-mapped memory areas etc..
	 */
	extern ulong empty_zero_page[PAGE_SIZE / sizeof(ulong)] __visible;
	#define ZERO_PAGE(vaddr) ((void)(vaddr),virt_to_page(empty_zero_page))

#endif /* _ASM_X86_PGTABLE_MACRO_H_ */
