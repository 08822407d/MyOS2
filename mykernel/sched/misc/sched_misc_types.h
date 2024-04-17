/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_MISC_TYPES_H_
#define _LINUX_SCHED_MISC_TYPES_H_

	#include "../sched_type_declaration.h"


	typedef struct kernel_clone_args {
		u64				flags;
		int __user		*pidfd;
		int __user		*child_tid;
		int __user		*parent_tid;
		const char		*name;
		int				exit_signal;
		u32				kthread			: 1;
		u32				io_thread		: 1;
		u32				user_worker		: 1;
		u32				no_files		: 1;
		ulong			stack;
		ulong			stack_size;
		ulong			tls;
		pid_t			*set_tid;
		/* Number of elements in *set_tid */
		size_t			set_tid_size;
		int				cgroup;
		int				idle;
		int				(*fn)(void *);
		void			*fn_arg;
		// struct cgroup *cgrp;
		// struct css_set *cset;
	} kclone_args_s;

#endif /* _LINUX_SCHED_MISC_TYPES_H_ */