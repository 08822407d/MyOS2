/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_TYPES_H_
#define _ASM_X86_PGTABLE_TYPES_H_

	#include "../mm_type_declaration_arch.h"


	typedef struct arch_p4d {
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
	} __attribute__((packed)) arch_p4d_T;

	typedef struct arch_pud {
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
	} __attribute__((packed)) arch_pud_T;

	typedef struct arch_pmd {
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
	} __attribute__((packed)) arch_pmd_T;

	typedef struct arch_pte {
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
	} __attribute__((packed)) arch_pte_T;

	typedef struct pgprot {
		pgprotval_t pgprot;
	} pgprot_t;

	typedef union p4d_union {
		p4dval_t	val;
		arch_p4d_T	defs;
	} p4d_t;

	typedef union pud_union {
		pudval_t	val;
		arch_pud_T	defs;
	} pud_t;

	typedef union pmd_union {
		pmdval_t	val;
		arch_pmd_T	defs;
	} pmd_t;

	typedef union pte_union {
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

#endif /* _ASM_X86_PGTABLE_TYPES_H_ */