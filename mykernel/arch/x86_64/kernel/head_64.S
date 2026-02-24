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

	__STARTUP
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
// #include "grub2_head_tags.S"
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
	/*
	 * Save the multiboot information structure address and magic
	 */
	movq	%rax,		mbi_magic(%rip)
	movq	%rbx,		mbi_base(%rip)
	/*
	 * At this point the CPU runs in 64bit mode CS.L = 1 CS.D = 0,
	 * and someone has loaded an identity mapped page table
	 * for us.  These identity mapped page tables map all of the
	 * kernel pages and possibly all of memory.
	 *
	 * %RSI holds the physical address of the boot_params structure
	 * provided by the bootloader. Preserve it in %R15 so C function calls
	 * will not clobber it.
	 *
	 * We come here either directly from a 64bit bootloader, or from
	 * arch/x86/boot/compressed/head_64.S.
	 *
	 * We only come here initially at boot nothing else comes here.
	 *
	 * Since we may be loaded at an address different from what we were
	 * compiled to run at we first fixup the physical addresses in our page
	 * tables and then reload them.
	 */
	movq	%rsi,		%r15

	/* Set up the stack for verify_cpu() */
	leaq	tmp_kstack_top(%rip),	%rsp

	movq	%cr4,		%rax
	bts		$16,		%rax	// enable fsgsbase instuctions
	movq	%rax,		%cr4

	/* Setup GSBASE to allow stack canary access for C code */
	movl	$MSR_GS_BASE,			%ecx
	leaq	INIT_PER_CPU_VAR(fixed_percpu_data)(%rip),	%rdx
	movl	%edx,		%eax
	shrq	$32,		%rdx
	wrmsr

	/*
	 * Perform pagetable fixups. Additionally, if SME is active, encrypt
	 * the kernel and retrieve the modifier (SME encryption mask if SME
	 * is active) to be added to the initial pgdir entry that will be
	 * programmed into CR3.
	 */
	leaq	_text(%rip),%rdi
	movq	%r15,		%rsi
	call	__startup_64

	/* Form the CR3 value being sure to include the CR3 modifier */
	leaq	early_top_pgt(%rip),	%rcx
	addq	%rcx,		%rax

	/*
	 * Switch to early_top_pgt which still has the identity mappings
	 * present.
	 */
	movq	%rax,		%cr3

	/* Branch to the common startup code at its kernel virtual address */
	// ANNOTATE_RETPOLINE_SAFE
	jmp		*0f(%rip)
SYM_CODE_END(code_64_start)

	__INITRODATA
0:	.quad	common_startup_64

	.text
SYM_INNER_LABEL(common_startup_64, SYM_L_LOCAL)

	/* set up data segments */
	xorl	%eax,		%eax
	movl	%eax,		%ds
	movl	%eax,		%ss
	movl	%eax,		%es

	/*
	 * We don't really need to load %fs or %gs, but load them anyway
	 * to kill any stale realmode selectors.  This allows execution
	 * under VT hardware.
	 */
	movl	%eax,		%fs
	movl	%eax,		%gs

	/* Check if nx is implemented */
	movl	$0x80000001,%eax
	cpuid
	movl	%edx,		%edi

// enable_SCE:
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
/*
 * Each PGD needs to be 8k long and 8k aligned.  We do not
 * ever go out to userspace with these, so we do not
 * strictly *need* the second page, but this allows us to
 * have a single set_pgd() implementation that does not
 * need to worry about whether it has 4k or 8k to work
 * with.
 *
 * This ensures PGDs are 8k long:
 */
#define SYM_DATA_START_PTI_ALIGNED(name) \
			SYM_DATA_START_PAGE_ALIGNED(name)

	__INITDATA
	.balign 4

SYM_DATA_START_PTI_ALIGNED(early_top_pgt)
	.fill	511,8,0
	.quad	level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE_NOENC
SYM_DATA_END(early_top_pgt)

SYM_DATA_START_PAGE_ALIGNED(early_dynamic_pgts)
	.fill	512*EARLY_DYNAMIC_PAGE_TABLES,8,0
SYM_DATA_END(early_dynamic_pgts)

	.data

SYM_DATA_START_PAGE_ALIGNED(level3_kernel_pgt)
	.fill	510,8,0
	/* (2^48-(2*1024*1024*1024)-((2^39)*511))/(2^30) = 510 */
	.quad	level2_kernel_pgt - __START_KERNEL_map + _KERNPG_TABLE_NOENC
	// .quad	level2_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE_NOENC
	.fill	1,8,0 // 暂时不映射 fixmap 区，等以后需要了再说
SYM_DATA_END(level3_kernel_pgt)

SYM_DATA_START_PAGE_ALIGNED(level2_kernel_pgt)
	/*
	 * Kernel high mapping.
	 *
	 * The kernel code+data+bss must be located below KERNEL_IMAGE_SIZE in
	 * virtual address space, which is 1 GiB if RANDOMIZE_BASE is enabled,
	 * 512 MiB otherwise.
	 *
	 * (NOTE: after that starts the module area, see MODULES_VADDR.)
	 *
	 * This table is eventually used by the kernel during normal runtime.
	 * Care must be taken to clear out undesired bits later, like _PAGE_RW
	 * or _PAGE_GLOBAL in some cases.
	 */
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC, KERNEL_IMAGE_SIZE/PMD_SIZE)
SYM_DATA_END(level2_kernel_pgt)

SYM_DATA_START_PAGE_ALIGNED(level2_fixmap_pgt)
	.fill	512,8,0
	// .fill	(512 - 4 - FIXMAP_PMD_NUM),8,0
	// pgtno = 0
	// .rept (FIXMAP_PMD_NUM)
	// .quad level1_fixmap_pgt + (pgtno << PAGE_SHIFT) - __START_KERNEL_map \
	// 	+ _PAGE_TABLE_NOENC;
	// pgtno = pgtno + 1
	// .endr
	// /* 6 MB reserved space + a 2MB hole */
	// .fill	4,8,0
SYM_DATA_END(level2_fixmap_pgt)


	__INITDATA
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

	.data
	.align 16
/* This must match the first entry in level2_kernel_pgt */
SYM_DATA(phys_base, .quad 0x0)
EXPORT_SYMBOL(phys_base)

	__PAGE_ALIGNED_BSS
SYM_DATA_START_PAGE_ALIGNED(empty_zero_page)
	.skip PAGE_SIZE
SYM_DATA_END(empty_zero_page)
EXPORT_SYMBOL(empty_zero_page)