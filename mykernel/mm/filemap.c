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
// #include <linux/export.h>
#include <linux/kernel/compiler.h>
// #include <linux/dax.h>
#include <linux/fs/fs.h>
#include <linux/sched/signal.h>
// #include <linux/uaccess.h>
// #include <linux/capability.h>
// #include <linux/kernel_stat.h>
#include <linux/mm/gfp.h>
#include <linux/mm/mm.h>
// #include <linux/swap.h>
// #include <linux/swapops.h>
#include <linux/mm/mman.h>
#include <linux/mm/pagemap.h>
#include <linux/fs/file.h>
// #include <linux/uio.h>
// #include <linux/error-injection.h>
// #include <linux/hash.h>
// #include <linux/writeback.h>
// #include <linux/backing-dev.h>
// #include <linux/pagevec.h>
// #include <linux/security.h>
// #include <linux/cpuset.h>
// #include <linux/hugetlb.h>
// #include <linux/memcontrol.h>
#include <linux/mm/shmem_fs.h>
#include <linux/mm/rmap.h>
// #include <linux/delayacct.h>
// #include <linux/psi.h>
// #include <linux/ramfs.h>
// #include <linux/page_idle.h>
// #include <linux/migrate.h>
#include <asm/pgalloc.h>
// #include <asm/tlbflush.h>
#include "internal.h"

// #define CREATE_TRACE_POINTS
// #include <trace/events/filemap.h>

/*
 * FIXME: remove all knowledge of the buffer layer from the core VM
 */
// #include <linux/buffer_head.h> /* for try_to_free_buffers */

#include <asm/mman.h>

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




/* This is used for a general mmap of a disk file */

int generic_file_mmap(file_s *file, vma_s *vma)
{
	loff_t pos;
	file->f_op->read(file, (void *)vma->vm_start,
			vma->vm_end - vma->vm_start, &pos);
	// struct address_space *mapping = file->f_mapping;

	// if (!mapping->a_ops->readpage)
	// 	return -ENOEXEC;
	// file_accessed(file);
	// vma->vm_ops = &generic_file_vm_ops;
	return 0;
}