// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/buffer.c
 *
 *  Copyright (C) 1991, 1992, 2002  Linus Torvalds
 */

/*
 * Start bdflush() with kernel_thread not syscall - Paul Gortmaker, 12/95
 *
 * Removed a lot of unnecessary code and simplified things now that
 * the buffer cache isn't our primary cache - Andrew Tridgell 12/96
 *
 * Speed up hash, lru, and free list operations.  Use gfp() for allocating
 * hash table, use SLAB cache for buffer heads. SMP threading.  -DaveM
 *
 * Added 32k buffer block sizes - these are required older ARM systems. - RMK
 *
 * async buffer flushing, 1999 Andrea Arcangeli <andrea@suse.de>
 */

#include <linux/kernel/kernel.h>
#include <linux/sched/signal.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/fs.h>
// #include <linux/iomap.h>
#include <linux/mm/mm.h>
// #include <linux/percpu.h>
#include <linux/kernel/slab.h>
// #include <linux/capability.h>
#include <linux/block/blkdev.h>
#include <linux/fs/file.h>
// #include <linux/quotaops.h>
// #include <linux/highmem.h>
// #include <linux/export.h>
// #include <linux/backing-dev.h>
// #include <linux/writeback.h>
// #include <linux/hash.h>
// #include <linux/suspend.h>
// #include <linux/block/buffer_head.h>
// #include <linux/task_io_accounting_ops.h>
// #include <linux/bio.h>
#include <linux/kernel/cpu.h>
#include <linux/kernel/bitops.h>
// #include <linux/mpage.h>
// #include <linux/bit_spinlock.h>
// #include <linux/pagevec.h>
#include <linux/sched/mm.h>
// #include <trace/events/block.h>
// #include <linux/fscrypt.h>
#include "internal.h"


// /*
//  * __getblk_gfp() will locate (and, if necessary, create) the buffer_head
//  * which corresponds to the passed block_device, block and size. The
//  * returned buffer has its reference count incremented.
//  *
//  * __getblk_gfp() will lock up the machine if grow_dev_page's
//  * try_to_free_buffers() attempt is failing.  FIXME, perhaps?
//  */
// buffer_head_s *
// __getblk_gfp(blk_dev_s *bdev, sector_t block, unsigned size)
// {
// 	// // struct buffer_head *bh = __find_get_block(bdev, block, size);
// 	// if (bh == NULL)
// 	// 	bh = __getblk_slow(bdev, block, size);
	
// 	buffer_head_s *bh = kmalloc(sizeof(buffer_head_s), GFP_KERNEL);

// 	return bh;
// }

// /*
//  * Decrement a buffer_head's reference count.  If all buffers against a page
//  * have zero reference count, are clean and unlocked, and if the page is clean
//  * and unlocked then try_to_free_buffers() may strip the buffers from the page
//  * in preparation for freeing it (sometimes, rarely, buffers are removed from
//  * a page but it ends up not being freed, and buffers may later be reattached).
//  */
// void __brelse(buffer_head_s * buf)
// {
// 	// if (atomic_read(&buf->b_count)) {
// 	// 	put_bh(buf);
// 	// 	return;
// 	// }

// 	if (buf != NULL)
// 	{
// 		if (buf->b_data != NULL)
// 			kfree(buf->b_data);

// 		kfree(buf);
// 	}
// }

// /**
//  *  __bread_gfp() - reads a specified block and returns the bh
//  *  @bdev: the block_device to read from
//  *  @block: number of block
//  *  @size: size (in bytes) to read
//  *  @gfp: page allocation flag
//  *
//  *  Reads a specified block, and returns buffer head that contains it.
//  *  The page cache can be allocated from non-movable area
//  *  not to prevent page migration if you set gfp to zero.
//  *  It returns NULL if the block was unreadable.
//  */
// buffer_head_s *
// __bread_gfp(blk_dev_s *bdev, sector_t block, unsigned size)
// {
// 	buffer_head_s *bh = __getblk_gfp(bdev, block, size);

// 	// if (likely(bh) && !buffer_uptodate(bh))
// 	// 	bh = __bread_slow(bh);
// 	return bh;
// }