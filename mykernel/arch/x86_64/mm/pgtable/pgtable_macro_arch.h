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
	static inline void pgd_clear(pgd_t *pgd)	{ }

	/*
	 * (p4ds are folded into pgds so this doesn't get actually called,
	 * but the define is needed for a generic inline function.)
	 */
	#define set_pgd(pgdptr, pgdval)		set_p4d((p4d_t *)(pgdptr), (p4d_t) { pgdval })
	#define p4d_ent_offset(pgd, addr)	((p4d_t *)pgd);
	#define arch_p4d_val(x)				(arch_pgd_val((x).pgd))
	#define arch_make_p4d(x)			((p4d_t) { arch_make_pgd(x) })

	extern pgd_t init_top_pgt[];
	#define swapper_pg_dir init_top_pgt

	#define set_pte(ptep, pte)	WRITE_ONCE(*(ptep), (pte))
	#define pte_clear(ptep)		set_pte((ptep), arch_make_pte(0))

	#define set_pmd(pmdp, pmd)	WRITE_ONCE(*(pmdp), (pmd))
	#define pmd_clear(pmdp)		set_pmd((pmdp), arch_make_pmd(0))

	#define set_pud(pudp, pud)	WRITE_ONCE(*(pudp), (pud))
	#define pud_clear(pudp)		set_pud((pudp), arch_make_pud(0))

	#define set_p4d(p4dp, p4d)	WRITE_ONCE(*(p4dp), (p4d))
	#define p4d_clear(p4dp)		set_p4d((p4dp), arch_make_p4d(0))

	#define __pgd(x)			arch_make_pgd(x)
	#define __pud(x)			arch_make_pud(x)
	#define __pmd(x)			arch_make_pmd(x)
	#define __pte(x)			arch_make_pte(x)

	/*
	 * Currently stuck as a macro due to indirect forward reference to
	 * linux/mmzone.h's __section_mem_map_addr() definition:
	 */
	#define p4d_page(p4d)	pfn_to_page(p4d_pfn(p4d))


	/*
	 * ZERO_PAGE is a global shared page that is always zero: used
	 * for zero-mapped memory areas etc..
	 */
	extern ulong empty_zero_page[PAGE_SIZE / sizeof(ulong)] __visible;
	#define ZERO_PAGE(vaddr) ((void)(vaddr),virt_to_page(empty_zero_page))

#endif /* _ASM_X86_PGTABLE_MACRO_H_ */
