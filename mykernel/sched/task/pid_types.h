/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_TYPES_H_
#define _LINUX_PID_TYPES_H_

	#include <linux/kernel/lock_ipc.h>


	enum pid_type
	{
		PIDTYPE_PID,
		PIDTYPE_TGID,
		PIDTYPE_PGID,
		PIDTYPE_SID,
		PIDTYPE_MAX,
	};

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
	typedef struct upid {
		int nr;
		// struct pid_namespace *ns;
	} upid_s;

	typedef struct pid {
		atomic_t		count;
		unsigned int	level;
		spinlock_t		lock;
		// /* lists of tasks that use this pid */
		// struct hlist_head tasks[PIDTYPE_MAX];
		// struct hlist_head inodes;
		// /* wait queue for pidfd notifications */
		// wait_queue_head_t wait_pidfd;
		// struct rcu_head rcu;
		upid_s			numbers[1];
	} pid_s;

#endif /* _LINUX_PID_TYPES_H_ */