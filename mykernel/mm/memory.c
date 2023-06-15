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
#include <asm/pgalloc.h>
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


static int
copy_pte_range(vma_s *dst_vma, vma_s *src_vma, pmd_t *dst_pmd,
		pmd_t *src_pmd, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pte_t *orig_src_pte, *orig_dst_pte;
	pte_t *src_pte, *dst_pte;
	// spinlock_t *src_ptl, *dst_ptl;
	int progress, ret = 0;
	// int rss[NR_MM_COUNTERS];
	// swp_entry_t entry = (swp_entry_t){0};
	// struct page *prealloc = NULL;

again:
	// progress = 0;
	// init_rss_vec(rss);

	// dst_pte = pte_alloc_map_lock(dst_mm, dst_pmd, addr, &dst_ptl);
	// if (!dst_pte) {
	// 	ret = -ENOMEM;
	// 	goto out;
	// }
	// src_pte = pte_offset_map(src_pmd, addr);
	// src_ptl = pte_lockptr(src_mm, src_pmd);
	// spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);
	// orig_src_pte = src_pte;
	// orig_dst_pte = dst_pte;
	// arch_enter_lazy_mmu_mode();

	// do {
	// 	/*
	// 	 * We are holding two locks at this point - either of them
	// 	 * could generate latencies in another task on another CPU.
	// 	 */
	// 	if (progress >= 32) {
	// 		progress = 0;
	// 		if (need_resched() ||
	// 		    spin_needbreak(src_ptl) || spin_needbreak(dst_ptl))
	// 			break;
	// 	}
	// 	if (arch_pte_none(*src_pte)) {
	// 		progress++;
	// 		continue;
	// 	}
	// 	if (unlikely(!arch_pte_present(*src_pte))) {
	// 		ret = copy_nonpresent_pte(dst_mm, src_mm,
	// 					  dst_pte, src_pte,
	// 					  dst_vma, src_vma,
	// 					  addr, rss);
	// 		if (ret == -EIO) {
	// 			entry = pte_to_swp_entry(*src_pte);
	// 			break;
	// 		} else if (ret == -EBUSY) {
	// 			break;
	// 		} else if (!ret) {
	// 			progress += 8;
	// 			continue;
	// 		}

	// 		/*
	// 		 * Device exclusive entry restored, continue by copying
	// 		 * the now present pte.
	// 		 */
	// 		WARN_ON_ONCE(ret != -ENOENT);
	// 	}
	// 	/* copy_present_pte() will clear `*prealloc' if consumed */
	// 	ret = copy_present_pte(dst_vma, src_vma, dst_pte, src_pte,
	// 			       addr, rss, &prealloc);
	// 	/*
	// 	 * If we need a pre-allocated page for this pte, drop the
	// 	 * locks, allocate, and try again.
	// 	 */
	// 	if (unlikely(ret == -EAGAIN))
	// 		break;
	// 	if (unlikely(prealloc)) {
	// 		/*
	// 		 * pre-alloc page cannot be reused by next time so as
	// 		 * to strictly follow mempolicy (e.g., alloc_page_vma()
	// 		 * will allocate page according to address).  This
	// 		 * could only happen if one pinned pte changed.
	// 		 */
	// 		put_page(prealloc);
	// 		prealloc = NULL;
	// 	}
	// 	progress += 8;
	// } while (dst_pte++, src_pte++, addr += PAGE_SIZE, addr != end);

	// arch_leave_lazy_mmu_mode();
	// spin_unlock(src_ptl);
	// pte_unmap(orig_src_pte);
	// add_mm_rss_vec(dst_mm, rss);
	// pte_unmap_unlock(orig_dst_pte, dst_ptl);
	// cond_resched();

	// if (ret == -EIO) {
	// 	VM_WARN_ON_ONCE(!entry.val);
	// 	if (add_swap_count_continuation(entry, GFP_KERNEL) < 0) {
	// 		ret = -ENOMEM;
	// 		goto out;
	// 	}
	// 	entry.val = 0;
	// } else if (ret == -EBUSY) {
	// 	goto out;
	// } else if (ret ==  -EAGAIN) {
	// 	prealloc = page_copy_prealloc(src_mm, src_vma, addr);
	// 	if (!prealloc)
	// 		return -ENOMEM;
	// } else if (ret) {
	// 	VM_WARN_ON_ONCE(1);
	// }

	/* We've captured and resolved the error. Reset, try again. */
	ret = 0;

	if (addr != end)
		goto again;
out:
	// if (prealloc)
	// 	put_page(prealloc);
	return ret;
}

static inline int
copy_pmd_range(vma_s *dst_vma, vma_s *src_vma, pud_t *dst_pud,
		pud_t *src_pud, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pmd_t *src_pmd, *dst_pmd;
	unsigned long next;

	dst_pmd = pmd_alloc(dst_mm, dst_pud, addr);
	if (!dst_pmd)
		return -ENOMEM;
	src_pmd = pmd_offset(src_pud, addr);
	do {
		next = next_pmd_addr_end(addr, end);
		// if (is_swap_pmd(*src_pmd) || pmd_trans_huge(*src_pmd)
		// 	|| pmd_devmap(*src_pmd)) {
		// 	int err;
		// 	VM_BUG_ON_VMA(next-addr != HPAGE_PMD_SIZE, src_vma);
		// 	err = copy_huge_pmd(dst_mm, src_mm, dst_pmd, src_pmd,
		// 			    addr, dst_vma, src_vma);
		// 	if (err == -ENOMEM)
		// 		return -ENOMEM;
		// 	if (!err)
		// 		continue;
		// 	/* fall through */
		// }
		if (pmd_none_or_clear_bad(src_pmd))
			continue;
		if (copy_pte_range(dst_vma, src_vma, dst_pmd, src_pmd,
				   addr, next))
			return -ENOMEM;
	} while (dst_pmd++, src_pmd++, addr = next, addr != end);
	return 0;
}

static inline int
copy_pud_range(vma_s *dst_vma, vma_s *src_vma, p4d_t *dst_p4d,
		p4d_t *src_p4d, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pud_t *src_pud, *dst_pud;
	unsigned long next;

	dst_pud = pud_alloc(dst_mm, dst_p4d, addr);
	if (!dst_pud)
		return -ENOMEM;
	src_pud = pud_offset(src_p4d, addr);
	do {
		next = next_pud_addr_end(addr, end);
		// if (pud_trans_huge(*src_pud) || pud_devmap(*src_pud)) {
		// 	int err;

		// 	VM_BUG_ON_VMA(next-addr != HPAGE_PUD_SIZE, src_vma);
		// 	err = copy_huge_pud(dst_mm, src_mm,
		// 			    dst_pud, src_pud, addr, src_vma);
		// 	if (err == -ENOMEM)
		// 		return -ENOMEM;
		// 	if (!err)
		// 		continue;
		// 	/* fall through */
		// }
		if (pud_none_or_clear_bad(src_pud))
			continue;
		if (copy_pmd_range(dst_vma, src_vma, dst_pud, src_pud,
				   addr, next))
			return -ENOMEM;
	} while (dst_pud++, src_pud++, addr = next, addr != end);
	return 0;
}

static inline int
copy_p4d_range(vma_s *dst_vma, vma_s *src_vma, pgd_t *dst_pgd_ent,
		pgd_t *src_pgd_ent, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	p4d_t *src_p4d_ent, *dst_p4d_ent;
	unsigned long next;

	dst_p4d_ent = p4d_alloc(dst_mm, dst_pgd_ent, addr);
	if (!dst_p4d_ent)
		return -ENOMEM;
	src_p4d_ent = arch_p4d_offset(src_pgd_ent, addr);
	do {
		next = next_p4d_addr_end(addr, end);
		// 四级映射下p4d页必有效，所以不检查
		// if (p4d_none_or_clear_bad(src_p4d))
		// 	continue;
		if (copy_pud_range(dst_vma, src_vma,
				dst_p4d_ent, src_p4d_ent, addr, next))
			return -ENOMEM;
	} while (dst_p4d_ent++, src_p4d_ent++, addr = next, addr != end);
	return 0;
}

int
copy_page_range(vma_s *dst_vma, vma_s *src_vma)
{
	pgd_t *src_pgd_ent, *dst_pgd_ent;
	unsigned long next;
	unsigned long addr = src_vma->vm_start;
	unsigned long end = src_vma->vm_end;
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	// struct mmu_notifier_range range;
	bool is_cow;
	int ret;

	// /*
	//  * Don't copy ptes where a page fault will fill them correctly.
	//  * Fork becomes much lighter when there are big shared or private
	//  * readonly mappings. The tradeoff is that copy_page_range is more
	//  * efficient than faulting.
	//  */
	// if (!(src_vma->vm_flags & (VM_HUGETLB | VM_PFNMAP | VM_MIXEDMAP)))
	// 	return 0;

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

	ret = 0;
	dst_pgd_ent = pgd_offset(dst_mm, addr);
	src_pgd_ent = pgd_offset(src_mm, addr);
	do {
		next = next_pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(src_pgd_ent))
			continue;
		if (copy_p4d_range(dst_vma, src_vma,
				dst_pgd_ent, src_pgd_ent, addr, next)) {
			ret = -ENOMEM;
			break;
		}
	} while (dst_pgd_ent++, src_pgd_ent++, addr = next, addr != end);

	// if (is_cow) {
	// 	raw_write_seqcount_end(&src_mm->write_protect_seq);
	// 	mmu_notifier_invalidate_range_end(&range);
	// }
	return ret;
}



/*
 * Allocate page upper directory.
 * We've already handled the fast-path in-line.
 */
int __pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address)
{
	pud_t *new = pud_alloc_one(mm, address);
	if (!new)
		return -ENOMEM;

	spin_lock(&mm->page_table_lock);
	if (!arch_p4d_present(*p4d)) {
		// mm_inc_nr_puds(mm);
		smp_wmb(); /* See comment in pmd_install() */
		arch_p4d_populate(mm, p4d, new);
	} else	/* Another has populated it */
		pud_free(mm, new);
	spin_unlock(&mm->page_table_lock);
	return 0;
}

/*
 * Allocate page middle directory.
 * We've already handled the fast-path in-line.
 */
int __pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address)
{
	spinlock_t *ptl;
	pmd_t *new = pmd_alloc_one(mm, address);
	if (!new)
		return -ENOMEM;

	spin_lock(&mm->page_table_lock);
	if (!arch_pud_present(*pud)) {
		// mm_inc_nr_pmds(mm);
		smp_wmb(); /* See comment in pmd_install() */
		arch_pud_populate(mm, pud, new);
	} else {	/* Another has populated it */
		pmd_free(mm, new);
	}
	spin_unlock(&mm->page_table_lock);
	return 0;
}