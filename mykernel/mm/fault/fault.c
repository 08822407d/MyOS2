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
#include <linux/kernel/sched_const.h>
#include <linux/sched/coredump.h>
#include <asm-generic/pgalloc.h>

#include "../mm_types.h"

ulong	max_mapnr;
page_s	*mem_map;
/*
 * A number of key systems in x86 including ioremap() rely on the assumption
 * that high_memory defines the upper bound on direct map memory, then end
 * of ZONE_NORMAL.  Under CONFIG_DISCONTIG this means that max_low_pfn and
 * highstart_pfn must be the same; there must be no gap between ZONE_NORMAL
 * and ZONE_HIGHMEM.
 */
void	*high_memory;
ulong	highest_memmap_pfn __read_mostly;


/*
 * vm_normal_page -- This function gets the "page_s" associated with a pte.
 *
 * "Special" mappings do not wish to be associated with a "page_s" (either
 * it doesn't exist, or it exists but they don't want to touch it). In this
 * case, NULL is returned here. "Normal" mappings do have a page_s.
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
page_s *vm_normal_page(vma_s *vma, ulong addr, pte_t pte)
{
	ulong pfn = pte_pfn(pte);

	// if (IS_ENABLED(CONFIG_ARCH_HAS_PTE_SPECIAL)) {
	// 	if (likely(!pte_special(pte)))
	// 		goto check_pfn;
	// 	if (vma->vm_ops && vma->vm_ops->find_special_page)
	// 		return vma->vm_ops->find_special_page(vma, addr);
	// 	if (vma->vm_flags & (VM_PFNMAP | VM_MIXEDMAP))
	// 		return NULL;
	// 	if (is_zero_pfn(pfn))
	// 		return NULL;
	// 	if (pte_devmap(pte))
	// 		return NULL;

	// 	print_bad_pte(vma, addr, pte, NULL);
	// 	return NULL;
	// }

	// /* !CONFIG_ARCH_HAS_PTE_SPECIAL case follows: */

	// if (unlikely(vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP))) {
	// 	if (vma->vm_flags & VM_MIXEDMAP) {
	// 		if (!pfn_valid(pfn))
	// 			return NULL;
	// 		goto out;
	// 	} else {
	// 		unsigned long off;
	// 		off = (addr - vma->vm_start) >> PAGE_SHIFT;
	// 		if (pfn == vma->vm_pgoff + off)
	// 			return NULL;
	// 		if (!is_cow_mapping(vma->vm_flags))
	// 			return NULL;
	// 	}
	// }

	// if (is_zero_pfn(pfn))
	// 	return NULL;

check_pfn:
	if (unlikely(pfn > highest_memmap_pfn)) {
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
		pte_t *src_pte, ulong addr, pte_t pte, page_s *page) {
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
		pte_t *dst_pte, pte_t *src_pte, ulong addr) {
	mm_s *src_mm = src_vma->vm_mm;
	ulong vm_flags = src_vma->vm_flags;
	pte_t pte = *src_pte;
	page_s *page;

	page = vm_normal_page(src_vma, addr, pte);
	if (page) {
		int retval;

		retval = copy_present_page(dst_vma, src_vma,
					dst_pte, src_pte, addr, pte, page);
		if (retval <= 0)
			return retval;

		get_page(page);
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
copy_pte_range(vma_s *dst_vma, vma_s *src_vma, pmd_t
		*dst_pmd_ent, pmd_t *src_pmd_ent, ulong addr, ulong end) {

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
		if (unlikely(!arch_pte_present(*src_pte))) {
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
		pud_t *src_pud_ent, ulong addr, ulong end) {
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pmd_t *src_pmd_ent, *dst_pmd_ent;
	ulong next;

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
		p4d_t *src_p4d_ent, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pud_t *src_pud_ent, *dst_pud_ent;
	ulong next;

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
		pgd_t *src_pgd_ent, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	p4d_t *src_p4d_ent, *dst_p4d_ent;
	ulong next;

	dst_p4d_ent = p4d_alloc(dst_mm, dst_pgd_ent, addr);
	if (!dst_p4d_ent)
		return -ENOMEM;
	src_p4d_ent = p4d_ent_offset(src_pgd_ent, addr);
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
	ulong next;
	ulong addr = src_vma->vm_start;
	ulong end = src_vma->vm_end;
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
		if (unlikely(copy_p4d_range(dst_vma, src_vma,
				dst_pgd_ent, src_pgd_ent, addr, next))) {
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


static inline bool
cow_user_page(page_s *dst, page_s *src, vm_fault_s *vmf) {
	bool ret;
	// void *kaddr;
	// void __user *uaddr;
	// bool locked = false;
	vma_s *vma = vmf->vma;
	mm_s *mm = vma->vm_mm;
	ulong addr = vmf->address;

	if (likely(src)) {
		// copy_user_highpage(dst, src, addr, vma);
		copy_user_page((void *)page_to_virt(dst),
				(void *)page_to_virt(src), addr, dst);
		return true;
	}

	// /*
	//  * If the source page was a PFN mapping, we don't have
	//  * a "page_s" for it. We do a best-effort copy by
	//  * just copying from the original user address. If that
	//  * fails, we just zero-fill it. Live with it.
	//  */
	// kaddr = kmap_atomic(dst);
	// uaddr = (void __user *)(addr & PAGE_MASK);

	// /*
	//  * On architectures with software "accessed" bits, we would
	//  * take a double page fault, so mark it accessed here.
	//  */
	// if (arch_faults_on_old_pte() && !pte_young(vmf->orig_pte)) {
	// 	pte_t entry;

	// 	vmf->pte = pte_offset_map_lock(mm, vmf->pmd, addr, &vmf->ptl);
	// 	locked = true;
	// 	if (!likely(pte_same(*vmf->pte, vmf->orig_pte))) {
	// 		/*
	// 		 * Other thread has already handled the fault
	// 		 * and update local tlb only
	// 		 */
	// 		update_mmu_tlb(vma, addr, vmf->pte);
	// 		ret = false;
	// 		goto pte_unlock;
	// 	}

	// 	entry = pte_mkyoung(vmf->orig_pte);
	// 	if (ptep_set_access_flags(vma, addr, vmf->pte, entry, 0))
	// 		update_mmu_cache(vma, addr, vmf->pte);
	// }

	// /*
	//  * This really shouldn't fail, because the page is there
	//  * in the page tables. But it might just be unreadable,
	//  * in which case we just give up and fill the result with
	//  * zeroes.
	//  */
	// if (__copy_from_user_inatomic(kaddr, uaddr, PAGE_SIZE)) {
	// 	if (locked)
	// 		goto warn;

	// 	/* Re-validate under PTL if the page is still mapped */
	// 	vmf->pte = pte_offset_map_lock(mm, vmf->pmd, addr, &vmf->ptl);
	// 	locked = true;
	// 	if (!likely(pte_same(*vmf->pte, vmf->orig_pte))) {
	// 		/* The PTE changed under us, update local tlb */
	// 		update_mmu_tlb(vma, addr, vmf->pte);
	// 		ret = false;
	// 		goto pte_unlock;
	// 	}

	// 	/*
	// 	 * The same page can be mapped back since last copy attempt.
	// 	 * Try to copy again under PTL.
	// 	 */
	// 	if (__copy_from_user_inatomic(kaddr, uaddr, PAGE_SIZE)) {
	// 		/*
	// 		 * Give a warn in case there can be some obscure
	// 		 * use-case
	// 		 */
// warn:
	// 		WARN_ON_ONCE(1);
	// 		clear_page(kaddr);
	// 	}
	// }

	ret = true;

pte_unlock:
	// if (locked)
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// kunmap_atomic(kaddr);
	// flush_dcache_page(dst);

	return ret;
}

static gfp_t
__get_fault_gfp_mask(vma_s *vma) {
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
 * Handle the case of a page which we actually need to copy to a new page.
 *
 * Called with mmap_lock locked and the old page referenced, but
 * without the ptl held.
 *
 * High level logic flow:
 *
 * - Allocate a page, copy the content of the old page to the new one.
 * - Handle book keeping and accounting - cgroups, mmu-notifiers, etc.
 * - Take the PTL. If the pte changed, bail out and release the allocated page
 * - If the pte is still the way we remember it, update the page table and all
 *   relevant references. This includes dropping the reference the page-table
 *   held to the old page, as well as updating the rmap.
 * - In any case, unlock the PTL and drop the reference we took to the old page.
 */
static vm_fault_t
wp_page_copy(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	mm_s *mm = vma->vm_mm;
	page_s *old_page = vmf->page;
	page_s *new_page = NULL;
	pte_t entry;
	int page_copied = 0;
	// struct mmu_notifier_range range;

	// if (unlikely(anon_vma_prepare(vma)))
	// 	goto oom;

	// if (is_zero_pfn(pte_pfn(vmf->orig_pte))) {
	// 	new_page = alloc_zeroed_user_highpage_movable(vma,
	// 						      vmf->address);
	// 	if (!new_page)
	// 		goto oom;
	// } else {
	// 	new_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma,
	// 			vmf->address);
		new_page = alloc_page(GFP_HIGHUSER_MOVABLE);
		if (!new_page)
			goto oom;

		if (!cow_user_page(new_page, old_page, vmf)) {
			/*
			 * COW failed, if the fault was solved by other,
			 * it's fine. If not, userspace would re-fault on
			 * the same address and we will handle the fault
			 * from the second attempt.
			 */
			put_page(new_page);
			if (old_page)
				put_page(old_page);
			return 0;
		}
	// }

	// if (mem_cgroup_charge(page_folio(new_page), mm, GFP_KERNEL))
	// 	goto oom_free_new;
	// cgroup_throttle_swaprate(new_page, GFP_KERNEL);

	// __SetPageUptodate(new_page);

	// mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma, mm,
	// 			vmf->address & PAGE_MASK,
	// 			(vmf->address & PAGE_MASK) + PAGE_SIZE);
	// mmu_notifier_invalidate_range_start(&range);

	/*
	 * Re-check the pte - we dropped the lock
	 */
	// vmf->pte = pte_offset_map_lock(mm, vmf->pmd, vmf->address, &vmf->ptl);
	if (likely(pte_same(*vmf->pte, vmf->orig_pte))) {
		// if (old_page) {
		// 	if (!PageAnon(old_page)) {
		// 		dec_mm_counter_fast(mm,
		// 				mm_counter_file(old_page));
		// 		inc_mm_counter_fast(mm, MM_ANONPAGES);
		// 	}
		// } else {
		// 	inc_mm_counter_fast(mm, MM_ANONPAGES);
		// }
		// flush_cache_page(vma, vmf->address, pte_pfn(vmf->orig_pte));
		// entry = mk_pte(new_page, vma->vm_page_prot);
		entry = arch_make_pte(page_to_phys(new_page) |
					_PAGE_ACCESSED |  _PAGE_PRESENT | _PAGE_USER);
		// entry = pte_sw_mkyoung(entry);
		entry = maybe_mkwrite(pte_mkdirty(entry), vma);

		// /*
		//  * Clear the pte entry and flush it first, before updating the
		//  * pte with the new entry, to keep TLBs on different CPUs in
		//  * sync. This code used to set the new PTE then flush TLBs, but
		//  * that left a window where the new PTE could be loaded into
		//  * some TLBs while the old PTE remains in others.
		//  */
		// ptep_clear_flush_notify(vma, vmf->address, vmf->pte);
		// page_add_new_anon_rmap(new_page, vma, vmf->address, false);
		// lru_cache_add_inactive_or_unevictable(new_page, vma);
		// /*
		//  * We call the notify macro here because, when using secondary
		//  * mmu page tables (such as kvm shadow page tables), we want the
		//  * new page to be mapped directly into the secondary page table.
		//  */
		// set_pte_at_notify(mm, vmf->address, vmf->pte, entry);
		set_pte_at(mm, vmf->address, vmf->pte, entry);
		// update_mmu_cache(vma, vmf->address, vmf->pte);
		// if (old_page) {
		// 	/*
		// 	 * Only after switching the pte to the new page may
		// 	 * we remove the mapcount here. Otherwise another
		// 	 * process may come and find the rmap count decremented
		// 	 * before the pte is switched to the new page, and
		// 	 * "reuse" the old page writing into it while our pte
		// 	 * here still points into it and can be read by other
		// 	 * threads.
		// 	 *
		// 	 * The critical issue is to order this
		// 	 * page_remove_rmap with the ptp_clear_flush above.
		// 	 * Those stores are ordered by (if nothing else,)
		// 	 * the barrier present in the atomic_add_negative
		// 	 * in page_remove_rmap.
		// 	 *
		// 	 * Then the TLB flush in ptep_clear_flush ensures that
		// 	 * no process can access the old page before the
		// 	 * decremented mapcount is visible. And the old page
		// 	 * cannot be reused until after the decremented
		// 	 * mapcount is visible. So transitively, TLBs to
		// 	 * old page will be flushed before it can be reused.
		// 	 */
		// 	page_remove_rmap(old_page, false);
		// }

		/* Free the old page.. */
		new_page = old_page;
		page_copied = 1;
	} else {
		// update_mmu_tlb(vma, vmf->address, vmf->pte);
	}

	if (new_page)
		put_page(new_page);

	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	/*
	 * No need to double call mmu_notifier->invalidate_range() callback as
	 * the above ptep_clear_flush_notify() did already call it.
	 */
	// mmu_notifier_invalidate_range_only_end(&range);
	if (old_page) {
		// /*
		//  * Don't let another task, with possibly unlocked vma,
		//  * keep the mlocked page.
		//  */
		// if (page_copied && (vma->vm_flags & VM_LOCKED)) {
		// 	lock_page(old_page);	/* LRU manipulation */
		// 	if (PageMlocked(old_page))
		// 		munlock_vma_page(old_page);
		// 	unlock_page(old_page);
		// }
		// if (page_copied)
		// 	free_swap_cache(old_page);
		put_page(old_page);
	}
	return 0;
oom_free_new:
	put_page(new_page);
oom:
	if (old_page)
		put_page(old_page);
	return VM_FAULT_OOM;
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
static vm_fault_t
do_wp_page(vm_fault_s *vmf) __releases(vmf->ptl) {
	vma_s *vma = vmf->vma;

	// if (userfaultfd_pte_wp(vma, *vmf->pte)) {
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 	return handle_userfault(vmf, VM_UFFD_WP);
	// }

	// /*
	//  * Userfaultfd write-protect can defer flushes. Ensure the TLB
	//  * is flushed in this case before copying.
	//  */
	// if (unlikely(userfaultfd_wp(vmf->vma) &&
	// 	     mm_tlb_flush_pending(vmf->vma->vm_mm)))
	// 	flush_tlb_page(vmf->vma, vmf->address);

	vmf->page = vm_normal_page(vma, vmf->address, *vmf->pte);
	// if (!vmf->page) {
	// 	/*
	// 	 * VM_MIXEDMAP !pfn_valid() case, or VM_SOFTDIRTY clear on a
	// 	 * VM_PFNMAP VMA.
	// 	 *
	// 	 * We should not cow pages in a shared writeable mapping.
	// 	 * Just mark the pages writable and/or call ops->pfn_mkwrite.
	// 	 */
	// 	if ((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
	// 			     (VM_WRITE|VM_SHARED))
	// 		return wp_pfn_shared(vmf);

	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 	return wp_page_copy(vmf);
	// }

	// /*
	//  * Take out anonymous pages first, anonymous shared vmas are
	//  * not dirty accountable.
	//  */
	// if (PageAnon(vmf->page)) {
	// 	page_s *page = vmf->page;

	// 	/* PageKsm() doesn't necessarily raise the page refcount */
	// 	if (PageKsm(page) || page_count(page) != 1)
	// 		goto copy;
	// 	if (!trylock_page(page))
	// 		goto copy;
	// 	if (PageKsm(page) || page_mapcount(page) != 1 || page_count(page) != 1) {
	// 		unlock_page(page);
	// 		goto copy;
	// 	}
	// 	/*
	// 	 * Ok, we've got the only map reference, and the only
	// 	 * page count reference, and the page is locked,
	// 	 * it's dark out, and we're wearing sunglasses. Hit it.
	// 	 */
	// 	unlock_page(page);
	// 	wp_page_reuse(vmf);
	// 	return VM_FAULT_WRITE;
	// } else if (unlikely((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
	// 				(VM_WRITE|VM_SHARED))) {
	// 	return wp_page_shared(vmf);
	// }
copy:
	/*
	 * Ok, we need to copy. Oh, well..
	 */
	get_page(vmf->page);

	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return wp_page_copy(vmf);
}


/*
 * We enter with non-exclusive mmap_lock (to exclude vma changes,
 * but allow concurrent faults), and pte mapped but not yet locked.
 * We return with mmap_lock still held, but pte unmapped and unlocked.
 */
static vm_fault_t
do_anonymous_page(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	page_s *page;
	vm_fault_t ret = 0;
	pte_t entry;

	/* File mapping without ->vm_ops ? */
	if (vma->vm_flags & VM_SHARED)
		return VM_FAULT_SIGBUS;

	/*
	 * Use pte_alloc() instead of pte_alloc_map().  We can't run
	 * pte_offset_map() on pmds where a huge pmd might be created
	 * from a different thread.
	 *
	 * pte_alloc_map() is safe to use under mmap_write_lock(mm) or when
	 * parallel threads are excluded by other means.
	 *
	 * Here we only have mmap_read_lock(mm).
	 */
	// if (pte_alloc(vma->vm_mm, vmf->pmd))
	if (vmf->pte == NULL)
		return VM_FAULT_OOM;

	// /* See comment in handle_pte_fault() */
	// if (unlikely(pmd_trans_unstable(vmf->pmd)))
	// 	return 0;

	// /* Use the zero-page for reads */
	// if (!(vmf->flags & FAULT_FLAG_WRITE) &&
	// 		!mm_forbids_zeropage(vma->vm_mm)) {
	// 	entry = pte_mkspecial(pfn_pte(my_zero_pfn(vmf->address),
	// 					vma->vm_page_prot));
	// 	vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd,
	// 			vmf->address, &vmf->ptl);
	// 	if (!pte_none(*vmf->pte)) {
	// 		update_mmu_tlb(vma, vmf->address, vmf->pte);
	// 		goto unlock;
	// 	}
	// 	ret = check_stable_address_space(vma->vm_mm);
	// 	if (ret)
	// 		goto unlock;
	// 	/* Deliver the page fault to userland, check inside PT lock */
	// 	if (userfaultfd_missing(vma)) {
	// 		pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 		return handle_userfault(vmf, VM_UFFD_MISSING);
	// 	}
	// 	goto setpte;
	// }

	// /* Allocate our own private page. */
	// if (unlikely(anon_vma_prepare(vma)))
	// 	goto oom;
	// page = alloc_zeroed_user_highpage_movable(vma, vmf->address);
	page = alloc_page(GFP_USER);
	if (!page)
		goto oom;

	// if (mem_cgroup_charge(page_folio(page), vma->vm_mm, GFP_KERNEL))
	// 	goto oom_free_page;
	// cgroup_throttle_swaprate(page, GFP_KERNEL);

	// /*
	//  * The memory barrier inside __SetPageUptodate makes sure that
	//  * preceding stores to the page contents become visible before
	//  * the set_pte_at() write.
	//  */
	// __SetPageUptodate(page);

	// entry = mk_pte(page, vma->vm_page_prot);
	entry = arch_make_pte(page_to_phys(page) |
				_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT);
	// entry = pte_sw_mkyoung(entry);
	if (vma->vm_flags & VM_WRITE)
		entry = pte_mkwrite(pte_mkdirty(entry));

	// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd, vmf->address,
	// 		&vmf->ptl);
	*vmf->pte = entry;
	if (!arch_pte_none(*vmf->pte)) {
		// update_mmu_cache(vma, vmf->address, vmf->pte);
		goto release;
	}

	// ret = check_stable_address_space(vma->vm_mm);
	// if (ret)
	// 	goto release;

	// /* Deliver the page fault to userland, check inside PT lock */
	// if (userfaultfd_missing(vma)) {
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 	put_page(page);
	// 	return handle_userfault(vmf, VM_UFFD_MISSING);
	// }

	// inc_mm_counter_fast(vma->vm_mm, MM_ANONPAGES);
	// page_add_new_anon_rmap(page, vma, vmf->address, false);
	// lru_cache_add_inactive_or_unevictable(page, vma);
setpte:
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte, entry);

	/* No need to invalidate - it was non-present before */
	// update_mmu_cache(vma, vmf->address, vmf->pte);
unlock:
	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return ret;
release:
	put_page(page);
	goto unlock;
oom_free_page:
	put_page(page);
oom:
	return VM_FAULT_OOM;
}

/*
 * The mmap_lock must have been held on entry, and may have been
 * released depending on flags and vma->vm_ops->fault() return value.
 * See filemap_fault() and __lock_page_retry().
 */
static vm_fault_t
__do_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	vm_fault_t ret;

	// /*
	//  * Preallocate pte before we take page_lock because this might lead to
	//  * deadlocks for memcg reclaim which waits for pages under writeback:
	//  *				lock_page(A)
	//  *				SetPageWriteback(A)
	//  *				unlock_page(A)
	//  * lock_page(B)
	//  *				lock_page(B)
	//  * pte_alloc_one
	//  *   shrink_page_list
	//  *     wait_on_page_writeback(A)
	//  *				SetPageWriteback(B)
	//  *				unlock_page(B)
	//  *				# flush A, B to clear the writeback
	//  */
	// if (pmd_none(*vmf->pmd) && !vmf->prealloc_pte) {
	// 	vmf->prealloc_pte = pte_alloc_one(vma->vm_mm);
	// 	if (!vmf->prealloc_pte)
	// 		return VM_FAULT_OOM;
	// }

	ret = vma->vm_ops->fault(vmf);
	// if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY |
	// 		    VM_FAULT_DONE_COW)))
	// 	return ret;

	// if (unlikely(PageHWPoison(vmf->page))) {
	// 	page_s *page = vmf->page;
	// 	vm_fault_t poisonret = VM_FAULT_HWPOISON;
	// 	if (ret & VM_FAULT_LOCKED) {
	// 		if (page_mapped(page))
	// 			unmap_mapping_pages(page_mapping(page),
	// 					    page->index, 1, false);
	// 		/* Retry if a clean page was removed from the cache. */
	// 		if (invalidate_inode_page(page))
	// 			poisonret = VM_FAULT_NOPAGE;
	// 		unlock_page(page);
	// 	}
	// 	put_page(page);
	// 	vmf->page = NULL;
	// 	return poisonret;
	// }

	// if (unlikely(!(ret & VM_FAULT_LOCKED)))
	// 	lock_page(vmf->page);
	// else
	// 	VM_BUG_ON_PAGE(!PageLocked(vmf->page), vmf->page);

	// return ret;
}


/**
 * finish_fault - finish page fault once we have prepared the page to fault
 *
 * @vmf: structure describing the fault
 *
 * This function handles all that is needed to finish a page fault once the
 * page to fault in is prepared. It handles locking of PTEs, inserts PTE for
 * given page, adds reverse page mapping, handles memcg charges and LRU
 * addition.
 *
 * The function expects the page to be locked and on success it consumes a
 * reference of a page being mapped (for the PTE which maps it).
 *
 * Return: %0 on success, %VM_FAULT_ code in case of error.
 */
vm_fault_t finish_fault(vm_fault_s *vmf)
{
	vma_s *vma = vmf->vma;
	page_s *page;
	vm_fault_t ret;
	ulong page_addr = 0;

	/* Did we COW the page? */
	if ((vmf->flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED))
		page = vmf->cow_page;
	else
		page = vmf->page;

	page_addr = page_to_phys(page);

	// /*
	//  * check even for read faults because we might have lost our CoWed
	//  * page
	//  */
	// if (!(vma->vm_flags & VM_SHARED)) {
	// 	ret = check_stable_address_space(vma->vm_mm);
	// 	if (ret)
	// 		return ret;
	// }

	// if (pmd_none(*vmf->pmd)) {
	// 	if (PageTransCompound(page)) {
	// 		ret = do_set_pmd(vmf, page);
	// 		if (ret != VM_FAULT_FALLBACK)
	// 			return ret;
	// 	}

	// 	if (vmf->prealloc_pte)
	// 		pmd_install(vma->vm_mm, vmf->pmd, &vmf->prealloc_pte);
	// 	else if (unlikely(pte_alloc(vma->vm_mm, vmf->pmd)))
	// 		return VM_FAULT_OOM;
	// }

	// /*
	//  * See comment in handle_pte_fault() for how this scenario happens, we
	//  * need to return NOPAGE so that we drop this page.
	//  */
	// if (pmd_devmap_trans_unstable(vmf->pmd))
	// 	return VM_FAULT_NOPAGE;

	// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd,
	// 			      vmf->address, &vmf->ptl);
	*vmf->pte = arch_make_pte(PAGE_SHARED_EXEC | _PAGE_PAT | page_addr);
	ret = 0;
	// /* Re-check under ptl */
	// if (likely(!vmf_pte_changed(vmf))) {
	// 	do_set_pte(vmf, page, vmf->address);

	// 	/* no need to invalidate: a not-present page won't be cached */
	// 	update_mmu_cache(vma, vmf->address, vmf->pte);

	// 	ret = 0;
	// } else {
	// 	update_mmu_tlb(vma, vmf->address, vmf->pte);
	// 	ret = VM_FAULT_NOPAGE;
	// }

	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return ret;
}


/*
 * do_fault_around() tries to map few pages around the fault address. The hope
 * is that the pages will be needed soon and this will lower the number of
 * faults to handle.
 *
 * It uses vm_ops->map_pages() to map the pages, which skips the page if it's
 * not ready to be mapped: not up-to-date, locked, etc.
 *
 * This function is called with the page table lock taken. In the split ptlock
 * case the page table lock only protects only those entries which belong to
 * the page table corresponding to the fault address.
 *
 * This function doesn't cross the VMA boundaries, in order to call map_pages()
 * only once.
 *
 * fault_around_bytes defines how many bytes we'll try to map.
 * do_fault_around() expects it to be set to a power of two less than or equal
 * to PTRS_PER_PTE.
 *
 * The virtual address of the area that we map is naturally aligned to
 * fault_around_bytes rounded down to the machine page size
 * (and therefore to page order).  This way it's easier to guarantee
 * that we don't cross page table boundaries.
 */
static vm_fault_t
do_fault_around(vm_fault_s *vmf) {
	// unsigned long address = vmf->address, nr_pages, mask;
	// pgoff_t start_pgoff = vmf->pgoff;
	// pgoff_t end_pgoff;
	// int off;

	// nr_pages = READ_ONCE(fault_around_bytes) >> PAGE_SHIFT;
	// mask = ~(nr_pages * PAGE_SIZE - 1) & PAGE_MASK;

	// address = max(address & mask, vmf->vma->vm_start);
	// off = ((vmf->address - address) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
	// start_pgoff -= off;

	// /*
	//  *  end_pgoff is either the end of the page table, the end of
	//  *  the vma or nr_pages from start_pgoff, depending what is nearest.
	//  */
	// end_pgoff = start_pgoff -
	// 	((address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1)) +
	// 	PTRS_PER_PTE - 1;
	// end_pgoff = min3(end_pgoff, vma_pages(vmf->vma) + vmf->vma->vm_pgoff - 1,
	// 		start_pgoff + nr_pages - 1);

	// if (pmd_none(*vmf->pmd)) {
	// 	vmf->prealloc_pte = pte_alloc_one(vmf->vma->vm_mm);
	// 	if (!vmf->prealloc_pte)
	// 		return VM_FAULT_OOM;
	// }

	// return vmf->vma->vm_ops->map_pages(vmf, start_pgoff, end_pgoff);
}

static vm_fault_t
do_read_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	vm_fault_t ret = 0;

	// /*
	//  * Let's call ->map_pages() first and use ->fault() as fallback
	//  * if page by the offset is not ready to be mapped (cold cache or
	//  * something).
	//  */
	// if (vma->vm_ops->map_pages && fault_around_bytes >> PAGE_SHIFT > 1) {
	// 	if (likely(!userfaultfd_minor(vmf->vma))) {
	// 		ret = do_fault_around(vmf);
	// 		if (ret)
	// 			return ret;
	// 	}
	// }

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		put_page(vmf->page);
	return ret;
}

static vm_fault_t
do_cow_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	vm_fault_t ret;

	// if (unlikely(anon_vma_prepare(vma)))
	// 	return VM_FAULT_OOM;

	// vmf->cow_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, vmf->address);
	vmf->cow_page = alloc_page(GFP_USER);
	if (!vmf->cow_page)
		return VM_FAULT_OOM;

	// if (mem_cgroup_charge(page_folio(vmf->cow_page), vma->vm_mm,
	// 			GFP_KERNEL)) {
	// 	put_page(vmf->cow_page);
	// 	return VM_FAULT_OOM;
	// }
	// cgroup_throttle_swaprate(vmf->cow_page, GFP_KERNEL);

	ret = __do_fault(vmf);
	// if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
	// 	goto uncharge_out;
	// if (ret & VM_FAULT_DONE_COW)
	// 	return ret;

	// copy_user_highpage(vmf->cow_page, vmf->page, vmf->address, vma);
	// __SetPageUptodate(vmf->cow_page);

	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
	// put_page(vmf->page);
	// if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
	// 	goto uncharge_out;
	return ret;
uncharge_out:
	put_page(vmf->cow_page);
	return ret;
}

static vm_fault_t
do_shared_fault(vm_fault_s *vmf) {
	// vma_s *vma = vmf->vma;
	// vm_fault_t ret, tmp;

	// ret = __do_fault(vmf);
	// if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
	// 	return ret;

	// /*
	//  * Check if the backing address space wants to know that the page is
	//  * about to become writable
	//  */
	// if (vma->vm_ops->page_mkwrite) {
	// 	unlock_page(vmf->page);
	// 	tmp = do_page_mkwrite(vmf);
	// 	if (unlikely(!tmp ||
	// 			(tmp & (VM_FAULT_ERROR | VM_FAULT_NOPAGE)))) {
	// 		put_page(vmf->page);
	// 		return tmp;
	// 	}
	// }

	// ret |= finish_fault(vmf);
	// if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
	// 				VM_FAULT_RETRY))) {
	// 	unlock_page(vmf->page);
	// 	put_page(vmf->page);
	// 	return ret;
	// }

	// ret |= fault_dirty_shared_page(vmf);
	// return ret;
}

/*
 * We enter with non-exclusive mmap_lock (to exclude vma changes,
 * but allow concurrent faults).
 * The mmap_lock may have been released depending on flags and our
 * return value.  See filemap_fault() and __folio_lock_or_retry().
 * If mmap_lock is released, vma may become invalid (for example
 * by other thread calling munmap()).
 */
static vm_fault_t
do_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	mm_s *vm_mm = vma->vm_mm;
	vm_fault_t ret;

	/*
	 * The VMA was not fully populated on mmap() or missing VM_DONTEXPAND
	 */
	if (!vma->vm_ops->fault) {
		// /*
		//  * If we find a migration pmd entry or a none pmd entry, which
		//  * should never happen, return SIGBUS
		//  */
		// if (unlikely(!pmd_present(*vmf->pmd)))
		// 	ret = VM_FAULT_SIGBUS;
		// else {
		// 	vmf->pte = pte_offset_map_lock(vmf->vma->vm_mm,
		// 				       vmf->pmd,
		// 				       vmf->address,
		// 				       &vmf->ptl);
		// 	/*
		// 	 * Make sure this is not a temporary clearing of pte
		// 	 * by holding ptl and checking again. A R/M/W update
		// 	 * of pte involves: take ptl, clearing the pte so that
		// 	 * we don't have concurrent modification by hardware
		// 	 * followed by an update.
		// 	 */
		// 	if (unlikely(pte_none(*vmf->pte)))
		// 		ret = VM_FAULT_SIGBUS;
		// 	else
		// 		ret = VM_FAULT_NOPAGE;

		// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
		// }
	} else if (!(vmf->flags & FAULT_FLAG_WRITE))
		ret = do_read_fault(vmf);
	else if (!(vma->vm_flags & VM_SHARED))
		ret = do_cow_fault(vmf);
	else
		ret = do_shared_fault(vmf);

	// /* preallocated pagetable is unused: free it */
	// if (vmf->prealloc_pte) {
	// 	pte_free(vm_mm, vmf->prealloc_pte);
	// 	vmf->prealloc_pte = NULL;
	// }
	return ret;
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
static vm_fault_t
handle_pte_fault(vm_fault_s *vmf) {
	pte_t entry;

	if (unlikely(arch_pmd_none(*vmf->pmd))) {
		/*
		 * Leave __pte_alloc() until later: because vm_ops->fault may
		 * want to allocate huge page, and if we expose page table
		 * for an instant, it will be difficult to retract from
		 * concurrent faults and from rmap lookups.
		 */
		vmf->pte = NULL;
	} else {
		// /*
		//  * If a huge pmd materialized under us just retry later.  Use
		//  * pmd_trans_unstable() via pmd_devmap_trans_unstable() instead
		//  * of pmd_trans_huge() to ensure the pmd didn't become
		//  * pmd_trans_huge under us and then back to pmd_none, as a
		//  * result of MADV_DONTNEED running immediately after a huge pmd
		//  * fault in a different thread of this mm, in turn leading to a
		//  * misleading pmd_trans_huge() retval. All we have to ensure is
		//  * that it is a regular pmd that we can walk with
		//  * pte_offset_map() and we can do that through an atomic read
		//  * in C, which is what pmd_trans_unstable() provides.
		//  */
		// if (pmd_devmap_trans_unstable(vmf->pmd))
		// 	return 0;
		/*
		 * A regular pmd is established and it can't morph into a huge
		 * pmd from under us anymore at this point because we hold the
		 * mmap_lock read mode and khugepaged takes it in write mode.
		 * So now it's safe to run pte_offset_map().
		 */
		// vmf->pte = pte_offset_map(vmf->pmd, vmf->address);
		vmf->orig_pte = *vmf->pte;

		/*
		 * some architectures can have larger ptes than wordsize,
		 * e.g.ppc44x-defconfig has CONFIG_PTE_64BIT=y and
		 * CONFIG_32BIT=y, so READ_ONCE cannot guarantee atomic
		 * accesses.  The code below just needs a consistent view
		 * for the ifs and we later double check anyway with the
		 * ptl lock held. So here a barrier will do.
		 */
		barrier();
		// if (arch_pte_none(vmf->orig_pte)) {
		// 	pte_unmap(vmf->pte);
		// 	vmf->pte = NULL;
		// }
	}

	// static vm_fault_t do_pte_missing(struct vm_fault *vmf)
	if (arch_pte_none(*vmf->pte)) {
		if (vma_is_anonymous(vmf->vma))
			return do_anonymous_page(vmf);
		else
			return do_fault(vmf);
	}

	// if (!pte_present(vmf->orig_pte))
	// 	return do_swap_page(vmf);

	// if (pte_protnone(vmf->orig_pte) && vma_is_accessible(vmf->vma))
	// 	return do_numa_page(vmf);

	// vmf->ptl = pte_lockptr(vmf->vma->vm_mm, vmf->pmd);
	// spin_lock(vmf->ptl);
	entry = vmf->orig_pte;
	// if (unlikely(!pte_same(*vmf->pte, entry))) {
	// 	update_mmu_tlb(vmf->vma, vmf->address, vmf->pte);
	// 	goto unlock;
	// }
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_writable(*vmf->pte))
			return do_wp_page(vmf);
		entry = pte_mkdirty(entry);
	}
	entry = pte_mkyoung(entry);
	// if (ptep_set_access_flags(vmf->vma, vmf->address, vmf->pte, entry,
	// 			vmf->flags & FAULT_FLAG_WRITE)) {
	// 	update_mmu_cache(vmf->vma, vmf->address, vmf->pte);
	// } else {
	// 	/* Skip spurious TLB flush for retried page fault */
	// 	if (vmf->flags & FAULT_FLAG_TRIED)
	// 		goto unlock;
	// 	/*
	// 	 * This is needed only for protection faults but the arch code
	// 	 * is not yet telling us if this is a protection fault or not.
	// 	 * This still avoids useless tlb flushes for .text page faults
	// 	 * with threads.
	// 	 */
	// 	if (vmf->flags & FAULT_FLAG_WRITE)
	// 		flush_tlb_fix_spurious_fault(vmf->vma, vmf->address);
	// }
unlock:
	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return 0;
}

/*
 * By the time we get here, we already hold the mm semaphore
 *
 * The mmap_lock may have been released depending on flags and our
 * return value.  See filemap_fault() and __folio_lock_or_retry().
 */
// vm_fault_t handle_mm_fault(vma_s *vma, unsigned long address,
// 		unsigned int flags, pt_regs_s *regs)
vm_fault_t myos_handle_mm_fault(vma_s *vma,
		pt_regs_s *regs, ulong address, uint flags)
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
	uint dirty = flags & FAULT_FLAG_WRITE;
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

	ret = handle_pte_fault(&vmf);
fail:
	return ret;
}


/*
 * Allocate page upper directory.
 * We've already handled the fast-path in-line.
 */
// int __pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address)
int __myos_pud_alloc(mm_s *mm, p4d_t *p4d, ulong address)
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
	// spin_unlock(&mm->page_table_lock);
	return 0;
}

/*
 * Allocate page middle directory.
 * We've already handled the fast-path in-line.
 */
// int __pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address)
int __myos_pmd_alloc(mm_s *mm, pud_t *pud, ulong address)
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
	// spin_unlock(&mm->page_table_lock);
	return 0;
}

int __myos_pte_alloc(mm_s *mm, pmd_t *pmd, ulong address)
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
	// spin_unlock(&mm->page_table_lock);
	return 0;
}

pte_t *myos_creat_one_page_mapping(mm_s *mm,
		virt_addr_t addr, page_s *page)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	pgd = pgd_ent_offset(mm, addr);
	p4d = p4d_alloc(mm, pgd, addr);
	if (!p4d) {
		goto fail;
	}
	pud = pud_alloc(mm, p4d, addr);
	if (!pud) {
		goto fail;
	}
	pmd = pmd_alloc(mm, pud, addr);
	if (!pmd) {
		goto fail;
	}
	pte = pte_alloc(mm, pmd, addr);
	if (!pmd) {
		goto fail;
	}
	if (page != NULL)
	{
		atomic_inc(&(page->_mapcount));
		ulong page_addr = page_to_phys(page);
		*pte = arch_make_pte(PAGE_SHARED_EXEC | _PAGE_PAT | page_addr);
	}
	return pte;

fail:
	return ERR_PTR(-ENOMEM);
}

page_s *myos_get_one_page_from_mapping(mm_s *mm, virt_addr_t addr)
{
	page_s *retval = NULL;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	pgd = pgd_ent_offset(mm, addr);
	p4d = p4d_ent_offset(pgd, addr);
	if (arch_p4d_none(*p4d)) {
		goto fail;
	}
	pud = pud_ent_offset(p4d, addr);
	if (arch_pud_none(*pud)) {
		goto fail;
	}
	pmd = pmd_ent_offset(pud, addr);
	if (arch_pmd_none(*pmd)) {
		goto fail;
	}
	pte = pte_ent_offset(pmd, addr);
	if (arch_pte_none(*pte)) {
		goto fail;
	}

	phys_addr_t page_paddr = arch_pte_addr(*pte);
	retval = phys_to_page(page_paddr);

fail:
	return retval;
}

int myos_map_range(mm_s *mm, virt_addr_t start, virt_addr_t end)
{
	int ret = 0;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	pte_t *pte_ent;
	ulong addr = start;

	do
	{
		pgd = pgd_ent_offset(mm, addr);
		p4d = p4d_alloc(mm, pgd, addr);
		if (!p4d) {
			ret = VM_FAULT_OOM;
			goto fail;
		}
		pud = pud_alloc(mm, p4d, addr);
		if (!pud) {
			ret = VM_FAULT_OOM;
			goto fail;
		}
		pmd = pmd_alloc(mm, pud, addr);
		if (!pmd) {
			ret = VM_FAULT_OOM;
			goto fail;
		}
		pte = pte_alloc(mm, pmd, addr);
		if (!pmd) {
			ret = VM_FAULT_OOM;
			goto fail;
		}
		page_s *newpage = alloc_page(GFP_USER);
		atomic_inc(&(newpage->_mapcount));
		ulong page_addr = page_to_phys(newpage);
		*pte = arch_make_pte(PAGE_SHARED_EXEC | _PAGE_PAT | page_addr);

		addr += PAGE_SIZE;
	} while (addr < end);
	
	barrier();

fail:
	return ret;
}