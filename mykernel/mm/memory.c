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
#include <linux/sched/coredump.h>
// #include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
// #include <linux/hugetlb.h>
#include <linux/mm/mman.h>
// #include <linux/swap.h>
// #include <linux/highmem.h>
#include <linux/mm/pagemap.h>
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

unsigned long highest_memmap_pfn __read_mostly;


/*
 * vm_normal_page -- This function gets the "struct page" associated with a pte.
 *
 * "Special" mappings do not wish to be associated with a "struct page" (either
 * it doesn't exist, or it exists but they don't want to touch it). In this
 * case, NULL is returned here. "Normal" mappings do have a struct page.
 *
 * There are 2 broad cases. Firstly, an architecture may define a pte_special()
 * pte bit, in which case this function is trivial. Secondly, an architecture
 * may not have a spare pte bit, which requires a more complicated scheme,
 * described below.
 *
 * A raw VM_PFNMAP mapping (ie. one that is not COWed) is always considered a
 * special mapping (even if there are underlying and valid "struct pages").
 * COWed pages of a VM_PFNMAP are always normal.
 *
 * The way we recognize COWed pages within VM_PFNMAP mappings is through the
 * rules set up by "remap_pfn_range()": the vma will have the VM_PFNMAP bit
 * set, and the vm_pgoff will point to the first PFN mapped: thus every special
 * mapping will always honor the rule
 *
 *	pfn_of_page == vma->vm_pgoff + ((addr - vma->vm_start) >> PAGE_SHIFT)
 *
 * And for normal mappings this is false.
 *
 * This restricts such mappings to be a linear translation from virtual address
 * to pfn. To get around this restriction, we allow arbitrary mappings so long
 * as the vma is not a COW mapping; in that case, we know that all ptes are
 * special (because none can have been COWed).
 *
 *
 * In order to support COW of arbitrary special mappings, we have VM_MIXEDMAP.
 *
 * VM_MIXEDMAP mappings can likewise contain memory with or without "struct
 * page" backing, however the difference is that _all_ pages with a struct
 * page (that is, those where pfn_valid is true) are refcounted and considered
 * normal pages by the VM. The disadvantage is that pages are refcounted
 * (which can be slower and simply not an option for some PFNMAP users). The
 * advantage is that we don't have to follow the strict linearity rule of
 * PFNMAP mappings in order to support COWable mappings.
 *
 */
page_s *vm_normal_page(vma_s *vma, unsigned long addr, pte_t pte)
{
	unsigned long pfn = pte_pfn(pte);

// 	if (IS_ENABLED(CONFIG_ARCH_HAS_PTE_SPECIAL)) {
// 		if (likely(!pte_special(pte)))
// 			goto check_pfn;
// 		if (vma->vm_ops && vma->vm_ops->find_special_page)
// 			return vma->vm_ops->find_special_page(vma, addr);
// 		if (vma->vm_flags & (VM_PFNMAP | VM_MIXEDMAP))
// 			return NULL;
// 		if (is_zero_pfn(pfn))
// 			return NULL;
// 		if (pte_devmap(pte))
// 			return NULL;

// 		print_bad_pte(vma, addr, pte, NULL);
// 		return NULL;
// 	}

// 	/* !CONFIG_ARCH_HAS_PTE_SPECIAL case follows: */

// 	if (unlikely(vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP))) {
// 		if (vma->vm_flags & VM_MIXEDMAP) {
// 			if (!pfn_valid(pfn))
// 				return NULL;
// 			goto out;
// 		} else {
// 			unsigned long off;
// 			off = (addr - vma->vm_start) >> PAGE_SHIFT;
// 			if (pfn == vma->vm_pgoff + off)
// 				return NULL;
// 			if (!is_cow_mapping(vma->vm_flags))
// 				return NULL;
// 		}
// 	}

// 	if (is_zero_pfn(pfn))
// 		return NULL;

check_pfn:
	if (pfn > highest_memmap_pfn) {
		// print_bad_pte(vma, addr, pte, NULL);
		return NULL;
	}

	/*
	 * NOTE! We still have PageReserved() pages in the page tables.
	 * eg. VDSO mappings can cause them to exist.
	 */
out:
	return pfn_to_page(pfn);
}


/*
 * Copy a present and normal page if necessary.
 *
 * NOTE! The usual case is that this doesn't need to do
 * anything, and can just return a positive value. That
 * will let the caller know that it can just increase
 * the page refcount and re-use the pte the traditional
 * way.
 *
 * But _if_ we need to copy it because it needs to be
 * pinned in the parent (and the child should get its own
 * copy rather than just a reference to the same page),
 * we'll do that here and return zero to let the caller
 * know we're done.
 *
 * And if we need a pre-allocated page but don't yet have
 * one, return a negative error to let the preallocation
 * code know so that it can do so outside the page table
 * lock.
 */
static inline int
copy_present_page(vma_s *dst_vma, vma_s *src_vma, pte_t *dst_pte,
		pte_t *src_pte, unsigned long addr, pte_t pte, page_s *page) {
	page_s *new_page;

	/*
	 * What we want to do is to check whether this page may
	 * have been pinned by the parent process.  If so,
	 * instead of wrprotect the pte on both sides, we copy
	 * the page immediately so that we'll always guarantee
	 * the pinned page won't be randomly replaced in the
	 * future.
	 *
	 * The page pinning checks are just "has this mm ever
	 * seen pinning", along with the (inexact) check of
	 * the page count. That might give false positives for
	 * for pinning, but it will work correctly.
	 */
	// if (likely(!page_needs_cow_for_dma(src_vma, page)))
	// 	return 1;
	// static inline bool
	// page_needs_cow_for_dma(vma_s *vma, page_s *page)
	// {
		if (!is_cow_mapping(src_vma->vm_flags))
			return 1;

		if (!test_bit(MMF_HAS_PINNED, &src_vma->vm_mm->flags))
			return 1;

	// 	return page_maybe_dma_pinned(page);
	// }

	// new_page = *prealloc;
	// if (!new_page)
	// 	return -EAGAIN;

	// /*
	//  * We have a prealloc page, all good!  Take it
	//  * over and copy the page & arm it.
	//  */
	// *prealloc = NULL;
	// copy_user_highpage(new_page, page, addr, src_vma);
	// __SetPageUptodate(new_page);
	// page_add_new_anon_rmap(new_page, dst_vma, addr, false);
	// lru_cache_add_inactive_or_unevictable(new_page, dst_vma);
	// rss[mm_counter(new_page)]++;

	// /* All done, just insert the new page copy in the child */
	// pte = mk_pte(new_page, dst_vma->vm_page_prot);
	// pte = maybe_mkwrite(pte_mkdirty(pte), dst_vma);
	// if (userfaultfd_pte_wp(dst_vma, *src_pte))
	// 	/* Uffd-wp needs to be delivered to dest pte as well */
	// 	pte = pte_wrprotect(pte_mkuffd_wp(pte));
	// set_pte_at(dst_vma->vm_mm, addr, dst_pte, pte);
	return 0;
}

/*
 * Copy one pte.  Returns 0 if succeeded, or -EAGAIN if one preallocated page
 * is required to copy this pte.
 */
static inline int
copy_present_pte(vma_s *dst_vma, vma_s *src_vma,
		pte_t *dst_pte, pte_t *src_pte, unsigned long addr) {
	mm_s *src_mm = src_vma->vm_mm;
	unsigned long vm_flags = src_vma->vm_flags;
	pte_t pte = *src_pte;
	page_s *page;

	page = vm_normal_page(src_vma, addr, pte);
	if (page) {
		int retval;

		retval = copy_present_page(dst_vma, src_vma,
					dst_pte, src_pte, addr, pte, page);
		if (retval <= 0)
			return retval;

		// get_page(page);
		// page_dup_rmap(page, false);
		// rss[mm_counter(page)]++;
	}

	/*
	 * If it's a COW mapping, write protect it both
	 * in the parent and the child
	 */
	if (is_cow_mapping(vm_flags) && pte_writable(pte)) {
		arch_ptep_set_wrprotect(src_pte);
		pte = pte_wrprotect(pte);
	}

	/*
	 * If it's a shared mapping, mark it clean in
	 * the child
	 */
	if (vm_flags & VM_SHARED)
		pte = pte_mkclean(pte);
	pte = pte_mkold(pte);

	// if (!userfaultfd_wp(dst_vma))
	// 	pte = pte_clear_uffd_wp(pte);

	set_pte_at(dst_vma->vm_mm, addr, dst_pte, pte);
	return 0;
}


static int
copy_pte_range(vma_s *dst_vma, vma_s *src_vma, pmd_t *dst_pmd_ent,
		pmd_t *src_pmd_ent, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pte_t *orig_src_pte, *orig_dst_pte;
	pte_t *src_pte, *dst_pte;
	// spinlock_t *src_ptl, *dst_ptl;
	int progress, ret = 0;
	// int rss[NR_MM_COUNTERS];
	// swp_entry_t entry = (swp_entry_t){0};
	// page_s *prealloc = NULL;

again:
	progress = 0;
	// init_rss_vec(rss);

	// dst_pte = pte_alloc_map_lock(dst_mm, dst_pmd_ent, addr, &dst_ptl);
	dst_pte = pte_alloc(dst_mm, dst_pmd_ent, addr);
	// if (!dst_pte) {
	// 	ret = -ENOMEM;
	// 	goto out;
	// }
	src_pte = pte_ent_offset(src_pmd_ent, addr);
	// src_ptl = pte_lockptr(src_mm, src_pmd_ent);
	// spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);
	orig_src_pte = src_pte;
	orig_dst_pte = dst_pte;
	// arch_enter_lazy_mmu_mode();

	do {
		/*
		 * We are holding two locks at this point - either of them
		 * could generate latencies in another task on another CPU.
		 */
		// if (progress >= 32) {
		// 	progress = 0;
		// 	if (need_resched() ||
		// 	    spin_needbreak(src_ptl) || spin_needbreak(dst_ptl))
		// 		break;
		// }
		if (arch_pte_none(*src_pte)) {
			progress++;
			continue;
		}
		if (!arch_pte_present(*src_pte)) {
			// ret = copy_nonpresent_pte(dst_mm, src_mm,
			// 			  dst_pte, src_pte,
			// 			  dst_vma, src_vma,
			// 			  addr, rss);
			// if (ret == -EIO) {
			// 	entry = pte_to_swp_entry(*src_pte);
			// 	break;
			// } else if (ret == -EBUSY) {
			// 	break;
			// } else if (!ret) {
			// 	progress += 8;
				continue;
			// }
			// /*
			//  * Device exclusive entry restored, continue by copying
			//  * the now present pte.
			//  */
			// WARN_ON_ONCE(ret != -ENOENT);
		}
		/* copy_present_pte() will clear `*prealloc' if consumed */
		ret = copy_present_pte(dst_vma, src_vma, dst_pte, src_pte, addr);
		// /*
		//  * If we need a pre-allocated page for this pte, drop the
		//  * locks, allocate, and try again.
		//  */
		// if (unlikely(ret == -EAGAIN))
		// 	break;
		// if (unlikely(prealloc)) {
		// 	/*
		// 	 * pre-alloc page cannot be reused by next time so as
		// 	 * to strictly follow mempolicy (e.g., alloc_page_vma()
		// 	 * will allocate page according to address).  This
		// 	 * could only happen if one pinned pte changed.
		// 	 */
		// 	put_page(prealloc);
		// 	prealloc = NULL;
		// }
		// progress += 8;
	} while (dst_pte++, src_pte++, addr += PAGE_SIZE, addr != end);

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

	// if (addr != end)
	// 	goto again;
out:
	// if (prealloc)
	// 	put_page(prealloc);
	return ret;
}

static inline int
copy_pmd_range(vma_s *dst_vma, vma_s *src_vma, pud_t *dst_pud_ent,
		pud_t *src_pud_ent, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pmd_t *src_pmd_ent, *dst_pmd_ent;
	unsigned long next;

	dst_pmd_ent = pmd_alloc(dst_mm, dst_pud_ent, addr);
	if (!dst_pmd_ent)
		return -ENOMEM;
	src_pmd_ent = pmd_ent_offset(src_pud_ent, addr);
	do {
		next = next_pmd_addr_end(addr, end);
		// if (is_swap_pmd(*src_pmd_ent) || pmd_trans_huge(*src_pmd_ent)
		// 	|| pmd_devmap(*src_pmd_ent)) {
		// 	int err;
		// 	VM_BUG_ON_VMA(next-addr != HPAGE_PMD_SIZE, src_vma);
		// 	err = copy_huge_pmd(dst_mm, src_mm, dst_pmd_ent, src_pmd_ent,
		// 			    addr, dst_vma, src_vma);
		// 	if (err == -ENOMEM)
		// 		return -ENOMEM;
		// 	if (!err)
		// 		continue;
		// 	/* fall through */
		// }
		if (pmd_none_or_clear_bad(src_pmd_ent))
			continue;
		if (copy_pte_range(dst_vma, src_vma, dst_pmd_ent, src_pmd_ent,
				   addr, next))
			return -ENOMEM;
	} while (dst_pmd_ent++, src_pmd_ent++, addr = next, addr != end);
	return 0;
}

static inline int
copy_pud_range(vma_s *dst_vma, vma_s *src_vma, p4d_t *dst_p4d_ent,
		p4d_t *src_p4d_ent, unsigned long addr, unsigned long end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pud_t *src_pud_ent, *dst_pud_ent;
	unsigned long next;

	dst_pud_ent = pud_alloc(dst_mm, dst_p4d_ent, addr);
	if (!dst_pud_ent)
		return -ENOMEM;
	src_pud_ent = pud_ent_offset(src_p4d_ent, addr);
	do {
		next = next_pud_addr_end(addr, end);
		// if (pud_trans_huge(*src_pud_ent) || pud_devmap(*src_pud_ent)) {
		// 	int err;

		// 	VM_BUG_ON_VMA(next-addr != HPAGE_PUD_SIZE, src_vma);
		// 	err = copy_huge_pud(dst_mm, src_mm,
		// 			    dst_pud_ent, src_pud_ent, addr, src_vma);
		// 	if (err == -ENOMEM)
		// 		return -ENOMEM;
		// 	if (!err)
		// 		continue;
		// 	/* fall through */
		// }
		if (pud_none_or_clear_bad(src_pud_ent))
			continue;
		if (copy_pmd_range(dst_vma, src_vma, dst_pud_ent, src_pud_ent,
				   addr, next))
			return -ENOMEM;
	} while (dst_pud_ent++, src_pud_ent++, addr = next, addr != end);
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
		// if (p4d_none_or_clear_bad(src_p4d_ent))
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
	dst_pgd_ent = pgd_ent_offset(dst_mm, addr);
	src_pgd_ent = pgd_ent_offset(src_mm, addr);
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



static gfp_t __get_fault_gfp_mask(vma_s *vma)
{
	file_s *vm_file = vma->vm_file;

	// if (vm_file)
	// 	return mapping_gfp_mask(vm_file->f_mapping) | __GFP_FS | __GFP_IO;

	/*
	 * Special mappings (e.g. VDSO) do not have any file so fake
	 * a default GFP_KERNEL for them.
	 */
	return GFP_KERNEL;
}


/*
 * This routine handles present pages, when users try to write
 * to a shared page. It is done by copying the page to a new address
 * and decrementing the shared-page counter for the old page.
 *
 * Note that this routine assumes that the protection checks have been
 * done by the caller (the low-level page fault routine in most cases).
 * Thus we can safely just mark it writable once we've done any necessary
 * COW.
 *
 * We also mark the page dirty at this point even though the page will
 * change only once the write actually happens. This avoids a few races,
 * and potentially makes it more efficient.
 *
 * We enter with non-exclusive mmap_lock (to exclude vma changes,
 * but allow concurrent faults), with pte both mapped and locked.
 * We return with mmap_lock still held, but pte unmapped and unlocked.
 */
static vm_fault_t do_wp_page(vm_fault_s *vmf) __releases(vmf->ptl)
{
	vma_s *vma = vmf->vma;

// 	if (userfaultfd_pte_wp(vma, *vmf->pte)) {
// 		pte_unmap_unlock(vmf->pte, vmf->ptl);
// 		return handle_userfault(vmf, VM_UFFD_WP);
// 	}

// 	/*
// 	 * Userfaultfd write-protect can defer flushes. Ensure the TLB
// 	 * is flushed in this case before copying.
// 	 */
// 	if (unlikely(userfaultfd_wp(vmf->vma) &&
// 		     mm_tlb_flush_pending(vmf->vma->vm_mm)))
// 		flush_tlb_page(vmf->vma, vmf->address);

// 	vmf->page = vm_normal_page(vma, vmf->address, vmf->orig_pte);
// 	if (!vmf->page) {
// 		/*
// 		 * VM_MIXEDMAP !pfn_valid() case, or VM_SOFTDIRTY clear on a
// 		 * VM_PFNMAP VMA.
// 		 *
// 		 * We should not cow pages in a shared writeable mapping.
// 		 * Just mark the pages writable and/or call ops->pfn_mkwrite.
// 		 */
// 		if ((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
// 				     (VM_WRITE|VM_SHARED))
// 			return wp_pfn_shared(vmf);

// 		pte_unmap_unlock(vmf->pte, vmf->ptl);
// 		return wp_page_copy(vmf);
// 	}

// 	/*
// 	 * Take out anonymous pages first, anonymous shared vmas are
// 	 * not dirty accountable.
// 	 */
// 	if (PageAnon(vmf->page)) {
// 		struct page *page = vmf->page;

// 		/* PageKsm() doesn't necessarily raise the page refcount */
// 		if (PageKsm(page) || page_count(page) != 1)
// 			goto copy;
// 		if (!trylock_page(page))
// 			goto copy;
// 		if (PageKsm(page) || page_mapcount(page) != 1 || page_count(page) != 1) {
// 			unlock_page(page);
// 			goto copy;
// 		}
// 		/*
// 		 * Ok, we've got the only map reference, and the only
// 		 * page count reference, and the page is locked,
// 		 * it's dark out, and we're wearing sunglasses. Hit it.
// 		 */
// 		unlock_page(page);
// 		wp_page_reuse(vmf);
// 		return VM_FAULT_WRITE;
// 	} else if (unlikely((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
// 					(VM_WRITE|VM_SHARED))) {
// 		return wp_page_shared(vmf);
// 	}
// copy:
// 	/*
// 	 * Ok, we need to copy. Oh, well..
// 	 */
// 	get_page(vmf->page);

// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
// 	return wp_page_copy(vmf);
}


/*
 * These routines also need to handle stuff like marking pages dirty
 * and/or accessed for architectures that don't do it in hardware (most
 * RISC architectures).  The early dirtying is also good on the i386.
 *
 * There is also a hook called "update_mmu_cache()" that architectures
 * with external mmu caches can use to update those (ie the Sparc or
 * PowerPC hashed page tables that act as extended TLBs).
 *
 * We enter with non-exclusive mmap_lock (to exclude vma changes, but allow
 * concurrent faults).
 *
 * The mmap_lock may have been released depending on flags and our return value.
 * See filemap_fault() and __folio_lock_or_retry().
 */
// static vm_fault_t handle_pte_fault(vm_fault_s *vmf)
static vm_fault_t myos_handle_pte_fault(vm_fault_s *vmf)
{
	// vmf->ptl = pte_lockptr(vmf->vma->vm_mm, vmf->pmd);
	// spin_lock(vmf->ptl);
	// entry = vmf->orig_pte;
	// if (unlikely(!pte_same(*vmf->pte, entry))) {
	// 	update_mmu_tlb(vmf->vma, vmf->address, vmf->pte);
	// 	goto unlock;
	// }
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_writable(*vmf->pte))
			return do_wp_page(vmf);
		// entry = pte_mkdirty(entry);
	}
}

/*
 * By the time we get here, we already hold the mm semaphore
 *
 * The mmap_lock may have been released depending on flags and our
 * return value.  See filemap_fault() and __folio_lock_or_retry().
 */
// vm_fault_t handle_mm_fault(vma_s *vma, unsigned long address,
// 		unsigned int flags, pt_regs_s *regs)
vm_fault_t myos_handle_mm_fault(vma_s *vma, pt_regs_s *regs,
		unsigned long address, unsigned int flags)
{
	vm_fault_t ret;
	__set_current_state(TASK_RUNNING);

	vm_fault_s vmf = {
		.vma		= vma,
		.address	= address & PAGE_MASK,
		.flags		= flags,
		.pgoff		= linear_page_index(vma, address),
		.gfp_mask	= __get_fault_gfp_mask(vma),
	};
	unsigned int dirty = flags & FAULT_FLAG_WRITE;
	mm_s *mm = vma->vm_mm;
	pgd_t *pgd = pgd_ent_offset(mm, address);

	vmf.p4d = p4d_alloc(mm, pgd, address);
	if (!vmf.p4d)
	{
		ret = VM_FAULT_OOM;
		goto fail;
	}
	vmf.pud = pud_alloc(mm, vmf.p4d, address);
	if (!vmf.pud)
	{
		ret = VM_FAULT_OOM;
		goto fail;
	}
	vmf.pmd = pmd_alloc(mm, vmf.pud, address);
	if (!vmf.pmd)
	{
		ret = VM_FAULT_OOM;
		goto fail;
	}
	vmf.pte = pte_alloc(mm, vmf.pmd, address);
	if (!vmf.pmd)
	{
		ret = VM_FAULT_OOM;
		goto fail;
	}
	barrier();

	ret = myos_handle_pte_fault(&vmf);
fail:
	return ret;
}


/*
 * Allocate page upper directory.
 * We've already handled the fast-path in-line.
 */
// int __pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address)
int __myos_pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address)
{
	pud_t *new = pud_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	// spin_lock(&mm->page_table_lock);
	if (!arch_p4d_present(*p4d)) {
		smp_wmb(); /* See comment in pmd_install() */
		*p4d = arch_make_p4d(_PAGE_TABLE | __pa(new));
	} else	/* Another has populated it */
		pud_free(new);
	// spin_unlock_no_resched(&mm->page_table_lock);
	return 0;
}

/*
 * Allocate page middle directory.
 * We've already handled the fast-path in-line.
 */
// int __pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address)
int __myos_pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address)
{
	pmd_t *new = pmd_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	// spin_lock(&mm->page_table_lock);
	if (!arch_pud_present(*pud)) {
		smp_wmb(); /* See comment in pmd_install() */
		*pud = arch_make_pud(_PAGE_TABLE | __pa(new));
	} else {	/* Another has populated it */
		pmd_free(new);
	}
	// spin_unlock_no_resched(&mm->page_table_lock);
	return 0;
}

int __myos_pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address)
{
	pte_t *new = pte_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	// spin_lock(&mm->page_table_lock);
	if (!arch_pmd_present(*pmd)) {
		smp_wmb(); /* See comment in pmd_install() */
		*pmd = arch_make_pmd(_PAGE_TABLE | __pa(new));
	} else {	/* Another has populated it */
		pte_free(new);
	}
	// spin_unlock_no_resched(&mm->page_table_lock);
	return 0;
}