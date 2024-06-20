/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _ASM_X86_PTRACE_ABI_H
#define _ASM_X86_PTRACE_ABI_H

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
