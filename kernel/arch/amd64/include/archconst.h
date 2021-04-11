#ifndef _AMD64_ACONST_H
#define _AMD64_ACONST_H 1

#include "../../../include/const.h"
	/* Constants for protected mode. */
	#define CONFIG_MAX_CPUS		8
	#define CONFIG_KSTACK_SIZE	4096
	#define CONFIG_PAGE_SIZE	CONST_2M
	#define CONFIG_PAGE_SHIFT	SHIFT_2M
	#define CONFIG_PAGE_MASKF	MASKF_2M	// align to page low bound
	#define CONFIG_PAGE_ALIGH	ALIGN_2M	// align to page high bound
	#define CONFIG_MAX_PHYMEM	(64 * CONST_1G)
	#define CONFIG_MAX_VIRMEM	(512 * CONST_1G)
	#define PAGE_NR				(CONFIG_MAX_PHYMEM / CONFIG_PAGE_SIZE)
	#define PDPT_NR				((CONFIG_MAX_VIRMEM + (1ULL << SHIFT_PML4E) -1) >> SHIFT_PML4E)



	/* Table sizes. */
	#define IDT_SIZE 256	/* the table is set to it's maximal size */

	/* GDT layout (SYSENTER/SYSEXIT compliant) */
	#define NULL_DESC_INDEX		0
	#define KERN_CS_INDEX       1
	#define KERN_SS_INDEX       2
	#define USER_SS_INDEX       3
	#define USER_CS_INDEX       4
	#define TSS_INDEX_FIRST     5
	#define TSS_INDEX(cpu)      (TSS_INDEX_FIRST + (cpu * 2))	/* per cpu kernel tss */
	#define GDT_SIZE            (TSS_INDEX(CONFIG_MAX_CPUS) + 1)/* LDT descriptor */

	#define SEG_SELECTOR(i)		((i) * 8)
	#define KERN_CS_SELECTOR	SEG_SELECTOR(KERN_CS_INDEX)
	#define KERN_SS_SELECTOR	SEG_SELECTOR(KERN_SS_INDEX)
	#define USER_SS_SELECTOR	(SEG_SELECTOR(USER_SS_INDEX) | USER_PRIVILEGE)
	#define USER_CS_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define TSS_SELECTOR(cpu)	SEG_SELECTOR(TSS_INDEX(cpu))

	#define SEGDESC_SIZE		0x08
	#define GATEDESC_SIZE		0x10

	/* Privileges. */
	#define KERN_PRIVILEGE		0	/* kernel and interrupt handlers */
	#define USER_PRIVILEGE		3	/* servers and user processes */

	/* AMD64 page */
	#define	PGENT_SIZE			CONST_4K
	#define	PGENT_NR			(PGENT_SIZE / 8)
	#define GETF_PGENT(val)		((PGENT_NR - 1) & val)
	#define SHIFT_PML4E			39
	#define SHIFT_PDPTE			30
	#define SHIFT_PDE			21
	#define SHIFT_PTE			12

	/* MSR registers addresses */
	#define IA32_EFER			0xC0000080
	#define	MSR_IA32_EFER_SCE	0x00000001
	#define MSR_IA32_EFER_LME	0x00000100
	#define MSR_IA32_EFER_LMA	0x00000400
	#define MSR_IA32_EFER_NXE	0x00000800

	#define MSR_IA32_STAR		0xC0000081
	#define MSR_IA32_LSTAR		0xC0000082
	#define MSR_IA32_CSTAR		0xC0000083
	#define MSR_IA32_FMASK		0xC0000084


#endif /* _AMD64_ACONST_H */