// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/open.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/lib/string.h>
#include <linux/mm/mm.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
// #include <linux/fsnotify.h>
// #include <linux/module.h>
#include <linux/device/tty.h>
#include <linux/fs/namei.h>
// #include <linux/backing-dev.h>
// #include <linux/capability.h>
// #include <linux/securebits.h>
// #include <linux/security.h>
#include <linux/kernel/mount.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/slab.h>
// #include <linux/uaccess.h>
#include <linux/fs/fs.h>
// #include <linux/personality.h>
// #include <linux/pagemap.h>
// #include <linux/syscalls.h>
// #include <linux/rcupdate.h>
// #include <linux/audit.h>
// #include <linux/falloc.h>
// #include <linux/fs_struct.h>
// #include <linux/ima.h>
// #include <linux/dnotify.h>
// #include <linux/compat.h>
// #include <linux/mnt_idmapping.h>
#include "internal.h"


#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
#define O_PATH_FLAGS		(O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)


open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
	};

	/* O_PATH beats everything else. */
	if (how.flags & O_PATH)
		how.flags &= O_PATH_FLAGS;
	/* Modes should only be set for create-like flags. */
	if (!WILL_CREATE(how.flags))
		how.mode = 0;
	return how;
}

// Linux function proto:
// static int do_dentry_open(file_s *f, struct inode *inode,
//			  int (*open)(struct inode *, file_s *))
static int do_dentry_open(file_s * f, inode_s * inode)
{
	unsigned long error = -ENOERR;
	f->f_op = inode->i_fop;
	if(f->f_op && f->f_op->open)
		error = f->f_op->open(f->f_path.dentry->d_inode, f);

	if(f->f_mode & O_TRUNC)
	{
		f->f_path.dentry->d_inode->i_size = 0;
	}
	if(f->f_mode & O_APPEND)
	{
		f->f_pos = f->f_path.dentry->d_inode->i_size;
	}
}

int build_open_flags(const open_how_s *how, open_flags_s *op)
{
	uint64_t flags = how->flags;
	uint64_t strip = FMODE_NONOTIFY | O_CLOEXEC;
	int lookup_flags = 0;
	int acc_mode = ACC_MODE(flags);

	/*
	 * Strip flags that either shouldn't be set by userspace like
	 * FMODE_NONOTIFY or that aren't relevant in determining struct
	 * open_flags like O_CLOEXEC.
	 */
	flags &= ~strip;

	/*
	 * Older syscalls implicitly clear all of the invalid flags or argument
	 * values before calling build_open_flags(), but openat2(2) checks all
	 * of its arguments.
	 */
	if (flags & ~VALID_OPEN_FLAGS)
		return -EINVAL;
	if (how->resolve & ~VALID_RESOLVE_FLAGS)
		return -EINVAL;

	/* Scoping flags are mutually exclusive. */
	if ((how->resolve & RESOLVE_BENEATH) && (how->resolve & RESOLVE_IN_ROOT))
		return -EINVAL;

	/* Deal with the mode. */
	if (WILL_CREATE(flags)) {
		if (how->mode & ~S_IALLUGO)
			return -EINVAL;
		op->mode = how->mode | S_IFREG;
	} else {
		if (how->mode != 0)
			return -EINVAL;
		op->mode = 0;
	}

	/*
	 * In order to ensure programs get explicit errors when trying to use
	 * O_TMPFILE on old kernels, O_TMPFILE is implemented such that it
	 * looks like (O_DIRECTORY|O_RDWR & ~O_CREAT) to old kernels. But we
	 * have to require userspace to explicitly set it.
	 */
	if (flags & __O_TMPFILE) {
		if ((flags & O_TMPFILE_MASK) != O_TMPFILE)
			return -EINVAL;
		if (!(acc_mode & MAY_WRITE))
			return -EINVAL;
	}
	if (flags & O_PATH) {
		/* O_PATH only permits certain other flags to be set. */
		if (flags & ~O_PATH_FLAGS)
			return -EINVAL;
		acc_mode = 0;
	}

	/*
	 * O_SYNC is implemented as __O_SYNC|O_DSYNC.  As many places only
	 * check for O_DSYNC if the need any syncing at all we enforce it's
	 * always set instead of having to deal with possibly weird behaviour
	 * for malicious applications setting only __O_SYNC.
	 */
	if (flags & __O_SYNC)
		flags |= O_DSYNC;

	op->open_flag = flags;

	/* O_TRUNC implies we need access checks for write permissions */
	if (flags & O_TRUNC)
		acc_mode |= MAY_WRITE;

	/* Allow the LSM permission hook to distinguish append
	   access from general write access. */
	if (flags & O_APPEND)
		acc_mode |= MAY_APPEND;

	op->acc_mode = acc_mode;

	op->intent = flags & O_PATH ? 0 : LOOKUP_OPEN;

	if (flags & O_CREAT) {
		op->intent |= LOOKUP_CREATE;
		if (flags & O_EXCL) {
			op->intent |= LOOKUP_EXCL;
			flags |= O_NOFOLLOW;
		}
	}

	if (flags & O_DIRECTORY)
		lookup_flags |= LOOKUP_DIRECTORY;
	if (!(flags & O_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;

	if (how->resolve & RESOLVE_NO_XDEV)
		lookup_flags |= LOOKUP_NO_XDEV;
	if (how->resolve & RESOLVE_NO_MAGICLINKS)
		lookup_flags |= LOOKUP_NO_MAGICLINKS;
	if (how->resolve & RESOLVE_NO_SYMLINKS)
		lookup_flags |= LOOKUP_NO_SYMLINKS;
	if (how->resolve & RESOLVE_BENEATH)
		lookup_flags |= LOOKUP_BENEATH;
	if (how->resolve & RESOLVE_IN_ROOT)
		lookup_flags |= LOOKUP_IN_ROOT;
	if (how->resolve & RESOLVE_CACHED) {
		/* Don't bother even trying for create/truncate/tmpfile open */
		if (flags & (O_TRUNC | O_CREAT | O_TMPFILE))
			return -EAGAIN;
		lookup_flags |= LOOKUP_CACHED;
	}

	op->lookup_flags = lookup_flags;
	return 0;
}

/*==============================================================================================*
 *									syscall apis for open										*
 *==============================================================================================*/
/**
 * file_open_name - open file and return file pointer
 *
 * @name:	struct filename containing path to open
 * @flags:	open flags as per the open(2) second argument
 * @mode:	mode for the new file if O_CREAT is set, else ignored
 *
 * This is the helper to open a file from kernelspace if you really
 * have to.  But in generally you should not do this, so please move
 * along, nothing to see here..
 */
file_s *file_open_name(filename_s *name, int flags, umode_t mode)
{
	open_flags_s op;
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
}

/**
 * filp_open - open file and return file pointer
 *
 * @filename:	path to open
 * @flags:	open flags as per the open(2) second argument
 * @mode:	mode for the new file if O_CREAT is set, else ignored
 *
 * This is the helper to open a file from kernelspace if you really
 * have to.  But in generally you should not do this, so please move
 * along, nothing to see here..
 */
file_s *filp_open(const char *filename, int flags, umode_t mode)
{
	filename_s *name = getname_kernel(filename);
	file_s *file = ERR_CAST(name);
	
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
		putname(name);
	}
	return file;
}

file_s *file_open_root(const path_s *root, const char *filename,
				int flags, umode_t mode)
{
	// open_flags_s op;
	// open_how_s how = build_open_how(flags, mode);
	// int err = build_open_flags(&how, &op);
	// if (err)
	// 	return ERR_PTR(err);
	// return do_file_open_root(root, filename, &op);
}

static long do_sys_openat2(int dfd, const char *filename, open_how_s *how)
{
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;

	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
			fd = PTR_ERR(f);
		else
			fd_install(fd, f);
	}
	putname(tmp);
	return fd;
}

// Linux function proto:
// long do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}

long sys_chdir(const char *filename)
{
	path_s path;
	task_s *curr = current;
	int error;
	unsigned int lookup_flags = LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
retry:
	error = user_path_at(AT_FDCWD, filename, lookup_flags, &path);
	if (error)
		goto out;

	curr->fs->pwd = path;

// dput_and_out:
// 	path_put(&path);
// 	if (retry_estale(error, lookup_flags)) {
// 		lookup_flags |= LOOKUP_REVAL;
// 		goto retry;
// 	}
out:
	return error;
}

/**
 * vfs_open - open the file at the given path
 * @path: path to open
 * @file: newly allocated file with f_flag initialized
 * @cred: credentials to use
 */
int vfs_open(const path_s * path, file_s * file)
{
	file->f_path = *path;
	return do_dentry_open(file, path->dentry->d_inode);
}

/*==============================================================================================*
 *									syscall apis for open										*
 *==============================================================================================*/
/*
 * "id" is the POSIX thread ID. We use the
 * files pointer for this..
 */
int filp_close(file_s *filp)
{
	int retval = 0;

	// if (!file_count(filp)) {
	// 	printk(KERN_ERR "VFS: Close: file count is 0\n");
	// 	return 0;
	// }

	// if (filp->f_op->flush)
	// 	retval = filp->f_op->flush(filp, id);

	// if (!(filp->f_mode & FMODE_PATH)) {
	// 	dnotify_flush(filp, id);
	// 	locks_remove_posix(filp, id);
	// }
	fput(filp);
	return retval;
}