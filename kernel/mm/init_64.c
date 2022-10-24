// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/arch/x86_64/mm/init.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Copyright (C) 2000  Pavel Machek <pavel@ucw.cz>
 *  Copyright (C) 2002,2003 Andi Kleen <ak@suse.de>
 */

// #include <linux/signal.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/kernel.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <linux/kernel/types.h>
// #include <linux/ptrace.h>
// #include <linux/mman.h>
#include <linux/mm/mm.h>
// #include <linux/swap.h>
// #include <linux/smp.h>
#include <linux/init/init.h>
// #include <linux/initrd.h>
// #include <linux/pagemap.h>
#include <linux/mm/memblock.h>
// #include <linux/proc_fs.h>
// #include <linux/pci.h>
#include <linux/mm/pfn.h>
// #include <linux/poison.h>
// #include <linux/dma-mapping.h>
// #include <linux/memory.h>
// #include <linux/memory_hotplug.h>
// #include <linux/memremap.h>
// #include <linux/nmi.h>
#include <linux/mm/gfp.h>
// #include <linux/kcore.h>
// #include <linux/bootmem_info.h>

#include <asm/processor.h>
// #include <asm/bios_ebda.h>
// #include <linux/uaccess.h>
// #include <asm/pgalloc.h>
// #include <asm/dma.h>
// #include <asm/fixmap.h>
#include <asm/e820-api.h>
// #include <asm/apic.h>
// #include <asm/tlb.h>
// #include <asm/mmu_context.h>
// #include <asm/proto.h>
// #include <asm/smp.h>
// #include <asm/sections.h>
// #include <asm/kdebug.h>
// #include <asm/numa.h>
// #include <asm/set_memory.h>
// #include <asm/init.h>
// #include <asm/uv/uv.h>
#include <asm/setup.h>
// #include <asm/ftrace.h>

// #include "mm_internal.h"

// #include "ident_map.c"



/*
 * NOTE: pagetable_init alloc all the fixmap pagetables contiguous on the
 * physical space so we can cache the place of the first one and move
 * around without checking the pgd every time.
 */

/* Bits supported by the hardware: */
pteval_t __supported_pte_mask __read_mostly = ~0;
/* Bits allowed in normal kernel mappings: */
pteval_t __default_kernel_pte_mask __read_mostly = ~0;

void __init paging_init(void)
{
	// sparse_init();

	// /*
	//  * clear the default setting with node 0
	//  * note: don't use nodes_clear here, that is really clearing when
	//  *	 numa support is not compiled in, and later node_set_state
	//  *	 will not set it back.
	//  */
	// node_clear_state(0, N_MEMORY);
	// node_clear_state(0, N_NORMAL_MEMORY);

	zone_sizes_init();
}

void __init mem_init(void)
{
	// pci_iommu_alloc();

	/* clear_bss() already clear the empty_zero_page */

	/* this will put all memory onto the freelists */
	memblock_free_all();
	// after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	// /*
	//  * Must be done after boot memory is put on freelist, because here we
	//  * might set fields in deferred struct pages that have not yet been
	//  * initialized, and memblock_free_all() initializes all the reserved
	//  * deferred pages for us.
	//  */
	// register_page_bootmem_info();

	// /* Register memory areas for /proc/kcore */
	// if (get_gate_vma(&init_mm))
	// 	kclist_add(&kcore_vsyscall, (void *)VSYSCALL_ADDR, PAGE_SIZE, KCORE_USER);

	// preallocate_vmalloc_pages();
}