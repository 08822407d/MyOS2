/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_TYPES_H_
#define _LINUX_PID_TYPES_H_

	#include "../sched_type_declaration.h"


	/*
	 * What is pid_s?
	 *
	 * A pid_s is the kernel's internal notion of a process identifier.
	 * It refers to individual tasks, process groups, and sessions.  While
	 * there are processes attached to it the pid_s lives in a hash
	 * table, so it and then the processes that it refers to can be found
	 * quickly from the numeric pid value.  The attached processes may be
	 * quickly accessed by following pointers from pid_s.
	 *
	 * Storing pid_t values in the kernel and referring to them later has a
	 * problem.  The process originally with that pid may have exited and the
	 * pid allocator wrapped, and another process could have come along
	 * and been assigned that pid.
	 *
	 * Referring to user space processes by holding a reference to struct
	 * task_struct has a problem.  When the user space process exits
	 * the now useless task_struct is still kept.  A task_struct plus a
	 * stack consumes around 10K of low kernel memory.  More precisely
	 * this is THREAD_SIZE + sizeof(task_s).  By comparison
	 * a pid_s is about 64 bytes.
	 *
	 * Holding a reference to pid_s solves both of these problems.
	 * It is small so holding a reference does not consume a lot of
	 * resources, and since a new pid_s is allocated when the numeric pid
	 * value is reused (when pids wrap around) we don't mistakenly refer to new
	 * processes.
	 */


	/*
	 * struct upid is used to get the id of the pid_s, as it is
	 * seen in particular namespace. Later the pid_s is found with
	 * find_pid_ns() using the int nr and struct pid_namespace *ns.
	 */
	struct upid {
		int			nr;
		pid_ns_s	*ns;
	};

	struct pid {
		atomic_t		count;
		uint			level;
		spinlock_t		lock;
		/* lists of tasks that use this pid */
		HList_hdr_s		tasks[PIDTYPE_MAX];
		HList_hdr_s		inodes;
		/* wait queue for pidfd notifications */
		// wait_queue_head_t wait_pidfd;
		// struct rcu_head rcu;
		upid_s			numbers[1];
	};

	struct pid_namespace {
		// struct idr idr;
		// struct rcu_head rcu;
		uint			pid_allocated;
		task_s			*child_reaper;
		kmem_cache_s	*pid_cachep;
		uint			level;
		pid_ns_s		*parent;
	// #ifdef CONFIG_BSD_PROCESS_ACCT
	// 	struct fs_pin *bacct;
	// #endif
		// struct user_namespace *user_ns;
		// struct ucounts *ucounts;
		int				reboot;	/* group exit code if this pidns was rebooted */
		// struct ns_common ns;
	// #if defined(CONFIG_SYSCTL) && defined(CONFIG_MEMFD_CREATE)
	// 	int memfd_noexec_scope;
	// #endif
	} __randomize_layout;

#endif /* _LINUX_PID_TYPES_H_ */