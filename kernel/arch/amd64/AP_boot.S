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
	mov		$(_APboot_tmp_Stack_top - _APboot_base),		%esp

	//	get base address
	mov		%cs,		%ax
	movzx	%ax,		%esi
	shll	$4,			%esi

	//	load idt gdt
	lgdtl	_AP_tmp_GDT - _APboot_base
	lidtl	_AP_tmp_IDT - _APboot_base

	//	enable protected mode
	smsw	%ax
	bts	$0	,%ax
	lmsw	%ax

	//	go to 32 code
	hlt
	// ljmpl	*(_APU_Code32_vector - _APU_boot_base)

.balign 4
_AP_tmp_IDT:
	.word	0
	.word	0,0

.balign 4
_AP_tmp_GDT:
	.short	_AP_tmp_GDT_end - _AP_tmp_GDT - 1
	.long	_AP_tmp_GDT - _APboot_base
	.short	0
	.quad	0x0020980000000000
	.quad	0x0000920000000000
	.quad	0x00cf9a000000ffff
	.quad	0x00cf92000000ffff
_AP_tmp_GDT_end:

.balign 4
_APboot_tmp_Stack_start:
	.org	0x400
_APboot_tmp_Stack_top: