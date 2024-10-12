#ifndef _LINUX_PGTABLE_MACRO_H_
#define _LINUX_PGTABLE_MACRO_H_


	#define KERNEL_PGD_BOUNDARY		pgd_index(PAGE_OFFSET)
	#define KERNEL_PGD_PTRS			(PTRS_PER_PGD - KERNEL_PGD_BOUNDARY)


	#define pte_unmap_unlock(pte, ptl)	do {	\
				spin_unlock(ptl);				\
				pte_unmap(pte);					\
			} while (0)

	/*
	 * When walking page tables, get the address of the next boundary,
	 * or the end address of the range if that comes earlier.  Although no
	 * vma end wraps to 0, rounded up __boundary may wrap to 0 throughout.
	 */

	/*
	 * Return the bound's end address of the pgd entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define pgd_ent_bound_end(addr, end) ({							\
				unsigned long __boundary = 							\
					((addr) + PGDIR_SIZE) & PGDIR_MASK;				\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})

	/*
	 * Return the bound's end address of the p4d entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define p4d_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + P4D_SIZE) & P4D_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})

	/*
	 * Return the bound's end address of the pud entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define pud_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PUD_SIZE) & PUD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})

	/*
	 * Return the bound's end address of the pmd entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define pmd_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PMD_SIZE) & PMD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})

	#define pgd_addr_end	pgd_ent_bound_end
	#define p4d_addr_end	p4d_ent_bound_end
	#define pud_addr_end	pud_ent_bound_end
	#define pmd_addr_end	pmd_ent_bound_end


	#define pgd_offset		pgd_ent_ptr_in_mm
	#define p4d_offset		p4de_ptr_in_pgd
	#define pud_offset		pude_ptr_in_p4d
	#define pmd_offset		pmde_ptr_in_pud
	#define pte_offset		pte_ptr_in_pmd

	#define pgd_index				p4de_index_in_pgd
	#define pgdp_get				p4dp_get_p4de
	#define pgd_none_or_clear_bad	p4de_none_or_clear_bad


	#define pte_offset_kernel		pte_ptr_in_pmd
	#define ptep_get_lockless		ptep_get_pte
	#define pmdp_get_lockless		pmdp_get_pmde

#endif /* _LINUX_PGTABLE_MACRO_H_ */