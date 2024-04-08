#ifndef _LINUX_PGTABLE_MACRO_H_
#define _LINUX_PGTABLE_MACRO_H_

	#define p4d_alloc(mm, pgd, addr)	p4d_ent_offset(pgd, addr)

	/*
	 * When walking page tables, get the address of the next boundary,
	 * or the end address of the range if that comes earlier.  Although no
	 * vma end wraps to 0, rounded up __boundary may wrap to 0 throughout.
	 */
	#define next_pgd_addr_end(addr, end) ({							\
				unsigned long __boundary = 							\
					((addr) + PGDIR_SIZE) & PGDIR_MASK;				\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	#define pgd_addr_end next_pgd_addr_end

	#define next_p4d_addr_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + P4D_SIZE) & P4D_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	#define p4d_addr_end next_p4d_addr_end

	#define next_pud_addr_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PUD_SIZE) & PUD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	#define pud_addr_end next_pud_addr_end

	#define next_pmd_addr_end(addr, end) ({							\
				unsigned long __boundary =							\
					((addr) + PMD_SIZE) & PMD_MASK;					\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
	#define pmd_addr_end next_pmd_addr_end

#endif /* _LINUX_PGTABLE_MACRO_H_ */