/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_STRUCT_H_
#define _LINUX_FS_STRUCT_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "fs_struct_types.h"


	#ifdef DEBUG

		extern void
		get_fs_root(taskfs_s *fs, path_s *root);

		extern void
		get_fs_pwd(taskfs_s *fs, path_s *pwd);

	#endif
	
	#if defined(SPINLOCK_SMP_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		get_fs_root(taskfs_s *fs, path_s *root) {
			spin_lock(&fs->lock);
			*root = fs->root;
			path_get(root);
			spin_unlock(&fs->lock);
		}

		PREFIX_STATIC_INLINE
		void
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
			spin_lock(&fs->lock);
			*pwd = fs->pwd;
			path_get(pwd);
			spin_unlock(&fs->lock);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_FS_STRUCT_H_ */