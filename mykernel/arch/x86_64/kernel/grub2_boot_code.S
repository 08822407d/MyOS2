/*==============================================================================================*
 *											grub2 boot entry                            		*
 *==============================================================================================*/
.code32
.balign 8
multiboot_entry:
	jmp		.
// 	cli
// 	movl	$0,			%esi
// //	store boot info
// 	movl	%ebx,		multiboot2_info_base(%esi)
// //	start initialize
// 	// open PAE
// 	call	set_temp_PDE_32
// 	movl	%cr4,		%eax
// 	bts		$5,			%eax
// 	bts		$16,		%eax	// enable fsgsbase instuctions
// 	movl	%eax,		%cr4

// 	movl	$0x01,		boot_from_grub2(%esi)
// set_tmp_page:
// 	// set page table
// 	movl	$__PML4E,	%eax
// 	movl	%eax,		%cr3
// 	// enable long mode
// 	movl	$0xC0000080,%ecx
// 	rdmsr
// 	bts		$8,			%eax
// 	wrmsr
// 	// enable PE & pagin
// 	movl	%cr0,		%eax
// 	bts		$0,			%eax
// 	bts		$31,		%eax
// 	movl	%eax,		%cr0
// go_to_64_code:
// 	lgdt	TMP_GDT_PTR(%esi)
// 	jmp		$0x08,		$code_64_start

// // 32bit fuction
// set_temp_PDE_32:
// 	movl	$0x83,		%eax
// 	movl	$__PDE,		%edi
// 	movl	$0x200,		%ecx
// l1:
// 	movl	%eax,		(%edi)
// 	addl	$0x200000,	%eax
// 	addl	$8,			%edi
// 	loop	l1
// 	retl

// .section .unpaged.data
// //=======init page
// .align 8

// .org	0x0000
// __PML4E:
// .globl	__PML4E
// 	.quad	__PDPTE + 3
// 	.fill	255,8,0
// 	.quad	__PDPTE + 3
// 	.fill	255,8,0

// .org	0x1000
// __PDPTE:
// 	.quad	__PDE + 3
// 	.fill	511,8,0

// .org	0x2000
// __PDE:
// 	.fill	512,8,0

// //=======temp GDT_table
// TMP_GDT_TBL:
// 	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
// 	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
// 	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
// 	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	08*/
// 	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	10*/
// GDT_END:

// TMP_GDT_PTR:
// GDT_LIMIT:	.word	GDT_END - TMP_GDT_TBL - 1
// GDT_BASE:	.quad	TMP_GDT_TBL
// //=======temp IDT_ptr
// TMP_IDT_PTR:
// 	.word	0
// 	.word	0,0

// .balign 4
// multiboot2_info_base:
// 	.quad	0x0

// boot_from_grub2:
// 	.quad	0x0

