// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/export.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/fs/fs.h>
#include <linux/fs/path.h>
#include <linux/kernel/slab.h>
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



void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	// kmem_cache_free(fs_cachep, fs);
	kfree(fs);
}

void exit_fs(task_s *tsk)
{
	taskfs_s *fs = tsk->fs;

	if (fs) {
		int kill;
		// task_lock(tsk);
		// spin_lock(&fs->lock);
		tsk->fs = NULL;
		kill = !--fs->users;
		// spin_unlock(&fs->lock);
		// task_unlock(tsk);
		if (kill)
			free_fs_struct(fs);
	}
}

taskfs_s *copy_fs_struct(taskfs_s *old)
{
	// taskfs_s *fs = kmem_cache_alloc(fs_cachep, GFP_KERNEL);
	taskfs_s *fs = kzalloc(sizeof(taskfs_s), GFP_KERNEL);
	/* We don't need to lock fs - think why ;-) */
	if (fs) {
		fs->users = 1;
		fs->in_exec = 0;
		// spin_lock_init(&fs->lock);
		// seqcount_spinlock_init(&fs->seq, &fs->lock);
		fs->umask = old->umask;

		// spin_lock(&old->lock);
		fs->root = old->root;
		path_get(&fs->root);
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
		// spin_unlock(&old->lock);
	}
	return fs;
}