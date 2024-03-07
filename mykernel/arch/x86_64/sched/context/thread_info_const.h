/* SPDX-License-Identifier: GPL-2.0 */
/* thread_info.h: low-level thread information
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds and Dave Miller
 */
#ifndef _ASM_X86_THREAD_INFO_CONST_H_
#define _ASM_X86_THREAD_INFO_CONST_H_

	/*
	 * TOP_OF_KERNEL_STACK_PADDING is a number of unused bytes that we
	 * reserve at the top of the kernel stack.  We do it because of a nasty
	 * 32-bit corner case.  On x86_32, the hardware stack frame is
	 * variable-length.  Except for vm86 mode, pt_regs_s assumes a
	 * maximum-length frame.  If we enter from CPL 0, the top 8 bytes of
	 * pt_regs don't actually exist.  Ordinarily this doesn't matter, but it
	 * does in at least one case:
	 *
	 * If we take an NMI early enough in SYSENTER, then we can end up with
	 * pt_regs that extends above sp0.  On the way out, in the espfix code,
	 * we can read the saved SS value, but that value will be above sp0.
	 * Without this offset, that can result in a page fault.  (We are
	 * careful that, in this case, the value we read doesn't matter.)
	 *
	 * In vm86 mode, the hardware frame is much longer still, so add 16
	 * bytes to make room for the real-mode segments.
	 *
	 * x86_64 has a fixed-length stack frame.
	 */
	#define TOP_OF_KERNEL_STACK_PADDING	0

	/*
	 * thread information flags
	 * - these are process state flags that various assembly files
	 *   may need to access
	 */
	#define TIF_NOTIFY_RESUME			1	/* callback before returning to user */
	#define TIF_SIGPENDING				2	/* signal pending */
	#define TIF_NEED_RESCHED			3	/* rescheduling necessary */
	#define TIF_SINGLESTEP				4	/* reenable singlestep on user return*/
	#define TIF_SSBD					5	/* Speculative store bypass disable */
	#define TIF_SPEC_IB					9	/* Indirect branch speculation mitigation */
	#define TIF_SPEC_L1D_FLUSH			10	/* Flush L1D on mm switches (processes) */
	#define TIF_USER_RETURN_NOTIFY		11	/* notify kernel of userspace return */
	#define TIF_UPROBE					12	/* breakpointed or singlestepping */
	#define TIF_PATCH_PENDING			13	/* pending live patching update */
	#define TIF_NEED_FPU_LOAD			14	/* load FPU on return to userspace */
	#define TIF_NOCPUID					15	/* CPUID is not accessible in userland */
	#define TIF_NOTSC					16	/* TSC is not accessible in userland */
	#define TIF_NOTIFY_SIGNAL			17	/* signal notifications exist */
	#define TIF_MEMDIE					20	/* is terminating due to OOM killer */
	#define TIF_POLLING_NRFLAG			21	/* idle is polling for TIF_NEED_RESCHED */
	#define TIF_IO_BITMAP				22	/* uses I/O bitmap */
	#define TIF_SPEC_FORCE_UPDATE		23	/* Force speculation MSR update in context switch */
	#define TIF_FORCED_TF				24	/* true if TF in eflags artificially */
	#define TIF_BLOCKSTEP				25	/* set when we want DEBUGCTLMSR_BTF */
	#define TIF_LAZY_MMU_UPDATES		27	/* task is updating the mmu lazily */
	#define TIF_ADDR32					29	/* 32-bit address space on 64 bits */

	#define _TIF_NOTIFY_RESUME			(1 << TIF_NOTIFY_RESUME)
	#define _TIF_SIGPENDING				(1 << TIF_SIGPENDING)
	#define _TIF_NEED_RESCHED			(1 << TIF_NEED_RESCHED)
	#define _TIF_SINGLESTEP				(1 << TIF_SINGLESTEP)
	#define _TIF_SSBD					(1 << TIF_SSBD)
	#define _TIF_SPEC_IB				(1 << TIF_SPEC_IB)
	#define _TIF_SPEC_L1D_FLUSH			(1 << TIF_SPEC_L1D_FLUSH)
	#define _TIF_USER_RETURN_NOTIFY		(1 << TIF_USER_RETURN_NOTIFY)
	#define _TIF_UPROBE					(1 << TIF_UPROBE)
	#define _TIF_PATCH_PENDING			(1 << TIF_PATCH_PENDING)
	#define _TIF_NEED_FPU_LOAD			(1 << TIF_NEED_FPU_LOAD)
	#define _TIF_NOCPUID				(1 << TIF_NOCPUID)
	#define _TIF_NOTSC					(1 << TIF_NOTSC)
	#define _TIF_NOTIFY_SIGNAL			(1 << TIF_NOTIFY_SIGNAL)
	#define _TIF_POLLING_NRFLAG			(1 << TIF_POLLING_NRFLAG)
	#define _TIF_IO_BITMAP				(1 << TIF_IO_BITMAP)
	#define _TIF_SPEC_FORCE_UPDATE		(1 << TIF_SPEC_FORCE_UPDATE)
	#define _TIF_FORCED_TF				(1 << TIF_FORCED_TF)
	#define _TIF_BLOCKSTEP				(1 << TIF_BLOCKSTEP)
	#define _TIF_LAZY_MMU_UPDATES		(1 << TIF_LAZY_MMU_UPDATES)
	#define _TIF_ADDR32					(1 << TIF_ADDR32)

#endif /* _ASM_X86_THREAD_INFO_CONST_H_ */
