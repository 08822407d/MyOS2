/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _ASM_X86_PTRACE_ABI_H
#define _ASM_X86_PTRACE_ABI_H

	#if defined(__ASSEMBLY__) || defined(__FRAME_OFFSETS)

	#	define REG_SIZE		8
	/*
	 * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
	 * unless syscall needs a complete, fully filled "struct pt_regs".
	 */
	#	define FRAME_BASE	0
	#	define R15			FRAME_BASE
	#	define R14			(R15 + REG_SIZE)
	#	define R13			(R14 + REG_SIZE)
	#	define R12			(R13 + REG_SIZE)
	#	define RBP			(R12 + REG_SIZE)
	#	define RBX			(RBP + REG_SIZE)
	/* These regs are callee-clobbered. Always saved on kernel entry. */
	#	define R11			(RBX + REG_SIZE)
	#	define R10			(R11 + REG_SIZE)
	#	define R9			(R10 + REG_SIZE)
	#	define R8			(R9  + REG_SIZE)
	#	define RAX			(R8  + REG_SIZE)
	#	define RCX			(RAX + REG_SIZE)
	#	define RDX			(RCX + REG_SIZE)
	#	define RSI			(RDX + REG_SIZE)
	#	define RDI			(RSI + REG_SIZE)
	// MYOS2 add this into struct pt_reg
	#	define IRQ_NR		(RDI + REG_SIZE)
	/*
	 * On syscall entry, this is syscall#. On CPU exception, this is error code.
	 * On hw interrupt, it's IRQ number:
	 */
	#	define ORIG_RAX 	(IRQ_NR + REG_SIZE)
	/* Return frame for iretq */
	#	define RIP			(ORIG_RAX + REG_SIZE)
	#	define CS			(RIP + REG_SIZE)
	#	define EFLAGS		(CS + REG_SIZE)
	#	define RSP			(EFLAGS + REG_SIZE)
	#	define SS			(RSP + REG_SIZE)
	#	define FRAME_END	SS
	#	define FRAME_SIZE	(FRAME_END - FRAME_BASE)
	#endif /* __ASSEMBLY__ */

	/* top of stack page */

	// /* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
	// #define PTRACE_GETREGS				12
	// #define PTRACE_SETREGS				13
	// #define PTRACE_GETFPREGS			14
	// #define PTRACE_SETFPREGS			15
	// #define PTRACE_GETFPXREGS			18
	// #define PTRACE_SETFPXREGS			19

	// #define PTRACE_OLDSETOPTIONS		21

	// /* only useful for access 32bit programs / kernels */
	// #define PTRACE_GET_THREAD_AREA		25
	// #define PTRACE_SET_THREAD_AREA		26

	// #define PTRACE_ARCH_PRCTL			30

	// #define PTRACE_SYSEMU				31
	// #define PTRACE_SYSEMU_SINGLESTEP	32

	// #define PTRACE_SINGLEBLOCK			33	/* resume execution until next branch */

	#ifndef __ASSEMBLY__
	#	include <linux/kernel/types.h>
	#endif

#endif /* _ASM_X86_PTRACE_ABI_H */
