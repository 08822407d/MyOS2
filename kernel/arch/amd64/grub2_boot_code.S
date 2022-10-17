/*==============================================================================================*
 *											grub2 boot entry                            		*
 *==============================================================================================*/
.code32
.balign 8
multiboot_entry:
	cli
	movl	$0,			%esi
//	store boot info
	movl	%ebx,		multiboot2_info_base(%esi)
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
	lgdt	TMP_GDT_PTR(%esi)
	jmp		$0x08,		$code_64_start

// 32bit fuction
set_temp_PDE_32:
	movl	$0x83,		%eax
	movl	$__PDE,		%edi
	movl	$0x200,		%ecx
l1:
	movl	%eax,		(%edi)
	addl	$0x200000,	%eax
	addl	$8,			%edi
	loop	l1
	retl