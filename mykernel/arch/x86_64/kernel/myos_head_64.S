#include <linux/kernel/linkage.h>
#include <linux/init/init.h>
#include <asm/processor-flags.h>
#include <asm/page.h>
#include <asm/msr.h>

#include <uefi/multiboot2.h>
#define GRUB_MULTIBOOT_ARCHITECTURE_I386 MULTIBOOT_ARCHITECTURE_I386

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
	.long	-(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
//===========================================
	.align	8
address_tag_start:
	.short	MULTIBOOT_HEADER_TAG_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	address_tag_end - address_tag_start
	/* header_addr */
	.long	multiboot_header
	/* load_addr */
	.long	_k_phys_start
	/* load_end_addr */
	.long	_k_phys_edata
	/* bss end addr */
	.long	_k_phys_ebss
address_tag_end:
//===========================================

#ifdef GRUB2_BOOTUP_SUPPORT
	#include "grub2_boot_tags.S"
#endif

//===========================================
	.align	8
multiboot_header_tag_end_begin:
	.short	MULTIBOOT_HEADER_TAG_END
	.short	0
	.long	8
multiboot_header_tag_end_end:
//===========================================
multiboot_header_end:


#ifdef GRUB2_BOOTUP_SUPPORT
	#include "grub2_boot_code.S"
#endif

/*==============================================================================================*
 *											long mode code                              		*
 *==============================================================================================*/
.code64
.balign 8
code_64_start:
	movq	$0,			%rax
	movq	%rax,		%ds
	movq	%rax,		%es
	movq	%rax,		%fs
	movq	%rax,		%gs

	movq    $tmp_kstack_top,	%rax
	movq    %rax,		%rsp
	// set page map
	callq	creat_higher_half_pagemap

	movq	%cr4,		%rax
	bts		$16,		%rax	// enable fsgsbase instuctions
	movq	%rax,		%cr4

	/* Check if nx is implemented */
	movl	$0x80000001,%eax
	cpuid
	movl	%edx,		%edi

	/* Setup EFER (Extended Feature Enable Register) */
	movl	$MSR_EFER,	%ecx
	rdmsr
	btsl	$_EFER_SCE,	%eax	/* Enable System Call */
	/* Setup cr0 */
	movl	$CR0_STATE,	%eax
	/* Make changes effective */
	movq	%rax,		%cr0

skip_set_pde:
	// reset cs:rip at same time jump to higher half
	movq	%cs,		%rax
	pushq	%rax
	movabsq	$higher_half_entry,	%rax
	pushq   %rax
	lretq

hang:
	jmp     hang

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
.text
.code64
.balign 8
SYM_CODE_START(higher_half_entry)
	movabsq	$idletsk,		%rbx
	addq	$THREAD_SIZE,	%rbx
	movq    %rbx,		%rsp

	pushq   $0
	popfq
	pushq   $0

	// here init single core ia32-e environment
	callq	x86_64_start_kernel

	// here is the idle task for all cores
	movq	$0,			%rdi
	callq	idle


/*==============================================================================================*
 *											data section								 		*
 *==============================================================================================*/
.section .unpaged.data
//=======temp stack
.align 8
tmp_kstack_start:
	.fill  128,8,0
SYM_CODE_START(tmp_kstack_top)