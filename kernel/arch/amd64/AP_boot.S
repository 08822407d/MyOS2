#include <arch/amd64/asm.h>

#include <include/memory.h>

.balign	0x1000
.text

/*==============================================================================================*
 *										real mode code                              			*
 *==============================================================================================*/
.code16
ENTRY(_APboot_start)
_APboot_base = .
	cli
	wbinvd
	mov		$0,			%ax
	mov		%ax,		%ss
	mov		$_APboot_tmp_Stack_top,	%esp
	mov		$_APboot_base,			%esi
	// change the seg register to zero
	jmp		$0,			$reform_address
reform_address:
	mov		%cs,		%ax
	mov		%ax,		%ds
	mov		%ax,		%es
	mov		%ax,		%fs
	mov		%ax,		%gs
	// load idt gdt
	lgdtl	_AP_tmp_GDTptr
	lidtl	_AP_tmp_IDTptr
	// enable protected mode
	smsw	%ax
	bts		$0,			%ax
	lmsw	%ax
	// goto 32bit code
	jmp		$0x18, $_AP_code32


/*==============================================================================================*
 *										prot mode code                              			*
 *==============================================================================================*/
.balign 4
.code32
_AP_code32:
	movl	$0x20,		%eax
	movl	%eax,		%ds
	movl	%eax,		%es
	movl	%eax,		%ss
	movl	%eax,		%fs
	movl	%eax,		%gs
	// open PAE
	movl	%cr4,		%eax
	bts		$5,			%eax
	movl	%eax,		%cr4
	// set page table
	movl	$__PML4E,	%eax
	movl	%eax,		%cr3
	// enable long mode
	movl	$0xC0000080,	%ecx
	rdmsr
	bts		$8,			%eax
	wrmsr
	// enable PE & paging
	movl	%cr0,		%eax
	bts		$0,			%eax
	bts		$31,		%eax
	movl	%eax,		%cr0
//	go to 64 code
	jmp		$0x08,		$_AP_code64


/*==============================================================================================*
 *										long mode code                              			*
 *==============================================================================================*/
.code64
.balign 8
_AP_code64:
	movq	$0x10,		%rax
	movq	%rax,		%ds
	movq	%rax,		%es
	movq	%rax,		%fs
	movq	%rax,		%gs
	movq	%rax,		%ss

	movabsq	kernel_cr3,	%rax
	movq	%rax,		%cr3

	// enalble x2apic and get lapic_id
	movq 	$0x1b,		%rcx
	rdmsr
	bts		$10,		%rax
	bts		$11,		%rax
	wrmsr
	movq	$0x802,		%rcx
	rdmsr
	// enable fsgsbase instrutctions
	movq	%cr4,		%rcx
	bts		$16,		%rcx
	movq	%rcx,		%cr4

	// compair lapic_id in table to get current AP's index
	movq	%rax,		%r8
	movabsq	$apic_id,	%rax
	movq	$0,			%rbx
l1:
	incq	%rbx	//	here is a pre-requisit that BSP's apic_id is 0
	movq	(%rax, %rbx, 8),	%r9
	cmpq	%r9,		%r8
	jne		l1

	// get idle_task_PCB for current AP and set rsp to task's kstack
	movabsq	idle_tasks,	%rax
	movq	(%rax, %rbx, 8),	%rax
	addq	$TASK_KSTACK_SIZE,	%rax
	movq	%rax,		%rsp

	movq	%rbx,		%rdi
	movabsq $idle,		%rax
	pushq   $0x08
	pushq   %rax
	lretq



/*==============================================================================================*
 *											data section										*
 *==============================================================================================*/
.balign 4
_AP_tmp_IDTptr:
	.word	0
	.word	0,0

.balign 4
_AP_tmp_GDTptr:
	.short	_AP_tmp_GDT_end - _AP_tmp_GDT - 1
	.long	_AP_tmp_GDT
_AP_tmp_GDT:
	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	18*/
	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	20*/
_AP_tmp_GDT_end:

// .balign 4
// _AP_code32_vector:
// 	.long	_AP_code32 - _APboot_base
// 	.word	0x18,0

.balign 4
_APboot_tmp_Stack_start:
	.org	0x400
_APboot_tmp_Stack_top: