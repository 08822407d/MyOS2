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
#include <linux/init/init.h>
#include <asm/cache.h>
#include <asm/percpu.h>
#include <insns/x86msr_const.h>
#include <mm/mm_const_arch.h>

#include <processor/processor_const_arch.h>

#include <uefi/multiboot2.h>
#define GRUB_MULTIBOOT_ARCHITECTURE_I386 MULTIBOOT_ARCHITECTURE_I386

#define SYM_DATA_START_PAGE_ALIGNED(name)			\
	SYM_START(name, SYM_L_GLOBAL, .balign PAGE_SIZE)

.section .unpaged.text, "ax"
.code64

SYM_CODE_START_NOALIGN(startup_64)
	cli
	jmp		code_64_start
SYM_CODE_END(startup_64)

/*==============================================================================================*
 *										multiboot2 header                           			*
 *==============================================================================================*/
	/* Align 8 Bytes boundary. */
	.align	8
	/* Multiboot header. */
multiboot_header:
	/* magic */
	.long	MULTIBOOT2_HEADER_MAGIC
	/* ISA: i386 */
	.long	GRUB_MULTIBOOT_ARCHITECTURE_I386
	/* Header length. */
	.long	multiboot_header_end - multiboot_header
	/* checksum */
	.long	-(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + \
			(multiboot_header_end - multiboot_header))
//===========================================
#include "grub2_head_tags.S"
//===========================================
	.align	8
multiboot_header_tag_end_begin:
	.short	MULTIBOOT_HEADER_TAG_END
	.short	0
	.long	8
multiboot_header_tag_end_end:
//===========================================
multiboot_header_end:

/*==============================================================================================*
 *											long mode code                              		*
 *==============================================================================================*/
.balign 8
SYM_CODE_START(code_64_start)
	movq	%rax,		mbi_magic(%rip)
	movq	%rbx,		mbi_base(%rip)
	movq	$0,			%rdx
	movq	%rdx,		%ds
	movq	%rdx,		%es
	movq	%rdx,		%fs
	movq	%rdx,		%gs

	movq	$tmp_kstack_top,	%rdx
	movq	%rdx,		%rsp
	// set page map
	callq	creat_higher_half_pagemap

	movq	%cr4,		%rax
	bts		$16,		%rax	// enable fsgsbase instuctions
	movq	%rax,		%cr4

	/* Check if nx is implemented */
	movl	$0x80000001,%eax
	cpuid
	movl	%edx,		%edi
enable_SCE:
	/* Setup EFER (Extended Feature Enable Register) */
	movl	$MSR_EFER,	%ecx
	rdmsr
	/*
	 * Preserve current value of EFER for comparison and to skip
	 * EFER writes if no change was made (for TDX guest)
	 */
	movl    %eax,		%edx
	btsl	$_EFER_SCE,	%eax	/* Enable System Call */
	/* Avoid writing EFER if no change was made (for TDX guest) */
	cmpl	%edx,		%eax
	je		1f
	xor		%edx,		%edx
	wrmsr				/* Make changes effective */
1:
	/* Setup cr0 */
	movl	$CR0_STATE,	%eax
	/* Make changes effective */
	movq	%rax,		%cr0

skip_set_pde:
	// reset cs:rip at same time jump to higher half
	movq	%cs,		%rax
	pushq	%rax
	movabsq	$higher_half_entry,	%rax
	pushq	%rax
	lretq

hang:
	jmp		hang

// if boot from UEFI, all physical page all ready mapped
// to lower half, here duplicate lower 256 PML4 entries to
// higher 256 ones(to creat tmp higher half page mapping)
creat_higher_half_pagemap:
	cld
	movq	%cr3,		%rsi
	movq	%cr3,		%rdi
	addq	$2048,		%rdi
	movq	$256,		%rcx
	rep
	movsq
	movq	%cr3,		%rax
	movq	%rax,		%cr3
	ret


/*==============================================================================================*
 *									higher half text section							 		*
 *==============================================================================================*/
.section .init.text, "ax"
.code64
.balign 8
SYM_CODE_START(higher_half_entry)
	movabsq	$init_stack,	%rbx
	addq	$THREAD_SIZE,	%rbx
	movq	%rbx,		%rsp

	pushq	$0
	popfq
	pushq	$0

	// here init single core ia32-e environment
	callq	x86_64_start_kernel

	jmp		.


/*==============================================================================================*
 *											data section								 		*
 *==============================================================================================*/
.section .unpaged.data
//=======temp stack
.align 8
tmp_kstack_start:
	.fill	128,8,0
SYM_CODE_START(tmp_kstack_top)

.balign 8
SYM_CODE_START(mbi_magic)
	.quad	0x0
SYM_CODE_START(mbi_base)
	.quad	0x0


	__PAGE_ALIGNED_BSS
SYM_DATA_START_PAGE_ALIGNED(empty_zero_page)
	.skip PAGE_SIZE
SYM_DATA_END(empty_zero_page)
EXPORT_SYMBOL(empty_zero_page)