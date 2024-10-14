/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_MACRO_H_
#define _ASM_X86_PGTABLE_MACRO_H_


	#define set_p4d					set_p4d_ent
	#define set_pud					set_pud_ent
	#define set_pmd					set_pmd_ent

	#define set_p4d_safe			set_p4d_ent_safe
	#define set_pud_safe			set_pud_ent_safe
	#define set_pmd_safe			set_pmd_ent_safe

	#define p4d_ent_clear(p4dp)		set_p4d((p4dp), arch_make_p4de(0))
	#define pud_ent_clear(pudp)		set_pud((pudp), arch_make_pude(0))
	#define pmd_ent_clear(pmdp)		set_pmd((pmdp), arch_make_pmde(0))
	#define pte_ent_clear(ptep)		set_pte((ptep), arch_make_pte(0))

	#define p4d_clear			p4d_ent_clear
	#define pud_clear			pud_ent_clear
	#define pmd_clear			pmd_ent_clear
	#define pte_clear			pte_ent_clear

	#define __p4d(x)			arch_make_p4de(x)
	#define __pud(x)			arch_make_pude(x)
	#define __pmd(x)			arch_make_pmde(x)
	#define __pte(x)			arch_make_pte(x)

	#define pte_write(ptep)		pte_writable(ptep)
	#define pte_present			pte_is_present
	#define pte_none			pte_is_none

	/*
	 * Currently stuck as a macro due to indirect forward reference to
	 * linux/mmzone.h's __section_mem_map_addr() definition:
	 */
	#define p4d_page(p4d)		pfn_to_page(p4de_pfn(p4d))
	/*
	 * Currently stuck as a macro due to indirect forward reference to
	 * linux/mmzone.h's __section_mem_map_addr() definition:
	 */
	#define pud_page(pud)		pfn_to_page(pude_pfn(pud))
	/*
	 * Currently stuck as a macro due to indirect forward reference to
	 * linux/mmzone.h's __section_mem_map_addr() definition:
	 */
	#define pmd_page(pmd)		pfn_to_page(pmde_pfn(pmd))

	#define pte_page(pte)		pfn_to_page(pte_pfn(pte))


	#define p4d_none			p4d_ent_is_none
	#define pud_none			pud_ent_is_none
	#define pmd_none			pmd_ent_is_none

	#define p4d_bad				p4d_ent_is_bad
	#define pud_bad				pud_ent_is_bad
	#define pmd_bad				pmd_ent_is_bad

	#define p4d_present			p4d_ent_is_present
	#define pud_present			pud_ent_is_present
	#define pmd_present			pmd_ent_is_present

	#define p4d_same			p4d_ents_same
	#define pud_same			pud_ents_same
	#define pmd_same			pmd_ents_same

	#define p4d_page_vaddr		p4de_pointed_page_vaddr
	#define pud_page_vaddr		pude_pointed_page_vaddr
	#define pmd_page_vaddr		pmde_pointed_page_vaddr

	#define p4d_val				arch_p4de_val
	#define pud_val				arch_pude_val
	#define pmd_val				arch_pmde_val


	#define arch_make_pgde		arch_make_p4de
	#define arch_pgde_val		arch_p4de_val
	#define set_pgd				set_p4d
	#define pgd_clear			p4d_clear
	#define __pgd(x)			__p4d(x)
	#define pgd_page(pgd)		p4d_page(p4d)
	#define pgd_none			p4d_none
	#define pgd_bad				p4d_bad
	#define pgd_present			p4d_present
	#define pgd_same			p4d_same
	#define pgd_page_vaddr		p4d_page_vaddr
	#define pgd_val				p4d_val



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
