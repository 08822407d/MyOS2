// SPDX-License-Identifier: GPL-2.0-only
// #include <linux/export.h>
// #include <linux/sched/signal.h>
// #include <linux/sched/task.h>
#include <linux/fs/fs.h>
#include <linux/fs/path.h>
// #include <linux/kernel/slab.h>
#include <linux/sched/fs_struct.h>
#include "internal.h"

/*
 * Replace the fs->{rootmnt,root} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_root(taskfs_s *fs, const path_s *path)
{
	struct path old_root;

	// path_get(path);
	// old_root = fs->root;
	fs->root = *path;
	// if (old_root.dentry)
	// 	path_put(&old_root);
}

/*
 * Replace the fs->{pwdmnt,pwd} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_pwd(taskfs_s *fs, const path_s *path)
{
	struct path old_pwd;

	// path_get(path);
	// old_pwd = fs->pwd;
	fs->pwd = *path;
	// if (old_pwd.dentry)
	// 	path_put(&old_pwd);
}