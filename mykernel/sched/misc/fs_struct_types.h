/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_STRUCT_TYPES_H_
#define _LINUX_FS_STRUCT_TYPES_H_

	#include "../sched_type_declaration.h"
	#include <linux/fs/path.h>


	typedef struct fs_struct {
		int			users;
		spinlock_t	lock;
		// seqcount_spinlock_t seq;
		int			umask;
		int			in_exec;
		path_s		root, pwd;
	} taskfs_s;

#endif /* _LINUX_FS_STRUCT_TYPES_H_ */