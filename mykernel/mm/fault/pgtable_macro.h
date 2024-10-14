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


	#define p4dp_get		p4d_entp_get_ent
	#define pudp_get		pud_entp_get_ent
	#define pmdp_get		pmd_entp_get_ent
	#define ptep_get		pgtbl_entp_get_ent

	#define p4d_index		ent_index_in_p4d
	#define pud_index		ent_index_in_pud
	#define pmd_index		ent_index_in_pmd
	#define pte_index		ent_index_in_pgtbl

	#define pgd_offset		pgd_ent_ptr_in_mm
	#define p4d_offset		p4d_entp_from_vaddr_and_pgd_entp
	#define pud_offset		pud_entp_from_vaddr_and_p4d_entp
	#define pmd_offset		pmd_entp_from_vaddr_and_pud_entp
	#define pte_offset		pgtbl_entp_from_vaddr_and_pmd_entp

	#define p4d_populate	fill_pud_to_p4d_ent
	#define pud_populate	fill_pmd_to_pud_ent
	#define pmd_populate	fill_pgtbl_to_pmd_ent

	#define p4d_populate_safe	fill_pud_to_p4d_ent_safe
	#define pud_populate_safe	fill_pmd_to_pud_ent_safe
	#define pmd_populate_safe	fill_pgtbl_to_pmd_ent_safe

	#define pgd_index				ent_index_in_p4d
	#define pgdp_get				p4d_entp_get_ent
	#define pgd_none_or_clear_bad	p4d_ent_none_or_clear_bad


	#define pte_alloc				pgtble_alloc
	#define pte_alloc_map			pgtble_alloc_map
	#define pte_alloc_map_lock		pgtble_alloc_map_lock
	#define pte_offset_kernel		pgtbl_entp_from_vaddr_and_pmd_entp
	#define ptep_get_lockless		pgtbl_entp_get_ent
	#define pmdp_get_lockless		pmd_entp_get_ent

#endif /* _LINUX_PGTABLE_MACRO_H_ */