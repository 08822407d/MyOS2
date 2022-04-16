#include <sys/stat.h>

#include <string.h>
#include <errno.h>

#include <include/fs/file.h>
#include <include/fs/namei.h>
#include <include/fs/openat2.h>

#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
#define O_PATH_FLAGS		(O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)


// Linux function proto:
// static int do_dentry_open(struct file *f, struct inode *inode,
//			  int (*open)(struct inode *, struct file *))
static int do_dentry_open(file_s * f, inode_s * inode)
{
	unsigned long error = -ENOERR;
	f->f_ops = inode->f_ops;
	if(f->f_ops && f->f_ops->open)
		error = f->f_ops->open(f->dentry->dir_inode, f);

	if(f->mode & O_TRUNC)
	{
		f->dentry->dir_inode->file_size = 0;
	}
	if(f->mode & O_APPEND)
	{
		f->position = f->dentry->dir_inode->file_size;
	}
}

inline int build_open_flags(const open_how_s *how, open_flags_s *op)
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

static long do_sys_openat2(int dfd, const char *filename, open_how_s *how)
{
	int fd = -1;
	long err = 0;

	filename_s * name = getname(filename);
	fd = get_unused_fd_flags(0);
	if (fd >=0)
	{
		file_s * fp = do_filp_open(dfd, name, 0);
		if (fp == NULL)
		{
			err = -ENFILE;
		}
		else
		{
			fd_install(fd, fp);
		}
	}
	else
	{
		err = -ENFILE;
	}
	putname(name);
	
	return fd;
}

// Linux function proto:
// long do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
	};
	/* O_PATH beats everything else. */
	if (how.flags & O_PATH)
		how.flags &= O_PATH_FLAGS;
	/* Modes should only be set for create-like flags. */
	if (!WILL_CREATE(how.flags))
		how.mode = 0;

	return do_sys_openat2(dfd, filename, &how);
}

long sys_chdir(const char *filename)
{
	path_s path;
	task_s *curr = curr_tsk;
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

long kopen(const char * filename, int flags)
{
	filename_s name;
	int fd;

	name.name = filename;
	name.len = strlen(filename);
	fd = get_unused_fd_flags(0);
	if (fd >=0)
	{
		file_s * fp = do_filp_open(0, &name, flags);
		if (fp == NULL)
		{

		}
		else
		{
			fd_install(fd, fp);
		}
	}
	else
	{
		return -ENFILE;
	}
	
	return fd;
}

/**
 * vfs_open - open the file at the given path
 * @path: path to open
 * @file: newly allocated file with f_flag initialized
 * @cred: credentials to use
 */
int __vfs_open(const path_s * path, file_s * file)
{
	file->dentry = path->dentry;
	return do_dentry_open(file, path->dentry->dir_inode);
}