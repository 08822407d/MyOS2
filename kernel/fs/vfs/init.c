// SPDX-License-Identifier: GPL-2.0
/*
 * Routines that mimic syscalls, but don't use the user address space or file
 * descriptors.  Only for init/ and related early init code.
 */
#include <linux/init/init.h>
#include <linux/kernel/mount.h>
#include <linux/fs/namei.h>
#include <linux/fs/fs.h>
// #include <linux/fs_struct.h>
#include <linux/fs/file.h>
// #include <linux/init_syscalls.h>
// #include <linux/security.h>
#include <linux/fs/internal.h>


#include <linux/kernel/err.h>


int init_mkdir(const char *pathname, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	int error;

	dentry = kern_path_create(AT_FDCWD, pathname, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	error = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	done_path_create(&path, dentry);
	return error;
}

void do_name(void)
{
	init_mkdir("dev", 16877);
	init_mkdir("root", 16832);
	init_mkdir(".", 16877);
	init_mkdir("kernel", 16877);
	init_mkdir("conf", 16877);
	init_mkdir("etc", 16877);
	init_mkdir("usr", 16877);
}