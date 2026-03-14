// SPDX-License-Identifier: GPL-2.0
/*
 *  prepare to run common code
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 */

#define DISABLE_BRANCH_PROFILING

/* cpu_feature_enabled() cannot be used this early */
#define USE_EARLY_PGTABLE_L5

#include <linux/init/init.h>
#include <linux/kernel/linkage.h>
#include <linux/kernel/types.h>
#include <linux/kernel/kernel.h>
#include <linux/lib/string.h>
#include <linux/smp/percpu.h>
#include <linux/kernel/start_kernel.h>

#include <asm/init.h>
#include <asm/smp.h>
#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/kdebug.h>
#include <asm/trapnr.h>
#include <asm/processor.h>

#include <asm/mm.h>


#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>


/*
 * Manage page tables very early on.
 */
extern pmd_t early_dynamic_pgts[EARLY_DYNAMIC_PAGE_TABLES][PTRS_PER_PMD];
static uint __initdata next_early_pgt;
pmdval_t early_pmd_flags = __PAGE_KERNEL_LARGE & ~(_PAGE_GLOBAL | _PAGE_NX);


/*
 * This code is compiled using PIC codegen because it will execute from the
 * early 1:1 mapping of memory, which deviates from the mapping expected by the
 * linker. Due to this deviation, taking the address of a global variable will
 * produce an ambiguous result when using the plain & operator.  Instead,
 * rip_rel_ptr() must be used, which will return the RIP-relative address in
 * the 1:1 mapping of memory. Kernel virtual addresses can be determined by
 * subtracting p2v_offset from the RIP-relative address.
 */
ulong __head
__startup_64(ulong p2v_offset, struct boot_params *bp) {
	pmd_t (*early_pgts)[PTRS_PER_PMD] = rip_rel_ptr(&early_dynamic_pgts[0]);
	ulong physaddr = (ulong)rip_rel_ptr(&_text);
	ulong va_text, va_end;
	ulong pgtable_flags;
	ulong load_delta;
	pgdval_t *pgd;
	p4dval_t *p4d;
	pudval_t *pud;
	pmdval_t *pmd, pmd_entry;
	int i;

	/* Is the address too large? */
	if (physaddr >> MAX_PHYSMEM_BITS)
		for (;;);

	/*
	 * Compute the delta between the address I am compiled to run at
	 * and the address I am actually running at.
	 */
	phys_base = load_delta = __START_KERNEL_map + p2v_offset;

	/* Is the address not 2M aligned? */
	if (load_delta & ~PMD_MASK)
		for (;;); // 在MyOS2中假定 load_delta == 0

	va_text = physaddr - p2v_offset;
	va_end  = (ulong)rip_rel_ptr(&_end) - p2v_offset;

	/* Fixup the physical addresses in the page table */

	pgd = &rip_rel_ptr(&early_top_pgt[0])->val;
	pgd[pgd_index(__START_KERNEL_map)] += load_delta;

	level3_kernel_pgt[PTRS_PER_PUD - 2].val += load_delta;
	level3_kernel_pgt[PTRS_PER_PUD - 1].val += load_delta;

	// for (i = FIXMAP_PMD_TOP; i > FIXMAP_PMD_TOP - FIXMAP_PMD_NUM; i--)
	// 	level2_fixmap_pgt[i].pmd += load_delta;

	/*
	 * Set up the identity mapping for the switchover.  These
	 * entries should *NOT* have the global bit set!  This also
	 * creates a bunch of nonsense entries but that is fine --
	 * it avoids problems around wraparound.
	 */

	pud = &early_pgts[0]->val;
	pmd = &early_pgts[1]->val;
	next_early_pgt = 2;

	pgtable_flags = _KERNPG_TABLE_NOENC;

	// if (!la57)
	i = (physaddr >> PGDIR_SHIFT) % PTRS_PER_PGD;
	pgd[i + 0] = (pgdval_t)pud + pgtable_flags;
	pgd[i + 1] = (pgdval_t)pud + pgtable_flags;

	i = physaddr >> PUD_SHIFT;
	pud[(i + 0) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
	pud[(i + 1) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;

	pmd_entry = __PAGE_KERNEL_LARGE_EXEC & ~_PAGE_GLOBAL;
	/* Filter out unsupported __PAGE_KERNEL_* bits: */
	pmd_entry +=  physaddr;

	for (i = 0; i < DIV_ROUND_UP(_end - _text, PMD_SIZE); i++) {
		int idx = i + (physaddr >> PMD_SHIFT);
		pmd[idx % PTRS_PER_PMD] = pmd_entry + i * PMD_SIZE;
	}


	// copy boot_params or multiboot_MBI to a safe place for later use
	u64 *mbi_base_p = rip_rel_ptr(&mbi_base);
	void *mbi_dst = rip_rel_ptr(&multiboot_MBI);
	void *bp_dst = rip_rel_ptr(&boot_params);

	if (*rip_rel_ptr(&mbi_magic) == MULTIBOOT2_BOOTLOADER_MAGIC) {
		memcpy(mbi_dst, (const void *)(*mbi_base_p), sizeof(multiboot_MBI));
	} else {
		memcpy(bp_dst, (const void *)bp, sizeof(boot_params));
	}


#ifdef DEBUG
	rip_rel_ptr(&init_state)->early_kernel_memmap = 1;
	rip_rel_ptr(&init_state)->boot_params_stored = 1;
#endif

	return 0;
}


/* Don't add a printk in there. printk relies on the PDA which is not initialized 
   yet. */
static void __init
clear_bss(void) {
	memset(__bss_start, 0,
		(unsigned long) __bss_stop - (unsigned long) __bss_start);
}

asmlinkage __visible void __init
x86_64_start_kernel(char * real_mode_data)
{
	clear_bss();

	/*
	 * This needs to happen *before* kasan_early_init() because latter maps stuff
	 * into that page.
	 */
	clear_page(init_top_pgt);

	idt_setup_early_handler();


	extern void myos_early_init_system(void);

	myos_early_init_system();

	/* set init_top_pgt kernel high mapping*/
	init_top_pgt[511] = early_top_pgt[511];

	// void __init x86_64_start_reservations(char *real_mode_data)
	// {
		/* version is always not zero if it is copied */
		start_kernel();
	// }
}




/*
 * Setup boot CPU state needed before kernel switches to virtual addresses.
 */
void __head startup_64_setup_gdt_idt(void)
{
	struct desc_ptr startup_gdt_descr = {
		.address = (ulong)(rip_rel_ptr(&gdt_page)->gdt),
		.size    = GDT_SIZE - 1,
	};

	/* Load GDT */
	native_load_gdt(&startup_gdt_descr);

	/* New GDT is live - reload data segment registers */
	asm volatile(	"movl	%%eax,	%%ds	\n"
		    		"movl	%%eax,	%%ss	\n"
		    		"movl	%%eax,	%%es	\n"
				:	
				:	"a"(__KERNEL_DS)
				:	"memory"
				);

	// startup_64_load_idt(handler);
}