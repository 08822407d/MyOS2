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
#include <linux/sched/mm.h>
// #include <linux/sched/coredump.h>
// #include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
// #include <linux/hugetlb.h>
// #include <linux/mman.h>
// #include <linux/swap.h>
// #include <linux/highmem.h>
// #include <linux/pagemap.h>
// #include <linux/memremap.h>
// #include <linux/ksm.h>
#include <linux/mm/rmap.h>
// #include <linux/export.h>
// #include <linux/delayacct.h>
#include <linux/init/init.h>
// #include <linux/pfn_t.h>
// #include <linux/writeback.h>
// #include <linux/memcontrol.h>
// #include <linux/mmu_notifier.h>
// #include <linux/swapops.h>
#include <linux/kernel/elf.h>
#include <linux/mm/gfp.h>
// #include <linux/migrate.h>
#include <linux/lib/string.h>
// #include <linux/debugfs.h>
// #include <linux/userfaultfd_k.h>
// #include <linux/dax.h>
// #include <linux/oom.h>
// #include <linux/numa.h>
// #include <linux/perf_event.h>
#include <linux/kernel/ptrace.h>
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



// static inline int
// copy_p4d_range(vma_s *dst_vma, vma_s *src_vma,
// 		pgd_t *dst_pgd, pgd_t *src_pgd,
// 		unsigned long addr, unsigned long end) {
// 	mm_s *dst_mm = dst_vma->vm_mm;
// 	p4d_t *src_p4d, *dst_p4d;
// 	unsigned long next;

// 	dst_p4d = p4d_alloc(dst_mm, dst_pgd, addr);
// 	if (!dst_p4d)
// 		return -ENOMEM;
// 	src_p4d = p4d_offset(src_pgd, addr);
// 	do {
// 		next = p4d_addr_end(addr, end);
// 		if (p4d_none_or_clear_bad(src_p4d))
// 			continue;
// 		if (copy_pud_range(dst_vma, src_vma, dst_p4d, src_p4d,
// 				   addr, next))
// 			return -ENOMEM;
// 	} while (dst_p4d++, src_p4d++, addr = next, addr != end);
// 	return 0;
// }

int
copy_page_range(vma_s *dst_vma, vma_s *src_vma)
{
	pgd_t *src_pgd, *dst_pgd;
	unsigned long next;
	unsigned long addr = src_vma->vm_start;
	unsigned long end = src_vma->vm_end;
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	// struct mmu_notifier_range range;
	bool is_cow;
	int ret;

	/*
	 * Don't copy ptes where a page fault will fill them correctly.
	 * Fork becomes much lighter when there are big shared or private
	 * readonly mappings. The tradeoff is that copy_page_range is more
	 * efficient than faulting.
	 */
	if (!(src_vma->vm_flags & (VM_HUGETLB | VM_PFNMAP | VM_MIXEDMAP)))
		return 0;

	// if (is_vm_hugetlb_page(src_vma))
	// 	return copy_hugetlb_page_range(dst_mm, src_mm, src_vma);

	// if (unlikely(src_vma->vm_flags & VM_PFNMAP)) {
	// 	/*
	// 	 * We do not free on error cases below as remove_vma
	// 	 * gets called on error from higher level routine
	// 	 */
	// 	ret = track_pfn_copy(src_vma);
	// 	if (ret)
	// 		return ret;
	// }

	/*
	 * We need to invalidate the secondary MMU mappings only when
	 * there could be a permission downgrade on the ptes of the
	 * parent mm. And a permission downgrade will only happen if
	 * is_cow_mapping() returns true.
	 */
	is_cow = is_cow_mapping(src_vma->vm_flags);

	// if (is_cow) {
	// 	mmu_notifier_range_init(&range, MMU_NOTIFY_PROTECTION_PAGE,
	// 				0, src_vma, src_mm, addr, end);
	// 	mmu_notifier_invalidate_range_start(&range);
	// 	/*
	// 	 * Disabling preemption is not needed for the write side, as
	// 	 * the read side doesn't spin, but goes to the mmap_lock.
	// 	 *
	// 	 * Use the raw variant of the seqcount_t write API to avoid
	// 	 * lockdep complaining about preemptibility.
	// 	 */
	// 	mmap_assert_write_locked(src_mm);
	// 	raw_write_seqcount_begin(&src_mm->write_protect_seq);
	// }

	// ret = 0;
	// dst_pgd = pgd_offset(dst_mm, addr);
	// src_pgd = pgd_offset(src_mm, addr);
	// do {
	// 	next = pgd_addr_end(addr, end);
	// 	if (pgd_none_or_clear_bad(src_pgd))
	// 		continue;
	// 	if (unlikely(copy_p4d_range(dst_vma, src_vma, dst_pgd, src_pgd,
	// 				    addr, next))) {
	// 		ret = -ENOMEM;
	// 		break;
	// 	}
	// } while (dst_pgd++, src_pgd++, addr = next, addr != end);

	// if (is_cow) {
	// 	raw_write_seqcount_end(&src_mm->write_protect_seq);
	// 	mmu_notifier_invalidate_range_end(&range);
	// }
	return ret;
}