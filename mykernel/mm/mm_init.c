// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0-only
/*
 * mm_init.c - Memory initialisation verification and debugging
 *
 * Copyright 2008 IBM Corporation, 2008
 * Author Mel Gorman <mel@csn.ul.ie>
 *
 */
#include <linux/kernel/kernel.h>
#include <linux/init/init.h>
// #include <linux/kobject.h>
// #include <linux/export.h>
// #include <linux/memory.h>
// #include <linux/notifier.h>
#include <linux/kernel/sched.h>
// #include <linux/mman.h>
#include <linux/mm/memblock.h>
// #include <linux/page-isolation.h>
// #include <linux/padata.h>
#include <linux/kernel/nmi.h>
// #include <linux/buffer_head.h>
// #include <linux/kmemleak.h>
// #include <linux/kfence.h>
// #include <linux/page_ext.h>
// #include <linux/pti.h>
#include <linux/mm/pgtable.h>
// #include <linux/swap.h>
// #include <linux/cma.h>
// #include "internal.h"
// #include "slab.h"
// #include "shuffle.h"

// #include <asm/setup.h>


/*
 * Set up kernel memory allocators
 */
void __init mm_core_init(void)
{
	// /* Initializations relying on SMP setup */
	// build_all_zonelists(NULL);
	// page_alloc_init_cpuhp();

	// /*
	//  * page_ext requires contiguous pages,
	//  * bigger than MAX_ORDER unless SPARSEMEM.
	//  */
	// page_ext_init_flatmem();
	// mem_debugging_and_hardening_init();
	// kfence_alloc_pool();
	// report_meminit();
	// kmsan_init_shadow();
	// stack_depot_early_init();
	mem_init();
	// mem_init_print_info();
	// kmem_cache_init();
	// /*
	//  * page_owner must be initialized after buddy is ready, and also after
	//  * slab is ready so that stack_depot_init() works properly
	//  */
	// page_ext_init_flatmem_late();
	// kmemleak_init();
	// ptlock_cache_init();
	// pgtable_cache_init();
	// debug_objects_mem_init();
	// vmalloc_init();
	// /* If no deferred init page_ext now, as vmap is fully initialized */
	// if (!deferred_struct_pages)
	// 	page_ext_init();
	// /* Should be run before the first non-init thread is created */
	// init_espfix_bsp();
	// /* Should be run after espfix64 is set up. */
	// pti_init();
	// kmsan_init_runtime();
	// mm_cache_init();
}