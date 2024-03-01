/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PGTABLE_H
#define _LINUX_PGTABLE_H

	#include <linux/mm/mm.h>
	#include <linux/debug/bug.h>
	#include <linux/lib/errno.h>


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

	static inline unsigned long
	pte_index(unsigned long address) {
		return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
	}

	static inline unsigned long
	pmd_index(unsigned long address) {
		return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
	}

	static inline unsigned long
	pud_index(unsigned long address) {
		return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
	}

	/* Must be a compile-time constant, so implement it as a macro */
	static inline unsigned long
	pgd_index(unsigned long address) {
		return (address >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1);
	}

	// static inline pte_t
	// *pte_offset(pmd_t *pmd, unsigned long address) {
	// 	return arch_pmd_pgtable(*pmd) + pte_index(address);
	// }
	#define pte_ent_offset(pmdp, address)	\
				(pte_t *)(arch_pmd_pgtable(*pmdp) + pte_index(address))

	#define pte_unmap(pte) ((void)(pte))	/* NOP */

	/* Find an entry in the second-level page table.. */
	// static inline pmd_t
	// *pmd_offset(pud_t *pud, unsigned long address) {
	// 	return arch_pud_pgtable(*pud) + pmd_index(address);
	// }
	#define pmd_ent_offset(pudp, address)	\
				(pmd_t *)(arch_pud_pgtable(*pudp) + pmd_index(address))

	// static inline pud_t
	// *pud_offset(p4d_t *p4d, unsigned long address) {
	// 	return arch_p4d_pgtable(*p4d) + pud_index(address);
	// }
	#define pud_ent_offset(p4dp, address)	\
				(pud_t *)(arch_p4d_pgtable(*p4dp) + pud_index(address))

	// static inline pgd_t
	// *pgd_offset_pgd(pgd_t *pgd, unsigned long address) {
	// 	return (pgd + pgd_index(address));
	// };
	#define pgd_ent_offset(mmp, address)	\
				(pgd_t *)((mmp)->pgd + pgd_index(address))


	/*
	 * When walking page tables, get the address of the next boundary,
	 * or the end address of the range if that comes earlier.  Although no
	 * vma end wraps to 0, rounded up __boundary may wrap to 0 throughout.
	 */

	#define next_pgd_addr_end(addr, end) ({								\
				unsigned long __boundary = 							\
					((addr) + PGDIR_SIZE) & PGDIR_MASK;				\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})

	#ifndef next_p4d_addr_end
	#	define next_p4d_addr_end(addr, end) ({								\
					unsigned long __boundary =							\
						((addr) + P4D_SIZE) & P4D_MASK;					\
					(__boundary - 1 < (end) - 1)? __boundary: (end);	\
				})
	#endif

	#ifndef next_pud_addr_end
	#	define next_pud_addr_end(addr, end) ({								\
					unsigned long __boundary =							\
						((addr) + PUD_SIZE) & PUD_MASK;					\
					(__boundary - 1 < (end) - 1)? __boundary: (end);	\
				})
	#endif

	#ifndef next_pmd_addr_end
	#	define next_pmd_addr_end(addr, end) ({								\
					unsigned long __boundary =							\
						((addr) + PMD_SIZE) & PMD_MASK;					\
					(__boundary - 1 < (end) - 1)? __boundary: (end);	\
				})
	#endif

	static inline int
	pgd_none_or_clear_bad(pgd_t *pgd) {
		if (pgd_none(*pgd))
			return 1;
		if (pgd_bad(*pgd)) {
			pgd_clear(pgd);
			return 1;
		}
		return 0;
	}

	static inline int
	p4d_none_or_clear_bad(p4d_t *p4d) {
		if (arch_p4d_none(*p4d))
			return 1;
		if (arch_p4d_bad(*p4d)) {
			p4d_clear(p4d);
			return 1;
		}
		return 0;
	}

	static inline int
	pud_none_or_clear_bad(pud_t *pud) {
		if (arch_pud_none(*pud))
			return 1;
		if (arch_pud_bad(*pud)) {
			pud_clear(pud);
			return 1;
		}
		return 0;
	}

	static inline int pmd_none_or_clear_bad(pmd_t *pmd) {
		if (arch_pmd_none(*pmd))
			return 1;
		if (arch_pmd_bad(*pmd)) {
			pmd_clear(pmd);
			return 1;
		}
		return 0;
	}

#endif /* _LINUX_PGTABLE_H */
