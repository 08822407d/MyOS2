/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_STRUCT_H
#define _LINUX_FS_STRUCT_H

	#include <linux/fs/path.h>
	// #include <linux/spinlock.h>
	// #include <linux/seqlock.h>

	typedef struct fs_struct {
		int		users;
		// spinlock_t lock;
		// seqcount_spinlock_t seq;
		int		umask;
		int		in_exec;
		path_s	root, pwd;
	} taskfs_s;

	// extern struct kmem_cache *fs_cachep;

	// extern void exit_fs(task_s *);
	extern void set_fs_root(taskfs_s *, const path_s *);
	extern void set_fs_pwd(taskfs_s *, const path_s *);
	// extern taskfs_s *copy_fs_struct(taskfs_s *);
	// extern void free_fs_struct(taskfs_s *);
	// extern int unshare_fs_struct(void);

	// static inline void get_fs_root(taskfs_s *fs, path_s *root)
	// {
	// 	spin_lock(&fs->lock);
	// 	*root = fs->root;
	// 	path_get(root);
	// 	spin_unlock(&fs->lock);
	// }

	// static inline void get_fs_pwd(taskfs_s *fs, path_s *pwd)
	// {
	// 	spin_lock(&fs->lock);
	// 	*pwd = fs->pwd;
	// 	path_get(pwd);
	// 	spin_unlock(&fs->lock);
	// }

	// extern bool current_chrooted(void);

#endif /* _LINUX_FS_STRUCT_H */