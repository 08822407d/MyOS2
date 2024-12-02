// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/read_write.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/kernel/mm_api.h>
#include <linux/kernel/stat.h>
// #include <linux/sched/xacct.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>
#include <linux/kernel/uio.h>
// #include <linux/fsnotify.h>
// #include <linux/security.h>
#include <linux/kernel/export.h>
#include <linux/kernel/syscalls.h>
// #include <linux/mm/pagemap.h>
// #include <linux/splice.h>
// #include <linux/compat.h>
#include <linux/kernel/mount.h>
#include <linux/fs/fs.h>
#include <linux/fs/internal.h>

#include <linux/kernel/uaccess.h>
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
	return 0;

	if (likely((ssize_t) count < 0))
		return -EINVAL;

	if (ppos) {
		loff_t pos = *ppos;

	// 	if (unlikely(pos < 0)) {
	// 		if (!unsigned_offsets(file))
	// 			return -EINVAL;
	// 		if (count >= -pos) /* both values are in 0..LLONG_MAX */
	// 			return -EOVERFLOW;
	// 	} else if (unlikely((loff_t) (pos + count) < 0)) {
	// 		if (!unsigned_offsets(file))
	// 			return -EINVAL;
	// 	}
	}

	// return security_file_permission(file,
	// 			read_write == READ ? MAY_READ : MAY_WRITE);
}
EXPORT_SYMBOL(rw_verify_area);

static ssize_t
new_sync_read(file_s *filp, char __user *buf,
		size_t len, loff_t *ppos) {

	kiocb_s kiocb;
	iov_iter_s iter;
	ssize_t ret;

	init_sync_kiocb(&kiocb, filp);
	kiocb.ki_pos = (ppos ? *ppos : 0);
	iov_iter_ubuf(&iter, ITER_DEST, buf, len);

	// ret = call_read_iter(filp, &kiocb, &iter);
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
}


ssize_t __kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	kvec_s iov = {
		.iov_base	= buf,
		.iov_len	= min_t(size_t, count, MAX_RW_COUNT),
	};
	kiocb_s kiocb;
	iov_iter_s iter;
	ssize_t ret;

	if (WARN_ON_ONCE(!(file->f_mode & FMODE_READ)))
		return -EINVAL;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;


	init_sync_kiocb(&kiocb, file);
	kiocb.ki_pos = pos ? *pos : 0;
	iov_iter_kvec(&iter, READ, &iov, 1, iov.iov_len);
	if (file->f_op->read_iter)
		ret = file->f_op->read_iter(&kiocb, &iter);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
	else
		ret = -EINVAL;
	// if (ret > 0) {
	// 	if (pos)
	// 		*pos = kiocb.ki_pos;
	// 	fsnotify_access(file);
	// 	add_rchar(current, ret);
	// }
	// inc_syscr(current);
	return ret;
}

ssize_t kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	ssize_t ret;

	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	return __kernel_read(file, buf, count, pos);
}
EXPORT_SYMBOL(kernel_read);

ssize_t vfs_read(file_s *file, char __user *buf,
		size_t count, loff_t *pos)
{
	ssize_t ret;

	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
		return -EFAULT;

	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;

	if (file->f_op->read_iter)
		ret = new_sync_read(file, buf, count, pos);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
	else
		ret = -EINVAL;
	// if (ret > 0) {
	// 	fsnotify_access(file);
	// 	add_rchar(current, ret);
	// }
	// inc_syscr(current);
	return ret;
}


/* file_ppos returns &file->f_pos or NULL if file is stream */
static inline loff_t *file_ppos(file_s *file) {
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
}

ssize_t ksys_read(uint fd, char __user *buf, size_t count)
{
	fd_s f = myos_fdget_pos(fd);
	ssize_t ret = -EBADF;

	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
		}
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
	}
	return ret;
}




static ssize_t
new_sync_write(file_s *filp, const char __user *buf, size_t len, loff_t *ppos) {
	kiocb_s kiocb;
	iov_iter_s iter;
	ssize_t ret;

	init_sync_kiocb(&kiocb, filp);
	kiocb.ki_pos = (ppos ? *ppos : 0);
	iov_iter_ubuf(&iter, ITER_SOURCE, (void __user *)buf, len);

	ret = filp->f_op->write_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
}

/* caller is responsible for file_start_write/file_end_write */
ssize_t __kernel_write_iter(file_s *file,
		iov_iter_s *from, loff_t *pos)
{
	kiocb_s kiocb;
	ssize_t ret;

	if (WARN_ON_ONCE(!(file->f_mode & FMODE_WRITE)))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
	/*
	 * Also fail if ->write_iter and ->write are both wired up as that
	 * implies very convoluted semantics.
	 */
	// if (unlikely(!file->f_op->write_iter || file->f_op->write))
	// 	return warn_unsupported(file, "write");

	init_sync_kiocb(&kiocb, file);
	kiocb.ki_pos = pos ? *pos : 0;
	ret = file->f_op->write_iter(&kiocb, from);
	if (ret > 0) {
		if (pos)
			*pos = kiocb.ki_pos;
		// fsnotify_modify(file);
		// add_wchar(current, ret);
	}
	// inc_syscw(current);
	return ret;
}

/* caller is responsible for file_start_write/file_end_write */
ssize_t __kernel_write(file_s *file, const void *buf,
		size_t count, loff_t *pos)
{
	kvec_s iov = {
		.iov_base	= (void *)buf,
		.iov_len	= min_t(size_t, count, MAX_RW_COUNT),
	};
	iov_iter_s iter;
	iov_iter_kvec(&iter, ITER_SOURCE, &iov, 1, iov.iov_len);
	return __kernel_write_iter(file, &iter, pos);
}
/*
 * This "EXPORT_SYMBOL_GPL()" is more of a "EXPORT_SYMBOL_DONTUSE()",
 * but autofs is one of the few internal kernel users that actually
 * wants this _and_ can be built as a module. So we need to export
 * this symbol for autofs, even though it really isn't appropriate
 * for any other kernel modules.
 */
EXPORT_SYMBOL_GPL(__kernel_write);

ssize_t kernel_write(file_s *file, const void *buf,
		size_t count, loff_t *pos)
{
	ssize_t ret;

	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;

	// file_start_write(file);
	ret =  __kernel_write(file, buf, count, pos);
	// file_end_write(file);
	return ret;
}
EXPORT_SYMBOL(kernel_write);

ssize_t vfs_write(file_s *file, const char __user *buf, size_t count, loff_t *pos)
{
	ssize_t ret;

	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
		return -EFAULT;

	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
	// file_start_write(file);
	if (file->f_op->write)
		ret = file->f_op->write(file, buf, count, pos);
	else if (file->f_op->write_iter)
		ret = new_sync_write(file, buf, count, pos);
	else
		ret = -EINVAL;
	// if (ret > 0) {
	// 	fsnotify_modify(file);
	// 	add_wchar(current, ret);
	// }
	// inc_syscw(current);
	// file_end_write(file);
	return ret;
}

ssize_t ksys_write(unsigned int fd, const char __user *buf, size_t count)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;

	if (filp != NULL) {
		loff_t pos, *ppos = file_ppos(filp);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
		}
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
	}

	return ret;
}

static ssize_t
vfs_writev(file_s *file, const iov_s *vec,
		ulong vlen, loff_t *pos, rwf_t flags) {

	iov_s iovstack[UIO_FASTIOV];
	iov_s *iov = iovstack;
	iov_iter_s iter;
	size_t tot_len;
	ssize_t ret = 0;

	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_WRITE))
	// 	return -EINVAL;

	// ret = import_iovec(ITER_SOURCE, vec, vlen,
	// 		ARRAY_SIZE(iovstack), &iov, &iter);
	// if (ret < 0)
	// 	return ret;

	// tot_len = iov_iter_count(&iter);
	// if (!tot_len)
	// 	goto out;

	// ret = rw_verify_area(WRITE, file, pos, tot_len);
	// if (ret < 0)
	// 	goto out;

	// file_start_write(file);
	// if (file->f_op->write_iter)
	// 	ret = do_iter_readv_writev(file, &iter, pos, WRITE, flags);
	// else
	// 	ret = do_loop_readv_writev(file, &iter, pos, WRITE, flags);
	// if (ret > 0)
	// 	fsnotify_modify(file);
	// file_end_write(file);
// out:
	// kfree(iov);
	// return ret;

	for (int i = 0; i < vlen; i++)
		ret += file->f_op->write(file, vec[i].iov_base, vec[i].iov_len, pos);

	return ret;
}

ssize_t do_writev(ulong fd, const iov_s __user *vec, ulong vlen, rwf_t flags)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;

	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
		}
		ret = vfs_writev(f.file, vec, vlen, ppos, flags);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
	}

	// if (ret > 0)
	// 	add_wchar(current, ret);
	// inc_syscw(current);
	return ret;
}
