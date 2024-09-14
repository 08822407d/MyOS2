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
	*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
	virt_addr_t vaddr = page_to_virt(*pgcache_ptr);

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
// 	addr_spc_s *mapping = vmf->vma->vm_file->f_mapping;
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


/**
 * filemap_read - Read data from the page cache.
 * @iocb: The iocb to read.
 * @iter: Destination for the data.
 * @already_read: Number of bytes already read by the caller.
 *
 * Copies data from the page cache.  If the data is not currently present,
 * uses the readahead and read_folio address_space operations to fetch it.
 *
 * Return: Total number of bytes copied, including those already read by
 * the caller.  If an error happens before any bytes are copied, returns
 * a negative error number.
 */
ssize_t
simple_filemap_read(kiocb_s *iocb, iov_iter_s *iter)
{
	file_s *filp = iocb->ki_filp;
	addr_spc_s *mapping = filp->f_mapping;
	inode_s *inode = mapping->host;
	void *bufp = iter->kvec->iov_base;
	loff_t
		start = iocb->ki_pos,
		end = start + iter->kvec->iov_len;
	
	for (loff_t pgcache_pos = PAGE_ALIGN_DOWN(start); pgcache_pos < end; pgcache_pos += PAGE_SIZE) {
		loff_t temp_pos = pgcache_pos;
		virt_addr_t vaddr = 0;
		page_s **pgcache_ptr = &(mapping->page_array[pgcache_pos >> PAGE_SHIFT]);
		if (*pgcache_ptr == NULL) {
			*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
			vaddr = page_to_virt(*pgcache_ptr);
			filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &temp_pos);
		}
		BUG_ON(*pgcache_ptr == NULL);
		vaddr = page_to_virt(*pgcache_ptr);

		loff_t inpage_start = max(pgcache_pos, start) % PAGE_SIZE;
		loff_t len = 0;
		if (pgcache_pos + PAGE_SIZE < end)
			len = PAGE_SIZE - inpage_start;
		else
			len = end % PAGE_SIZE - inpage_start;

		memcpy(bufp, (void *)(vaddr + inpage_start), len);
		bufp += len;
	}
	return bufp - iter->kvec->iov_base;
}
EXPORT_SYMBOL_GPL(simple_filemap_read);

/**
 * generic_file_read_iter - generic filesystem read routine
 * @iocb:	kernel I/O control block
 * @iter:	destination for the data read
 *
 * This is the "read_iter()" routine for all filesystems
 * that can use the page cache directly.
 *
 * The IOCB_NOWAIT flag in iocb->ki_flags indicates that -EAGAIN shall
 * be returned when no data can be read without waiting for I/O requests
 * to complete; it doesn't prevent readahead.
 *
 * The IOCB_NOIO flag in iocb->ki_flags indicates that no new I/O
 * requests shall be made for the read or for readahead.  When no data
 * can be read, -EAGAIN shall be returned.  When readahead would be
 * triggered, a partial, possibly empty read shall be returned.
 *
 * Return:
 * * number of bytes copied, even for partial reads
 * * negative error code (or 0 if IOCB_NOIO) if nothing was read
 */
ssize_t
generic_file_read_iter(kiocb_s *iocb, iov_iter_s *iter)
{
	// size_t count = iov_iter_count(iter);
	size_t count = iter->count;
	ssize_t retval = 0;

	if (!count)
		return 0; /* skip atime */

	// if (iocb->ki_flags & IOCB_DIRECT) {
	// 	file_s *file = iocb->ki_filp;
	// 	addr_spc_s *mapping = file->f_mapping;
	// 	inode_s *inode = mapping->host;

	// 	retval = kiocb_write_and_wait(iocb, count);
	// 	if (retval < 0)
	// 		return retval;
	// 	file_accessed(file);

	// 	retval = mapping->a_ops->direct_IO(iocb, iter);
	// 	if (retval >= 0) {
	// 		iocb->ki_pos += retval;
	// 		count -= retval;
	// 	}
	// 	if (retval != -EIOCBQUEUED)
	// 		iov_iter_revert(iter, count - iov_iter_count(iter));

	// 	/*
	// 	 * Btrfs can have a short DIO read if we encounter
	// 	 * compressed extents, so if there was an error, or if
	// 	 * we've already read everything we wanted to, or if
	// 	 * there was a short read because we hit EOF, go ahead
	// 	 * and return.  Otherwise fallthrough to buffered io for
	// 	 * the rest of the read.  Buffered reads will not work for
	// 	 * DAX files, so don't bother trying.
	// 	 */
	// 	if (retval < 0 || !count || IS_DAX(inode))
	// 		return retval;
	// 	if (iocb->ki_pos >= i_size_read(inode))
	// 		return retval;
	// }

	return simple_filemap_read(iocb, iter);
}
EXPORT_SYMBOL(generic_file_read_iter);


ssize_t
generic_perform_write(kiocb_s *iocb, iov_iter_s *i)
{
	file_s *file = iocb->ki_filp;
	loff_t pos = iocb->ki_pos;
	addr_spc_s *mapping = file->f_mapping;
	const addr_spc_ops_s *a_ops = mapping->a_ops;
	long status = 0;
	ssize_t written = 0;

	pr_alert("API not implemented - %s\n", "generic_file_write_iter");
	while (1);
}
EXPORT_SYMBOL(generic_perform_write);

/**
 * __generic_file_write_iter - write data to a file
 * @iocb:	IO state structure (file, offset, etc.)
 * @from:	iov_iter with data to write
 *
 * This function does all the work needed for actually writing data to a
 * file. It does all basic checks, removes SUID from the file, updates
 * modification times and calls proper subroutines depending on whether we
 * do direct IO or a standard buffered write.
 *
 * It expects i_rwsem to be grabbed unless we work on a block device or similar
 * object which does not need locking at all.
 *
 * This function does *not* take care of syncing data in case of O_SYNC write.
 * A caller has to handle it. This is mainly due to the fact that we want to
 * avoid syncing under i_rwsem.
 *
 * Return:
 * * number of bytes written, even for truncated writes
 * * negative error code if no data has been written at all
 */
ssize_t
__generic_file_write_iter(kiocb_s *iocb, iov_iter_s *from)
{
	file_s *file = iocb->ki_filp;
	addr_spc_s *mapping = file->f_mapping;
	inode_s *inode = mapping->host;
	ssize_t ret;

	// ret = file_remove_privs(file);
	// if (ret)
	// 	return ret;

	// ret = file_update_time(file);
	// if (ret)
	// 	return ret;

	// if (iocb->ki_flags & IOCB_DIRECT) {
	// 	ret = generic_file_direct_write(iocb, from);
	// 	/*
	// 	 * If the write stopped short of completing, fall back to
	// 	 * buffered writes.  Some filesystems do this for writes to
	// 	 * holes, for example.  For DAX files, a buffered write will
	// 	 * not succeed (even if it did, DAX does not handle dirty
	// 	 * page-cache pages correctly).
	// 	 */
	// 	if (ret < 0 || !iov_iter_count(from) || IS_DAX(inode))
	// 		return ret;
	// 	return direct_write_fallback(iocb, from, ret,
	// 			generic_perform_write(iocb, from));
	// }

	return generic_perform_write(iocb, from);
}
EXPORT_SYMBOL(__generic_file_write_iter);


/**
 * generic_file_write_iter - write data to a file
 * @iocb:	IO state structure
 * @from:	iov_iter with data to write
 *
 * This is a wrapper around __generic_file_write_iter() to be used by most
 * filesystems. It takes care of syncing the file in case of O_SYNC file
 * and acquires i_rwsem as needed.
 * Return:
 * * negative error code if no data has been written at all of
 *   vfs_fsync_range() failed for a synchronous write
 * * number of bytes written, even for truncated writes
 */
ssize_t generic_file_write_iter(kiocb_s *iocb, iov_iter_s *from)
{
	file_s *file = iocb->ki_filp;
	inode_s *inode = file->f_mapping->host;
	ssize_t ret;

	// inode_lock(inode);
	// ret = generic_write_checks(iocb, from);
	// if (ret > 0)
	// 	ret = __generic_file_write_iter(iocb, from);
	// inode_unlock(inode);

	// if (ret > 0)
	// 	ret = generic_write_sync(iocb, ret);
	return ret;
}
EXPORT_SYMBOL(generic_file_write_iter);


/* This is used for a general mmap of a disk file */
int generic_file_mmap(file_s *file, vma_s *vma)
{
	addr_spc_s *mapping = file->f_mapping;

	// if (!mapping->a_ops->read_folio)
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
}
EXPORT_SYMBOL(generic_file_mmap);