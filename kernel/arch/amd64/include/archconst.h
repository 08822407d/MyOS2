#ifndef _AMD64_ACONST_H_
#define _AMD64_ACONST_H_ 1

#include "../../../include/const.h"

	/* Constants for protected mode. */
	#define CONFIG_MAX_CPUS		256
	#define CONFIG_CPUSTACK_SIZE	4096
	#define CONFIG_PAGE_SIZE	CONST_2M
	#define CONFIG_PAGE_SHIFT	SHIFT_2M
	#define CONFIG_PAGE_MASKF	MASKF_2M	// align to page low bound
	#define CONFIG_PAGE_ALIGH	ALIGN_2M	// align to page high bound
	#define CONFIG_MAX_PHYMEM	(64 * CONST_1G)
	#define CONFIG_MAX_VIRMEM	(512 * CONST_1G)
	#define PDPTE_NR			(CONFIG_MAX_VIRMEM >> SHIFT_PDPTE)
	#define PDE_NR				(CONFIG_MAX_VIRMEM >> SHIFT_PDE)


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
	#define TSS_INDEX(cpu)      (TSS_INDEX_FIRST + (cpu * 2))	/* per cpu kernel tss */
	#define GDT_SIZE            (TSS_INDEX(CONFIG_MAX_CPUS) + 1)/* LDT descriptor */
	// #define GDT_SIZE            (TSS_INDEX(CONFIG_MAX_CPUS) + 1)/* LDT descriptor */

	#define SEG_SELECTOR(i)		((i) * 8)
	#define KERN_CS_SELECTOR	SEG_SELECTOR(KERN_CS_INDEX)
	#define KERN_SS_SELECTOR	SEG_SELECTOR(KERN_SS_INDEX)
	#define USER_SS_SELECTOR	(SEG_SELECTOR(USER_SS_INDEX) | USER_PRIVILEGE)
	#define USER_CS_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define USER_CS_DUP_SELECTOR	(SEG_SELECTOR(USER_CS_INDEX) | USER_PRIVILEGE)
	#define USER_SS_DUP_SELECTOR	(SEG_SELECTOR(USER_SS_INDEX) | USER_PRIVILEGE)
	#define TSS_SELECTOR(cpu)	SEG_SELECTOR(TSS_INDEX(cpu))

	#define SEGDESC_SIZE		0x08
	#define GATEDESC_SIZE		0x10

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

	#define CANONICAL_ADDR(x)		((x) & 0xFFFFFFFFFFFF)
	#define ARCH_PGS_ADDR(x)		(MASKF_4K(CANONICAL_ADDR(x)))
	#define ARCH_PGS_ATTR(x)		(x & 0x1FF)
	#define ARCH_PGE_IS_LAST(x)		(x | ARCH_PG_PAT)
	#define ARCH_PGE_NOT_LAST(x)	(x & 0x3F)

	/* MSR registers addresses */
	#define IA32_EFER			0xC0000080
	#define	MSR_IA32_EFER_SCE	0x00000001
	#define MSR_IA32_EFER_LME	0x00000100
	#define MSR_IA32_EFER_LMA	0x00000400
	#define MSR_IA32_EFER_NXE	0x00000800

	#define MSR_IA32_SYSENTER_CS	0x174
	#define MSR_IA32_SYSENTER_ESP	0x175
	#define MSR_IA32_SYSENTER_EIP	0x176

	#define MSR_IA32_STAR		0xC0000081
	#define MSR_IA32_LSTAR		0xC0000082
	#define MSR_IA32_CSTAR		0xC0000083
	#define MSR_IA32_FMASK		0xC0000084

	/* i386 flags register */ // copyied from darwin-xnu
	#ifndef EFL_CF
		#define EFL_CF          0x00000001              /* carry */
		#define EFL_PF          0x00000004              /* parity of low 8 bits */
		#define EFL_AF          0x00000010              /* carry out of bit 3 */
		#define EFL_ZF          0x00000040              /* zero */
		#define EFL_SF          0x00000080              /* sign */
		#define EFL_TF          0x00000100              /* trace trap */
		#define EFL_IF          0x00000200              /* interrupt enable */
		#define EFL_DF          0x00000400              /* direction */
		#define EFL_OF          0x00000800              /* overflow */
		#define EFL_IOPL        0x00003000              /* IO privilege level: */
		#define EFL_IOPL_KERNEL 0x00000000              /* kernel */
		#define EFL_IOPL_USER   0x00003000              /* user */
		#define EFL_NT          0x00004000              /* nested task */
		#define EFL_RF          0x00010000              /* resume without tracing */
		#define EFL_VM          0x00020000              /* virtual 8086 mode */
		#define EFL_AC          0x00040000              /* alignment check */
		#define EFL_VIF         0x00080000              /* virtual interrupt flag */
		#define EFL_VIP         0x00100000              /* virtual interrupt pending */
		#define EFL_ID          0x00200000              /* cpuID instruction */
	#endif

#endif /* _AMD64_ACONST_H_ */