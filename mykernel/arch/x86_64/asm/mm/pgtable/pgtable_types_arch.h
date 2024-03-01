/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_TYPES_H_
#define _ASM_X86_PGTABLE_TYPES_H_

	#ifndef __ASSEMBLY__
	#	include <linux/kernel/types.h>

		/*
		 * These are used to make use of C type-checking..
		 */
		typedef unsigned long pteval_t;
		typedef unsigned long pmdval_t;
		typedef unsigned long pudval_t;
		typedef unsigned long p4dval_t;
		typedef unsigned long pgdval_t;
		typedef unsigned long pgprotval_t;

		typedef struct __attribute__((packed)) {
			unsigned long
				P		: 1,
				RW		: 1,
				US		: 1,
				PWT		: 1,
				PCD		: 1,
				A		: 1,
						: 6,
				PHYADDR	: 36,
						: 15,
				XD		: 1;
		} arch_pgd_T;
	
		typedef struct __attribute__((packed)) {
			unsigned long
				P		: 1,
				RW		: 1,
				US		: 1,
				PWT		: 1,
				PCD		: 1,
				A		: 1,
						: 1,
				PS		: 1,
						: 1,
						: 3,
				PHYADDR	: 36,
						: 15,
				XD		: 1;
		} arch_pud_T;

		typedef struct __attribute__((packed)) {
			unsigned long
				P		: 1,
				RW		: 1,
				US		: 1,
				PWT		: 1,
				PCD		: 1,
				A		: 1,
						: 1,
				PS		: 1,
						: 1,
						: 3,
				PHYADDR	: 36,
						: 15,
				XD		: 1;
		} arch_pmd_T;

		typedef struct __attribute__((packed)) {
			unsigned long
				P		: 1,
				RW		: 1,
				US		: 1,
				PWT		: 1,
				PCD		: 1,
				A		: 1,
				D		: 1,
				PAT		: 1,
				G		: 1,
						: 3,
				PHYADDR	: 36,
						: 15,
				XD		: 1;
		} arch_pte_T;

		// extern unsigned int pgdir_shift;
		// extern unsigned int ptrs_per_p4d;

        typedef struct pgprot { pgprotval_t pgprot; } pgprot_t;

        typedef union {
            pgdval_t	val;
            arch_pgd_T	defs;
        } pgd_t;


        typedef union {
            pgd_t		pgd;
            arch_pgd_T	defs;
        } p4d_t;

		typedef union {
			pudval_t	val;
			arch_pud_T	defs;
		} pud_t;

		typedef union {
			pmdval_t	val;
			arch_pmd_T	defs;
		} pmd_t;

		typedef union {
			pteval_t	val;
			arch_pte_T	defs;
		} pte_t;


		enum pg_level {
			PG_LEVEL_NONE,
			PG_LEVEL_4K,
			PG_LEVEL_2M,
			PG_LEVEL_1G,
			PG_LEVEL_512G,
			PG_LEVEL_NUM
		};

	#endif /* !__ASSEMBLY__ */

#endif /* _ASM_X86_PGTABLE_TYPES_H_ */