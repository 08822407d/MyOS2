/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  linux/arch/x86_64/entry.S
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2000, 2001, 2002  Andi Kleen SuSE Labs
 *  Copyright (C) 2000  Pavel Machek <pavel@suse.cz>
 *
 * entry.S contains the system-call and fault low-level handling routines.
 *
 * Some of this is documented in Documentation/x86/entry_64.rst
 *
 * A note on terminology:
 * - iret frame:	Architecture defined interrupt frame from SS to RIP
 *			at the top of the kernel process stack.
 *
 * Some macro usage:
 * - SYM_FUNC_START/END:Define functions in the symbol table.
 * - idtentry:		Define exception entry points.
 */
#include <linux/kernel/linkage.h>
// #include <asm/segment.h>
#include <asm/cache.h>
// #include <asm/errno.h>
// #include <asm/asm-offsets.h>
#include <asm/msr.h>
#include <asm/unistd.h>
// #include <asm/thread_info.h>
// #include <asm/hw_irq.h>
#include <asm/page_types.h>
// #include <asm/irqflags.h>
// #include <asm/paravirt.h>
// #include <asm/percpu.h>
// #include <asm/asm.h>
// #include <asm/smap.h>
#include <asm/pgtable_types.h>
// #include <asm/export.h>
#include <asm/frame.h>
// #include <asm/trapnr.h>
// #include <asm/nospec-branch.h>
#include <asm/fsgsbase.h>
#include <linux/kernel/err.h>

// #include "calling.h"

.code64
.section .entry.text, "ax"



/*
 * %rdi: prev task
 * %rsi: next task
 */
.pushsection .text, "ax"
SYM_FUNC_START(__switch_to_asm)
	/*
	 * Save callee-saved registers
	 * This must match the order in inactive_task_frame
	 */
	pushq	%rbp
	pushq	%rbx
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* switch stack */
	// movq	%rsp, TASK_threadsp(%rdi)
	// movq	TASK_threadsp(%rsi), %rsp
	movq	TASK_threadsp(%rip),	%rbx
	movq	%rsp,		(%rbx, %rdi)
	movq	(%rbx, %rsi),			%rsp

// #ifdef CONFIG_STACKPROTECTOR
// 	movq	TASK_stack_canary(%rsi), %rbx
// 	movq	%rbx, PER_CPU_VAR(fixed_percpu_data) + stack_canary_offset
// #endif

// #ifdef CONFIG_RETPOLINE
// 	/*
// 	 * When switching from a shallower to a deeper call stack
// 	 * the RSB may either underflow or use entries populated
// 	 * with userspace addresses. On CPUs where those concerns
// 	 * exist, overwrite the RSB with entries which capture
// 	 * speculative execution to prevent attack.
// 	 */
// 	FILL_RETURN_BUFFER %r12, RSB_CLEAR_LOOPS, X86_FEATURE_RSB_CTXSW
// #endif

	/* restore callee-saved registers */
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	popq	%rbp

	jmp	__switch_to
SYM_FUNC_END(__switch_to_asm)
.popsection

/*
 * A newly forked process directly context switches into this address.
 *
 * rax: prev task we switched from
 * rbx: kernel thread func (NULL for user thread)
 * r12: kernel thread arg
 */
.pushsection .text, "ax"
SYM_CODE_START(ret_from_fork)
	// UNWIND_HINT_EMPTY
	movq	%rax,		%rdi
	// call	schedule_tail		/* rdi: 'prev' task parameter */

	testq	%rbx,		%rbx	/* from kernel_thread? */
	jnz		1f					/* kernel threads are uncommon */

2:
// 	UNWIND_HINT_REGS
// 	movq	%rsp, %rdi
// 	call	syscall_exit_to_user_mode	/* returns with IRQs disabled */
// 	jmp	swapgs_restore_regs_and_return_to_usermode
	jmp		sysexit_entp

1:
	/* kernel thread */
	// UNWIND_HINT_EMPTY
	movq	%r12,		%rdi
	// CALL_NOSPEC rbx
	callq	*%rbx
	// /*
	//  * A kernel thread is allowed to return here after successfully
	//  * calling kernel_execve().  Exit to userspace to complete the execve()
	//  * syscall.
	//  */
	// movq	$0, RAX(%rsp)
	// jmp	2b
	jmp		.
SYM_CODE_END(ret_from_fork)
.popsection