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
#include <linux/fs/file.h>
#include <linux/fs/shmem_fs.h>


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
 *    ->invalidate_lock		(simple_filemap_fault)
 *      ->lock_page		(simple_filemap_fault, access_process_vm)
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


/**
 * simple_filemap_fault - read in file data for page fault handling
 * @vmf:	vm_fault_s containing details of the fault
 *
 * simple_filemap_fault() is invoked via the vma operations vector for a
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
vm_fault_t simple_filemap_fault(vm_fault_s *vmf)
{
	file_s *file = vmf->vma->vm_file;
	if (IS_ERR_OR_NULL(file))
		return VM_FAULT_ERROR;
	addr_spc_s *mapping = file->f_mapping;
	if (IS_ERR_OR_NULL(mapping))
		return VM_FAULT_ERROR;

	pgoff_t index = vmf->pgoff;
	loff_t pos = index << PAGE_SHIFT;
	page_s **pgcache_ptr = &mapping->page_array[index];
	BUG_ON(*pgcache_ptr != NULL);
	*pgcache_ptr = alloc_page(GFP_USER);
	virt_addr_t vaddr = page_to_virt(*pgcache_ptr);

	memset((void *)vaddr, 0, PAGE_SIZE);
	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
	vmf->page = *pgcache_ptr;

	return 0;
}


vm_fault_t
simple_filemap_map_page(vm_fault_s *vmf, pgoff_t pgoff)
{
	vma_s *vma = vmf->vma;
	file_s *file = vma->vm_file;
	addr_spc_s *mapping = file->f_mapping;
	page_s *page;

	page = mapping->page_array[pgoff];
	if (page != NULL) {
		*vmf->pte = mk_pte(page,
			__pg(_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT));
		return 0;
	}
	else
		return VM_FAULT_NOPAGE;
}


vm_fault_t filemap_page_mkwrite(vm_fault_s *vmf)
{
// 	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
// 	folio_s *folio = page_folio(vmf->page);
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
	.fault				= simple_filemap_fault,
	.map_single_page	= simple_filemap_map_page,
	.page_mkwrite		= filemap_page_mkwrite,
};

/* This is used for a general mmap of a disk file */

int generic_file_mmap(file_s *file, vma_s *vma)
{
	// addr_spc_s *mapping = file->f_mapping;

	// if (!mapping->a_ops->readpage)
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
}