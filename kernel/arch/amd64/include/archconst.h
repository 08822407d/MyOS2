#ifndef _AMD64_ACONST_H
#define _AMD64_ACONST_H 1

	#define CONFIG_MAX_CPUS 8

	/* Constants for protected mode. */

	/* Table sizes. */
	#define IDT_SIZE 256	/* the table is set to it's maximal size */

	/* GDT layout (SYSENTER/SYSEXIT compliant) */
	#define NULL_DESC_INDEX		0
	#define KERN_CS_INDEX       1
	#define KERN_DS_INDEX       2
	#define USER_CS_INDEX       3
	#define USER_DS_INDEX       4
	#define TSS_INDEX_FIRST     5
	#define TSS_INDEX(cpu)      (TSS_INDEX_FIRST + (cpu * 2))	/* per cpu kernel tss */
	#define GDT_SIZE            (TSS_INDEX(CONFIG_MAX_CPUS) + 1)/* LDT descriptor */

	#define SEGDESC_SIZE	8
	#define GATEDESC_SIZE	16

	/* Privileges. */
	#define KERN_PRIVILEGE       0	/* kernel and interrupt handlers */
	#define USER_PRIVILEGE       3	/* servers and user processes */

#endif /* _AMD64_ACONST_H */