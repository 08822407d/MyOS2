/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PATH_H
#define _LINUX_PATH_H

	#include <linux/fs/vfs_s_defs.h>

	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;

	extern void path_get(const path_s *);
	extern void path_put(const path_s *);

	static inline int
	path_equal(const path_s *path1, const path_s *path2) {
		return path1->mnt == path2->mnt && path1->dentry == path2->dentry;
	}

	static inline void
	path_put_init(path_s *path) {
		path_put(path);
		*path = (path_s) { };
	}

#endif  /* _LINUX_PATH_H */