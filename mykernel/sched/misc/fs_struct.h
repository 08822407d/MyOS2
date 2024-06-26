/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_STRUCT_H_
#define _LINUX_FS_STRUCT_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern void
		get_fs_root(taskfs_s *fs, path_s *root);

		extern void
		get_fs_pwd(taskfs_s *fs, path_s *pwd);

	#endif
	
	#if defined(FS_STRUCT_DEFINATION) || !(DEBUG)

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