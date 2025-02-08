// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_DESC_CONST_H_
#define _ASM_X86_DESC_CONST_H_

#include <asm/mm_const.h>

	/* Boot IDT definitions */
	#define	BOOT_IDT_ENTRIES		32

	/* Access rights as returned by LAR */
	#define AR_TYPE_RODATA			(0 * (1 << 9))
	#define AR_TYPE_RWDATA			(1 * (1 << 9))
	#define AR_TYPE_RODATA_EXPDOWN	(2 * (1 << 9))
	#define AR_TYPE_RWDATA_EXPDOWN	(3 * (1 << 9))
	#define AR_TYPE_XOCODE			(4 * (1 << 9))
	#define AR_TYPE_XRCODE			(5 * (1 << 9))
	#define AR_TYPE_XOCODE_CONF		(6 * (1 << 9))
	#define AR_TYPE_XRCODE_CONF		(7 * (1 << 9))
	#define AR_TYPE_MASK			(7 * (1 << 9))

	#define AR_DPL0					(0 * (1 << 13))
	#define AR_DPL3					(3 * (1 << 13))
	#define AR_DPL_MASK				(3 * (1 << 13))

	#define AR_A					(1 << 8)   /* "Accessed" */
	#define AR_S					(1 << 12)  /* If clear, "System" segment */
	#define AR_P					(1 << 15)  /* "Present" */
	#define AR_AVL					(1 << 20)  /* "AVaiLable" (no HW effect) */
	#define AR_L					(1 << 21)  /* "Long mode" for code segments */
	#define AR_DB					(1 << 22)  /* D/B, effect depends on type */
	#define AR_G					(1 << 23)  /* "Granularity" (limit in pages) */


	// /* Simple and small GDT entries for booting only: */
	// #define GDT_ENTRY_BOOT_CS				2
	// #define GDT_ENTRY_BOOT_DS				3
	// #define GDT_ENTRY_BOOT_TSS				4
	// #define __BOOT_CS						(GDT_ENTRY_BOOT_CS*8)
	// #define __BOOT_DS						(GDT_ENTRY_BOOT_DS*8)
	// #define __BOOT_TSS						(GDT_ENTRY_BOOT_TSS*8)

	/*
	 * Bottom two bits of selector give the ring
	 * privilege level
	 */
	#define SEGMENT_RPL_MASK		0x3

	/*
	 * When running on Xen PV, the actual privilege level of the kernel is 1,
	 * not 0. Testing the Requested Privilege Level in a segment selector to
	 * determine whether the context is user mode or kernel mode with
	 * SEGMENT_RPL_MASK is wrong because the PV kernel's privilege level
	 * matches the 0x3 mask.
	 *
	 * Testing with USER_SEGMENT_RPL_MASK is valid for both native and Xen PV
	 * kernels because privilege level 2 is never used.
	 */
	#define USER_SEGMENT_RPL_MASK	0x2

	/* Kernel mode is privilege level 0: */
	#define KRNL_RPL				0x0
	/* User mode is privilege level 3: */
	#define USER_RPL				0x3

	// /* Bit 2 is Table Indicator (TI): selects between LDT or GDT */
	// #define SEGMENT_TI_MASK					0x4
	// /* LDT segment has TI set ... */
	// #define SEGMENT_LDT						0x4
	// /* ... GDT has it cleared */
	// #define SEGMENT_GDT						0x0

	#define GDT_ENTRY_INVALID_SEG	0

	// #include <asm/cache.h>

	#if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
	#  define GDT_ENTRY_KERNEL_CS				1
	#  define GDT_ENTRY_KERNEL_DS				2
	#  define GDT_ENTRY_DEFAULT_USER_DS			3
	#  define GDT_ENTRY_DEFAULT_USER_CS			4
	#  define GDT_ENTRY_DEFAULT_USER_CS_DUP		5
	#  define GDT_ENTRY_DEFAULT_USER_DS_DUP		6
	#else
	# define GDT_ENTRY_KERNEL32_CS				1
	# define GDT_ENTRY_KERNEL_CS				2
	# define GDT_ENTRY_KERNEL_DS				3
	/*
	 * We cannot use the same code segment descriptor for user and kernel mode,
	 * not even in long flat mode, because of different DPL.
	 *
	 * GDT layout to get 64-bit SYSCALL/SYSRET support right. SYSRET hardcodes
	 * selectors:
	 *
	 *   if returning to 32-bit userspace: cs = STAR.SYSRET_CS,
	 *   if returning to 64-bit userspace: cs = STAR.SYSRET_CS+16,
	 *
	 * ss = STAR.SYSRET_CS+8 (in either case)
	 *
	 * thus USER_DS should be between 32-bit and 64-bit code selectors:
	 */
	#  define GDT_ENTRY_DEFAULT_USER32_CS		4
	#  define GDT_ENTRY_DEFAULT_USER_DS			5
	#  define GDT_ENTRY_DEFAULT_USER_CS			6
	#endif

	/* Needs two entries */
	#define GDT_ENTRY_TSS			8
	/* Needs two entries */
	#define GDT_ENTRY_LDT			10

	#define GDT_ENTRY_TLS_MIN		12
	#define GDT_ENTRY_TLS_MAX		14

	#define GDT_ENTRY_CPUNODE		15

	/*
	 * Number of entries in the GDT table:
	 */
	#define GDT_ENTRIES				16

	#if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
	#  define __KERNEL_CS		(GDT_ENTRY_KERNEL_CS * 8)
	#  define __KERNEL_DS		(GDT_ENTRY_KERNEL_DS * 8)
	#  define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS * 8 + 3)
	#  define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS * 8 + 3)
	#  define __USER_CS_DUP		(GDT_ENTRY_DEFAULT_USER_CS_DUP * 8 + 3)
	#  define __USER_DS_DUP		(GDT_ENTRY_DEFAULT_USER_DS_DUP * 8 + 3)
	#else
	/*
	 * Segment selector values corresponding to the above entries:
	 *
	 * Note, selectors also need to have a correct RPL,
	 * expressed with the +3 value for user-space selectors:
	 */
	#  define __KERNEL32_CS		(GDT_ENTRY_KERNEL32_CS * 8)
	#  define __KERNEL_CS		(GDT_ENTRY_KERNEL_CS * 8)
	#  define __KERNEL_DS		(GDT_ENTRY_KERNEL_DS * 8)
	#  define __USER32_CS		(GDT_ENTRY_DEFAULT_USER32_CS * 8 + 3)
	#  define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS * 8 + 3)
	#  define __USER32_DS		__USER_DS
	#  define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS * 8 + 3)
	#  define __CPUNODE_SEG		(GDT_ENTRY_CPUNODE * 8 + 3)
	#endif
	#define __TSS_SEG			(GDT_ENTRY_TSS * 8)

	#define IDT_ENTRIES				256
	#define NUM_EXCEPTION_VECTORS	32

	/* Bitmask of exception vectors which push an error code on the stack: */
	#define EXCEPTION_ERRCODE_MASK		0x20027d00
	// /*
	//  * According to Intel Manual Volume 3 - June 2023
	//  * CHAPTER 6 : NTERRUPT AND EXCEPTION HANDLING - 6.15
	//  */
	// #define EXCEPTION_ERRCODE_MASK	(	\
	// 			(1 << X86_TRAP_DF)	|	\
	// 			(1 << X86_TRAP_TS)	|	\
	// 			(1 << X86_TRAP_NP)	|	\
	// 			(1 << X86_TRAP_SS)	|	\
	// 			(1 << X86_TRAP_GP)	|	\
	// 			(1 << X86_TRAP_PF)	|	\
	// 			(1 << X86_TRAP_AC)	|	\
	// 			(1 << X86_TRAP_CP)	|	\
	// 		)

	#define GDT_SIZE				(GDT_ENTRIES * 8)
	#define GDT_ENTRY_TLS_ENTRIES	3
	#define TLS_SIZE				(GDT_ENTRY_TLS_ENTRIES* 8)

#endif /* _ASM_X86_DESC_CONST_H_ */
