#ifndef _ASM_X86_MM_TYPE_DECLARATION_H_
#define _ASM_X86_MM_TYPE_DECLARATION_H_

	struct task_struct;
	typedef struct task_struct task_s;

	#include <linux/kernel/types.h>
	// #include <linux/kernel/sched_types.h>


	/* earlymem arch */
	struct e820_entry;
	typedef struct e820_entry e820_entry_s;
	struct e820_table;
	typedef struct e820_table e820_table_s;

	/* pgtable */
	/*
	 * These are used to make use of C type-checking..
	 */
	typedef ulong pteval_t;
	typedef ulong pmdval_t;
	typedef ulong pudval_t;
	typedef ulong p4dval_t;
	typedef ulong pgdval_t;
	typedef ulong pgprotval_t;
	struct arch_pgd;
	typedef struct arch_pgd arch_pgd_T;
	struct arch_pud;
	typedef struct arch_pud arch_pud_T;
	struct arch_pmd;
	typedef struct arch_pmd arch_pmd_T;
	struct arch_pte;
	typedef struct arch_pte arch_pte_T;
	union pgd_union;
	typedef union pgd_union pgd_t;
	union p4d_union;
	typedef union p4d_union p4d_t;
	union pud_union;
	typedef union pud_union pud_t;
	union pmd_union;
	typedef union pmd_union pmd_t;
	union pte_union;
	typedef union pte_union pte_t;


	#include <linux/kernel/mm_type_declaration.h>

#endif /* _ASM_X86_MM_TYPE_DECLARATION_H_ */