// SPDX-License-Identifier: GPL-2.0-only
/*
 *	linux/mm/filemap.c
 *
 * Copyright (C) 1994-1999  Linus Torvalds
 */

/*
 * This file handles the generic file mmap semantics used by
 * most "normal" filesystems (but you don't /have/ to use this:
 * the NFS filesystem used to do this differently, for example)
 */
#include <linux/kernel/export.h>
#include <linux/compiler/compiler.h>
#include <linux/fs/fs.h>
#include <linux/sched/signal.h>
#include <linux/kernel/uaccess.h>
#include <linux/mm/gfp.h>
#include <linux/mm/mm.h>
#include <linux/mm/mman.h>
#include <linux/mm/pagemap.h>
#include <linux/fs/file.h>
#include <linux/mm/shmem_fs.h>
#include <linux/mm/rmap.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>


/*
 * FIXME: remove all knowledge of the buffer layer from the core VM
 */
#include <asm/uapi_mman.h>

/*
 * Shared mappings implemented 30.11.1994. It's not fully working yet,
 * though.
 *
 * Shared mappings now work. 15.8.1995  Bruno.
 *
 * finished 'unifying' the page and buffer cache and SMP-threaded the
 * page-cache, 21.05.1999, Ingo Molnar <mingo@redhat.com>
 *
 * SMP-threaded pagemap-LRU 1999, Andrea Arcangeli <andrea@suse.de>
 */

/*
 * Lock ordering:
 *
 *  ->i_mmap_rwsem		(truncate_pagecache)
 *    ->private_lock		(__free_pte->__set_page_dirty_buffers)
 *      ->swap_lock		(exclusive_swap_page, others)
 *        ->i_pages lock
 *
 *  ->i_rwsem
 *    ->invalidate_lock		(acquired by fs in truncate path)
 *      ->i_mmap_rwsem		(truncate->unmap_mapping_range)
 *
 *  ->mmap_lock
 *    ->i_mmap_rwsem
 *      ->page_table_lock or pte_lock	(various, mainly in memory.c)
 *        ->i_pages lock	(arch-dependent flush_dcache_mmap_lock)
 *
 *  ->mmap_lock
 *    ->invalidate_lock		(filemap_fault)
 *      ->lock_page		(filemap_fault, access_process_vm)
 *
 *  ->i_rwsem			(generic_perform_write)
 *    ->mmap_lock		(fault_in_readable->do_page_fault)
 *
 *  bdi->wb.list_lock
 *    sb_lock			(fs/fs-writeback.c)
 *    ->i_pages lock		(__sync_single_inode)
 *
 *  ->i_mmap_rwsem
 *    ->anon_vma.lock		(vma_adjust)
 *
 *  ->anon_vma.lock
 *    ->page_table_lock or pte_lock	(anon_vma_prepare and various)
 *
 *  ->page_table_lock or pte_lock
 *    ->swap_lock		(try_to_unmap_one)
 *    ->private_lock		(try_to_unmap_one)
 *    ->i_pages lock		(try_to_unmap_one)
 *    ->lruvec->lru_lock	(follow_page->mark_page_accessed)
 *    ->lruvec->lru_lock	(check_pte_range->isolate_lru_page)
 *    ->private_lock		(page_remove_rmap->set_page_dirty)
 *    ->i_pages lock		(page_remove_rmap->set_page_dirty)
 *    bdi.wb->list_lock		(page_remove_rmap->set_page_dirty)
 *    ->inode->i_lock		(page_remove_rmap->set_page_dirty)
 *    ->memcg->move_lock	(page_remove_rmap->lock_page_memcg)
 *    bdi.wb->list_lock		(zap_pte_range->set_page_dirty)
 *    ->inode->i_lock		(zap_pte_range->set_page_dirty)
 *    ->private_lock		(zap_pte_range->__set_page_dirty_buffers)
 *
 * ->i_mmap_rwsem
 *   ->tasklist_lock            (memory_failure, collect_procs_ao)
 */


static page_s
*myos_readpage(vm_fault_s *vmf)
{
	vma_s *vma = vmf->vma;
	file_s *filp = vma->vm_file;
	if (filp == NULL)
		return NULL;

	int pg_idx = (vmf->address - vma->vm_start) / PAGE_SIZE;
	loff_t pos = (vma->vm_pgoff + pg_idx) * PAGE_SIZE;
	page_s *retval = alloc_page(GFP_USER);
	virt_addr_t vaddr = page_to_virt(retval);
	memset((void *)vaddr, 0, PAGE_SIZE);
	filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &pos);

	return retval;
}

/**
 * filemap_fault - read in file data for page fault handling
 * @vmf:	vm_fault_s containing details of the fault
 *
 * filemap_fault() is invoked via the vma operations vector for a
 * mapped memory region to read in file data during a page fault.
 *
 * The goto's are kind of ugly, but this streamlines the normal case of having
 * it in the page cache, and handles the special cases reasonably without
 * having a lot of duplicated code.
 *
 * vma->vm_mm->mmap_lock must be held on entry.
 *
 * If our return value has VM_FAULT_RETRY set, it's because the mmap_lock
 * may be dropped before doing I/O or by lock_folio_maybe_drop_mmap().
 *
 * If our return value does not have VM_FAULT_RETRY set, the mmap_lock
 * has not been released.
 *
 * We never return with VM_FAULT_RETRY and a bit from VM_FAULT_ERROR set.
 *
 * Return: bitwise-OR of %VM_FAULT_ codes.
 */
vm_fault_t filemap_fault(vm_fault_s *vmf)
{
	int error;
	file_s *file = vmf->vma->vm_file;
	file_s *fpin = NULL;
	// struct address_space *mapping = file->f_mapping;
	// struct inode *inode = mapping->host;
	pgoff_t max_idx, index = vmf->pgoff;
	// struct folio *folio;
	vm_fault_t ret = 0;
	// bool mapping_locked = false;

	// max_idx = DIV_ROUND_UP(i_size_read(inode), PAGE_SIZE);
	// if (unlikely(index >= max_idx))
	// 	return VM_FAULT_SIGBUS;

	// /*
	//  * Do we have something in the page cache already?
	//  */
	// folio = filemap_get_folio(mapping, index);
	// if (likely(folio)) {
	// 	/*
	// 	 * We found the page, so try async readahead before waiting for
	// 	 * the lock.
	// 	 */
	// 	if (!(vmf->flags & FAULT_FLAG_TRIED))
	// 		fpin = do_async_mmap_readahead(vmf, folio);
	// 	if (unlikely(!folio_test_uptodate(folio))) {
	// 		filemap_invalidate_lock_shared(mapping);
	// 		mapping_locked = true;
	// 	}
	// } else {
	// 	/* No page in the page cache at all */
	// 	count_vm_event(PGMAJFAULT);
	// 	count_memcg_event_mm(vmf->vma->vm_mm, PGMAJFAULT);
	// 	ret = VM_FAULT_MAJOR;
	// 	fpin = do_sync_mmap_readahead(vmf);
// retry_find:
	// 	/*
	// 	 * See comment in filemap_create_folio() why we need
	// 	 * invalidate_lock
	// 	 */
	// 	if (!mapping_locked) {
	// 		filemap_invalidate_lock_shared(mapping);
	// 		mapping_locked = true;
	// 	}
	// 	folio = __filemap_get_folio(mapping, index,
	// 				  FGP_CREAT|FGP_FOR_MMAP,
	// 				  vmf->gfp_mask);
	// 	if (!folio) {
	// 		if (fpin)
	// 			goto out_retry;
	// 		filemap_invalidate_unlock_shared(mapping);
	// 		return VM_FAULT_OOM;
	// 	}
	// }

	// if (!lock_folio_maybe_drop_mmap(vmf, folio, &fpin))
	// 	goto out_retry;

	// /* Did it get truncated? */
	// if (unlikely(folio->mapping != mapping)) {
	// 	folio_unlock(folio);
	// 	folio_put(folio);
	// 	goto retry_find;
	// }
	// VM_BUG_ON_FOLIO(!folio_contains(folio, index), folio);

	// /*
	//  * We have a locked page in the page cache, now we need to check
	//  * that it's up-to-date. If not, it is going to be due to an error.
	//  */
	// if (unlikely(!folio_test_uptodate(folio))) {
	// 	/*
	// 	 * The page was in cache and uptodate and now it is not.
	// 	 * Strange but possible since we didn't hold the page lock all
	// 	 * the time. Let's drop everything get the invalidate lock and
	// 	 * try again.
	// 	 */
	// 	if (!mapping_locked) {
	// 		folio_unlock(folio);
	// 		folio_put(folio);
	// 		goto retry_find;
	// 	}
	// 	goto page_not_uptodate;
	// }

	// /*
	//  * We've made it this far and we had to drop our mmap_lock, now is the
	//  * time to return to the upper layer and have it re-find the vma and
	//  * redo the fault.
	//  */
	// if (fpin) {
	// 	folio_unlock(folio);
	// 	goto out_retry;
	// }
	// if (mapping_locked)
	// 	filemap_invalidate_unlock_shared(mapping);

	// /*
	//  * Found the page and have a reference on it.
	//  * We must recheck i_size under page lock.
	//  */
	// max_idx = DIV_ROUND_UP(i_size_read(inode), PAGE_SIZE);
	// if (unlikely(index >= max_idx)) {
	// 	folio_unlock(folio);
	// 	folio_put(folio);
	// 	return VM_FAULT_SIGBUS;
	// }

	// vmf->page = folio_file_page(folio, index);
	vmf->page = myos_readpage(vmf);
	return ret | VM_FAULT_LOCKED;

// page_not_uptodate:
	// /*
	//  * Umm, take care of errors if the page isn't up-to-date.
	//  * Try to re-read it _once_. We do this synchronously,
	//  * because there really aren't any performance issues here
	//  * and we need to check for errors.
	//  */
	// fpin = maybe_unlock_mmap_for_io(vmf, fpin);
	// error = filemap_read_folio(file, mapping, folio);
	// if (fpin)
	// 	goto out_retry;
	// folio_put(folio);

	// if (!error || error == AOP_TRUNCATED_PAGE)
	// 	goto retry_find;
	// filemap_invalidate_unlock_shared(mapping);

	// return VM_FAULT_SIGBUS;

// out_retry:
	// /*
	//  * We dropped the mmap_lock, we need to return to the fault handler to
	//  * re-find the vma and come back and find our hopefully still populated
	//  * page.
	//  */
	// if (folio)
	// 	folio_put(folio);
	// if (mapping_locked)
	// 	filemap_invalidate_unlock_shared(mapping);
	// if (fpin)
	// 	fput(fpin);
	// return ret | VM_FAULT_RETRY;
}


vm_fault_t filemap_map_pages(vm_fault_s *vmf,
		pgoff_t start_pgoff, pgoff_t end_pgoff)
{
// 	vma_s *vma = vmf->vma;
// 	file_s *file = vma->vm_file;
// 	struct address_space *mapping = file->f_mapping;
// 	pgoff_t last_pgoff = start_pgoff;
// 	unsigned long addr;
// 	XA_STATE(xas, &mapping->i_pages, start_pgoff);
// 	struct folio *folio;
// 	struct page *page;
// 	unsigned int mmap_miss = READ_ONCE(file->f_ra.mmap_miss);
// 	vm_fault_t ret = 0;

// 	rcu_read_lock();
// 	folio = first_map_page(mapping, &xas, end_pgoff);
// 	if (!folio)
// 		goto out;

// 	if (filemap_map_pmd(vmf, &folio->page)) {
// 		ret = VM_FAULT_NOPAGE;
// 		goto out;
// 	}

// 	addr = vma->vm_start + ((start_pgoff - vma->vm_pgoff) << PAGE_SHIFT);
// 	vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd, addr, &vmf->ptl);
// 	do {
// again:
// 		page = folio_file_page(folio, xas.xa_index);
// 		if (PageHWPoison(page))
// 			goto unlock;

// 		if (mmap_miss > 0)
// 			mmap_miss--;

// 		addr += (xas.xa_index - last_pgoff) << PAGE_SHIFT;
// 		vmf->pte += xas.xa_index - last_pgoff;
// 		last_pgoff = xas.xa_index;

// 		if (!pte_none(*vmf->pte))
// 			goto unlock;

// 		/* We're about to handle the fault */
// 		if (vmf->address == addr)
// 			ret = VM_FAULT_NOPAGE;

// 		do_set_pte(vmf, page, addr);
// 		/* no need to invalidate: a not-present page won't be cached */
// 		update_mmu_cache(vma, addr, vmf->pte);
// 		if (folio_more_pages(folio, xas.xa_index, end_pgoff)) {
// 			xas.xa_index++;
// 			folio_ref_inc(folio);
// 			goto again;
// 		}
// 		folio_unlock(folio);
// 		continue;
// unlock:
// 		if (folio_more_pages(folio, xas.xa_index, end_pgoff)) {
// 			xas.xa_index++;
// 			goto again;
// 		}
// 		folio_unlock(folio);
// 		folio_put(folio);
// 	} while ((folio = next_map_page(mapping, &xas, end_pgoff)) != NULL);
// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
// out:
// 	rcu_read_unlock();
// 	WRITE_ONCE(file->f_ra.mmap_miss, mmap_miss);
// 	return ret;
}

vm_fault_t filemap_page_mkwrite(vm_fault_s *vmf)
{
// 	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
// 	struct folio *folio = page_folio(vmf->page);
// 	vm_fault_t ret = VM_FAULT_LOCKED;

// 	sb_start_pagefault(mapping->host->i_sb);
// 	file_update_time(vmf->vma->vm_file);
// 	folio_lock(folio);
// 	if (folio->mapping != mapping) {
// 		folio_unlock(folio);
// 		ret = VM_FAULT_NOPAGE;
// 		goto out;
// 	}
// 	/*
// 	 * We mark the folio dirty already here so that when freeze is in
// 	 * progress, we are guaranteed that writeback during freezing will
// 	 * see the dirty folio and writeprotect it again.
// 	 */
// 	folio_mark_dirty(folio);
// 	folio_wait_stable(folio);
// out:
// 	sb_end_pagefault(mapping->host->i_sb);
// 	return ret;
}

const vm_ops_s generic_file_vm_ops = {
	.fault			= filemap_fault,
	.map_pages		= filemap_map_pages,
	.page_mkwrite	= filemap_page_mkwrite,
};

/* This is used for a general mmap of a disk file */

int generic_file_mmap(file_s *file, vma_s *vma)
{
	// struct address_space *mapping = file->f_mapping;

	// if (!mapping->a_ops->readpage)
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
}