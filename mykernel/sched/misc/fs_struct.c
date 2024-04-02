// SPDX-License-Identifier: GPL-2.0-only
#define FS_STRUCT_DEFINATION
#include "fs_struct.h"

/*
 * Replace the fs->{rootmnt,root} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_root(taskfs_s *fs, const path_s *path)
{
	path_s old_root;

	path_get(path);
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_root = fs->root;
	fs->root = *path;

	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
	if (old_root.dentry)
		path_put(&old_root);
}

/*
 * Replace the fs->{pwdmnt,pwd} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_pwd(taskfs_s *fs, const path_s *path)
{
	path_s old_pwd;

	path_get(path);
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_pwd = fs->pwd;
	fs->pwd = *path;

	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
	if (old_pwd.dentry)
		path_put(&old_pwd);
}

static inline int
replace_path(path_s *p, const path_s *old, const path_s *new) {
	if (likely(p->dentry != old->dentry || p->mnt != old->mnt))
		return 0;
	*p = *new;
	return 1;
}

void chroot_fs_refs(const path_s *old_root, const path_s *new_root)
{
	task_s *g, *p;
	taskfs_s *fs;
	int count = 0;

	// read_lock(&tasklist_lock);
	// do_each_thread(g, p) {
	// 	task_lock(p);
	// 	fs = p->fs;
	// 	if (fs) {
	// 		int hits = 0;
	// 		spin_lock(&fs->lock);
	// 		write_seqcount_begin(&fs->seq);
	// 		hits += replace_path(&fs->root, old_root, new_root);
	// 		hits += replace_path(&fs->pwd, old_root, new_root);
	// 		write_seqcount_end(&fs->seq);
	// 		while (hits--) {
	// 			count++;
	// 			path_get(new_root);
	// 		}
	// 		spin_unlock(&fs->lock);
	// 	}
	// 	task_unlock(p);
	// } while_each_thread(g, p);
	// read_unlock(&tasklist_lock);
	// while (count--)
	// 	path_put(old_root);
}

void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
}

void exit_fs(task_s *tsk)
{
	taskfs_s *fs = tsk->fs;

	if (fs) {
		int kill;
		task_lock(tsk);
		spin_lock(&fs->lock);

		tsk->fs = NULL;
		kill = !--fs->users;

		spin_unlock(&fs->lock);
		task_unlock(tsk);
		if (kill)
			free_fs_struct(fs);
	}
}

taskfs_s *copy_fs_struct(taskfs_s *old)
{
	taskfs_s *fs = kmem_cache_alloc(fs_cachep, GFP_KERNEL);
	/* We don't need to lock fs - think why ;-) */
	if (fs) {
		fs->users = 1;
		fs->in_exec = 0;

		spin_lock_init(&fs->lock);
		// seqcount_spinlock_init(&fs->seq, &fs->lock);
		fs->umask = old->umask;

		spin_lock(&old->lock);
		fs->root = old->root;
		path_get(&fs->root);
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
		spin_unlock(&old->lock);
	}
	return fs;
}


/* to be mentioned only in INIT_TASK */
taskfs_s init_fs = {
	.users		= 1,
	.lock		= __SPIN_LOCK_UNLOCKED(init_fs.lock),
	// .seq		= SEQCNT_SPINLOCK_ZERO(init_fs.seq, &init_fs.lock),
	.umask		= 0022,
};