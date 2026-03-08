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


/* Code in __startup_64() can be relocated during execution, but the compiler
 * doesn't have to generate PC-relative relocations when accessing globals from
 * that function. Clang actually does not generate them, which leads to
 * boot-time crashes. To work around this problem, every global pointer must
 * be accessed using RIP_REL_REF().
 */
ulong __head
__startup_64(ulong physaddr, struct boot_params *bp) {
	pmd_t (*early_pgts)[PTRS_PER_PMD] = RIP_REL_REF(early_dynamic_pgts);
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
	load_delta = physaddr - (ulong)(_text - __START_KERNEL_map);
	RIP_REL_REF(phys_base) = load_delta;

	/* Is the address not 2M aligned? */
	if (load_delta & ~PMD_MASK)
		for (;;); // 在MyOS2中假定 load_delta == 0

	/* Fixup the physical addresses in the page table */

	pgd = &RIP_REL_REF(early_top_pgt)->val;
	// pgd[pgd_index(__START_KERNEL_map)] += load_delta;

	// RIP_REL_REF(level3_kernel_pgt)[PTRS_PER_PUD - 2].val += load_delta;
	// RIP_REL_REF(level3_kernel_pgt)[PTRS_PER_PUD - 1].val += load_delta;

	// for (i = FIXMAP_PMD_TOP; i > FIXMAP_PMD_TOP - FIXMAP_PMD_NUM; i--)
	// 	RIP_REL_REF(level2_fixmap_pgt)[i].pmd += load_delta;

	/*
	 * Set up the identity mapping for the switchover.  These
	 * entries should *NOT* have the global bit set!  This also
	 * creates a bunch of nonsense entries but that is fine --
	 * it avoids problems around wraparound.
	 */

	pud = &early_pgts[0]->val;
	pmd = &early_pgts[1]->val;
	RIP_REL_REF(next_early_pgt) = 2;

	pgtable_flags = _KERNPG_TABLE_NOENC;

	i = (physaddr >> PGDIR_SHIFT) % PTRS_PER_PGD;
	pgd[i + 0] = (pgdval_t)pud + pgtable_flags;
	pgd[i + 1] = (pgdval_t)pud + pgtable_flags;

	i = physaddr >> PUD_SHIFT;
	pud[(i + 0) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
	pud[(i + 1) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;

	pmd_entry = __PAGE_KERNEL_LARGE_EXEC & ~_PAGE_GLOBAL;
	/* Filter out unsupported __PAGE_KERNEL_* bits: */
	pmd_entry &= RIP_REL_REF(__supported_pte_mask);
	pmd_entry +=  physaddr;

	for (i = 0; i < DIV_ROUND_UP(_end - _text, PMD_SIZE); i++) {
		int idx = i + (physaddr >> PMD_SHIFT);
		pmd[idx % PTRS_PER_PMD] = pmd_entry + i * PMD_SIZE;
	}


	// copy boot_params or multiboot_MBI to a safe place for later use
	if (RIP_REL_REF(mbi_magic) == MULTIBOOT2_BOOTLOADER_MAGIC) {
		u64 mbi_phys = RIP_REL_REF(mbi_base);
		memcpy(&RIP_REL_REF(multiboot_MBI), (void *)mbi_phys, sizeof(RIP_REL_REF(multiboot_MBI)));
	} else {
		memcpy(&RIP_REL_REF(boot_params), bp, sizeof(RIP_REL_REF(boot_params)));
	}

#ifdef DEBUG
	init_state.early_kernel_memmap = 1;
	init_state.boot_params_stored = 1;
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

	// /*
	//  * This needs to happen *before* kasan_early_init() because latter maps stuff
	//  * into that page.
	//  */
	// clear_page(init_top_pgt);
	memset(init_top_pgt, 0, PAGE_SIZE);


	idt_setup_early_handler();


	extern void myos_early_init_system(void);
	extern void myos_early_init_smp(void);

	myos_early_init_system();
	// myos_early_init_smp();

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
		// .address = (ulong)&RIP_REL_REF(*gdt),
		.address = (ulong)&RIP_REL_REF(*(desc_s *)(__force ulong)init_per_cpu_var(gdt_page.gdt)),
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