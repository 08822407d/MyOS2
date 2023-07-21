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
#include <uapi/linux/errno.h>
#include <linux/lib/string.h>
#include <linux/kernel/types.h>
// #include <linux/ptrace.h>
#include <linux/mm/mman.h>
#include <linux/mm/mm.h>
// #include <linux/swap.h>
// #include <linux/smp.h>
#include <linux/init/init.h>
// #include <linux/initrd.h>
#include <linux/mm/pagemap.h>
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
#include <asm/pgalloc.h>
#include <asm/dma.h>
// #include <asm/fixmap.h>
// #include <asm/e820/api.h>
#include <asm/apic.h>
// #include <asm/tlb.h>
// #include <asm/mmu_context.h>
// #include <asm/proto.h>
// #include <asm/smp.h>
#include <asm/sections.h>
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
 * Create page table mapping for the physical memory for specific physical
 * addresses. Note that it can only be used to populate non-present entries.
 * The virtual and physical addresses have to be aligned on PMD level
 * down. It returns the last physical address mapped.
 */
// unsigned long __meminit
// kernel_physical_mapping_init(unsigned long paddr_start,
// 			     unsigned long paddr_end,
// 			     unsigned long page_size_mask, pgprot_t prot)
unsigned long __meminit
myos_kernel_physical_mapping_init(
		unsigned long paddr_start, unsigned long paddr_end)
{
	unsigned long addr = PFN_ALIGN(paddr_start);
	do
	{
		pgd_t *pgdp = pgd_ent_offset(&init_mm, addr);
		while (pgdp == 0);
		p4d_t *p4dp = p4d_ent_offset(pgdp, addr);
		while (p4dp == 0);
		pud_t *pudp = pud_ent_offset(p4dp, addr);
		while (pudp == 0);
		pmd_t *pmdp = pmd_ent_offset(pudp, addr);
		while (pmdp == 0);
		pte_t *ptep = pte_ent_offset(pmdp, addr);
		*ptep = arch_make_pte(__PAGE_KERNEL_EXEC | _PAGE_PAT | addr);

		addr += PAGE_SIZE;
	} while (addr < paddr_end);
}