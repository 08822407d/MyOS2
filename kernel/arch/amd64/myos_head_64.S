#include <linux/kernel/linkage.h>

#include <uefi/multiboot2.h>
#define GRUB_MULTIBOOT_ARCHITECTURE_I386 MULTIBOOT_ARCHITECTURE_I386

#include "../arch/amd64/include/archconst.h"

.text
.section .unpaged.text
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
	#include "grub2_boot_data.S"
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
	movq    $tmp_kstack_top,	%rax
	movq    %rax,		%rsp

//=======load GDTR
	lgdt	TMP_GDT_PTR(%rip)
//=======load dummy IDTR
	lidt	TMP_IDT_PTR(%rip)
//=======load cr3
	movq	boot_from_grub2,	%rax
	cmpq	$0x01,		%rax
	je		skip_set_pde
// if not boot from grub, need to set pages and cr4[16]

	callq	creat_higher_half_pagemap

	movq	%cr4,		%rax
	bts		$16,		%rax	// enable fsgsbase instuctions
	movq	%rax,		%cr4

skip_set_pde:
	// reset cs:rip at same time jump to higher half
	movabsq	$higher_half_entry,	%rax
	pushq   $0x08
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
	movabsq	$task0_PCB,	%rbx
	addq	$TASK_KSTACK_SIZE,	%rbx
	movq    $0x10,		%rax
	movq    %rax,		%ss
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
.data
.section .unpaged.data
//=======init page
.align 8

.org	0x0000
__PML4E:
.globl	__PML4E
	.quad	__PDPTE + 3
	.fill	255,8,0
	.quad	__PDPTE + 3
	.fill	255,8,0

.org	0x1000
__PDPTE:
	.quad	__PDE + 3
	.fill	511,8,0

.org	0x2000
__PDE:
	.fill	512,8,0

//=======temp GDT_table
TMP_GDT_TBL:
	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	08*/
	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	10*/
GDT_END:

TMP_GDT_PTR:
GDT_LIMIT:	.word	GDT_END - TMP_GDT_TBL - 1
GDT_BASE:	.quad	TMP_GDT_TBL
//=======temp IDT_ptr
TMP_IDT_PTR:
	.word	0
	.word	0,0

.balign 4
multiboot2_info_base:
	.quad	0x0

boot_from_grub2:
	.quad	0x0

//=======temp stack
.align 8
tmp_kstack_start:
	.fill  128,8,0
SYM_CODE_START(tmp_kstack_top)