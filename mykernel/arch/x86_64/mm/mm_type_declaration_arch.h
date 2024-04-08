#ifndef _ASM_X86_MM_TYPE_DECLARATION_H_
#define _ASM_X86_MM_TYPE_DECLARATION_H_

	struct task_struct;
	typedef struct task_struct task_s;

	#include <linux/kernel/types.h>
	#include <linux/kernel/mm_type_declaration.h>


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

#endif /* _ASM_X86_MM_TYPE_DECLARATION_H_ */