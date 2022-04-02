#include <sys/fcntl.h>

#include <string.h>
#include <errno.h>

#include <include/fs/file.h>
#include <include/fs/namei.h>


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

// Linux function proto:
// long do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
long do_sys_open(int dfd, const char * filename, int flags)
{
	// Linux call stack:
	//					||
	//					\/
	// static long do_sys_openat2(int dfd, const char __user *filename,
	//				   struct open_how *how)
	int fd = -1;
	long err = 0;

	filename_s * name = getname(filename);
	fd = get_unused_fd_flags(0);
	if (fd >=0)
	{
		file_s * fp = do_filp_open(dfd, name, flags);
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
	
	return -fd;
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
	
	return -ENOERR;
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