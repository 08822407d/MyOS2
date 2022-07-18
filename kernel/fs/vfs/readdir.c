// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/readdir.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 */

#include <linux/kernel/stddef.h>
#include <linux/kernel/kernel.h>
// #include <linux/export.h>
#include <linux/kernel/time.h>
// #include <linux/mm.h>
// #include <linux/errno.h>
#include <linux/kernel/stat.h>
#include <linux/fs/file.h>
#include <linux/fs/fs.h>
// #include <linux/fsnotify.h>
#include <uapi/dirent.h>
// #include <linux/security.h>
// #include <linux/syscalls.h>
// #include <linux/unistd.h>
// #include <linux/compat.h>
// #include <linux/uaccess.h>

// #include <asm/unaligned.h>


#include <string.h>
#include <errno.h>
#include <include/proto.h>
#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/arch_proto.h>

typedef struct getdents_callback64 {
	dir_ctxt_s ctx;
	linux_dirent64_s *current_dir;
	int prev_reclen;
	int count;
	int error;
} getdents_cbk64_s;

int iterate_dir(file_s *file, dir_ctxt_s *ctx)
{
	bool shared = false;
	int res = -ENOTDIR;
	if (file->f_op->iterate_shared)
		shared = true;
	else if (!file->f_op->iterate)
		goto out;

	ctx->pos = file->f_pos;
	if (shared)
		res = file->f_op->iterate_shared(file, ctx);
	else
		res = file->f_op->iterate(file, ctx);
	file->f_pos = ctx->pos;

out:
	return res;
}

static int filldir64(dir_ctxt_s *ctx, const char *name, int namelen,
				loff_t offset, u64 ino, unsigned int d_type)
{
// 	struct linux_dirent64 __user *dirent, *prev;
// 	struct getdents_callback64 *buf =
// 		container_of(ctx, struct getdents_callback64, ctx);
// 	int reclen = ALIGN(offsetof(struct linux_dirent64, d_name) + namlen + 1,
// 		sizeof(u64));
// 	int prev_reclen;

// 	buf->error = verify_dirent_name(name, namlen);
// 	if (unlikely(buf->error))
// 		return buf->error;
// 	buf->error = -EINVAL;	/* only used if we fail.. */
// 	if (reclen > buf->count)
// 		return -EINVAL;
// 	prev_reclen = buf->prev_reclen;
// 	if (prev_reclen && signal_pending(current))
// 		return -EINTR;
// 	dirent = buf->current_dir;
// 	prev = (void __user *)dirent - prev_reclen;
// 	if (!user_write_access_begin(prev, reclen + prev_reclen))
// 		goto efault;

// 	/* This might be 'dirent->d_off', but if so it will get overwritten */
// 	unsafe_put_user(offset, &prev->d_off, efault_end);
// 	unsafe_put_user(ino, &dirent->d_ino, efault_end);
// 	unsafe_put_user(reclen, &dirent->d_reclen, efault_end);
// 	unsafe_put_user(d_type, &dirent->d_type, efault_end);
// 	unsafe_copy_dirent_name(dirent->d_name, name, namlen, efault_end);
// 	user_write_access_end();

// 	buf->prev_reclen = reclen;
// 	buf->current_dir = (void __user *)dirent + reclen;
// 	buf->count -= reclen;
// 	return 0;

// efault_end:
// 	user_write_access_end();
// efault:
// 	buf->error = -EFAULT;
// 	return -EFAULT;
}

long sys_getdents64(unsigned int fd, linux_dirent64_s *dirent,
				unsigned int count)
{
	fd_s f;
	getdents_cbk64_s buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
	};
	int error;

	f = fdget_pos(fd);
	if (!f.file)
		return -EBADF;

	// error = iterate_dir(f.file, &buf.ctx);
	// int iterate_dir(struct file *file, dir_ctxt_s *ctx)
	// {
		buf.ctx.pos = f.file->f_pos;
		errno = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
	// }

	// if (error >= 0)
	// 	error = buf.error;
	// if (buf.prev_reclen) {
	// 	struct linux_dirent64 __user * lastdirent;
	// 	typeof(lastdirent->d_off) d_off = buf.ctx.pos;

	// 	lastdirent = (void __user *) buf.current_dir - buf.prev_reclen;
	// 	if (put_user(d_off, &lastdirent->d_off))
	// 		error = -EFAULT;
	// 	else
	// 		error = count - buf.count;
	// }
	fdput_pos(f);
	return error;
}

// int fill_dentry(void *buf, char *name, long namelen, long type, long offset);
// long sys_getdents64(unsigned int fd,
// 		linux_dirent64_s *dirent, unsigned int count)
// {
// 	long ret_val = 0;
// 	fd_s f = fdget_pos(fd);
// 	if (f.file->f_op != NULL &&
// 		f.file->f_op->iterate_shared != NULL)
// 		ret_val = f.file->f_op->iterate_shared(f.file, NULL);

// 	return ret_val;
// }

// int fill_dentry(void *buf, char *name, long namelen, long type, long offset)
// {
// 	linux_dirent64_s *dent = (linux_dirent64_s *)buf;
	
// 	if((unsigned long)buf < USERADDR_LIMIT &&
// 		!verify_area(buf,sizeof(linux_dirent64_s) + namelen))
// 		return -EFAULT;

// 	copy_to_user(dent->d_name, name, namelen);
// 	// memcpy(dent->d_name, name, namelen);
// 	dent->d_reclen = namelen;
// 	dent->d_type = type;
// 	dent->d_off = offset;
// 	return sizeof(linux_dirent64_s) + namelen;
// }