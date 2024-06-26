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

#include <asm/smp.h>
#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/kdebug.h>
#include <asm/trapnr.h>
#include <asm/processor.h>

#include <asm/mm.h>

/* Don't add a printk in there. printk relies on the PDA which is not initialized 
   yet. */
static void __init clear_bss(void)
{
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
	myos_early_init_smp();

	// void __init x86_64_start_reservations(char *real_mode_data)
	// {
		/* version is always not zero if it is copied */
		start_kernel();
	// }
}