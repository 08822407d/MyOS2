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
#include <linux/mm/mm.h>
#include <linux/lib/errno.h>
#include <linux/kernel/stat.h>
#include <linux/fs/file.h>
#include <linux/fs/fs.h>
// #include <linux/fsnotify.h>
#include <linux/kernel/dirent.h>
// #include <linux/security.h>
#include <linux/kernel/syscalls.h>
#include <uapi/linux/unistd.h>
// #include <linux/compat.h>
// #include <linux/uaccess.h>

// #include <asm/unaligned.h>


#include <obsolete/arch_proto.h>

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

/*
 * POSIX says that a dirent name cannot contain NULL or a '/'.
 *
 * It's not 100% clear what we should really do in this case.
 * The filesystem is clearly corrupted, but returning a hard
 * error means that you now don't see any of the other names
 * either, so that isn't a perfect alternative.
 *
 * And if you return an error, what error do you use? Several
 * filesystems seem to have decided on EUCLEAN being the error
 * code for EFSCORRUPTED, and that may be the error to use. Or
 * just EIO, which is perhaps more obvious to users.
 *
 * In order to see the other file names in the directory, the
 * caller might want to make this a "soft" error: skip the
 * entry, and return the error at the end instead.
 *
 * Note that this should likely do a "memchr(name, 0, len)"
 * check too, since that would be filesystem corruption as
 * well. However, that case can't actually confuse user space,
 * which has to do a strlen() on the name anyway to find the
 * filename length, and the above "soft error" worry means
 * that it's probably better left alone until we have that
 * issue clarified.
 *
 * Note the PATH_MAX check - it's arbitrary but the real
 * kernel limit on a possible path component, not NAME_MAX,
 * which is the technical standard limit.
 */
static int verify_dirent_name(const char *name, int len)
{
	if (len <= 0 || len >= PATH_MAX)
		return -EIO;
	if (memchr(name, '/', len))
		return -EIO;
	return 0;
}

static int filldir64(dir_ctxt_s *ctx, const char *name,
		int namelen, loff_t offset, u64 ino, unsigned int d_type)
{
	linux_dirent64_s *dirent, *prev;
	getdents_cbk64_s *buf =
			container_of(ctx, getdents_cbk64_s, ctx);
	int reclen = ALIGN(offsetof(linux_dirent64_s, d_name) + namelen + 1,
			sizeof(u64));
	int prev_reclen = 0;

	buf->error = verify_dirent_name(name, namelen);
	if (buf->error)
		return buf->error;
	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;

// efault_end:
// 	user_write_access_end();
efault:
	buf->error = -EFAULT;
	return -EFAULT;
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

	f = myos_fdget_pos(fd);
	if (!f.file)
		return -EBADF;

	// error = iterate_dir(f.file, &buf.ctx);
	// int iterate_dir(file_s *file, dir_ctxt_s *ctx)
	// {
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
	// }

	if (error >= 0)
		error = buf.error;
	if (buf.prev_reclen) {
	// 	linux_dirent64_s *lastdirent;
	// 	typeof(lastdirent->d_off) d_off = buf.ctx.pos;

	// 	lastdirent = (void *) buf.current_dir - buf.prev_reclen;
	// 	if (put_user(d_off, &lastdirent->d_off))
	// 		error = -EFAULT;
	// 	else
			error = count - buf.count;
	}
	fdput_pos(f);
	return error;
}