/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_MACRO_H_
#define _ASM_X86_PGTABLE_MACRO_H_


	#define KERNEL_PGD_BOUNDARY		pgd_index(PAGE_OFFSET)
	#define KERNEL_PGD_PTRS			(PTRS_PER_PGD - KERNEL_PGD_BOUNDARY)

	extern pgd_t init_top_pgt[];
	#define swapper_pg_dir init_top_pgt


	#define set_p4d_ent(p4dp, p4d)	WRITE_ONCE(*(p4dp), (p4d))
	#define set_p4d					set_p4d_ent
	#define set_pud_ent(pudp, pud)	WRITE_ONCE(*(pudp), (pud))
	#define set_pud					set_pud_ent
	#define set_pmd_ent(pmdp, pmd)	WRITE_ONCE(*(pmdp), (pmd))
	#define set_pmd					set_pmd_ent

	#define set_pte(ptep, pte)		WRITE_ONCE(*(ptep), (pte))

	/*
	 * (p4ds are folded into pgds so this doesn't get actually called,
	 * but the define is needed for a generic inline function.)
	 */
	#define set_pgd_ent(pgdp, pgdv)	set_p4d_ent((p4d_t *)(pgdp), (p4d_t) { pgdv })
	#define set_pgd					set_pgd_ent


	#define pgd_clear(pgdp)		set_pgd((pgdp), arch_make_pgd_ent(0))
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


	#define pgd_none			pgde_is_none
	#define p4d_none			p4de_is_none
	#define pud_none			pude_is_none
	#define pmd_none			pmde_is_none

	#define pgd_bad				pgde_is_bad
	#define p4d_bad				p4de_is_bad
	#define pud_bad				pude_is_bad
	#define pmd_bad				pmde_is_bad

	#define pgd_present			pgde_is_present
	#define p4d_present			p4de_is_present
	#define pud_present			pude_is_present
	#define pmd_present			pmde_is_present

	#define pgd_page_vaddr		pgde_pointed_page_vaddr
	#define p4d_page_vaddr		p4de_pointed_page_vaddr
	#define pud_page_vaddr		pude_pointed_page_vaddr
	#define pmd_page_vaddr		pmde_pointed_page_vaddr


	#define pte_ERROR(e)	pr_err(						\
				"%s:%d: bad pte %p(%016lx)\n",			\
				__FILE__, __LINE__, &(e), pte_val(e)	\
			)
	#define pmd_ERROR(e)	pr_err(						\
				"%s:%d: bad pmd %p(%016lx)\n",			\
				__FILE__, __LINE__, &(e), pmd_val(e)	\
			)
	#define pud_ERROR(e)	pr_err(						\
				"%s:%d: bad pud %p(%016lx)\n",			\
				__FILE__, __LINE__, &(e), pud_val(e)	\
			)
	#define p4d_ERROR(e)	pr_err(						\
				"%s:%d: bad p4d %p(%016lx)\n",			\
				__FILE__, __LINE__, &(e), p4d_val(e)	\
			)
	#define pgd_ERROR(e)	pr_err(						\
				"%s:%d: bad pgd %p(%016lx)\n",			\
				__FILE__, __LINE__, &(e), pgd_val(e)	\
			)


	/*
	 * ZERO_PAGE is a global shared page that is always zero: used
	 * for zero-mapped memory areas etc..
	 */
	extern ulong empty_zero_page[PAGE_SIZE / sizeof(ulong)] __visible;
	#define ZERO_PAGE(vaddr) ((void)(vaddr),virt_to_page(empty_zero_page))


#endif /* _ASM_X86_PGTABLE_MACRO_H_ */
