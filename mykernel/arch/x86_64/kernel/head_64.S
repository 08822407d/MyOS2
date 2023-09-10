/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  linux/arch/x86/kernel/head_64.S -- start in 32bit and switch to 64bit
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2000 Pavel Machek <pavel@suse.cz>
 *  Copyright (C) 2000 Karsten Keil <kkeil@suse.de>
 *  Copyright (C) 2001,2002 Andi Kleen <ak@suse.de>
 *  Copyright (C) 2005 Eric Biederman <ebiederm@xmission.com>
 */


#include <linux/kernel/linkage.h>
// #include <linux/threads.h>
#include <linux/init/init.h>
// #include <linux/pgtable.h>
#include <asm/segment.h>
#include <asm/page.h>
#include <asm/msr.h>
#include <asm/cache.h>
#include <asm/processor-flags.h>
#include <asm/percpu.h>
// #include <asm/nops.h>
// #include "../entry/calling.h"
// #include <asm/export.h>
// #include <asm/nospec-branch.h>
// #include <asm/fixmap.h>


	.text
	__HEAD
	.code64

	__INIT
SYM_CODE_START(early_idt_handler_array)
	i = 0
	.rept NR_EXCEPTION_VECTORS
	.if ((EXCEPTION_ERRCODE_MASK >> i) & 1) == 0
		pushq	$0	# Dummy error code, to make stack frame uniform
	.endif
	pushq	$i		# 72(%rsp) Vector number
	jmp		early_idt_handler_common
	// UNWIND_HINT_IRET_REGS
	i = i + 1
	.fill	early_idt_handler_array + i*EARLY_IDT_HANDLER_SIZE - ., 1, 0x90
	.endr
SYM_CODE_END(early_idt_handler_array)

SYM_CODE_START_LOCAL(early_idt_handler_common)
	// UNWIND_HINT_IRET_REGS offset=16
	/*
	 * The stack is the hardware frame, an error code or zero, and the
	 * vector number.
	 */
	cld

	// incl early_recursion_flag(%rip)

	/* The vector number is currently in the pt_regs->di slot. */
	pushq	%rsi					/* pt_regs->si */
	movq	8(%rsp),	%rsi		/* RSI = vector number */
	movq	%rdi,		8(%rsp)		/* pt_regs->di = RDI */
	pushq	%rdx					/* pt_regs->dx */
	pushq	%rcx					/* pt_regs->cx */
	pushq	%rax					/* pt_regs->ax */
	pushq	%r8						/* pt_regs->r8 */
	pushq	%r9						/* pt_regs->r9 */
	pushq	%r10					/* pt_regs->r10 */
	pushq	%r11					/* pt_regs->r11 */
	pushq	%rbx					/* pt_regs->bx */
	pushq	%rbp					/* pt_regs->bp */
	pushq	%r12					/* pt_regs->r12 */
	pushq	%r13					/* pt_regs->r13 */
	pushq	%r14					/* pt_regs->r14 */
	pushq	%r15					/* pt_regs->r15 */
	// UNWIND_HINT_REGS

	movq	%rsp,		%rdi		/* RDI = pt_regs; RSI is already trapnr */
	// call do_early_exception

	// decl early_recursion_flag(%rip)
	// jmp restore_regs_and_return_to_kernel
SYM_CODE_END(early_idt_handler_common)