#include "arch/amd64/asm.h"
#include "../../include/const.h"

.balign	0x1000
.text

.code16
ENTRY(_APboot_start)
_APboot_base = .
	cli
	wbinvd

	mov		%cs,		%ax
	mov		%ax,		%ds
	mov		%ax,		%es
	mov		%ax,		%fs
	mov		%ax,		%gs
	mov		%ax,		%ss
	movl	$(_APboot_tmp_Stack_top - _APboot_base),	%esp

	//	get base address
	mov		%cs,		%ax
	movzx	%ax,		%esi
	shll	$4,			%esi

	//	load idt gdt
	lgdtl	_AP_tmp_GDTptr - _APboot_base
	lidtl	_AP_tmp_IDTptr - _APboot_base

	//	enable protected mode
	smsw	%ax
	bts		$0,			%ax
	lmsw	%ax

	//	go to 32 code
	// jmp		$0x18, $_AP_code32

.balign 4
.code32
_AP_code32:
#	go to 64 code
	mov		$0x10,		%ax
	mov		%ax,		%ds
	mov		%ax,		%es
	mov		%ax,		%ss
	mov		%ax,		%fs
	mov		%ax,		%gs

#	open PAE
	movl	%cr4,	%eax
	bts		$5,		%eax
	movl	%eax,	%cr4

#	set page table
	movl	$0x90000,	%eax
	movl	%eax,	%cr3

#	enable long mode
	movl	$0xC0000080,	%ecx
	rdmsr

	bts	$8,	%eax
	wrmsr

#	enable PE & paging
	movl	%cr0,	%eax
	bts	$0,	%eax
	bts	$31,	%eax
	movl	%eax,	%cr0

	hlt


.balign 4
_AP_tmp_IDTptr:
	.word	0
	.word	0,0

.balign 4
_AP_tmp_GDTptr:
	.short	_AP_tmp_GDT_end - _AP_tmp_GDT - 1
	.long	_AP_tmp_GDT - _APboot_base
_AP_tmp_GDT:
	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	08*/
	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	10*/
_AP_tmp_GDT_end:

.balign 4
_APboot_tmp_Stack_start:
	.org	0x400
_APboot_tmp_Stack_top: