#ifndef _AMD64_ACONST_H_
#define _AMD64_ACONST_H_ 1

#include <linux/kernel/math.h>
#include <klib/const.h>

	#define USERADDR_LIMIT		0x00007FFFFFFFFFFF

	/* Table sizes. */
	#define IDT_SIZE			256	// the table is set to it's maximal size

	/* GDT layout (SYSENTER/SYSEXIT compliant) */ // partial copied from minix3
	#define NULL_DESC_INDEX		0
	#define KERN_CS_INDEX       1
	#define KERN_SS_INDEX       2
	#define USER_SS_INDEX       3
	#define USER_CS_INDEX       4
	#define USER_CS_INDEX_DUP   5
	#define USER_SS_INDEX_DUP   6
	#define TSS_INDEX_FIRST     7
	#define TSS_INDEX(cpu_idx)  (TSS_INDEX_FIRST + (cpu_idx * 2))	/* per cpu kernel tss */
	#define GDT_SIZE(lcpu_nr)	(TSS_INDEX(lcpu_nr) + 1)/* GDT descriptor */

	#define SEG_SELECTOR(i)		((i) * 8)
	#define KERN_CS_SELECTOR	SEG_SELECTOR(KERN_CS_INDEX)
	#define KERN_SS_SELECTOR	SEG_SELECTOR(KERN_SS_INDEX)
	#define USER_SS_SELECTOR	(SEG_SELECTOR(USER_SS_INDEX) | USER_PRIVILEGE)
	#define USER_CS_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define USER_CS_DUP_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define USER_SS_DUP_SELECTOR	(SEG_SELECTOR(USER_SS_INDEX) | USER_PRIVILEGE)
	#define TSS_SELECTOR(cpu)	SEG_SELECTOR(TSS_INDEX(cpu))

	/* Privileges. */
	#define KERN_PRIVILEGE		0	/* kernel and interrupt handlers */
	#define USER_PRIVILEGE		3	/* servers and user processes */

	/* AMD64 page */
	#define	PGENT_SIZE			CONST_4K
	#define	PGENT_NR			(PGENT_SIZE / 8)
	#define GET_PGENT_IDX(n)	((n) & (PGENT_NR - 1))
	#define GETF_PGENT(val)		((PGENT_NR - 1) & val)
	#define SHIFT_PML4E			39
	#define SHIFT_PDPTE			30
	#define SHIFT_PDE			21
	#define SHIFT_PTE			12
	#define ARCH_PG_PRESENT		(1 << 0ULL)
	#define ARCH_PG_RW			(1 << 1ULL)
	#define ARCH_PG_USER		(1 << 2ULL)
	#define ARCH_PG_PWT			(1 << 3ULL)
	#define ARCH_PG_PCD			(1 << 4ULL)
	#define ARCH_PG_ACCESSED	(1 << 5ULL)
	#define ARCH_PG_DIRTY		(1 << 6ULL)
	#define ARCH_PG_PAT			(1 << 7ULL)
	#define ARCH_PG_GLOBAL		(1 << 8ULL)

	#define CANONICAL_ADDR(x)	((x) & 0xFFFFFFFFFFFF)
	#define ARCH_PGS_ADDR(x)	round_down((CANONICAL_ADDR(x)), CONST_4K)
	#define ARCH_PGS_ATTR(x)	(x & 0x1FF)

	// process and task consts
	#define TASK_KSTACK_SIZE	(32 * CONST_1K)

#endif /* _AMD64_ACONST_H_ */