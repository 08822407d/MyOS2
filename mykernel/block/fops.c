// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 1991, 1992  Linus Torvalds
 * Copyright (C) 2001  Andrea Arcangeli <andrea@suse.de> SuSE
 * Copyright (C) 2016 - 2020 Christoph Hellwig
 */
#include <linux/init/init.h>
#include <linux/kernel/mm.h>
#include <linux/block/blkdev.h>
// #include <linux/block/buffer_head.h>
// #include <linux/mpage.h>
// #include <linux/uio.h>
#include <linux/fs/namei.h>
// #include <linux/task_io_accounting_ops.h>
// #include <linux/falloc.h>
// #include <linux/suspend.h>
#include <linux/fs/fs.h>
// #include <linux/module.h>
#include "blk.h"

/*
 * for a block special file file_inode(file)->i_size is zero
 * so we compute the size by hand (just as in block_read/write above)
 */
static loff_t blkdev_llseek(file_s *file, loff_t offset, int whence)
{
	// struct inode *bd_inode = bdev_file_inode(file);
	// loff_t retval;

	// inode_lock(bd_inode);
	// retval = fixed_size_llseek(file, offset, whence, i_size_read(bd_inode));
	// inode_unlock(bd_inode);
	// return retval;
}

static int blkdev_fsync(file_s *filp, loff_t start, loff_t end, int datasync)
{
	// struct block_device *bdev = filp->private_data;
	// int error;

	// error = file_write_and_wait_range(filp, start, end);
	// if (error)
	// 	return error;

	// /*
	//  * There is no need to serialise calls to blkdev_issue_flush with
	//  * i_mutex and doing so causes performance issues with concurrent
	//  * O_SYNC writers to a block device.
	//  */
	// error = blkdev_issue_flush(bdev);
	// if (error == -EOPNOTSUPP)
	// 	error = 0;

	// return error;
}

static int blkdev_open(inode_s *inode, file_s *filp)
{
	blk_dev_s *bdev;

	/*
	 * Preserve backwards compatibility and allow large file access
	 * even if userspace doesn't ask for it explicitly. Some mkfs
	 * binary needs it. We might want to drop this workaround
	 * during an unstable branch.
	 */
	filp->f_flags |= O_LARGEFILE;
	filp->f_mode |= FMODE_NOWAIT | FMODE_BUF_RASYNC;

	if (filp->f_flags & O_NDELAY)
		filp->f_mode |= FMODE_NDELAY;
	if (filp->f_flags & O_EXCL)
		filp->f_mode |= FMODE_EXCL;
	if ((filp->f_flags & O_ACCMODE) == 3)
		filp->f_mode |= FMODE_WRITE_IOCTL;

	bdev = blkdev_get_by_dev(inode->i_rdev, filp->f_mode);
	if (IS_ERR(bdev))
		return PTR_ERR(bdev);

	filp->private_data = bdev;
	// filp->f_mapping = bdev->bd_inode->i_mapping;
	// filp->f_wb_err = filemap_sample_wb_err(filp->f_mapping);

	filp->f_op = bdev->bd_disk->myos_bd_fops;

	return 0;
}

static int blkdev_close(inode_s *inode, file_s *filp)
{
	blk_dev_s *bdev = filp->private_data;

	// blkdev_put(bdev, filp->f_mode);
	return 0;
}

// /*
//  * Write data to the block device.  Only intended for the block device itself
//  * and the raw driver which basically is a fake block device.
//  *
//  * Does not take i_mutex for the write and thus is not for general purpose
//  * use.
//  */
// static ssize_t blkdev_write_iter(struct kiocb *iocb, struct iov_iter *from)
// {
// 	struct block_device *bdev = iocb->ki_filp->private_data;
// 	struct inode *bd_inode = bdev->bd_inode;
// 	loff_t size = bdev_nr_bytes(bdev);
// 	struct blk_plug plug;
// 	size_t shorted = 0;
// 	ssize_t ret;

// 	if (bdev_read_only(bdev))
// 		return -EPERM;

// 	if (IS_SWAPFILE(bd_inode) && !is_hibernate_resume_dev(bd_inode->i_rdev))
// 		return -ETXTBSY;

// 	if (!iov_iter_count(from))
// 		return 0;

// 	if (iocb->ki_pos >= size)
// 		return -ENOSPC;

// 	if ((iocb->ki_flags & (IOCB_NOWAIT | IOCB_DIRECT)) == IOCB_NOWAIT)
// 		return -EOPNOTSUPP;

// 	size -= iocb->ki_pos;
// 	if (iov_iter_count(from) > size) {
// 		shorted = iov_iter_count(from) - size;
// 		iov_iter_truncate(from, size);
// 	}

// 	blk_start_plug(&plug);
// 	ret = __generic_file_write_iter(iocb, from);
// 	if (ret > 0)
// 		ret = generic_write_sync(iocb, ret);
// 	iov_iter_reexpand(from, iov_iter_count(from) + shorted);
// 	blk_finish_plug(&plug);
// 	return ret;
// }

// static ssize_t blkdev_read_iter(struct kiocb *iocb, struct iov_iter *to)
// {
// 	struct block_device *bdev = iocb->ki_filp->private_data;
// 	loff_t size = bdev_nr_bytes(bdev);
// 	loff_t pos = iocb->ki_pos;
// 	size_t shorted = 0;
// 	ssize_t ret = 0;
// 	size_t count;

// 	if (unlikely(pos + iov_iter_count(to) > size)) {
// 		if (pos >= size)
// 			return 0;
// 		size -= pos;
// 		shorted = iov_iter_count(to) - size;
// 		iov_iter_truncate(to, size);
// 	}

// 	count = iov_iter_count(to);
// 	if (!count)
// 		goto reexpand; /* skip atime */

// 	if (iocb->ki_flags & IOCB_DIRECT) {
// 		struct address_space *mapping = iocb->ki_filp->f_mapping;

// 		if (iocb->ki_flags & IOCB_NOWAIT) {
// 			if (filemap_range_needs_writeback(mapping, pos,
// 							  pos + count - 1)) {
// 				ret = -EAGAIN;
// 				goto reexpand;
// 			}
// 		} else {
// 			ret = filemap_write_and_wait_range(mapping, pos,
// 							   pos + count - 1);
// 			if (ret < 0)
// 				goto reexpand;
// 		}

// 		file_accessed(iocb->ki_filp);

// 		ret = blkdev_direct_IO(iocb, to);
// 		if (ret >= 0) {
// 			iocb->ki_pos += ret;
// 			count -= ret;
// 		}
// 		iov_iter_revert(to, count - iov_iter_count(to));
// 		if (ret < 0 || !count)
// 			goto reexpand;
// 	}

// 	ret = filemap_read(iocb, to, ret);

// reexpand:
// 	if (unlikely(shorted))
// 		iov_iter_reexpand(to, iov_iter_count(to) + shorted);
// 	return ret;
// }

// #define	BLKDEV_FALLOC_FL_SUPPORTED							\
// 		(FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE |		\
// 		 FALLOC_FL_ZERO_RANGE | FALLOC_FL_NO_HIDE_STALE)

// static long blkdev_fallocate(file_s *file, int mode,
// 				loff_t start, loff_t len)
// {
// 	struct inode *inode = bdev_file_inode(file);
// 	struct block_device *bdev = I_BDEV(inode);
// 	loff_t end = start + len - 1;
// 	loff_t isize;
// 	int error;

// 	/* Fail if we don't recognize the flags. */
// 	if (mode & ~BLKDEV_FALLOC_FL_SUPPORTED)
// 		return -EOPNOTSUPP;

// 	/* Don't go off the end of the device. */
// 	isize = bdev_nr_bytes(bdev);
// 	if (start >= isize)
// 		return -EINVAL;
// 	if (end >= isize) {
// 		if (mode & FALLOC_FL_KEEP_SIZE) {
// 			len = isize - start;
// 			end = start + len - 1;
// 		} else
// 			return -EINVAL;
// 	}

// 	/*
// 	 * Don't allow IO that isn't aligned to logical block size.
// 	 */
// 	if ((start | len) & (bdev_logical_block_size(bdev) - 1))
// 		return -EINVAL;

// 	filemap_invalidate_lock(inode->i_mapping);

// 	/* Invalidate the page cache, including dirty pages. */
// 	error = truncate_bdev_range(bdev, file->f_mode, start, end);
// 	if (error)
// 		goto fail;

// 	switch (mode) {
// 	case FALLOC_FL_ZERO_RANGE:
// 	case FALLOC_FL_ZERO_RANGE | FALLOC_FL_KEEP_SIZE:
// 		error = blkdev_issue_zeroout(bdev, start >> SECTOR_SHIFT,
// 					     len >> SECTOR_SHIFT, GFP_KERNEL,
// 					     BLKDEV_ZERO_NOUNMAP);
// 		break;
// 	case FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE:
// 		error = blkdev_issue_zeroout(bdev, start >> SECTOR_SHIFT,
// 					     len >> SECTOR_SHIFT, GFP_KERNEL,
// 					     BLKDEV_ZERO_NOFALLBACK);
// 		break;
// 	case FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE | FALLOC_FL_NO_HIDE_STALE:
// 		error = blkdev_issue_discard(bdev, start >> SECTOR_SHIFT,
// 					     len >> SECTOR_SHIFT, GFP_KERNEL, 0);
// 		break;
// 	default:
// 		error = -EOPNOTSUPP;
// 	}

//  fail:
// 	filemap_invalidate_unlock(inode->i_mapping);
// 	return error;
// }

const file_ops_s def_blk_fops = {
	.open			= blkdev_open,
	.release		= blkdev_close,
	.llseek			= blkdev_llseek,
	// .read_iter		= blkdev_read_iter,
	// .write_iter		= blkdev_write_iter,
	// .iopoll			= iocb_bio_iopoll,
	.mmap			= generic_file_mmap,
	.fsync			= blkdev_fsync,
	// .unlocked_ioctl	= blkdev_ioctl,
// #ifdef CONFIG_COMPAT
// 	.compat_ioctl	= compat_blkdev_ioctl,
// #endif
	// .splice_read	= generic_file_splice_read,
	// .splice_write	= iter_file_splice_write,
	// .fallocate		= blkdev_fallocate,
};