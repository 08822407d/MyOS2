#include "arch/amd64/asm.h"
#include "include/archconst.h"

.text

ENTRY(start)
	cli
    movq    $stack_top,	%rax
    movq    %rax,		%rsp
//=======	load GDTR
	lgdt	GDT_POINTER(%rip)
//=======	load dummy IDTR
	lidt	IDT_POINTER(%rip)
//=======	load cr3
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
    movabsq $pre_init,	%rax
    pushq   $0x08
    pushq   %rax
    lretq
ret_from_preinit:
// reset stack to the one in c file(in higher half mem)
	movabsq	$kstacks,	%rbx
	addq	$CONFIG_KSTACK_SIZE, %rbx
    movq    $0x10,		%rax
    movq    %rax,		%ss
    movq    %rbx,		%rsp

    pushq   $ret_from_kmain
    movabsq $kmain,		%rax
    pushq   $0x08
    pushq   %rax
    lretq
ret_from_kmain:

hang:
    jmp     hang


.data
//=======	init page
.align 8

.org	0x0000
__PML4E:
	.quad	0x202003
	.fill	255,8,0
	.quad	0x202003
	.fill	255,8,0

.org	0x1000
__PDPTE:
	.quad	0x203003	/* 0x103003 */
	.fill	511,8,0

.org	0x2000
__PDE:
	.quad	0x000083	
	.quad	0x200083
	.quad	0x400083
	.quad	0x600083
	.quad	0x800083
	.quad	0xa00083
	.quad	0xc00083
	.quad	0xe00083
	.quad	0x1000083
	.quad	0x1200083
	.quad	0x1400083
	.quad	0x1600083
	.quad	0x1800083
	.quad	0x1a00083
	.quad	0x1c00083
	.quad	0x1e00083
	.fill	496,8,0

//=======	GDT_Table
GDT_Table:
	.quad	0x0000000000000000			/*0	NULL descriptor		       	00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.fill	125,8,0
GDT_END:

GDT_POINTER:
GDT_LIMIT:	.word	GDT_END - GDT_Table - 1
GDT_BASE:	.quad	GDT_Table
//=======	IDT_Table
IDT_Table:
	.fill  512,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT:	.word	IDT_END - IDT_Table - 1
IDT_BASE:	.quad	IDT_Table

//=======	temp stack
stack_start:
    .fill  512,8,0
stack_top: