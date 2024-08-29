#ifndef _LINUX_PGTABLE_MACRO_H_
#define _LINUX_PGTABLE_MACRO_H_

	#define p4d_alloc(mm, pgd, addr)	p4d_ent_ptr(pgd, addr)

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
	// #define pgd_addr_end pgd_ent_bound_end

	/*
	 * Return the bound's end address of the p4d entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define p4d_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + P4D_SIZE) & P4D_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	// #define p4d_addr_end p4d_ent_bound_end

	/*
	 * Return the bound's end address of the pud entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define pud_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PUD_SIZE) & PUD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	// #define pud_addr_end pud_ent_bound_end

	/*
	 * Return the bound's end address of the pmd entry which
	 * @addr located in, if the @ret lower than @end, return the @end
	 */
	#define pmd_ent_bound_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PMD_SIZE) & PMD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	// #define pmd_addr_end pmd_ent_bound_end

#endif /* _LINUX_PGTABLE_MACRO_H_ */