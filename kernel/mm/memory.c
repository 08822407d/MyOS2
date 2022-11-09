// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/mm/memory.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 */

/*
 * demand-loading started 01.12.91 - seems it is high on the list of
 * things wanted, and it should be easy to implement. - Linus
 */

/*
 * Ok, demand-loading was easy, shared pages a little bit tricker. Shared
 * pages started 02.12.91, seems to work. - Linus.
 *
 * Tested sharing by executing about 30 /bin/sh: under the old kernel it
 * would have taken more than the 6M I have free, but it worked well as
 * far as I could see.
 *
 * Also corrected some "invalidate()"s - I wasn't doing enough of them.
 */

/*
 * Real VM (paging to/from disk) started 18.12.91. Much more work and
 * thought has to go into this. Oh, well..
 * 19.12.91  -  works, somewhat. Sometimes I get faults, don't know why.
 *		Found it. Everything seems to work now.
 * 20.12.91  -  Ok, making the swap-device changeable like the root.
 */

/*
 * 05.04.94  -  Multi-page memory management added for v1.1.
 *              Idea by Alex Bligh (alex@cconcepts.co.uk)
 *
 * 16.07.99  -  Support of BIGMEM added by Gerhard Wichert, Siemens AG
 *		(Gerhard.Wichert@pdb.siemens.de)
 *
 * Aug/Sep 2004 Changed to four level page tables (Andi Kleen)
 */

// #include <linux/kernel_stat.h>
#include <linux/mm/mm.h>
// #include <linux/mm_inline.h>
// #include <linux/sched/mm.h>
// #include <linux/sched/coredump.h>
// #include <linux/sched/numa_balancing.h>
// #include <linux/sched/task.h>
// #include <linux/hugetlb.h>
// #include <linux/mman.h>
// #include <linux/swap.h>
// #include <linux/highmem.h>
// #include <linux/pagemap.h>
// #include <linux/memremap.h>
// #include <linux/ksm.h>
// #include <linux/rmap.h>
// #include <linux/export.h>
// #include <linux/delayacct.h>
#include <linux/init/init.h>
// #include <linux/pfn_t.h>
// #include <linux/writeback.h>
// #include <linux/memcontrol.h>
// #include <linux/mmu_notifier.h>
// #include <linux/swapops.h>
// #include <linux/elf.h>
#include <linux/mm/gfp.h>
// #include <linux/migrate.h>
#include <linux/lib/string.h>
// #include <linux/debugfs.h>
// #include <linux/userfaultfd_k.h>
// #include <linux/dax.h>
// #include <linux/oom.h>
// #include <linux/numa.h>
// #include <linux/perf_event.h>
// #include <linux/ptrace.h>
// #include <linux/vmalloc.h>

// #include <trace/events/kmem.h>

#include <asm/io.h>
// #include <asm/mmu_context.h>
// #include <asm/pgalloc.h>
// #include <linux/uaccess.h>
// #include <asm/tlb.h>
// #include <asm/tlbflush.h>

// #include "pgalloc-track.h"
#include "internal.h"

#ifndef CONFIG_NUMA
	unsigned long	max_mapnr;
	page_s	*mem_map;
#endif

/*
 * A number of key systems in x86 including ioremap() rely on the assumption
 * that high_memory defines the upper bound on direct map memory, then end
 * of ZONE_NORMAL.  Under CONFIG_DISCONTIG this means that max_low_pfn and
 * highstart_pfn must be the same; there must be no gap between ZONE_NORMAL
 * and ZONE_HIGHMEM.
 */
void	*high_memory;