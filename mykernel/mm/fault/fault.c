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
void *high_memory;
EXPORT_SYMBOL(high_memory);


ulong	zero_pfn __read_mostly;
EXPORT_SYMBOL(zero_pfn);

ulong	highest_memmap_pfn __read_mostly;

/*
 * CONFIG_MMU architectures set up ZERO_PAGE in their paging_init()
 */
int __init
init_zero_pfn(void) {
	zero_pfn = page_to_pfn(ZERO_PAGE(0));
	return 0;
}
early_initcall(init_zero_pfn);

static bool
vmf_pte_changed(struct vm_fault *vmf) {
	if (vmf->flags & FAULT_FLAG_ORIG_PTE_VALID)
		return !pte_same(ptep_get(vmf->pte_ptr), vmf->orig_pte);

	return !pte_none(ptep_get(vmf->pte_ptr));
}

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

static inline folio_s
*folio_prealloc(mm_s *src_mm, vma_s *vma, ulong addr, bool need_zero) {
	// struct folio *new_folio;

	// if (need_zero)
	// 	new_folio = vma_alloc_zeroed_movable_folio(vma, addr);
	// else
	// 	new_folio = vma_alloc_folio(GFP_HIGHUSER_MOVABLE, 0, vma,
	// 				    addr, false);

	// if (!new_folio)
	// 	return NULL;

	// if (mem_cgroup_charge(new_folio, src_mm, GFP_KERNEL)) {
	// 	folio_put(new_folio);
	// 	return NULL;
	// }
	// folio_throttle_swaprate(new_folio, GFP_KERNEL);

	// return new_folio;

	return folio_alloc(GFP_HIGHUSER_MOVABLE, 0);
}

static folio_s
*alloc_anon_folio(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	return folio_prealloc(vma->vm_mm, vma, vmf->address, true);
}

/*
 * Copy one pte.  Returns 0 if succeeded, or -EAGAIN if one preallocated page
 * is required to copy this pte.
 */
static inline int
copy_present_pte(vma_s *dst_vma, vma_s *src_vma,
		pte_t *dst_pte_ptr, pte_t *src_pte_ptr, ulong addr) {

	mm_s *src_mm = src_vma->vm_mm;
	ulong vm_flags = src_vma->vm_flags;
	pte_t pte = *src_pte_ptr;

	/*
	 * If it's a COW mapping, write protect it both
	 * in the parent and the child
	 */
	if (is_cow_mapping(vm_flags) && pte_write(pte)) {
		arch_ptep_set_wrprotect(src_pte_ptr);
		pte = pte_wrprotect(pte);
	}

	/*
	 * If it's a shared mapping, mark it clean in
	 * the child
	 */
	if (vm_flags & VM_SHARED)
		pte = pte_mkclean(pte);
	pte = pte_mkold(pte);

	set_pte_at(dst_vma->vm_mm, addr, dst_pte_ptr, pte);
	return 0;
}


static int
copy_pte_range(vma_s *dst_vma, vma_s *src_vma, pmd_t *dst_pmd_entp,
		pmd_t *src_pmd_entp, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pte_t *src_pte_ptr, *dst_pte_ptr;
	// pte_t *orig_src_pte_ptr, *orig_dst_pte_ptr;
	spinlock_t *src_ptl, *dst_ptl;
	int ret = 0;

	dst_pte_ptr = pte_alloc_map_lock(dst_mm, dst_pmd_entp, addr, &dst_ptl);
	if (!dst_pte_ptr) {
		// pte_unmap_unlock(dst_pte_ptr, dst_ptl);
		spin_unlock_no_resched(dst_ptl);
		/* ret == 0 */
		return -ENOMEM;
	}
	// src_pte_ptr = pte_offset_map_nolock(src_mm, src_pmd_entp, addr, &src_ptl);;
	src_pte_ptr = pte_offset_map_lock(src_mm, src_pmd_entp, addr, &src_ptl);;
	// orig_src_pte_ptr = src_pte_ptr;
	// orig_dst_pte_ptr = dst_pte_ptr;

	do {
		if (pte_none(*src_pte_ptr)) {
			continue;
		}
		// swap currently not supported.
		while (unlikely(!pte_present(*src_pte_ptr)));
		// if (unlikely(!pte_present(*src_pte_ptr))) {
		// 	ret = copy_nonpresent_pte(dst_mm, src_mm,
		// 				  dst_pte_ptr, src_pte_ptr,
		// 				  dst_vma, src_vma,
		// 				  addr, rss);
		// 	if (ret == -EIO) {
		// 		entry = pte_to_swp_entry(*src_pte_ptr);
		// 		break;
		// 	} else if (ret == -EBUSY) {
		// 		break;
		// 	} else if (!ret) {
		// 		continue;
		// 	}
		// 	/*
		// 	 * Device exclusive entry restored, continue by copying
		// 	 * the now present pte.
		// 	 */
		// 	WARN_ON_ONCE(ret != -ENOENT);
		// }
		/* copy_present_pte() will clear `*prealloc' if consumed */
		ret = copy_present_pte(dst_vma, src_vma,
				dst_pte_ptr, src_pte_ptr, addr);
		/*
		 * If we need a pre-allocated page for this pte, drop the
		 * locks, allocate, and try again.
		 */
		if (unlikely(ret == -EAGAIN))
			break;
	} while (dst_pte_ptr++, src_pte_ptr++, addr += PAGE_SIZE, addr != end);

	// pte_unmap_unlock(orig_src_pte_ptr, src_ptl);
	spin_unlock_no_resched(src_ptl);
	// pte_unmap_unlock(orig_dst_pte_ptr, dst_ptl);
	spin_unlock_no_resched(dst_ptl);
	// cond_resched();

	return 0;
}

static inline int
copy_pmd_range(vma_s *dst_vma, vma_s *src_vma, pud_t *dst_pud_entp,
		pud_t *src_pud_entp, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pmd_t *src_pmd_entp, *dst_pmd_entp;
	ulong next;

	dst_pmd_entp = pmd_alloc(dst_mm, dst_pud_entp, addr);
	if (!dst_pmd_entp)
		return -ENOMEM;
	src_pmd_entp = pmd_offset(src_pud_entp, addr);

	do {
		next = pmd_addr_end(addr, end);
		// MyOS2 doesn't support huge-page and swap,
		// So here skip corresponding check and handling
		if (pmd_ent_none_or_clear_bad(src_pmd_entp))
			continue;
		if (copy_pte_range(dst_vma, src_vma,
				dst_pmd_entp, src_pmd_entp, addr, next))
			return -ENOMEM;
	} while (dst_pmd_entp++, src_pmd_entp++, addr = next, addr != end);

	return 0;
}

static inline int
copy_pud_range(vma_s *dst_vma, vma_s *src_vma, p4d_t *dst_p4d_entp,
		p4d_t *src_p4d_entp, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pud_t *src_pud_entp, *dst_pud_entp;
	ulong next;

	dst_pud_entp = pud_alloc(dst_mm, dst_p4d_entp, addr);
	if (!dst_pud_entp)
		return -ENOMEM;
	src_pud_entp = pud_offset(src_p4d_entp, addr);

	do {
		next = pud_addr_end(addr, end);
		// MyOS2 doesn't support huge-page and swap,
		// So here skip corresponding check and handling
		if (pud_ent_none_or_clear_bad(src_pud_entp))
			continue;
		if (copy_pmd_range(dst_vma, src_vma,
				dst_pud_entp, src_pud_entp, addr, next))
			return -ENOMEM;
	} while (dst_pud_entp++, src_pud_entp++, addr = next, addr != end);

	return 0;
}

// 在启用四级映射时，实际上该子函数没做任何有意义的工作，
// 遍历的p4d实际上完全等于上一级的pgd
// 四级映射下p4d页必有效，所以可以不检查p4d_ent的有效性
static inline int
copy_p4d_range(vma_s *dst_vma, vma_s *src_vma, pgd_t *dst_pgd_entp,
		pgd_t *src_pgd_entp, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	p4d_t *src_p4d_entp, *dst_p4d_entp;
	ulong next;

	dst_p4d_entp = p4d_alloc(dst_mm, dst_pgd_entp, addr);
	if (!dst_p4d_entp)
		return -ENOMEM;
	src_p4d_entp = p4d_offset(src_pgd_entp, addr);

	do {
		next = p4d_addr_end(addr, end);
		if (p4d_ent_none_or_clear_bad(src_p4d_entp))
			continue;
		if (copy_pud_range(dst_vma, src_vma,
				dst_p4d_entp, src_p4d_entp, addr, next))
			return -ENOMEM;
	} while (dst_p4d_entp++, src_p4d_entp++, addr = next, addr != end);

	return 0;
}

int
copy_page_range(vma_s *dst_vma, vma_s *src_vma)
{
	ulong next;
	ulong addr = src_vma->vm_start;
	ulong end = src_vma->vm_end;
	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pgd_t *src_pgd_entp, *dst_pgd_entp;
	int ret = ENOERR;

	dst_pgd_entp = pgd_offset(dst_mm, addr);
	src_pgd_entp = pgd_offset(src_mm, addr);

	do {
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(src_pgd_entp))
			continue;
		if (copy_p4d_range(dst_vma, src_vma,
				dst_pgd_entp, src_pgd_entp, addr, next)) {
			ret = -ENOMEM;
			break;
		}
	} while (dst_pgd_entp++, src_pgd_entp++, addr = next, addr != end);

	return ret;
}


static inline int
__wp_page_copy_user(page_s *dst, page_s *src, vm_fault_s *vmf) {
	int ret;
	// void *kaddr;
	// void __user *uaddr;
	vma_s *vma = vmf->vma;
	mm_s *mm = vma->vm_mm;
	ulong addr = vmf->address;

	if (likely(src)) {
		copy_user_highpage(dst, src, addr, vma);
		return -ENOERR;
	}

	// /*
	//  * If the source page was a PFN mapping, we don't have
	//  * a "struct page" for it. We do a best-effort copy by
	//  * just copying from the original user address. If that
	//  * fails, we just zero-fill it. Live with it.
	//  */
	// kaddr = kmap_local_page(dst);
	// pagefault_disable();
	// uaddr = (void __user *)(addr & PAGE_MASK);

	// /*
	//  * On architectures with software "accessed" bits, we would
	//  * take a double page fault, so mark it accessed here.
	//  */
	// vmf->pte = NULL;
	// if (!arch_has_hw_pte_young() && !pte_young(vmf->orig_pte)) {
	// 	pte_t entry;

	// 	vmf->pte = pte_offset_map_lock(mm, vmf->pmd, addr, &vmf->ptl);
	// 	if (unlikely(!vmf->pte || !pte_same(ptep_get(vmf->pte), vmf->orig_pte))) {
	// 		/*
	// 		 * Other thread has already handled the fault
	// 		 * and update local tlb only
	// 		 */
	// 		if (vmf->pte)
	// 			update_mmu_tlb(vma, addr, vmf->pte);
	// 		ret = -EAGAIN;
	// 		goto pte_unlock;
	// 	}

	// 	entry = pte_mkyoung(vmf->orig_pte);
	// 	if (ptep_set_access_flags(vma, addr, vmf->pte, entry, 0))
	// 		update_mmu_cache_range(vmf, vma, addr, vmf->pte, 1);
	// }

	// /*
	//  * This really shouldn't fail, because the page is there
	//  * in the page tables. But it might just be unreadable,
	//  * in which case we just give up and fill the result with
	//  * zeroes.
	//  */
	// if (__copy_from_user_inatomic(kaddr, uaddr, PAGE_SIZE)) {
	// 	if (vmf->pte)
	// 		goto warn;

	// 	/* Re-validate under PTL if the page is still mapped */
	// 	vmf->pte = pte_offset_map_lock(mm, vmf->pmd, addr, &vmf->ptl);
	// 	if (unlikely(!vmf->pte || !pte_same(ptep_get(vmf->pte), vmf->orig_pte))) {
	// 		/* The PTE changed under us, update local tlb */
	// 		if (vmf->pte)
	// 			update_mmu_tlb(vma, addr, vmf->pte);
	// 		ret = -EAGAIN;
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
warn:
	// 		WARN_ON_ONCE(1);
	// 		clear_page(kaddr);
	// 	}
	// }

	ret = 0;

pte_unlock:
	// if (vmf->pte)
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// pagefault_enable();
	// kunmap_local(kaddr);
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
	const bool unshare = vmf->flags & FAULT_FLAG_UNSHARE;
	vma_s *vma = vmf->vma;
	mm_s *mm = vma->vm_mm;
	folio_s *old_folio = NULL;
	folio_s *new_folio = NULL;
	pte_t entry;
	int page_copied = 0;
	// struct mmu_notifier_range range;
	vm_fault_t ret;
	bool pfn_is_zero;

	if (vmf->page)
		old_folio = page_folio(vmf->page);
	// ret = vmf_anon_prepare(vmf);
	// if (unlikely(ret))
	// 	goto out;

	pfn_is_zero = is_zero_pfn(pte_pfn(vmf->orig_pte));
	new_folio = folio_prealloc(mm, vma, vmf->address, pfn_is_zero);
	if (!new_folio)
		goto oom;

	if (!pfn_is_zero) {
		int err = __wp_page_copy_user(&new_folio->page, vmf->page, vmf);
		if (err) {
			/*
			 * COW failed, if the fault was solved by other,
			 * it's fine. If not, userspace would re-fault on
			 * the same address and we will handle the fault
			 * from the second attempt.
			 */
			folio_put(new_folio);
			if (old_folio)
				folio_put(old_folio);

			return err == -EHWPOISON ? VM_FAULT_HWPOISON : 0;
		}
	}

	// __folio_mark_uptodate(new_folio);

	// mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, mm,
	// 			vmf->address & PAGE_MASK,
	// 			(vmf->address & PAGE_MASK) + PAGE_SIZE);
	// mmu_notifier_invalidate_range_start(&range);

	// /*
	//  * Re-check the pte - we dropped the lock
	//  */
	// vmf->pte = pte_offset_map_lock(mm, vmf->pmd, vmf->address, &vmf->ptl);
	if (likely(vmf->pte_ptr != NULL &&
			pte_same(ptep_get(vmf->pte_ptr), vmf->orig_pte))) {
		// if (old_folio) {
		// 	if (!folio_test_anon(old_folio)) {
		// 		dec_mm_counter(mm, mm_counter_file(old_folio));
		// 		inc_mm_counter(mm, MM_ANONPAGES);
		// 	}
		// } else {
		// 	ksm_might_unmap_zero_page(mm, vmf->orig_pte);
		// 	inc_mm_counter(mm, MM_ANONPAGES);
		// }
		// flush_cache_page(vma, vmf->address, pte_pfn(vmf->orig_pte));
		entry = mk_pte(&new_folio->page, vma->vm_page_prot);
		// entry = pte_sw_mkyoung(entry);
		if (unlikely(unshare)) {
			// if (pte_soft_dirty(vmf->orig_pte))
			// 	entry = pte_mksoft_dirty(entry);
			// if (pte_uffd_wp(vmf->orig_pte))
			// 	entry = pte_mkuffd_wp(entry);
		} else {
			entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		}

		// /*
		//  * Clear the pte entry and flush it first, before updating the
		//  * pte with the new entry, to keep TLBs on different CPUs in
		//  * sync. This code used to set the new PTE then flush TLBs, but
		//  * that left a window where the new PTE could be loaded into
		//  * some TLBs while the old PTE remains in others.
		//  */
		// ptep_clear_flush(vma, vmf->address, vmf->pte);
		// folio_add_new_anon_rmap(new_folio, vma, vmf->address, RMAP_EXCLUSIVE);
		// folio_add_lru_vma(new_folio, vma);
		BUG_ON(unshare && pte_write(entry));
		set_pte_at(mm, vmf->address, vmf->pte_ptr, entry);
		// update_mmu_cache_range(vmf, vma, vmf->address, vmf->pte, 1);
		// if (old_folio) {
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
		// 	 * folio_remove_rmap_pte() with the ptp_clear_flush
		// 	 * above. Those stores are ordered by (if nothing else,)
		// 	 * the barrier present in the atomic_add_negative
		// 	 * in folio_remove_rmap_pte();
		// 	 *
		// 	 * Then the TLB flush in ptep_clear_flush ensures that
		// 	 * no process can access the old page before the
		// 	 * decremented mapcount is visible. And the old page
		// 	 * cannot be reused until after the decremented
		// 	 * mapcount is visible. So transitively, TLBs to
		// 	 * old page will be flushed before it can be reused.
		// 	 */
		// 	folio_remove_rmap_pte(old_folio, vmf->page, vma);
		// }

		/* Free the old page.. */
		new_folio = old_folio;
		page_copied = 1;
		// pte_unmap_unlock(vmf->pte_ptr, vmf->ptl);
	} else {
		update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
		// pte_unmap_unlock(vmf->pte_ptr, vmf->ptl);
	}

	// mmu_notifier_invalidate_range_end(&range);

	if (new_folio)
		folio_put(new_folio);
	if (old_folio) {
		// if (page_copied)
		// 	free_swap_cache(old_folio);
		folio_put(old_folio);
	}

	return 0;
oom:
	ret = VM_FAULT_OOM;
out:
	if (old_folio)
		folio_put(old_folio);

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
	folio_s *folio = NULL;
	pte_t pte;

	vmf->page = vm_normal_page(vma, vmf->address, *vmf->pte_ptr);
	if (vmf->page)
		folio = page_folio(vmf->page);

	/*
	 * Shared mapping: we are guaranteed to have VM_WRITE and
	 * FAULT_FLAG_WRITE set at this point.
	 */
	if (vma->vm_flags & (VM_SHARED | VM_MAYSHARE)) {
		// catch this case, handler temporarily not implemented
		while (1);
		
		/*
		 * VM_MIXEDMAP !pfn_valid() case, or VM_SOFTDIRTY clear on a
		 * VM_PFNMAP VMA.
		 *
		 * We should not cow pages in a shared writeable mapping.
		 * Just mark the pages writable and/or call ops->pfn_mkwrite.
		 */
		// if (!vmf->page)
		// 	return wp_pfn_shared(vmf);
		// return wp_page_shared(vmf, folio);
	}

	// /*
	//  * Private mapping: create an exclusive anonymous page copy if reuse
	//  * is impossible. We might miss VM_WRITE for FOLL_FORCE handling.
	//  *
	//  * If we encounter a page that is marked exclusive, we must reuse
	//  * the page without further checks.
	//  */
	// if (folio && folio_test_anon(folio) &&
	//     (PageAnonExclusive(vmf->page) || wp_can_reuse_anon_folio(folio, vma))) {
	// 	if (!PageAnonExclusive(vmf->page))
	// 		SetPageAnonExclusive(vmf->page);
	// 	if (unlikely(unshare)) {
	// 		pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 		return 0;
	// 	}
	// 	wp_page_reuse(vmf, folio);
	// 	return 0;
	// }
	/*
	 * Ok, we need to copy. Oh, well..
	 */
	if (folio)
		folio_get(folio);

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
	folio_s *folio;
	vm_fault_t ret = 0;
	// int nr_pages = 1;
	pte_t entry;

	/* File mapping without ->vm_ops ? */
	if (vma->vm_flags & VM_SHARED)
		return VM_FAULT_SIGBUS;

	/*
	 * Use pte_alloc() instead of pte_alloc_map(), so that OOM can
	 * be distinguished from a transient failure of pte_offset_map().
	 */
	if (pte_alloc(vma->vm_mm, vmf->pmd_entp))
		return VM_FAULT_OOM;

	/* Use the zero-page for reads */
	if (!(vmf->flags & FAULT_FLAG_WRITE)) {
		entry = pte_mkspecial(pfn_pte(my_zero_pfn(vmf->address),
					vma->vm_page_prot));

		// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd,
		// 		vmf->address, &vmf->ptl);
		if (!vmf->pte_ptr)
			goto unlock;
		// if (vmf_pte_changed(vmf)) {
		// 	update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
		// 	goto unlock;
		// }
		// ret = check_stable_address_space(vma->vm_mm);
		// if (ret)
		// 	goto unlock;
		goto setpte;
	}

	// /* Allocate our own private page. */
	// ret = vmf_anon_prepare(vmf);
	// if (ret)
	// 	return ret;
	/* Returns NULL on OOM or ERR_PTR(-EAGAIN) if we must retry the fault */
	folio = alloc_anon_folio(vmf);
	if (IS_ERR(folio))
		return 0;
	if (!folio)
		goto oom;

	// nr_pages = folio_nr_pages(folio);
	// addr = ALIGN_DOWN(vmf->address, nr_pages * PAGE_SIZE);

	// /*
	//  * The memory barrier inside __folio_mark_uptodate makes sure that
	//  * preceding stores to the page contents become visible before
	//  * the set_pte_at() write.
	//  */
	// __folio_mark_uptodate(folio);

	entry = mk_pte(&folio->page, vma->vm_page_prot);
	if (vma->vm_flags & VM_WRITE)
		entry = pte_mkwrite(pte_mkdirty(entry));

	// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd, addr, &vmf->ptl);
	if (!vmf->pte_ptr)
		goto release;
	// if (vmf_pte_changed(vmf)) {
	// 	update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
	// 	goto release;
	// }

	// ret = check_stable_address_space(vma->vm_mm);
	// if (ret)
	// 	goto release;

	// folio_ref_add(folio, nr_pages - 1);
	// add_mm_counter(vma->vm_mm, MM_ANONPAGES, nr_pages);
	// folio_add_new_anon_rmap(folio, vma, vmf->address);
	// folio_add_lru_vma(folio, vma);
setpte:
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte_ptr, entry);

	// /* No need to invalidate - it was non-present before */
	// update_mmu_cache_range(vmf, vma, vmf->address, vmf->pte, 1);
unlock:
	// if (vmf->pte)
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	return ret;
release:
	folio_put(folio);
	goto unlock;
oom:
	return VM_FAULT_OOM;
}

/*
 * The mmap_lock must have been held on entry, and may have been
 * released depending on flags and vma->vm_ops->fault() return value.
 * See simple_filemap_fault() and __lock_page_retry().
 */
static vm_fault_t
__do_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	folio_s *folio;
	vm_fault_t ret;

	/*
	 * Preallocate pte before we take page_lock because this might lead to
	 * deadlocks for memcg reclaim which waits for pages under writeback:
	 *				lock_page(A)
	 *				SetPageWriteback(A)
	 *				unlock_page(A)
	 * lock_page(B)
	 *				lock_page(B)
	 * pte_alloc_one
	 *   shrink_page_list
	 *     wait_on_page_writeback(A)
	 *				SetPageWriteback(B)
	 *				unlock_page(B)
	 *				# flush A, B to clear the writeback
	 */
	while (pmd_none(*vmf->pmd_entp));


	ret = vma->vm_ops->fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
			VM_FAULT_RETRY | VM_FAULT_DONE_COW)))
		return ret;
	
	// We skip page-poisoned check

	folio = page_folio(vmf->page);
	// if (unlikely(!(ret & VM_FAULT_LOCKED)))
	// 	folio_lock(folio);
	// else
	// 	VM_BUG_ON_PAGE(!folio_test_locked(folio), vmf->page);

	return ret;
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
	folio_s *folio;
	vm_fault_t ret;
	bool is_cow = (vmf->flags & FAULT_FLAG_WRITE) &&
					!(vma->vm_flags & VM_SHARED);
	int type, nr_pages;
	pte_t entry;

	/* Did we COW the page? */
	if (is_cow)
		page = vmf->cow_page;
	else
		page = vmf->page;

	// /*
	//  * check even for read faults because we might have lost our CoWed
	//  * page
	//  */
	// if (!(vma->vm_flags & VM_SHARED)) {
	// 	ret = check_stable_address_space(vma->vm_mm);
	// 	if (ret)
	// 		return ret;
	// }

	// vmf->pmd must exist and correct, skip check
	// prealloc_pte not supported, skip check

	folio = page_folio(page);
	nr_pages = folio_nr_pages(folio);

	// /*
	//  * Using per-page fault to maintain the uffd semantics, and same
	//  * approach also applies to non-anonymous-shmem faults to avoid
	//  * inflating the RSS of the process.
	//  */
	// if (!vma_is_anon_shmem(vma) || unlikely(userfaultfd_armed(vma))) {
	// 	nr_pages = 1;
	// } else if (nr_pages > 1) {
	// 	pgoff_t idx = folio_page_idx(folio, page);
	// 	/* The page offset of vmf->address within the VMA. */
	// 	pgoff_t vma_off = vmf->pgoff - vmf->vma->vm_pgoff;
	// 	/* The index of the entry in the pagetable for fault page. */
	// 	pgoff_t pte_off = pte_index(vmf->address);

	// 	/*
	// 	 * Fallback to per-page fault in case the folio size in page
	// 	 * cache beyond the VMA limits and PMD pagetable limits.
	// 	 */
	// 	if (unlikely(vma_off < idx ||
	// 		    vma_off + (nr_pages - idx) > vma_pages(vma) ||
	// 		    pte_off < idx ||
	// 		    pte_off + (nr_pages - idx)  > PTRS_PER_PTE)) {
	// 		nr_pages = 1;
	// 	} else {
	// 		/* Now we can set mappings for the whole large folio. */
	// 		addr = vmf->address - idx * PAGE_SIZE;
	// 		page = &folio->page;
	// 	}
	// }

	// vmf->pte = pte_offset_map_lock(vma->vm_mm,
	// 				vmf->pmd, vmf->address, &vmf->ptl);
	if (!vmf->pte_ptr)
		return VM_FAULT_NOPAGE;

	// /* Re-check under ptl */
	// if (nr_pages == 1 && unlikely(vmf_pte_changed(vmf))) {
	// 	update_mmu_tlb(vma, addr, vmf->pte);
	// 	ret = VM_FAULT_NOPAGE;
	// 	goto unlock;
	// } else if (nr_pages > 1 && !pte_range_none(vmf->pte, nr_pages)) {
	// 	update_mmu_tlb_range(vma, addr, vmf->pte, nr_pages);
	// 	ret = VM_FAULT_NOPAGE;
	// 	goto unlock;
	// }

	// folio_ref_add(folio, nr_pages - 1);
	// set_pte_range(vmf, folio, page, nr_pages, addr);
	entry = mk_pte(page, vma->vm_page_prot);
	set_pte(vmf->pte_ptr, entry);
	// type = is_cow ? MM_ANONPAGES : mm_counter_file(folio);
	// add_mm_counter(vma->vm_mm, type, nr_pages);
	ret = 0;

unlock:
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
 * This function doesn't cross VMA or page table boundaries, in order to call
 * map_pages() and acquire a PTE lock only once.
 *
 * fault_around_pages defines how many pages we'll try to map.
 * do_fault_around() expects it to be set to a power of two less than or equal
 * to PTRS_PER_PTE.
 *
 * The virtual address of the area that we map is naturally aligned to
 * fault_around_pages * PAGE_SIZE rounded down to the machine page size
 * (and therefore to page order).  This way it's easier to guarantee
 * that we don't cross page table boundaries.
 */
static vm_fault_t
do_fault_around(vm_fault_s *vmf) {
	// pgoff_t nr_pages = READ_ONCE(fault_around_pages);
	pgoff_t nr_pages = 1;
	pgoff_t pte_off = pte_index(vmf->address);
	/* The page offset of vmf->address within the VMA. */
	pgoff_t vma_off = vmf->pgoff - vmf->vma->vm_pgoff;
	pgoff_t from_pte, to_pte;
	vm_fault_t ret = 0;

	/* The PTE offset of the start address, clamped to the VMA. */
	from_pte = max(ALIGN_DOWN(pte_off, nr_pages),
				pte_off - min(pte_off, vma_off));

	/* The PTE offset of the end address, clamped to the VMA and PTE. */
	to_pte = min3(from_pte + nr_pages, (pgoff_t)PTRS_PER_PTE,
				pte_off + vma_pages(vmf->vma) - vma_off) - 1;

	while (pmd_none(*vmf->pmd_entp));

	// rcu_read_lock();
	// ret = vmf->vma->vm_ops->map_pages(vmf,
	// 		vmf->pgoff + from_pte - pte_off,
	// 		vmf->pgoff + to_pte - pte_off);
	ret = vmf->vma->vm_ops->map_single_page(vmf,
			vmf->pgoff + from_pte - pte_off);
	// rcu_read_unlock();

	return ret;
}

static vm_fault_t
do_read_fault(vm_fault_s *vmf) {
	vm_fault_t ret = 0;
	folio_s *folio;

	/*
	 * Let's call ->map_pages() first and use ->fault() as fallback
	 * if page by the offset is not ready to be mapped (cold cache or
	 * something).
	 */
	// if (should_fault_around(vmf)) {
		ret = do_fault_around(vmf);
		if (ret == 0)
			return ret;
	// }

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	ret |= finish_fault(vmf);
	folio = page_folio(vmf->page);
	// folio_unlock(folio);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		folio_put(folio);
	return ret;
}

static vm_fault_t
do_cow_fault(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	folio_s *folio;
	vm_fault_t ret;

	folio = folio_prealloc(vma->vm_mm, vma, vmf->address, false);
	if (!folio)
		return VM_FAULT_OOM;
	vmf->cow_page = &folio->page;

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		goto uncharge_out;
	if (ret & VM_FAULT_DONE_COW)
		return ret;

	copy_user_highpage(vmf->cow_page, vmf->page, vmf->address, vma);
	// __folio_mark_uptodate(folio);

	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
	put_page(vmf->page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		goto uncharge_out;
	return ret;
uncharge_out:
	folio_put(folio);
	// put_page(vmf->cow_page);
	return ret;
}

static vm_fault_t
do_shared_fault(vm_fault_s *vmf) {
	pr_alert("API not implemented - 'do_shared_fault'\n");
	while (1);
	
	vma_s *vma = vmf->vma;
	folio_s *folio;
	vm_fault_t ret, tmp;

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	folio = page_folio(vmf->page);

	// /*
	//  * Check if the backing address space wants to know that the page is
	//  * about to become writable
	//  */
	// if (vma->vm_ops->page_mkwrite) {
	// 	folio_unlock(folio);
	// 	tmp = do_page_mkwrite(vmf, folio);
	// 	if (unlikely(!tmp ||
	// 			(tmp & (VM_FAULT_ERROR | VM_FAULT_NOPAGE)))) {
	// 		folio_put(folio);
	// 		return tmp;
	// 	}
	// }

	ret |= finish_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR |
				VM_FAULT_NOPAGE | VM_FAULT_RETRY))) {
		// folio_unlock(folio);
		folio_put(folio);
		return ret;
	}

	// ret |= fault_dirty_shared_page(vmf);
	return ret;
}

/*
 * We enter with non-exclusive mmap_lock (to exclude vma changes,
 * but allow concurrent faults).
 * The mmap_lock may have been released depending on flags and our
 * return value.  See simple_filemap_fault() and __folio_lock_or_retry().
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
		pr_alert("VMA has no vm_ops->fault\n");
		while (1);
		
		// Since we have set alloc or find the pte, 
		// and set vmf->pte_ptr pointing to it,
		// So here skip the check work done in linux
		/*
		 * Make sure this is not a temporary clearing of pte
		 * by holding ptl and checking again. A R/M/W update
		 * of pte involves: take ptl, clearing the pte so that
		 * we don't have concurrent modification by hardware
		 * followed by an update.
		 */
		if (unlikely(pte_none(ptep_get(vmf->pte_ptr))))
			ret = VM_FAULT_SIGBUS;
		else
			ret = VM_FAULT_NOPAGE;

		// pte_unmap_unlock(vmf->pte_ptr, vmf->ptl);
	} else if (!(vmf->flags & FAULT_FLAG_WRITE))
		// Reading or Executing cause fault
		ret = do_read_fault(vmf);
	else if (!(vma->vm_flags & VM_SHARED))
		// Writing cause fault, but address is not VM_SHARED
		ret = do_cow_fault(vmf);
	else
		// Writing cause fault, and address is VM_SHARED
		ret = do_shared_fault(vmf);

	// prealloc_pte not supported, skip check
	return ret;
}

static vm_fault_t
do_pte_missing(vm_fault_s *vmf) {
	if (vma_is_anonymous(vmf->vma))
		return do_anonymous_page(vmf);
	else
		return do_fault(vmf);
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
 * See simple_filemap_fault() and __folio_lock_or_retry().
 */
static vm_fault_t
handle_pte_fault(vm_fault_s *vmf) {
	pte_t entry;

	if (pte_none(vmf->orig_pte))
		return do_pte_missing(vmf);

	// if (!pte_present(vmf->orig_pte))
	// 	return do_swap_page(vmf);

	// if (pte_protnone(vmf->orig_pte) && vma_is_accessible(vmf->vma))
	// 	return do_numa_page(vmf);

	// spin_lock(vmf->ptl);
	entry = vmf->orig_pte;
	if (unlikely(!pte_same(ptep_get(vmf->pte_ptr), entry))) {
		update_mmu_tlb(vmf->vma, vmf->address, vmf->pte_ptr);
		goto unlock;
	}
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
		else if (likely(vmf->flags & FAULT_FLAG_WRITE))
			entry = pte_mkdirty(entry);
	}
	entry = pte_mkyoung(entry);
	// if (ptep_set_access_flags(vmf->vma, vmf->address, vmf->pte, entry,
	// 			vmf->flags & FAULT_FLAG_WRITE)) {
	// 	update_mmu_cache_range(vmf, vmf->vma, vmf->address,
	// 			vmf->pte, 1);
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
	// 		flush_tlb_fix_spurious_fault(vmf->vma, vmf->address,
	// 					     vmf->pte);
	// }
unlock:
	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return 0;
}

/*
 * By the time we get here, we already hold the mm semaphore
 *
 * The mmap_lock may have been released depending on flags and our
 * return value.  See simple_filemap_fault() and __folio_lock_or_retry().
 */
// vm_fault_t handle_mm_fault(vma_s *vma, unsigned long address,
// 		unsigned int flags, pt_regs_s *regs)
vm_fault_t myos_handle_mm_fault(vma_s *vma,
		ulong address, uint flags)
{
	vm_fault_t ret = VM_FAULT_OOM;
	__set_current_state(TASK_RUNNING);

	vm_fault_s vmf = {
		.vma		= vma,
		.address	= address & PAGE_MASK,
		.flags		= flags,
		.pgoff		= linear_page_index(vma, address),
		.gfp_mask	= __get_fault_gfp_mask(vma),
		.curr_tsk	= current,
	};
	uint dirty = flags & FAULT_FLAG_WRITE;
	mm_s *mm = vma->vm_mm;
	pgd_t *pgd = pgd_offset(mm, address);

	vmf.p4d_entp = p4d_alloc(mm, pgd, address);
	if (!vmf.p4d_entp) {
		goto fail;
	}
	vmf.pud_entp = pud_alloc(mm, vmf.p4d_entp, address);
	if (!vmf.pud_entp) {
		goto fail;
	}
	vmf.pmd_entp = pmd_alloc(mm, vmf.pud_entp, address);
	if (!vmf.pmd_entp) {
		goto fail;
	}
	if (pte_alloc(mm, vmf.pmd_entp)) {
		goto fail;
	}
	barrier();


	/*
	 * In MyOS2 case, the PMD will be sure exist,
	 * so we skip the check procedure which done in linux
	 */
	if (pmd_none(*(vmf.pmd_entp))) {
		pr_err("Current PMD is empty.");
		while (1);
	}
	vmf.pte_ptr = pte_offset_map_nolock(vmf.vma->vm_mm,
					vmf.pmd_entp, vmf.address, &(vmf.ptl));
	if (unlikely(vmf.pte_ptr == NULL))
		return 0;
	vmf.orig_pte = ptep_get_lockless(vmf.pte_ptr);
	vmf.flags |= FAULT_FLAG_ORIG_PTE_VALID;


	ret = handle_pte_fault(&vmf);
fail:
	return ret;
}



pte_t *myos_creat_one_page_mapping(mm_s *mm,
		virt_addr_t addr, page_s *page)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	pgd = pgd_offset(mm, addr);
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
	pte = pte_alloc_map(mm, pmd, addr);
	if (!pmd) {
		goto fail;
	}
	if (page != NULL)
	{
		atomic_inc(&(page->_mapcount));
		ulong page_addr = page_to_phys(page);
		*pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);
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

	pgd = pgd_offset(mm, addr);
	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d)) {
		goto fail;
	}
	pud = pud_offset(p4d, addr);
	if (pud_none(*pud)) {
		goto fail;
	}
	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd)) {
		goto fail;
	}
	pte = pte_offset(pmd, addr);
	if (pte_none(*pte)) {
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
		pgd = pgd_offset(mm, addr);
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
		pte = pte_alloc_map(mm, pmd, addr);
		if (!pmd) {
			ret = VM_FAULT_OOM;
			goto fail;
		}
		page_s *newpage = alloc_page(GFP_USER);
		atomic_inc(&(newpage->_mapcount));
		ulong page_addr = page_to_phys(newpage);
		*pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);

		addr += PAGE_SIZE;
	} while (addr < end);
	
	barrier();

fail:
	return ret;
}