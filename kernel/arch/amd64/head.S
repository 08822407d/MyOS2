#include "arch/amd64/asm.h"

#include "../../include/const.h"

#define ASM_FILE 1
#include "include/multiboot2.h"
#define GRUB_MULTIBOOT_ARCHITECTURE_I386 MULTIBOOT_ARCHITECTURE_I386

.text

.code64
.balign	8
ENTRY(start)
	cli
	jmp     code_64_start

/*===========================================================================*
 *								multiboot2 header                            *
 *===========================================================================*/
	/* Align 64 bits boundary. */
	.align  8
	/* Multiboot header. */
multiboot_header:
	/* magic */
	.long   MULTIBOOT2_HEADER_MAGIC
	/* ISA: i386 */
	.long   GRUB_MULTIBOOT_ARCHITECTURE_I386
	/* Header length. */
	.long   multiboot_header_end - multiboot_header
	/* checksum */
	.long   -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
//===========================================
	.align  8
address_tag_start:
	.short  MULTIBOOT_HEADER_TAG_ADDRESS
	.short  MULTIBOOT_HEADER_TAG_OPTIONAL
	.long   address_tag_end - address_tag_start
	/* header_addr */
	.long   multiboot_header
	/* load_addr */
	.long   _k_phy_start
	/* load_end_addr */
	.long   _k_phy_edata
	/* bss end addr */
	.long   _k_phy_ebss
address_tag_end:
//===========================================
	.align  8
entry_address_tag_start:
	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	entry_address_tag_end - entry_address_tag_start
	.long	multiboot_entry
entry_address_tag_end:
// use grub2 derectly boot into long mode failed
// can't find reason why step into fisrt instruction of
// my code entry will cause cpu reset
//===========================================
// 	.align	8
// efi_boot_service_tag_start:
// 	.short	MULTIBOOT_HEADER_TAG_EFI_BS
// 	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
// 	.long	efi_boot_service_tag_end - efi_boot_service_tag_start
// efi_boot_service_tag_end:
//===========================================
// 	.align	8
// efi_amd64_entry_address_tag_start:
// 	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64
// 	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
// 	.long	efi_amd64_entry_address_tag_end - efi_amd64_entry_address_tag_start
// 	.long	code_64_start
// efi_amd64_entry_address_tag_end:
//===========================================
	.align	8
framebuffer_tag_start:  
	.short	MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	framebuffer_tag_end - framebuffer_tag_start
	.long	1920
	.long	1080
	.long	32
framebuffer_tag_end:
//===========================================
	.align	8
multiboot_header_tag_end_begin:
	.short	MULTIBOOT_HEADER_TAG_END
	.short	0
	.long	8
multiboot_header_tag_end_end:
//===========================================
multiboot_header_end:

/*===========================================================================*
 *								grub2 boot entry                             *
 *===========================================================================*/
.code32
.balign 8
multiboot_entry:
	cli
	movl	$0,		%esi
//	store boot info
	movl	%ebx,	multiboot2_info_base(%esi)
//	start initialize
	// open PAE
	call	set_temp_PDE_32
	movl	%cr4,		%eax
	bts		$5,			%eax
	bts		$16,		%eax	// enable fsgsbase instuctions
	movl	%eax,		%cr4

	movl	$0x01,		boot_from_grub2(%esi)
set_tmp_page:
	// set page table
	movl	$__PML4E,	%eax
	movl	%eax,		%cr3
	// enable long mode
	movl	$0xC0000080,%ecx
	rdmsr
	bts		$8,			%eax
	wrmsr
	// enable PE & pagin
	movl	%cr0,		%eax
	bts		$0,			%eax
	bts		$31,		%eax
	movl	%eax,		%cr0
go_to_64_code:
	lgdt	GDT_POINTER(%esi)
	jmp		$0x08,		$code_64_start

// 32bit fuction
set_temp_PDE_32:
	movl	$0x83,	%eax
	movl	$__PDE,	%edi
	movl	$0x200,	%ecx
l1:
	movl	%eax,	(%edi)
	addl	$0x200000,	%eax
	addl	$8,		%edi
	loop	l1
	retl

/*===========================================================================*
 *								long mode code                               *
 *===========================================================================*/
.code64
.balign 8
code_64_start:
	movq    $stack_top,	%rax
	movq    %rax,		%rsp
//=======load GDTR
	lgdt	GDT_POINTER(%rip)
//=======load dummy IDTR
	lidt	IDT_POINTER(%rip)
//=======load cr3
	movq	boot_from_grub2,	%rax
	cmpq	$0x01,		%rax
	je		skip_set_pde
	// if not boot from grub, need to set pages and cr4[16]
	callq	set_temp_PDE_64
	movq	$0,	boot_from_grub2

	movq	%cr4,		%rax
	bts		$16,		%rax
	movq	%rax,		%cr4

	movq	$__PML4E,	%rax
	wrgsbase			%rax

skip_set_pde:
	movq	$__PML4E,	%rax
	movq	%rax,		%cr3
	movq    $temp_reload_cs, %rax
	pushq   $0x08
	pushq   %rax
	lretq

temp_reload_cs:
	movq    $stack_top,	%rbx
	movq    $0x10,		%rax
	movq    %rax,		%ss
	movq    %rbx,		%rsp

	pushq   $0
	popfq
	pushq   $0

	pushq   $ret_from_preinit
	movq	multiboot2_info_base(%rip),	%rdi
	movabsq $pre_init,	%rax
	pushq   $0x08
	pushq   %rax
	lretq
ret_from_preinit:
// reset stack to the stack in proc0_PCB(in higher half memory)
	movabsq	$proc0_PCB,	%rbx
	addq	$PROC_KSTACK_SIZE	, %rbx
	movq    $0x10,		%rax
	movq    %rax,		%ss
	movq    %rbx,		%rsp

	movq	$0,			%rdi	// BSP has index 0
	pushq   $ret_from_kmain
	movabsq $kmain,		%rax
	pushq   $0x08
	pushq   %rax
	lretq
ret_from_kmain:

hang:
	jmp     hang

set_temp_PDE_64:
	movq	$0x83,	%rax
	movq	$__PDE,	%rdi
	movq	$0x200,	%rcx
l2:
	movq	%rax,	(%rdi)
	addq	$0x200000,	%rax
	addq	$8,		%rdi
	loop	l2
	retq


/*===========================================================================*
 *								data section								 *
 *===========================================================================*/
.data
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

//=======GDT_Table
GDT_Table:
	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	08*/
	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	10*/
GDT_END:

GDT_POINTER:
GDT_LIMIT:	.word	GDT_END - GDT_Table - 1
GDT_BASE:	.quad	GDT_Table
//=======IDT_Table
IDT_Table:
	.fill  32,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT:	.word	IDT_END - IDT_Table - 1
IDT_BASE:	.quad	IDT_Table

.balign 4
multiboot2_info_base:
	.quad	0x0

boot_from_grub2:
	.quad	0x0

//=======temp stack
stack_start:
	.fill  128,8,0
stack_top: