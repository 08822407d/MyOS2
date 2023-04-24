// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/read_write.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/kernel/slab.h>
#include <linux/kernel/stat.h>
// #include <linux/sched/xacct.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>
// #include <linux/uio.h>
// #include <linux/fsnotify.h>
// #include <linux/security.h>
// #include <linux/export.h>
#include <linux/kernel/syscalls.h>
// #include <linux/pagemap.h>
// #include <linux/splice.h>
// #include <linux/compat.h>
#include <linux/kernel/mount.h>
#include <linux/fs/fs.h>
#include "internal.h"

// #include <linux/uaccess.h>
#include <asm/unistd.h>


#include <linux/kernel/kernel.h>

/**
 * noop_llseek - No Operation Performed llseek implementation
 * @file:	file structure to seek on
 * @offset:	file offset to seek to
 * @whence:	type of seek
 *
 * This is an implementation of ->llseek useable for the rare special case when
 * userspace expects the seek to succeed but the (device) file is actually not
 * able to perform the seek. In this case you use noop_llseek() instead of
 * falling back to the default implementation of ->llseek.
 */
loff_t noop_llseek(file_s *file, loff_t offset, int whence)
{
	return file->f_pos;
}



int rw_verify_area(int read_write, file_s *file, const loff_t *ppos, size_t count)
{
	// if (unlikely((ssize_t) count < 0))
	// 	return -EINVAL;

	// if (ppos) {
	// 	loff_t pos = *ppos;

	// 	if (unlikely(pos < 0)) {
	// 		if (!unsigned_offsets(file))
	// 			return -EINVAL;
	// 		if (count >= -pos) /* both values are in 0..LLONG_MAX */
	// 			return -EOVERFLOW;
	// 	} else if (unlikely((loff_t) (pos + count) < 0)) {
	// 		if (!unsigned_offsets(file))
	// 			return -EINVAL;
	// 	}
	// }

	// return security_file_permission(file,
	// 			read_write == READ ? MAY_READ : MAY_WRITE);
}


ssize_t __kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	// struct kvec iov = {
	// 	.iov_base	= buf,
	// 	.iov_len	= min_t(size_t, count, MAX_RW_COUNT),
	// };
	// struct kiocb kiocb;
	// struct iov_iter iter;
	// ssize_t ret;

	// if (WARN_ON_ONCE(!(file->f_mode & FMODE_READ)))
	// 	return -EINVAL;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;
	// /*
	//  * Also fail if ->read_iter and ->read are both wired up as that
	//  * implies very convoluted semantics.
	//  */
	// if (unlikely(!file->f_op->read_iter || file->f_op->read))
	// 	return warn_unsupported(file, "read");

	// init_sync_kiocb(&kiocb, file);
	// kiocb.ki_pos = pos ? *pos : 0;
	// iov_iter_kvec(&iter, READ, &iov, 1, iov.iov_len);
	// ret = file->f_op->read_iter(&kiocb, &iter);
	// if (ret > 0) {
	// 	if (pos)
	// 		*pos = kiocb.ki_pos;
	// 	fsnotify_access(file);
	// 	add_rchar(current, ret);
	// }
	// inc_syscr(current);
	// return ret;
}

ssize_t kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	ssize_t ret;

	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	return __kernel_read(file, buf, count, pos);
}