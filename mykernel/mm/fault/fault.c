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
		return !pte_same(ptep_get_pte(vmf->pte), vmf->orig_pte);

	return !pte_none(ptep_get_pte(vmf->pte));
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
	if (is_cow_mapping(vm_flags) && pte_writable(pte)) {
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
copy_pte_range(vma_s *dst_vma, vma_s *src_vma, pmd_t *dst_pmde_ptr,
		pmd_t *src_pmde_ptr, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pte_t *src_pte_ptr, *dst_pte_ptr;
	// spinlock_t *src_ptl, *dst_ptl;
	int ret = 0;

	dst_pte_ptr = ptd_alloc(dst_mm, dst_pmde_ptr, addr);
	if (!dst_pte_ptr)
		return -ENOMEM;
	src_pte_ptr = pte_ptr_in_pmd(src_pmde_ptr, addr);

	do {
		if (pte_none(*src_pte_ptr)) {
			continue;
		}
		if (unlikely(!pte_present(*src_pte_ptr))) {
			// ret = copy_nonpresent_pte(dst_mm, src_mm,
			// 			  dst_pte_ptr, src_pte_ptr,
			// 			  dst_vma, src_vma,
			// 			  addr, rss);
			// if (ret == -EIO) {
			// 	entry = pte_to_swp_entry(*src_pte_ptr);
			// 	break;
			// } else if (ret == -EBUSY) {
			// 	break;
			// } else if (!ret) {
			// 	continue;
			// }
			// /*
			//  * Device exclusive entry restored, continue by copying
			//  * the now present pte.
			//  */
			// WARN_ON_ONCE(ret != -ENOENT);
		}
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

	return 0;
}

static inline int
copy_pmd_range(vma_s *dst_vma, vma_s *src_vma, pud_t *dst_pude_ptr,
		pud_t *src_pude_ptr, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pmd_t *src_pmde_ptr, *dst_pmde_ptr;
	ulong next;

	dst_pmde_ptr = pmd_alloc(dst_mm, dst_pude_ptr, addr);
	if (!dst_pmde_ptr)
		return -ENOMEM;
	src_pmde_ptr = pmde_ptr_in_pud(src_pude_ptr, addr);

	do {
		next = pmd_ent_bound_end(addr, end);
		if (pmde_none_or_clear_bad(src_pmde_ptr))
			continue;
		if (copy_pte_range(dst_vma, src_vma,
				dst_pmde_ptr, src_pmde_ptr, addr, next))
			return -ENOMEM;
	} while (dst_pmde_ptr++, src_pmde_ptr++, addr = next, addr != end);

	return 0;
}

static inline int
copy_pud_range(vma_s *dst_vma, vma_s *src_vma, p4d_t *dst_p4de_ptr,
		p4d_t *src_p4de_ptr, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	pud_t *src_pude_ptr, *dst_pude_ptr;
	ulong next;

	dst_pude_ptr = pud_alloc(dst_mm, dst_p4de_ptr, addr);
	if (!dst_pude_ptr)
		return -ENOMEM;
	src_pude_ptr = pude_ptr_in_p4d(src_p4de_ptr, addr);

	do {
		next = pud_ent_bound_end(addr, end);
		if (pude_none_or_clear_bad(src_pude_ptr))
			continue;
		if (copy_pmd_range(dst_vma, src_vma,
				dst_pude_ptr, src_pude_ptr, addr, next))
			return -ENOMEM;
	} while (dst_pude_ptr++, src_pude_ptr++, addr = next, addr != end);

	return 0;
}

// 在启用四级映射时，实际上本子函数没做任何有意义的工作，
// 遍历的p4d实际上完全等于上一级的pgd
// 四级映射下p4d页必有效，所以可以不检查p4d_ent的有效性
static inline int
copy_p4d_range(vma_s *dst_vma, vma_s *src_vma, pgd_t *dst_pgde_ptr,
		pgd_t *src_pgde_ptr, ulong addr, ulong end) {

	mm_s *dst_mm = dst_vma->vm_mm;
	mm_s *src_mm = src_vma->vm_mm;
	p4d_t *src_p4de_ptr, *dst_p4de_ptr;
	ulong next;

	dst_p4de_ptr = p4d_alloc(dst_mm, dst_pgde_ptr, addr);
	if (!dst_p4de_ptr)
		return -ENOMEM;
	src_p4de_ptr = p4de_ptr_in_pgd(src_pgde_ptr, addr);

	do {
		next = p4d_ent_bound_end(addr, end);
		if (p4de_none_or_clear_bad(src_p4de_ptr))
			continue;
		if (copy_pud_range(dst_vma, src_vma,
				dst_p4de_ptr, src_p4de_ptr, addr, next))
			return -ENOMEM;
	} while (dst_p4de_ptr++, src_p4de_ptr++, addr = next, addr != end);

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
	pgd_t *src_pgde_ptr, *dst_pgde_ptr;
	int ret = ENOERR;

	/*
	 * Don't copy ptes where a page fault will fill them correctly.
	 * Fork becomes much lighter when there are big shared or private
	 * readonly mappings. The tradeoff is that copy_page_range is more
	 * efficient than faulting.
	 */

	// /*
	//  * We need to invalidate the secondary MMU mappings only when
	//  * there could be a permission downgrade on the ptes of the
	//  * parent mm. And a permission downgrade will only happen if
	//  * is_cow_mapping() returns true.
	//  */
	// bool is_cow = is_cow_mapping(src_vma->vm_flags);

	dst_pgde_ptr = pgd_ent_ptr_in_mm(dst_mm, addr);
	src_pgde_ptr = pgd_ent_ptr_in_mm(src_mm, addr);

	do {
		next = pgd_ent_bound_end(addr, end);
		if (pgd_none_or_clear_bad(src_pgde_ptr))
			continue;
		if (copy_p4d_range(dst_vma, src_vma,
				dst_pgde_ptr, src_pgde_ptr, addr, next)) {
			ret = -ENOMEM;
			break;
		}
	} while (dst_pgde_ptr++, src_pgde_ptr++, addr = next, addr != end);

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
		copy_user_highpage(dst, src, addr, vma);
		// copy_user_page((void *)page_to_virt(dst),
		// 		(void *)page_to_virt(src), addr, dst);

		// loff_t offset;
		// if ((offset = myosDBG_compare_page(dst, src)) != PAGE_SIZE)
		// 	pr_alert("COW copied page content changed at offset: %#08Lx.\n", offset);

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

	// vmf->page = vm_normal_page(vma, vmf->address, *vmf->pte);
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
	folio_s *folio;
	vm_fault_t ret = 0;
	pte_t entry;

	/* File mapping without ->vm_ops ? */
	if (vma->vm_flags & VM_SHARED)
		return VM_FAULT_SIGBUS;

	// /*
	//  * Use ptd_alloc() instead of pte_alloc_map().  We can't run
	//  * pte_offset_map() on pmds where a huge pmd might be created
	//  * from a different thread.
	//  *
	//  * pte_alloc_map() is safe to use under mmap_write_lock(mm) or when
	//  * parallel threads are excluded by other means.
	//  *
	//  * Here we only have mmap_read_lock(mm).
	//  */
	// if (ptd_alloc(vma->vm_mm, vmf->pmd))
	// 	return VM_FAULT_OOM;
	while (vmf->pte == NULL);

	// /* See comment in handle_pte_fault() */
	// if (unlikely(pmd_trans_unstable(vmf->pmd)))
	// 	return 0;

	/* Use the zero-page for reads */
	// if (!(vmf->flags & FAULT_FLAG_WRITE) &&
	// 		!mm_forbids_zeropage(vma->vm_mm)) {
	if (!(vmf->flags & FAULT_FLAG_WRITE)) {
		// entry = pte_mkspecial(pfn_pte(my_zero_pfn(vmf->address),
		// 				vma->vm_page_prot));
		entry = pfn_pte(my_zero_pfn(vmf->address), __pg(_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT));

		// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd,
		// 		vmf->address, &vmf->ptl);
		*vmf->pte = entry;
		if (!vmf->pte)
			goto unlock;
		if (vmf_pte_changed(vmf)) {
			// update_mmu_tlb(vma, vmf->address, vmf->pte);
			goto unlock;
		}
		// ret = check_stable_address_space(vma->vm_mm);
		// if (ret)
		// 	goto unlock;
		// /* Deliver the page fault to userland, check inside PT lock */
		// if (userfaultfd_missing(vma)) {
		// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
		// 	return handle_userfault(vmf, VM_UFFD_MISSING);
		// }
		// goto setpte;
	}

	// /* Allocate our own private page. */
	// if (unlikely(anon_vma_prepare(vma)))
	// 	goto oom;
	// folio = vma_alloc_zeroed_movable_folio(vma, vmf->address);
	folio = page_folio(alloc_page(GFP_USER | __GFP_ZERO));
	if (!folio)
		goto oom;

	// if (mem_cgroup_charge(folio, vma->vm_mm, GFP_KERNEL))
	// 	goto oom_free_page;
	// folio_throttle_swaprate(folio, GFP_KERNEL);

	// /*
	//  * The memory barrier inside __folio_mark_uptodate makes sure that
	//  * preceding stores to the page contents become visible before
	//  * the set_pte_at() write.
	//  */
	// __folio_mark_uptodate(folio);

	// entry = mk_pte(page, vma->vm_page_prot);
	entry = mk_pte(&folio->page, __pg(_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT));
	if (vma->vm_flags & VM_WRITE)
		entry = pte_mkwrite(pte_mkdirty(entry));

	// vmf->pte = pte_offset_map_lock(vma->vm_mm,
	// 		vmf->pmd, vmf->address, &vmf->ptl);
	*vmf->pte = entry;
	if (!vmf->pte)
		goto release;
	if (vmf_pte_changed(vmf)) {
		// update_mmu_tlb(vma, vmf->address, vmf->pte);
		goto release;
	}

	// ret = check_stable_address_space(vma->vm_mm);
	// if (ret)
	// 	goto release;

	// /* Deliver the page fault to userland, check inside PT lock */
	// if (userfaultfd_missing(vma)) {
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	// 	folio_put(folio);
	// 	return handle_userfault(vmf, VM_UFFD_MISSING);
	// }

	// inc_mm_counter(vma->vm_mm, MM_ANONPAGES);
	// folio_add_new_anon_rmap(folio, vma, vmf->address);
	// folio_add_lru_vma(folio, vma);
setpte:
	// if (uffd_wp)
	// 	entry = pte_mkuffd_wp(entry);
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte, entry);

	// /* No need to invalidate - it was non-present before */
	// update_mmu_cache_range(vmf, vma, vmf->address, vmf->pte, 1);
unlock:
	// if (vmf->pte)
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	return ret;
release:
	folio_put(folio);
	goto unlock;
oom_free_page:
	folio_put(folio);
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
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY |
			    VM_FAULT_DONE_COW)))
		return ret;

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
	// 	else if (unlikely(ptd_alloc(vma->vm_mm, vmf->pmd)))
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
	*vmf->pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);
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
	pgoff_t pte_off = pte_index_in_pmd(vmf->address);
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

	// if (pmd_none(*vmf->pmd)) {
	// 	vmf->prealloc_pte = pte_alloc_one(vmf->vma->vm_mm);
	// 	if (!vmf->prealloc_pte)
	// 		return VM_FAULT_OOM;
	// }

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

	// if (vmf->flags & FAULT_FLAG_VMA_LOCK) {
	// 	vma_end_read(vmf->vma);
	// 	return VM_FAULT_RETRY;
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
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		goto uncharge_out;
	if (ret & VM_FAULT_DONE_COW)
		return ret;

	copy_user_highpage(vmf->cow_page, vmf->page, vmf->address, vma);
	// __SetPageUptodate(vmf->cow_page);

	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
	put_page(vmf->page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		goto uncharge_out;
	return ret;
uncharge_out:
	put_page(vmf->cow_page);
	return ret;
}

static vm_fault_t
do_shared_fault(vm_fault_s *vmf) {
	pr_alert("API not implemented - 'do_shared_fault'\n");
	while (1);
	
	vma_s *vma = vmf->vma;
	vm_fault_t ret, tmp;

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

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
		
		// vmf->pte = pte_offset_map_lock(vmf->vma->vm_mm,
		// 			vmf->pmd, vmf->address, &vmf->ptl);
		if (unlikely(!vmf->pte))
			ret = VM_FAULT_SIGBUS;
		else {
			/*
			 * Make sure this is not a temporary clearing of pte
			 * by holding ptl and checking again. A R/M/W update
			 * of pte involves: take ptl, clearing the pte so that
			 * we don't have concurrent modification by hardware
			 * followed by an update.
			 */
			if (unlikely(pte_none(ptep_get_pte(vmf->pte))))
				ret = VM_FAULT_SIGBUS;
			else
				ret = VM_FAULT_NOPAGE;

			// pte_unmap_unlock(vmf->pte, vmf->ptl);
		}
	} else if (!(vmf->flags & FAULT_FLAG_WRITE))
		// Reading or Executing cause fault
		ret = do_read_fault(vmf);
	else if (!(vma->vm_flags & VM_SHARED))
		// Writing cause fault, but address is not VM_SHARED
		ret = do_cow_fault(vmf);
	else
		// Writing cause fault, and address is VM_SHARED
		ret = do_shared_fault(vmf);

	// /* preallocated pagetable is unused: free it */
	// if (vmf->prealloc_pte) {
	// 	pte_free(vm_mm, vmf->prealloc_pte);
	// 	vmf->prealloc_pte = NULL;
	// }
	return ret;
}

static vm_fault_t
do_pte_missing(vm_fault_s *vmf) {
	vmf->pte = ptd_alloc(vmf->vma->vm_mm, vmf->pmd, vmf->address);

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

	if (unlikely(pmd_none(*vmf->pmd))) {
		/*
		 * Leave __pte_alloc() until later: because vm_ops->fault may
		 * want to allocate huge page, and if we expose page table
		 * for an instant, it will be difficult to retract from
		 * concurrent faults and from rmap lookups.
		 */
		vmf->pte = NULL;
		vmf->flags &= ~FAULT_FLAG_ORIG_PTE_VALID;
	} else {
		/*
		 * A regular pmd is established and it can't morph into a huge
		 * pmd by anon khugepaged, since that takes mmap_lock in write
		 * mode; but shmem or file collapse to THP could still morph
		 * it into a huge pmd: just retry later if so.
		 */
		vmf->pte = pte_offset_map_nolock(vmf->vma->vm_mm,
						vmf->pmd, vmf->address, &vmf->ptl);
		if (unlikely(vmf->pte == NULL))
			return 0;
		vmf->orig_pte = ptep_get_lockless(vmf->pte);
		vmf->flags |= FAULT_FLAG_ORIG_PTE_VALID;

		if (pte_none(vmf->orig_pte)) {
			pte_unmap(vmf->pte);
			vmf->pte = NULL;
		}
	}

	if (vmf->pte == NULL)
		return do_pte_missing(vmf);

	// if (!pte_present(vmf->orig_pte))
	// 	return do_swap_page(vmf);

	// if (pte_protnone(vmf->orig_pte) && vma_is_accessible(vmf->vma))
	// 	return do_numa_page(vmf);

	// spin_lock(vmf->ptl);
	entry = vmf->orig_pte;
	// if (unlikely(!pte_same(*vmf->pte, entry))) {
	// 	update_mmu_tlb(vmf->vma, vmf->address, vmf->pte);
	// 	goto unlock;
	// }
	if (vmf->flags & FAULT_FLAG_WRITE) {
		vmf->page = vm_normal_page(vmf->vma, vmf->address, *(vmf->pte));
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
 * return value.  See simple_filemap_fault() and __folio_lock_or_retry().
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
		.curr_tsk	= current,
	};
	uint dirty = flags & FAULT_FLAG_WRITE;
	mm_s *mm = vma->vm_mm;
	pgd_t *pgd = pgd_ent_ptr_in_mm(mm, address);

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
	vmf.pte = ptd_alloc(mm, vmf.pmd, address);
	if (!vmf.pte)
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
int __pud_alloc(mm_s *mm, p4d_t *p4de_ptr, ulong address)
{
	spinlock_t *ptl;
	pud_t *new = pud_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	ptl = p4d_lock(mm, p4de_ptr);
	// spin_lock(&mm->page_table_lock);
	if (!p4d_present(*p4de_ptr)) {
		smp_wmb();	/* See comment in pmd_install() */
		*p4de_ptr = arch_make_p4de(_PAGE_TABLE | __pa(new));
	} else			/* Another has populated it */
		pud_free(new);
	// spin_unlock_no_resched(&mm->page_table_lock);
	spin_unlock_no_resched(ptl);
	return 0;
}

/*
 * Allocate page middle directory.
 * We've already handled the fast-path in-line.
 */
int __pmd_alloc(mm_s *mm, pud_t *pude_ptr, ulong address)
{
	spinlock_t *ptl;
	pmd_t *new = pmd_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	ptl = pud_lock(mm, pude_ptr);
	if (!pud_present(*pude_ptr)) {
		smp_wmb();	/* See comment in pmd_install() */
		*pude_ptr = arch_make_pude(_PAGE_TABLE | __pa(new));
	} else {		/* Another has populated it */
		pmd_free(new);
	}
	spin_unlock_no_resched(ptl);
	return 0;
}


int __pte_alloc(mm_s *mm, pmd_t *pmde_ptr, ulong address)
{
	spinlock_t *ptl;
	pte_t *new = pte_alloc_one(mm);
	if (!new)
		return -ENOMEM;

	ptl = pmd_lock(mm, pmde_ptr);
	if (!pmd_present(*pmde_ptr)) {
		smp_wmb();	/* See comment in pmd_install() */
		*pmde_ptr = arch_make_pmde(_PAGE_TABLE | __pa(new));
	} else {		/* Another has populated it */
		pte_free(new);
	}
	spin_unlock_no_resched(ptl);
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

	pgd = pgd_ent_ptr_in_mm(mm, addr);
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
	pte = ptd_alloc(mm, pmd, addr);
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

	pgd = pgd_ent_ptr_in_mm(mm, addr);
	p4d = p4de_ptr_in_pgd(pgd, addr);
	if (p4d_none(*p4d)) {
		goto fail;
	}
	pud = pude_ptr_in_p4d(p4d, addr);
	if (pud_none(*pud)) {
		goto fail;
	}
	pmd = pmde_ptr_in_pud(pud, addr);
	if (pmd_none(*pmd)) {
		goto fail;
	}
	pte = pte_ptr_in_pmd(pmd, addr);
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
		pgd = pgd_ent_ptr_in_mm(mm, addr);
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
		pte = ptd_alloc(mm, pmd, addr);
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