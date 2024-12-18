/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_H_
#define _ASM_X86_PGTABLE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const_arch.h"
	#include "../mm_types_arch.h"
	#include "../mm_api_arch.h"


	#ifdef DEBUG

		extern int
		pte_writable(pte_t pte);

		extern void
		set_pte_at(mm_s *mm, ulong addr, pte_t *ptep, pte_t pte);

		extern void
		arch_ptep_set_wrprotect(pte_t *ptep);

		extern pte_t
		pte_set_flags(pte_t pte, pteval_t set);
		extern pte_t
		pte_clear_flags(pte_t pte, pteval_t clear);

		extern pte_t
		pte_mkclean(pte_t pte);
		extern pte_t
		pte_mkold(pte_t pte);
		extern pte_t
		pte_wrprotect(pte_t pte);
		extern pte_t
		pte_mkexec(pte_t pte);
		extern pte_t
		pte_mkdirty(pte_t pte);
		extern pte_t
		pte_mkyoung(pte_t pte);
		extern pte_t
		pte_mkwrite(pte_t pte);



		extern int
		p4d_ent_is_none(p4d_t p4d_ent);
		extern int
		p4d_ent_is_bad(p4d_t p4d_ent);
		extern int
		p4d_ent_is_present(p4d_t p4d_ent);
		extern int
		p4d_ents_same(p4d_t a, p4d_t b);
		extern ulong
		p4de_pointed_page_vaddr(p4d_t p4d_ent);

		extern int
		pud_ent_is_none(pud_t pud_ent);
		extern int
		pud_ent_is_bad(pud_t pud_ent);
		extern int
		pud_ent_is_present(pud_t pud_ent);
		extern int
		pud_ents_same(pud_t a, pud_t b);
		extern ulong
		pude_pointed_page_vaddr(pud_t pud_ent);

		extern int
		pmd_ent_is_none(pmd_t pmd_ent);
		extern int
		pmd_ent_is_bad(pmd_t pmd_ent);
		extern int
		pmd_ent_is_present(pmd_t pmd_ent);
		extern int
		pmd_ents_same(pmd_t a, pmd_t b);
		extern ulong
		pmde_pointed_page_vaddr(pmd_t pud_ent);

		extern int
		pte_is_none(pte_t pte);
		extern int
		pte_is_present(pte_t a);
		extern int
		pte_same(pte_t a, pte_t b);



		extern p4d_t
		arch_make_p4de(pgdval_t val);
		extern void
		set_p4d_ent(p4d_t *p4de_ptr, p4d_t p4d_ent);
		extern void
		set_p4d_ent_safe(p4d_t *p4de_ptr, p4d_t p4d_ent);
		extern p4dval_t
		arch_p4de_val(p4d_t p4d_ent);
		extern p4dval_t
		arch_p4de_pfn_mask(p4d_t p4d_ent);
		extern p4dval_t
		P4DE_FLAG_MASK(p4d_t p4d_ent);
		extern p4dval_t
		arch_p4de_flags(p4d_t p4d_ent);
		extern ulong
		p4de_pfn(p4d_t p4d_ent);
		extern p4d_t
		pfn_p4de(ulong page_nr, pgprot_t pgprot);

		extern pud_t
		arch_make_pude(pmdval_t val);
		extern void
		set_pud_ent(pud_t *pude_ptr, pud_t pud_ent);
		extern void
		set_pud_ent_safe(pud_t *pude_ptr, pud_t pud_ent);
		extern pudval_t
		arch_pude_val(pud_t pud);
		extern pudval_t
		arch_pude_pfn_mask(pud_t pud);
		extern pudval_t
		PUDE_FLAG_MASK(pud_t pud);
		extern pudval_t
		arch_pude_flags(pud_t pud);
		extern ulong
		pude_pfn(pud_t pud_ent);
		extern pud_t
		pfn_pude(ulong page_nr, pgprot_t pgprot);

		extern pmd_t
		arch_make_pmde(pmdval_t val);
		extern void
		set_pmd_ent(pmd_t *pmde_ptr, pmd_t pmd_ent);
		extern void
		set_pmd_ent_safe(pmd_t *pmde_ptr, pmd_t pmd_ent);
		extern pmdval_t
		arch_pmde_val(pmd_t pmd);
		extern pmdval_t 
		arch_pmde_pfn_mask(pmd_t pmd);
		extern pmdval_t
		PMDE_FLAG_MASK(pmd_t pmd);
		extern pmdval_t
		arch_pmde_flags(pmd_t pmd);
		extern ulong
		pmde_pfn(pmd_t pmd_ent);
		extern pmd_t
		pfn_pmde(ulong page_nr, pgprot_t pgprot);

		extern pte_t
		arch_make_pte(pteval_t val);
		extern void
		set_pte(pte_t *pte_ptr, pte_t pte);
		extern void
		set_pte_safe(pte_t *pte_ptr, pte_t pte);
		extern pteval_t
		arch_pte_val(pte_t pte);
		extern pteval_t 
		arch_pte_pfn_mask(pte_t pte);
		extern pteval_t
		PTE_FLAG_MASK(pte_t pte);
		extern pteval_t
		arch_pte_flags(pte_t pte);
		extern phys_addr_t
		arch_pte_addr(pte_t pte);
		extern ulong
		pte_pfn(pte_t pte);
		extern pte_t
		pfn_pte(ulong page_nr, pgprot_t pgprot);



		extern bool
		__pte_needs_invert(u64 val);
		extern u64
		protnone_mask(u64 val);
		extern pgprotval_t
		massage_pgprot(pgprot_t pgprot);
		extern pgprotval_t
		check_pgprot(pgprot_t pgprot);

	#endif

	#include "pgtable_macro_arch.h"

	#if defined(ARCH_PGTABLE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		int
		pte_writable(pte_t pte) {
			return arch_pte_flags(pte) & _PAGE_RW;
		}

		PREFIX_STATIC_INLINE
		void
		set_pte_at(mm_s *mm, ulong addr, pte_t *ptep, pte_t pte) {
			// page_table_check_pte_set(mm, addr, ptep, pte);
			set_pte(ptep, pte);
		}

		PREFIX_STATIC_INLINE
		void
		arch_ptep_set_wrprotect(pte_t *ptep) {
			clear_bit(_PAGE_BIT_RW, (ulong *)&ptep->val);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_set_flags(pte_t pte, pteval_t set) {
			pteval_t v = arch_pte_val(pte);
			return arch_make_pte(v | set);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_clear_flags(pte_t pte, pteval_t clear) {
			pteval_t v = arch_pte_val(pte);
			return arch_make_pte(v & ~clear);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_mkclean(pte_t pte) {
			return pte_clear_flags(pte, _PAGE_DIRTY);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_mkold(pte_t pte) {
			return pte_clear_flags(pte, _PAGE_ACCESSED);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_wrprotect(pte_t pte) {
			return pte_clear_flags(pte, _PAGE_RW);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_mkexec(pte_t pte) {
			return pte_clear_flags(pte, _PAGE_NX);
		}

		PREFIX_STATIC_INLINE
		pte_t
		pte_mkdirty(pte_t pte) {
			return pte_set_flags(pte,
						_PAGE_DIRTY | _PAGE_SOFT_DIRTY);
		}
		PREFIX_STATIC_INLINE
		pte_t
		pte_mkyoung(pte_t pte) {
			return pte_set_flags(pte, _PAGE_ACCESSED);
		}
		PREFIX_STATIC_INLINE
		pte_t
		pte_mkwrite(pte_t pte) {
			return pte_set_flags(pte, _PAGE_RW);
		}



		PREFIX_STATIC_INLINE
		int
		p4d_ent_is_none(p4d_t p4d_ent) {
			return (arch_p4de_val(p4d_ent) &
						~(_PAGE_KNL_ERRATUM_MASK)) == 0;
		}
		PREFIX_STATIC_INLINE
		int
		p4d_ent_is_bad(p4d_t p4d_ent) {
			return (arch_p4de_flags(p4d_ent) &
						~(_KERNPG_TABLE | _PAGE_USER)) != 0;
		}
		PREFIX_STATIC_INLINE
		int
		p4d_ent_is_present(p4d_t p4d_ent) {
			return arch_p4de_flags(p4d_ent) & _PAGE_PRESENT;
		}
		PREFIX_STATIC_INLINE
		int
		p4d_ents_same(p4d_t a, p4d_t b) {
			return a.val == b.val;
		}
		PREFIX_STATIC_INLINE
		ulong
		p4de_pointed_page_vaddr(p4d_t p4d_ent) {
			return (ulong)__va(arch_p4de_val(p4d_ent) &
						arch_p4de_pfn_mask(p4d_ent));
		}


		PREFIX_STATIC_INLINE
		int
		pud_ent_is_none(pud_t pud_ent) {
			return (arch_pude_val(pud_ent) &
						~(_PAGE_KNL_ERRATUM_MASK)) == 0;
		}
		PREFIX_STATIC_INLINE
		int
		pud_ent_is_bad(pud_t pud_ent) {
			return (arch_pude_flags(pud_ent) &
						~(_KERNPG_TABLE | _PAGE_USER)) != 0;
		}
		PREFIX_STATIC_INLINE
		int
		pud_ent_is_present(pud_t pud_ent) {
			return arch_pude_flags(pud_ent) &
						(_PAGE_PRESENT | _PAGE_PSE);
		}
		PREFIX_STATIC_INLINE
		int
		pud_ents_same(pud_t a, pud_t b) {
			return a.val == b.val;
		}
		PREFIX_STATIC_INLINE
		ulong
		pude_pointed_page_vaddr(pud_t pud_ent) {
			return (ulong)__va(arch_pude_val(pud_ent) &
						arch_pude_pfn_mask(pud_ent));
		}


		PREFIX_STATIC_INLINE
		int
		pmd_ent_is_none(pmd_t pmd_ent) {
			/*
			 * Only check low word on 32-bit platforms,
			 * since it might be out of sync with upper half.
			 */
			return (arch_pmde_val(pmd_ent) &
						~_PAGE_KNL_ERRATUM_MASK) == 0;
		}
		PREFIX_STATIC_INLINE
		int
		pmd_ent_is_bad(pmd_t pmd_ent) {
			return (arch_pmde_flags(pmd_ent) &
						~(_KERNPG_TABLE | _PAGE_USER)) != 0;
		}
		PREFIX_STATIC_INLINE
		int
		pmd_ent_is_present(pmd_t pmd_ent) {
			/*
			 * Checking for _PAGE_PSE is needed too because
			 * split_huge_page will temporarily clear the present
			 * bit (but the _PAGE_PSE flag will remain set at all
			 * times while the _PAGE_PRESENT bit is clear).
			 */
			return arch_pmde_flags(pmd_ent) &
						(_PAGE_PRESENT | _PAGE_PSE);
		}
		PREFIX_STATIC_INLINE
		int
		pmd_ents_same(pmd_t a, pmd_t b) {
			return a.val == b.val;
		}
		PREFIX_STATIC_INLINE
		ulong
		pmde_pointed_page_vaddr(pmd_t pmd_ent) {
			return (ulong)__va(arch_pmde_val(pmd_ent) &
						arch_pmde_pfn_mask(pmd_ent));
		}


		PREFIX_STATIC_INLINE
		int
		pte_is_none(pte_t pte) {
			return !(pte.val & ~(_PAGE_KNL_ERRATUM_MASK));
		}
		PREFIX_STATIC_INLINE
		int
		pte_is_present(pte_t pte) {
			return arch_pte_flags(pte) &
						(_PAGE_PRESENT | _PAGE_PROTNONE);
		}
		PREFIX_STATIC_INLINE
		int
		pte_same(pte_t a, pte_t b) {
			return a.val == b.val;
		}


		// PREFIX_STATIC_INLINE
		// pgprot_t
		// pgprot_nx(pgprot_t prot) {
		// 	return __pgprot(pgprot_val(prot) | _PAGE_NX);
		// }


		PREFIX_STATIC_INLINE
		p4d_t
		arch_make_p4de(p4dval_t val) {
			return (p4d_t) { .val = val };
		}
		PREFIX_STATIC_INLINE
		void
		set_p4d_ent(p4d_t *p4de_ptr, p4d_t p4d_ent) {
			WRITE_ONCE(*(p4de_ptr), (p4d_ent));
		}
		PREFIX_STATIC_INLINE
		void
		set_p4d_ent_safe(p4d_t *p4de_ptr, p4d_t p4d_ent) {
			WARN_ON_ONCE(p4d_present(*p4de_ptr) &&
					!p4d_same(*p4de_ptr, p4d_ent));
			set_p4d(p4de_ptr, p4d_ent);
		}
		PREFIX_STATIC_INLINE
		p4dval_t
		arch_p4de_val(p4d_t p4d_ent) {
			return p4d_ent.val;
		}
		PREFIX_STATIC_INLINE
		p4dval_t
		arch_p4de_pfn_mask(p4d_t p4d_ent) {
			/* No 512 GiB huge pages yet */
			return PTE_PFN_MASK;
		}
		PREFIX_STATIC_INLINE
		p4dval_t
		P4DE_FLAG_MASK(p4d_t p4d_ent) {
			return ~arch_p4de_pfn_mask(p4d_ent);
		}
		PREFIX_STATIC_INLINE
		p4dval_t
		arch_p4de_flags(p4d_t p4d_ent) {
			return arch_p4de_val(p4d_ent) & P4DE_FLAG_MASK(p4d_ent);
		}
		PREFIX_STATIC_INLINE
		ulong
		p4de_pfn(p4d_t p4d_ent) {
			phys_addr_t pfn = arch_p4de_val(p4d_ent);
			pfn ^= protnone_mask(pfn);
			return (pfn & arch_p4de_pfn_mask(p4d_ent)) >> PAGE_SHIFT;
		}
		PREFIX_STATIC_INLINE
		p4d_t
		pfn_p4de(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_P4D_PAGE_MASK;
			return __p4d(pfn | check_pgprot(pgprot));
		}


		PREFIX_STATIC_INLINE
		pud_t
		arch_make_pude(pmdval_t val) {
			return (pud_t) { .val = val };
		}
		PREFIX_STATIC_INLINE
		void
		set_pud_ent(pud_t *pude_ptr, pud_t pud_ent) {
			WRITE_ONCE(*(pude_ptr), (pud_ent));
		}
		PREFIX_STATIC_INLINE
		void
		set_pud_ent_safe(pud_t *pude_ptr, pud_t pud_ent) {
			WARN_ON_ONCE(pud_present(*pude_ptr) &&
					!pud_same(*pude_ptr, pud_ent));
			set_pud(pude_ptr, pud_ent);
		}
		PREFIX_STATIC_INLINE
		pudval_t
		arch_pude_val(pud_t pud_ent) {
			return pud_ent.val;
		}
		PREFIX_STATIC_INLINE
		pudval_t
		arch_pude_pfn_mask(pud_t pud_ent) {
			return PTE_PFN_MASK;
		}
		PREFIX_STATIC_INLINE
		pudval_t
		PUDE_FLAG_MASK(pud_t pud_ent) {
			return ~arch_pude_pfn_mask(pud_ent);
		}
		PREFIX_STATIC_INLINE
		pudval_t
		arch_pude_flags(pud_t pud_ent) {
			return arch_pude_val(pud_ent) & PUDE_FLAG_MASK(pud_ent);
		}
		PREFIX_STATIC_INLINE
		ulong
		pude_pfn(pud_t pud_ent) {
			phys_addr_t pfn = arch_pude_val(pud_ent);
			pfn ^= protnone_mask(pfn);
			return (pfn & arch_pude_pfn_mask(pud_ent)) >> PAGE_SHIFT;
		}
		PREFIX_STATIC_INLINE
		pud_t
		pfn_pude(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_PUD_PAGE_MASK;
			return __pud(pfn | check_pgprot(pgprot));
		}


		PREFIX_STATIC_INLINE
		pmd_t
		arch_make_pmde(pmdval_t val) {
			return (pmd_t) { .val = val };
		}
		PREFIX_STATIC_INLINE
		void
		set_pmd_ent(pmd_t *pmde_ptr, pmd_t pmd_ent) {
			WRITE_ONCE(*(pmde_ptr), (pmd_ent));
		}
		PREFIX_STATIC_INLINE
		void
		set_pmd_ent_safe(pmd_t *pmde_ptr, pmd_t pmd_ent) {
			WARN_ON_ONCE(pmd_present(*pmde_ptr) &&
					!pmd_same(*pmde_ptr, pmd_ent));
			set_pmd(pmde_ptr, pmd_ent);
		}
		PREFIX_STATIC_INLINE
		pmdval_t
		arch_pmde_val(pmd_t pmd_ent) {
			return pmd_ent.val;
		}
		PREFIX_STATIC_INLINE
		pmdval_t 
		arch_pmde_pfn_mask(pmd_t pmd_ent) {
			return PTE_PFN_MASK;
		}
		PREFIX_STATIC_INLINE
		pmdval_t
		PMDE_FLAG_MASK(pmd_t pmd_ent) {
			return ~arch_pmde_pfn_mask(pmd_ent);
		}	
		PREFIX_STATIC_INLINE
		pmdval_t
		arch_pmde_flags(pmd_t pmd_ent) {
			return arch_pmde_val(pmd_ent) & PMDE_FLAG_MASK(pmd_ent);
		}
		PREFIX_STATIC_INLINE
		ulong
		pmde_pfn(pmd_t pmd_ent) {
			phys_addr_t pfn = arch_pmde_val(pmd_ent);
			pfn ^= protnone_mask(pfn);
			return (pfn & arch_pmde_pfn_mask(pmd_ent)) >> PAGE_SHIFT;
		}
		PREFIX_STATIC_INLINE
		pmd_t
		pfn_pmde(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_PMD_PAGE_MASK;
			return __pmd(pfn | check_pgprot(pgprot));
		}


		PREFIX_STATIC_INLINE
		pte_t
		arch_make_pte(pteval_t val) {
			return (pte_t) { .val = val };
		}
		PREFIX_STATIC_INLINE
		void
		set_pte(pte_t *pte_ptr, pte_t pte) {
			WRITE_ONCE(*(pte_ptr), (pte));
		}
		PREFIX_STATIC_INLINE
		void
		set_pte_safe(pte_t *pte_ptr, pte_t pte) {
			WARN_ON_ONCE(pte_present(*pte_ptr) &&
					!pte_same(*pte_ptr, pte));\
			set_pte(pte_ptr, pte);
		}
		PREFIX_STATIC_INLINE
		pteval_t
		arch_pte_val(pte_t pte) {
			return pte.val;
		}
		PREFIX_STATIC_INLINE
		pteval_t 
		arch_pte_pfn_mask(pte_t pte) {
			return PTE_PFN_MASK;
		}
		PREFIX_STATIC_INLINE
		pteval_t
		PTE_FLAG_MASK(pte_t pte) {
			return ~arch_pte_pfn_mask(pte);
		}	
		PREFIX_STATIC_INLINE
		pteval_t
		arch_pte_flags(pte_t pte) {
			return arch_pte_val(pte) & PTE_FLAG_MASK(pte);
		}
		PREFIX_STATIC_INLINE
		phys_addr_t
		arch_pte_addr(pte_t pte) {
			return arch_pte_val(pte) & ~PTE_FLAG_MASK(pte);
		}
		PREFIX_STATIC_INLINE
		ulong
		pte_pfn(pte_t pte) {
			phys_addr_t pfn = arch_pte_val(pte);
			pfn ^= protnone_mask(pfn);
			return (pfn & arch_pte_pfn_mask(pte)) >> PAGE_SHIFT;
		}
		PREFIX_STATIC_INLINE
		pte_t
		pfn_pte(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PTE_PFN_MASK;
			return __pte(pfn | check_pgprot(pgprot));
		}



		/*
		 * A clear pte value is special, and doesn't get inverted.
		 *
		 * Note that even users that only pass a pgprot_t (rather
		 * than a full pte) won't trigger the special zero case,
		 * because even PAGE_NONE has _PAGE_PROTNONE | _PAGE_ACCESSED
		 * set. So the all zero case really is limited to just the
		 * cleared page table entry case.
		 */
		PREFIX_STATIC_INLINE
		bool
		__pte_needs_invert(u64 val) {
			return val && !(val & _PAGE_PRESENT);
		}

		/* Get a mask to xor with the page table entry to get the correct pfn. */
		PREFIX_STATIC_INLINE
		u64
		protnone_mask(u64 val) {
			return __pte_needs_invert(val) ?  ~0ull : 0;
		}

		/*
		 * Mask out unsupported bits in a present pgprot.  Non-present pgprots
		 * can use those bits for other purposes, so leave them be.
		 */
		PREFIX_STATIC_INLINE
		pgprotval_t
		massage_pgprot(pgprot_t pgprot) {
			pgprotval_t protval = pgprot_val(pgprot);

			if (protval & _PAGE_PRESENT)
				protval &= __supported_pte_mask;

			return protval;
		}

		PREFIX_STATIC_INLINE
		pgprotval_t
		check_pgprot(pgprot_t pgprot) {
			pgprotval_t massaged_val = massage_pgprot(pgprot);

		// 	/* mmdebug.h can not be included here because of dependencies */
		// #ifdef CONFIG_DEBUG_VM
		// 	WARN_ONCE(pgprot_val(pgprot) != massaged_val,
		// 		"attempted to set unsupported pgprot: %016llx "
		// 		"bits: %016llx supported: %016llx\n",
		// 		(u64)pgprot_val(pgprot),
		// 		(u64)pgprot_val(pgprot) ^ massaged_val,
		// 		(u64)__supported_pte_mask);
		// #endif

			return massaged_val;
		}

	#endif


	// extern pgd_t early_top_pgt[PTRS_PER_PGD];
	// bool __init __early_make_pgtable(unsigned long address, pmdval_t pmd);

	// void ptdump_walk_pgd_level(struct seq_file *m, mm_s *mm);
	// void ptdump_walk_pgd_level_debugfs(struct seq_file *m, mm_s *mm,
	// 				bool user);
	// void ptdump_walk_pgd_level_checkwx(void);
	// void ptdump_walk_user_pgd_level_checkwx(void);

	// #ifdef CONFIG_DEBUG_WX
	// #define debug_checkwx()		ptdump_walk_pgd_level_checkwx()
	// #define debug_checkwx_user()	ptdump_walk_user_pgd_level_checkwx()
	// #else
	// #define debug_checkwx()		do { } while (0)
	// #define debug_checkwx_user()	do { } while (0)
	// #endif

	// extern mm_s *pgd_page_get_mm(page_s *page);

	// extern pmdval_t early_pmd_flags;

	// #ifdef CONFIG_PARAVIRT_XXL
	// #include <asm/paravirt.h>
	// #else  /* !CONFIG_PARAVIRT_XXL */

	// #define set_pte_atomic(ptep, pte)					\
	// 	native_set_pte_atomic(ptep, pte)



	// #define arch_end_context_switch(prev)	do {} while(0)
	// #endif	/* CONFIG_PARAVIRT_XXL */

	// /*
	// * The following only work if pte_is_present() is true.
	// * Undefined behaviour if not..
	// */
	// static inline int pte_dirty(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_DIRTY;
	// }

	// static inline int pte_young(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_ACCESSED;
	// }

	// static inline int pmd_dirty(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_DIRTY;
	// }

	// static inline int pmd_young(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_ACCESSED;
	// }

	// static inline int pud_dirty(pud_t pud)
	// {
	// 	return arch_pude_flags(pud) & _PAGE_DIRTY;
	// }

	// static inline int pud_young(pud_t pud)
	// {
	// 	return arch_pude_flags(pud) & _PAGE_ACCESSED;
	// }



	// static inline int pte_huge(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_PSE;
	// }

	// static inline int pte_global(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_GLOBAL;
	// }

	// static inline int pte_exec(pte_t pte)
	// {
	// 	return !(arch_pte_flags(pte) & _PAGE_NX);
	// }

	// static inline int pte_special(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_SPECIAL;
	// }

	// /* Entries that were set to PROT_NONE are inverted */

	// static inline u64 protnone_mask(u64 val);


	// #define p4d_leaf	p4d_large
	// static inline int p4d_large(p4d_t p4d)
	// {
	// 	/* No 512 GiB pages yet */
	// 	return 0;
	// }

	// #define pmd_leaf	pmd_large
	// static inline int pmd_large(pmd_t pte)
	// {
	// 	return arch_pmde_flags(pte) & _PAGE_PSE;
	// }

	// #ifdef CONFIG_TRANSPARENT_HUGEPAGE
	// /* NOTE: when predicate huge page, consider also pmd_devmap, or use pmd_large */
	// static inline int pmd_trans_huge(pmd_t pmd)
	// {
	// 	return (arch_pmde_val(pmd) & (_PAGE_PSE|_PAGE_DEVMAP)) == _PAGE_PSE;
	// }

	// #ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
	// static inline int pud_trans_huge(pud_t pud)
	// {
	// 	return (arch_pude_val(pud) & (_PAGE_PSE|_PAGE_DEVMAP)) == _PAGE_PSE;
	// }
	// #endif

	// #define has_transparent_hugepage has_transparent_hugepage
	// static inline int has_transparent_hugepage(void)
	// {
	// 	return boot_cpu_has(X86_FEATURE_PSE);
	// }

	// #ifdef CONFIG_ARCH_HAS_PTE_DEVMAP
	// static inline int pmd_devmap(pmd_t pmd)
	// {
	// 	return !!(arch_pmde_val(pmd) & _PAGE_DEVMAP);
	// }

	// #ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
	// static inline int pud_devmap(pud_t pud)
	// {
	// 	return !!(arch_pude_val(pud) & _PAGE_DEVMAP);
	// }
	// #else
	// static inline int pud_devmap(pud_t pud)
	// {
	// 	return 0;
	// }
	// #endif

	// static inline int pgd_devmap(pgd_t pgd)
	// {
	// 	return 0;
	// }
	// #endif
	// #endif /* CONFIG_TRANSPARENT_HUGEPAGE */


	// #ifdef CONFIG_HAVE_ARCH_USERFAULTFD_WP
	// static inline int pte_uffd_wp(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_UFFD_WP;
	// }

	// static inline pte_t pte_mkuffd_wp(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_UFFD_WP);
	// }

	// static inline pte_t pte_clear_uffd_wp(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_UFFD_WP);
	// }
	// #endif /* CONFIG_HAVE_ARCH_USERFAULTFD_WP */


	// static inline pte_t pte_mkhuge(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_PSE);
	// }

	// static inline pte_t pte_clrhuge(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_PSE);
	// }

	// static inline pte_t pte_mkglobal(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_GLOBAL);
	// }

	// static inline pte_t pte_clrglobal(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_GLOBAL);
	// }

	static inline pte_t pte_mkspecial(pte_t pte)
	{
		return pte_set_flags(pte, _PAGE_SPECIAL);
	}

	// static inline pte_t pte_mkdevmap(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_SPECIAL|_PAGE_DEVMAP);
	// }

	// static inline pmd_t pmd_set_flags(pmd_t pmd, pmdval_t set)
	// {
	// 	pmdval_t v = native_pmd_val(pmd);

	// 	return native_make_pmd(v | set);
	// }

	// static inline pmd_t pmd_clear_flags(pmd_t pmd, pmdval_t clear)
	// {
	// 	pmdval_t v = native_pmd_val(pmd);

	// 	return native_make_pmd(v & ~clear);
	// }

	// #ifdef CONFIG_HAVE_ARCH_USERFAULTFD_WP
	// static inline int pmd_uffd_wp(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_UFFD_WP;
	// }

	// static inline pmd_t pmd_mkuffd_wp(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_UFFD_WP);
	// }

	// static inline pmd_t pmd_clear_uffd_wp(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_UFFD_WP);
	// }
	// #endif /* CONFIG_HAVE_ARCH_USERFAULTFD_WP */

	// static inline pmd_t pmd_mkold(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_ACCESSED);
	// }

	// static inline pmd_t pmd_mkclean(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_DIRTY);
	// }

	// static inline pmd_t pmd_wrprotect(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_RW);
	// }

	// static inline pmd_t pmd_mkdirty(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_DIRTY | _PAGE_SOFT_DIRTY);
	// }

	// static inline pmd_t pmd_mkdevmap(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_DEVMAP);
	// }

	// static inline pmd_t pmd_mkhuge(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_PSE);
	// }

	// static inline pmd_t pmd_mkyoung(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_ACCESSED);
	// }

	// static inline pmd_t pmd_mkwrite(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_RW);
	// }

	// static inline pud_t pud_set_flags(pud_t pud, pudval_t set)
	// {
	// 	pudval_t v = native_pud_val(pud);

	// 	return native_make_pud(v | set);
	// }

	// static inline pud_t pud_clear_flags(pud_t pud, pudval_t clear)
	// {
	// 	pudval_t v = native_pud_val(pud);

	// 	return native_make_pud(v & ~clear);
	// }

	// static inline pud_t pud_mkold(pud_t pud)
	// {
	// 	return pud_clear_flags(pud, _PAGE_ACCESSED);
	// }

	// static inline pud_t pud_mkclean(pud_t pud)
	// {
	// 	return pud_clear_flags(pud, _PAGE_DIRTY);
	// }

	// static inline pud_t pud_wrprotect(pud_t pud)
	// {
	// 	return pud_clear_flags(pud, _PAGE_RW);
	// }

	// static inline pud_t pud_mkdirty(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_DIRTY | _PAGE_SOFT_DIRTY);
	// }

	// static inline pud_t pud_mkdevmap(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_DEVMAP);
	// }

	// static inline pud_t pud_mkhuge(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_PSE);
	// }

	// static inline pud_t pud_mkyoung(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_ACCESSED);
	// }

	// static inline pud_t pud_mkwrite(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_RW);
	// }

	// #ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
	// static inline int pte_soft_dirty(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_SOFT_DIRTY;
	// }

	// static inline int pmd_soft_dirty(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_SOFT_DIRTY;
	// }

	// static inline int pud_soft_dirty(pud_t pud)
	// {
	// 	return arch_pude_flags(pud) & _PAGE_SOFT_DIRTY;
	// }

	// static inline pte_t pte_mksoft_dirty(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_SOFT_DIRTY);
	// }

	// static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_SOFT_DIRTY);
	// }

	// static inline pud_t pud_mksoft_dirty(pud_t pud)
	// {
	// 	return pud_set_flags(pud, _PAGE_SOFT_DIRTY);
	// }

	// static inline pte_t pte_clear_soft_dirty(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_SOFT_DIRTY);
	// }

	// static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_SOFT_DIRTY);
	// }

	// static inline pud_t pud_clear_soft_dirty(pud_t pud)
	// {
	// 	return pud_clear_flags(pud, _PAGE_SOFT_DIRTY);
	// }

	// #endif /* CONFIG_HAVE_ARCH_SOFT_DIRTY */

	// static inline pmd_t pmd_mkinvalid(pmd_t pmd)
	// {
	// 	return pfn_pmde(pmde_pfn(pmd),
	// 			__pgprot(arch_pmde_flags(pmd) & ~(_PAGE_PRESENT|_PAGE_PROTNONE)));
	// }

	// static inline u64 flip_protnone_guard(u64 oldval, u64 val, u64 mask);

	// static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
	// {
	// 	pteval_t val = arch_pte_val(pte), oldval = val;

	// 	/*
	// 	* Chop off the NX bit (if present), and add the NX portion of
	// 	* the newprot (if present):
	// 	*/
	// 	val &= _PAGE_CHG_MASK;
	// 	val |= check_pgprot(newprot) & ~_PAGE_CHG_MASK;
	// 	val = flip_protnone_guard(oldval, val, PTE_PFN_MASK);
	// 	return __pte(val);
	// }

	// static inline pmd_t pmd_modify(pmd_t pmd, pgprot_t newprot)
	// {
	// 	pmdval_t val = arch_pmde_val(pmd), oldval = val;

	// 	val &= _HPAGE_CHG_MASK;
	// 	val |= check_pgprot(newprot) & ~_HPAGE_CHG_MASK;
	// 	val = flip_protnone_guard(oldval, val, PHYSICAL_PMD_PAGE_MASK);
	// 	return __pmd(val);
	// }

	// /*
	// * mprotect needs to preserve PAT and encryption bits when updating
	// * vm_page_prot
	// */
	// #define pgprot_modify pgprot_modify
	// static inline pgprot_t pgprot_modify(pgprot_t oldprot, pgprot_t newprot)
	// {
	// 	pgprotval_t preservebits = pgprot_val(oldprot) & _PAGE_CHG_MASK;
	// 	pgprotval_t addbits = pgprot_val(newprot) & ~_PAGE_CHG_MASK;
	// 	return __pgprot(preservebits | addbits);
	// }

	// #define pte_pgprot(x) __pgprot(arch_pte_flags(x))
	// #define pmd_pgprot(x) __pgprot(arch_pmde_flags(x))
	// #define pud_pgprot(x) __pgprot(arch_pude_flags(x))
	// #define p4d_pgprot(x) __pgprot(arch_p4de_flags(x))

	// #define canon_pgprot(p) __pgprot(massage_pgprot(p))

	// static inline pgprot_t arch_filter_pgprot(pgprot_t prot)
	// {
	// 	return canon_pgprot(prot);
	// }

	// static inline int is_new_memtype_allowed(u64 paddr, unsigned long size,
	// 					enum page_cache_mode pcm,
	// 					enum page_cache_mode new_pcm)
	// {
	// 	/*
	// 	* PAT type is always WB for untracked ranges, so no need to check.
	// 	*/
	// 	if (x86_platform.is_untracked_pat_range(paddr, paddr + size))
	// 		return 1;

	// 	/*
	// 	* Certain new memtypes are not allowed with certain
	// 	* requested memtype:
	// 	* - request is uncached, return cannot be write-back
	// 	* - request is write-combine, return cannot be write-back
	// 	* - request is write-through, return cannot be write-back
	// 	* - request is write-through, return cannot be write-combine
	// 	*/
	// 	if ((pcm == _PAGE_CACHE_MODE_UC_MINUS &&
	// 		new_pcm == _PAGE_CACHE_MODE_WB) ||
	// 		(pcm == _PAGE_CACHE_MODE_WC &&
	// 		new_pcm == _PAGE_CACHE_MODE_WB) ||
	// 		(pcm == _PAGE_CACHE_MODE_WT &&
	// 		new_pcm == _PAGE_CACHE_MODE_WB) ||
	// 		(pcm == _PAGE_CACHE_MODE_WT &&
	// 		new_pcm == _PAGE_CACHE_MODE_WC)) {
	// 		return 0;
	// 	}

	// 	return 1;
	// }

	// pmd_t *populate_extra_pmd(unsigned long vaddr);
	// pte_t *populate_extra_pte(unsigned long vaddr);

	// static inline pgd_t pti_set_user_pgtbl(pgd_t *pgdp, pgd_t pgd) {
	// 	return pgd;
	// }


	// #ifdef CONFIG_ARCH_HAS_PTE_DEVMAP
	// static inline int pte_devmap(pte_t a)
	// {
	// 	return (arch_pte_flags(a) & _PAGE_DEVMAP) == _PAGE_DEVMAP;
	// }
	// #endif

	// #define pte_accessible pte_accessible
	// static inline bool pte_accessible(mm_s *mm, pte_t a)
	// {
	// 	if (arch_pte_flags(a) & _PAGE_PRESENT)
	// 		return true;

	// 	if ((arch_pte_flags(a) & _PAGE_PROTNONE) &&
	// 			atomic_read(&mm->tlb_flush_pending))
	// 		return true;

	// 	return false;
	// }


	// #ifdef CONFIG_NUMA_BALANCING
	// /*
	// * These work without NUMA balancing but the kernel does not care. See the
	// * comment in include/linux/pgtable.h
	// */
	// static inline int pte_protnone(pte_t pte)
	// {
	// 	return (arch_pte_flags(pte) & (_PAGE_PROTNONE | _PAGE_PRESENT))
	// 		== _PAGE_PROTNONE;
	// }

	// static inline int pmd_protnone(pmd_t pmd)
	// {
	// 	return (arch_pmde_flags(pmd) & (_PAGE_PROTNONE | _PAGE_PRESENT))
	// 		== _PAGE_PROTNONE;
	// }
	// #endif /* CONFIG_NUMA_BALANCING */


	/*
	 * Conversion functions: convert a page and protection to a page entry,
	 * and a page entry and page directory to the page they refer to.
	 *
	 * (Currently stuck as a macro because of indirect forward reference
	 * to linux/mm.h:page_to_nid())
	 */
	#define mk_pte(page, pgprot)   pfn_pte(page_to_pfn(page), (pgprot))


	// #define pud_leaf	pud_large
	// static inline int pud_large(pud_t pud)
	// {
	// 	return (arch_pude_val(pud) & (_PAGE_PSE | _PAGE_PRESENT)) ==
	// 		(_PAGE_PSE | _PAGE_PRESENT);
	// }

	// extern int direct_gbpages;
	// extern void memblock_find_dma_reserve(void);
	// void __init poking_init(void);
	// unsigned long init_memory_mapping(unsigned long start,
	// 				unsigned long end, pgprot_t prot);

	// extern pgd_t trampoline_pgd_entry;

	// /* local pte updates need not use xchg for locking */
	// static inline pte_t native_local_ptep_get_and_clear(pte_t *ptep)
	// {
	// 	pte_t res = *ptep;

	// 	/* Pure native function needs no input for mm, addr */
	// 	native_pte_clear(NULL, 0, ptep);
	// 	return res;
	// }

	// static inline pmd_t native_local_pmdp_get_and_clear(pmd_t *pmdp)
	// {
	// 	pmd_t res = *pmdp;

	// 	native_pmd_clear(pmdp);
	// 	return res;
	// }

	// static inline pud_t native_local_pudp_get_and_clear(pud_t *pudp)
	// {
	// 	pud_t res = *pudp;

	// 	native_pud_clear(pudp);
	// 	return res;
	// }


	// static inline void set_pmd_at(mm_s *mm, unsigned long addr,
	// 				pmd_t *pmdp, pmd_t pmd)
	// {
	// 	page_table_check_pmd_set(mm, addr, pmdp, pmd);
	// 	set_pmd(pmdp, pmd);
	// }

	// static inline void set_pud_at(mm_s *mm, unsigned long addr,
	// 				pud_t *pudp, pud_t pud)
	// {
	// 	page_table_check_pud_set(mm, addr, pudp, pud);
	// 	native_set_pud(pudp, pud);
	// }


	// #define  __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
	// extern int ptep_set_access_flags(vma_s *vma,
	// 				unsigned long address, pte_t *ptep,
	// 				pte_t entry, int dirty);

	// #define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
	// extern int ptep_test_and_clear_young(vma_s *vma,
	// 					unsigned long addr, pte_t *ptep);

	// #define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
	// extern int ptep_clear_flush_young(vma_s *vma,
	// 				unsigned long address, pte_t *ptep);

	// #define __HAVE_ARCH_PTEP_GET_AND_CLEAR
	// static inline pte_t ptep_get_and_clear(mm_s *mm, unsigned long addr,
	// 					pte_t *ptep)
	// {
	// 	pte_t pte = native_ptep_get_and_clear(ptep);
	// 	page_table_check_pte_clear(mm, addr, pte);
	// 	return pte;
	// }

	// #define __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL
	// static inline pte_t ptep_get_and_clear_full(mm_s *mm,
	// 						unsigned long addr, pte_t *ptep,
	// 						int full)
	// {
	// 	pte_t pte;
	// 	if (full) {
	// 		/*
	// 		* Full address destruction in progress; paravirt does not
	// 		* care about updates and native needs no locking
	// 		*/
	// 		pte = native_local_ptep_get_and_clear(ptep);
	// 		page_table_check_pte_clear(mm, addr, pte);
	// 	} else {
	// 		pte = ptep_get_and_clear(mm, addr, ptep);
	// 	}
	// 	return pte;
	// }

	// #define __HAVE_ARCH_PTEP_CLEAR
	// static inline void ptep_clear(mm_s *mm, unsigned long addr,
	// 				pte_t *ptep)
	// {
	// 	if (IS_ENABLED(CONFIG_PAGE_TABLE_CHECK))
	// 		ptep_get_and_clear(mm, addr, ptep);
	// 	else
	// 		pte_clear(mm, addr, ptep);
	// }

	// #define __HAVE_ARCH_PTEP_SET_WRPROTECT


	// #define flush_tlb_fix_spurious_fault(vma, address) do { } while (0)

	// #define mk_pmd(page, pgprot)   pfn_pmde(page_to_pfn(page), (pgprot))

	// #define  __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS
	// extern int pmdp_set_access_flags(vma_s *vma,
	// 				unsigned long address, pmd_t *pmdp,
	// 				pmd_t entry, int dirty);
	// extern int pudp_set_access_flags(vma_s *vma,
	// 				unsigned long address, pud_t *pudp,
	// 				pud_t entry, int dirty);

	// #define __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG
	// extern int pmdp_test_and_clear_young(vma_s *vma,
	// 					unsigned long addr, pmd_t *pmdp);
	// extern int pudp_test_and_clear_young(vma_s *vma,
	// 					unsigned long addr, pud_t *pudp);

	// #define __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH
	// extern int pmdp_clear_flush_young(vma_s *vma,
	// 				unsigned long address, pmd_t *pmdp);


	// #define pmd_write pmd_write
	// static inline int pmd_write(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_RW;
	// }

	// #define __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
	// static inline pmd_t pmdp_huge_get_and_clear(mm_s *mm, unsigned long addr,
	// 					pmd_t *pmdp)
	// {
	// 	pmd_t pmd = native_pmdp_get_and_clear(pmdp);

	// 	page_table_check_pmd_clear(mm, addr, pmd);

	// 	return pmd;
	// }

	// #define __HAVE_ARCH_PUDP_HUGE_GET_AND_CLEAR
	// static inline pud_t pudp_huge_get_and_clear(mm_s *mm,
	// 					unsigned long addr, pud_t *pudp)
	// {
	// 	pud_t pud = native_pudp_get_and_clear(pudp);

	// 	page_table_check_pud_clear(mm, addr, pud);

	// 	return pud;
	// }

	// #define __HAVE_ARCH_PMDP_SET_WRPROTECT
	// static inline void pmdp_set_wrprotect(mm_s *mm,
	// 					unsigned long addr, pmd_t *pmdp)
	// {
	// 	clear_bit(_PAGE_BIT_RW, (unsigned long *)pmdp);
	// }

	// #define pud_write pud_write
	// static inline int pud_write(pud_t pud)
	// {
	// 	return arch_pude_flags(pud) & _PAGE_RW;
	// }

	// #ifndef pmdp_establish
	// #define pmdp_establish pmdp_establish
	// static inline pmd_t pmdp_establish(vma_s *vma,
	// 		unsigned long address, pmd_t *pmdp, pmd_t pmd)
	// {
	// 	page_table_check_pmd_set(vma->vm_mm, address, pmdp, pmd);
	// 	if (IS_ENABLED(CONFIG_SMP)) {
	// 		return xchg(pmdp, pmd);
	// 	} else {
	// 		pmd_t old = *pmdp;
	// 		WRITE_ONCE(*pmdp, pmd);
	// 		return old;
	// 	}
	// }
	// #endif
	// /*
	// * Page table pages are page-aligned.  The lower half of the top
	// * level is used for userspace and the top half for the kernel.
	// *
	// * Returns true for parts of the PGD that map userspace and
	// * false for the parts that map the kernel.
	// */
	// static inline bool pgdp_maps_userspace(void *__ptr)
	// {
	// 	unsigned long ptr = (unsigned long)__ptr;

	// 	return (((ptr & ~PAGE_MASK) / sizeof(pgd_t)) < PGD_KERNEL_START);
	// }

	// #define pgd_leaf	pgd_large
	// static inline int pgd_large(pgd_t pgd) { return 0; }

	// /*
	// * clone_pgd_range(pgd_t *dst, pgd_t *src, int count);
	// *
	// *  dst - pointer to pgd range anywhere on a pgd page
	// *  src - ""
	// *  count - the number of pgds to copy.
	// *
	// * dst and src can be on the same page, but the range must not overlap,
	// * and must not cross a page boundary.
	// */
	// static inline void clone_pgd_range(pgd_t *dst, pgd_t *src, int count)
	// {
	// 	memcpy(dst, src, count * sizeof(pgd_t));
	// }

	// #define PTE_SHIFT ilog2(PTRS_PER_PTE)
	// static inline int page_level_shift(enum pg_level level)
	// {
	// 	return (PAGE_SHIFT - PTE_SHIFT) + level * PTE_SHIFT;
	// }
	// static inline unsigned long page_level_size(enum pg_level level)
	// {
	// 	return 1UL << page_level_shift(level);
	// }
	// static inline unsigned long page_level_mask(enum pg_level level)
	// {
	// 	return ~(page_level_size(level) - 1);
	// }

	// /*
	// * The x86 doesn't have any external MMU info: the kernel page
	// * tables contain all the necessary information.
	// */
	// static inline void update_mmu_cache(vma_s *vma,
	// 		unsigned long addr, pte_t *ptep)
	// {
	// }
	// static inline void update_mmu_cache_pmd(vma_s *vma,
	// 		unsigned long addr, pmd_t *pmd)
	// {
	// }
	// static inline void update_mmu_cache_pud(vma_s *vma,
	// 		unsigned long addr, pud_t *pud)
	// {
	// }

	// #ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
	// static inline pte_t pte_swp_mksoft_dirty(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_SWP_SOFT_DIRTY);
	// }

	// static inline int pte_swp_soft_dirty(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_SWP_SOFT_DIRTY;
	// }

	// static inline pte_t pte_swp_clear_soft_dirty(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_SWP_SOFT_DIRTY);
	// }

	// #ifdef CONFIG_ARCH_ENABLE_THP_MIGRATION
	// static inline pmd_t pmd_swp_mksoft_dirty(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_SWP_SOFT_DIRTY);
	// }

	// static inline int pmd_swp_soft_dirty(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_SWP_SOFT_DIRTY;
	// }

	// static inline pmd_t pmd_swp_clear_soft_dirty(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_SWP_SOFT_DIRTY);
	// }
	// #endif
	// #endif

	// #ifdef CONFIG_HAVE_ARCH_USERFAULTFD_WP
	// static inline pte_t pte_swp_mkuffd_wp(pte_t pte)
	// {
	// 	return pte_set_flags(pte, _PAGE_SWP_UFFD_WP);
	// }

	// static inline int pte_swp_uffd_wp(pte_t pte)
	// {
	// 	return arch_pte_flags(pte) & _PAGE_SWP_UFFD_WP;
	// }

	// static inline pte_t pte_swp_clear_uffd_wp(pte_t pte)
	// {
	// 	return pte_clear_flags(pte, _PAGE_SWP_UFFD_WP);
	// }

	// static inline pmd_t pmd_swp_mkuffd_wp(pmd_t pmd)
	// {
	// 	return pmd_set_flags(pmd, _PAGE_SWP_UFFD_WP);
	// }

	// static inline int pmd_swp_uffd_wp(pmd_t pmd)
	// {
	// 	return arch_pmde_flags(pmd) & _PAGE_SWP_UFFD_WP;
	// }

	// static inline pmd_t pmd_swp_clear_uffd_wp(pmd_t pmd)
	// {
	// 	return pmd_clear_flags(pmd, _PAGE_SWP_UFFD_WP);
	// }
	// #endif /* CONFIG_HAVE_ARCH_USERFAULTFD_WP */

	// static inline u16 pte_flags_pkey(unsigned long arch_pte_flags)
	// {
	// #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	// 	/* ifdef to avoid doing 59-bit shift on 32-bit values */
	// 	return (arch_pte_flags & _PAGE_PKEY_MASK) >> _PAGE_BIT_PKEY_BIT0;
	// #else
	// 	return 0;
	// #endif
	// }

	// static inline bool __pkru_allows_pkey(u16 pkey, bool write)
	// {
	// 	u32 pkru = read_pkru();

	// 	if (!__pkru_allows_read(pkru, pkey))
	// 		return false;
	// 	if (write && !__pkru_allows_write(pkru, pkey))
	// 		return false;

	// 	return true;
	// }

	// /*
	// * 'pteval' can come from a PTE, PMD or PUD.  We only check
	// * _PAGE_PRESENT, _PAGE_USER, and _PAGE_RW in here which are the
	// * same value on all 3 types.
	// */
	// static inline bool __pte_access_permitted(unsigned long pteval, bool write)
	// {
	// 	unsigned long need_pte_bits = _PAGE_PRESENT|_PAGE_USER;

	// 	if (write)
	// 		need_pte_bits |= _PAGE_RW;

	// 	if ((pteval & need_pte_bits) != need_pte_bits)
	// 		return 0;

	// 	return __pkru_allows_pkey(pte_flags_pkey(pteval), write);
	// }

	// #define pte_access_permitted pte_access_permitted
	// static inline bool pte_access_permitted(pte_t pte, bool write)
	// {
	// 	return __pte_access_permitted(arch_pte_val(pte), write);
	// }

	// #define pmd_access_permitted pmd_access_permitted
	// static inline bool pmd_access_permitted(pmd_t pmd, bool write)
	// {
	// 	return __pte_access_permitted(arch_pmde_val(pmd), write);
	// }

	// #define pud_access_permitted pud_access_permitted
	// static inline bool pud_access_permitted(pud_t pud, bool write)
	// {
	// 	return __pte_access_permitted(arch_pude_val(pud), write);
	// }

	// #define __HAVE_ARCH_PFN_MODIFY_ALLOWED 1
	// extern bool pfn_modify_allowed(unsigned long pfn, pgprot_t prot);

	// static inline bool arch_has_pfn_modify_check(void)
	// {
	// 	return boot_cpu_has_bug(X86_BUG_L1TF);
	// }

	// #define arch_faults_on_old_pte arch_faults_on_old_pte
	// static inline bool arch_faults_on_old_pte(void)
	// {
	// 	return false;
	// }

#endif /* _ASM_X86_PGTABLE_H_ */
