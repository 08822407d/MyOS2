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
#include <generated/asm-offsets.h>
#include <asm/cache.h>
#include <asm/unistd.h>
#include <asm/hw_irq.h>
#include <asm/percpu.h>
#include <asm/asm.h>
#include <asm/trapnr.h>
#include <linux/kernel/err.h>
#include <processor/desc_const_arch.h>

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
	movq	%rsp, TASK_threadsp(%rdi)
	movq	TASK_threadsp(%rsi), %rsp
	// movq	TASK_threadsp(%rip),	%rbx
	// movq	%rsp,		(%rbx, %rdi)
	// movq	(%rbx, %rsi),			%rsp

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
	call	schedule_tail		/* rdi: 'prev' task parameter */

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


#include <asm/ptrace.h>
/*==============================================================================================*/
/*										interrupt entry								 			*/
/*==============================================================================================*/
/*
 * called by the exception interrupt vectors. If the exception does not push
 * errorcode, we assume that the vector handler pushed 0 instead. Next pushed
 * thing is the vector number. From this point on we can continue as if every
 * exception pushes an error code
 */
#define SAVEALL_SIZE		0x70

.macro PUSH_REGS
	pushq		%rdi
	pushq		%rsi
	pushq		%rdx
	pushq		%rcx
	pushq		%rax
	pushq		%r8
	pushq		%r9
	pushq		%r10
	pushq		%r11
	pushq		%rbx
	pushq		%rbp
	pushq		%r12
	pushq		%r13
	pushq		%r14
	pushq		%r15	
.endm

.macro POP_REGS
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
	popq		%rbp
	popq		%rbx
	popq		%r11
	popq		%r10
	popq		%r9
	popq		%r8
	popq		%rax
	popq		%rcx
	popq		%rdx
	popq		%rsi
	popq		%rdi
.endm

/*==============================================================================================*/
/*									syscenter and sysexit entry						 			*/
/*==============================================================================================*/
// user RSP -> R11 by hand
// user RIP -> R10 by hand
// kernel CS = IA32_SYSENTER_CS
// kernel SS = IA32_SYSENTER_CS + 8
// kernel RIP = IA32_SYSENTER_EIP
// kernel RSP = IA32_SYSENTER_ESP
SYM_CODE_START(sysenter_entp)
	cli
	subq		$0x38,		%rsp
	PUSH_REGS
	/* exchange user rip and rsp to corresponding position in struct pt_reg */
	xchgq		%r11,		R11(%rsp)
	xchgq		%r10,		R10(%rsp)
	xchgq		%r11,		RSP(%rsp)
	xchgq		%r10,		RIP(%rsp)
	xchgq		%r11,		R11(%rsp)
	xchgq		%r10,		R10(%rsp)

	sti

	/* IRQs are off. */
	movq		%rsp,		%rdi
	/* Sign extend the lower 32bit as syscall numbers are treated as int */
	movslq		%eax,		%rsi
	call		do_syscall_64		/* returns with IRQs disabled */

	// leaq		sys_call_table(%rip),	%rbx
	// callq		*(%rbx,%rax,8)
	// movq		%rax,		(SAVEALL_SIZE - 0x20)(%rsp)	// save return value to stack
// RDX -> user RSP
// RCX -> user RIP
// user CS = IA32_SYSENTER_CS + 32
// user SS = IA32_SYSENTER_CS + 40
SYM_CODE_START(sysexit_entp)
	cli
	/* exchange struct pt_reg rip and rsp to corresponding user register */
	xchgq		%r11,		RSP(%rsp)
	xchgq		%r10,		RIP(%rsp)
	xchgq		%r11,		R11(%rsp)
	xchgq		%r10,		R10(%rsp)
	xchgq		%r11,		RSP(%rsp)
	xchgq		%r10,		RIP(%rsp)
	POP_REGS
	addq		$0x38,		%rsp
	xchgq		%r10,		%rdx
	xchgq		%r11,		%rcx
	sti

	REX.W
	sysexitq

// /*==============================================================================================*/
// /*									syscall and sysret entry						 			*/
// /*==============================================================================================*/
// // user Rflags -> R11
// // user RIP -> RCX
// // kernel CS = IA32_STAR[47:32]
// // kernel SS = IA32_STAR[47:32] + 8
// // kernel RIP = IA32_LSTAR
// // kernel Rflags &= IA32_FMASK
// // !!! user RSP should be store by hand !!!
// // !!! kernel RSP should be load by hand !!!
// SYM_CODE_START(enter_syscall)
// 	cli
// 	swapgs
// 	movabsq		$cpustack_off,	%r10
// 	movq		(%r10),		%r10
// 	movq		%gs:(%r10),	%r10
// 	xchgq		%rsp,		%r10
// 	pushq		%r10				// save user rsp
// 	pushq		%r11				// save user rflags
// 	subq		$0x08,		%rsp
// 	pushq		%rcx
// 	/////////////////////////////////
// 	subq		$0x10,		%rsp	// skip vec_nr and err_code
// 	/////////////////////////////////

// 	movq		%rsp,		%rdi

// // R11 -> user Rflags
// // RCX -> user RIP
// // user CS = IA32_STAR[63:48] + 16
// // user SS = IA32_STAR[63:48] + 8
// // !!! user RSP should be restore by hand !!!
// // !!! kernel RSP should be store by hand !!!
// SYM_CODE_START(ret_from_syscall)

// 	/////////////////////////////////
// 	addq		$0x10,		%rsp	// skip err-code and vec-nr
// 	/////////////////////////////////
// 	// now user rip already in rcx, rflags in r11
// 	popq		%rcx
// 	addq		$0x10,		%rsp	// skip cs
// 	popq		%rsp
// 	swapgs
// 	sti

// 	sysretq


#include <asm/idtentry.h>
SYM_CODE_START(sa_entintr_retp)
	PUSH_REGS
	cld
	movq		%rsp,		%rdi

	callq 		excep_hwint_context

	POP_REGS
	addq		$0x10,		%rsp	// skip err-code and vec-nr
	sti
	iretq;


	.align IDT_ALIGN
SYM_CODE_START(exc_entries_start)
	vector=0
	.rept NR_EXCEPTION_VECTORS
0 :
	cli
	/* Push 0 for Error Number placeholder */
	.if ((EXCEPTION_ERRCODE_MASK >> vector) & 1) == 0
		.byte	0x6a, 0	# Dummy error code, to make stack frame uniform
	.endif
	.byte	0x6a, vector
	jmp	sa_entintr_retp
	/* Ensure that the above is IDT_ALIGN bytes max */
	.fill 0b + IDT_ALIGN - ., 1, 0x90
	vector = vector+1
	.endr
SYM_CODE_END(exc_entries_start)
SYM_CODE_START_NOALIGN(exc_entries_end)
/*
 * ASM code to emit the common vector entry stubs where each stub is
 * packed into IDT_ALIGN bytes.
 *
 * Note, that the 'pushq imm8' is emitted via '.byte 0x6a, vector' because
 * GCC treats the local vector variable as unsigned int and would expand
 * all vectors above 0x7F to a 5 byte push. The original code did an
 * adjustment of the vector number to be in the signed byte range to avoid
 * this. While clever it's mindboggling counterintuitive and requires the
 * odd conversion back to a real vector number in the C entry points. Using
 * .byte achieves the same thing and the only fixup needed in the C entry
 * point is to mask off the bits above bit 7 because the push is sign
 * extending.
 */
	.align IDT_ALIGN
SYM_CODE_START(irq_entries_start)
	vector=FIRST_EXTERNAL_VECTOR
	.rept NR_VECTORS - FIRST_EXTERNAL_VECTOR
0 :
	cli
	/* External hardware interrupts has no error code */
	.byte	0x6a, 0x0
	.byte	0x6a, vector
	jmp	sa_entintr_retp
	/* Ensure that the above is IDT_ALIGN bytes max */
	.fill 0b + IDT_ALIGN - ., 1, 0x90
	vector = vector+1
	.endr
SYM_CODE_END(irq_entries_start)
SYM_CODE_START_NOALIGN(irq_entries_end)

// 	.align IDT_ALIGN
// SYM_CODE_START(spurious_entries_start)
// 	vector=FIRST_SYSTEM_VECTOR
// 	.rept NR_SYSTEM_VECTORS
// 0 :
// 	.byte	0x6a, vector
// 	jmp	sa_entintr_retp
// 	/* Ensure that the above is IDT_ALIGN bytes max */
// 	.fill 0b + IDT_ALIGN - ., 1, 0x90
// 	vector = vector+1
// 	.endr
// SYM_CODE_END(spurious_entries_start)
// SYM_CODE_START_NOALIGN(spurious_entries_end)
