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

	#define SEG_SELECTOR(i)		((i) * 8)
	#define KERN_CS_SELECTOR	SEG_SELECTOR(KERN_CS_INDEX)
	#define KERN_DS_SELECTOR	SEG_SELECTOR(KERN_DS_INDEX)
	#define USER_CS_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define USER_DS_SELECTOR	(SEG_SELECTOR(USER_DS_INDEX) | USER_PRIVILEGE)
	#define TSS_SELECTOR(cpu)	SEG_SELECTOR(TSS_INDEX(cpu))

	#define SEGDESC_SIZE		0x08
	#define GATEDESC_SIZE		0x10

	/* Privileges. */
	#define KERN_PRIVILEGE		0	/* kernel and interrupt handlers */
	#define USER_PRIVILEGE		3	/* servers and user processes */

	#define	PGENT_SIZE			0x1000
	#define	PGENT_NR			(PGENT_SIZE / 8)

#endif /* _AMD64_ACONST_H */