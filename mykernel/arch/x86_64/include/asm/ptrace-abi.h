/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _ASM_X86_PTRACE_ABI_H
#define _ASM_X86_PTRACE_ABI_H

	#if defined(__ASSEMBLY__) || defined(__FRAME_OFFSETS)
	/*
	 * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
	 * unless syscall needs a complete, fully filled "struct pt_regs".
	 */
	#	define R15			0
	#	define R14			8
	#	define R13			16
	#	define R12			24
	#	define RBP			32
	#	define RBX			40
	/* These regs are callee-clobbered. Always saved on kernel entry. */
	#	define R11			48
	#	define R10			56
	#	define R9			64
	#	define R8			72
	#	define RAX			80
	#	define RCX			88
	#	define RDX			96
	#	define RSI			104
	#	define RDI			112
	/*
	 * On syscall entry, this is syscall#. On CPU exception, this is error code.
	 * On hw interrupt, it's IRQ number:
	 */
	#	define ORIG_RAX 	120
	/* Return frame for iretq */
	#	define RIP			128
	#	define CS			136
	#	define EFLAGS		144
	#	define RSP			152
	#	define SS			160
	#endif /* __ASSEMBLY__ */

	/* top of stack page */
	#define FRAME_SIZE 168

	/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
	#define PTRACE_GETREGS				12
	#define PTRACE_SETREGS				13
	#define PTRACE_GETFPREGS			14
	#define PTRACE_SETFPREGS			15
	#define PTRACE_GETFPXREGS			18
	#define PTRACE_SETFPXREGS			19

	#define PTRACE_OLDSETOPTIONS		21

	/* only useful for access 32bit programs / kernels */
	#define PTRACE_GET_THREAD_AREA		25
	#define PTRACE_SET_THREAD_AREA		26

	#define PTRACE_ARCH_PRCTL			30

	#define PTRACE_SYSEMU				31
	#define PTRACE_SYSEMU_SINGLESTEP	32

	#define PTRACE_SINGLEBLOCK			33	/* resume execution until next branch */

	#ifndef __ASSEMBLY__
	#	include <linux/kernel/types.h>
	#endif

#endif /* _ASM_X86_PTRACE_ABI_H */
