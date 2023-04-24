/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_H
#define _LINUX_PID_H

	// #include <linux/rculist.h>
	// #include <linux/wait.h>
	// #include <linux/refcount.h>

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

	// struct upid {
	// 	int nr;
	// 	struct pid_namespace *ns;
	// };

	typedef struct pid
	{
		// refcount_t count;
		// unsigned int level;
		// spinlock_t lock;
		// /* lists of tasks that use this pid */
		// struct hlist_head tasks[PIDTYPE_MAX];
		// struct hlist_head inodes;
		// /* wait queue for pidfd notifications */
		// wait_queue_head_t wait_pidfd;
		// struct rcu_head rcu;
		// struct upid numbers[1];
	} pid_s;

	// extern pid_s init_struct_pid;

	// extern const struct file_operations pidfd_fops;

	// file_s;

	// extern pid_s *pidfd_pid(const file_s *file);
	// pid_s *pidfd_get_pid(unsigned int fd, unsigned int *flags);
	// task_s *pidfd_get_task(int pidfd, unsigned int *flags);
	// int pidfd_create(pid_s *pid, unsigned int flags);

	// static inline pid_s *get_pid(pid_s *pid)
	// {
	// 	if (pid)
	// 		refcount_inc(&pid->count);
	// 	return pid;
	// }

	// extern void put_pid(pid_s *pid);
	// extern task_s *pid_task(pid_s *pid, enum pid_type);
	// static inline bool pid_has_task(pid_s *pid, enum pid_type type)
	// {
	// 	return !hlist_empty(&pid->tasks[type]);
	// }
	// extern task_s *get_pid_task(pid_s *pid, enum pid_type);

	// extern pid_s *get_task_pid(task_s *task, enum pid_type type);

	// /*
	// * these helpers must be called with the tasklist_lock write-held.
	// */
	// extern void attach_pid(task_s *task, enum pid_type);
	extern void attach_pid(task_s *task);
	// extern void detach_pid(task_s *task, enum pid_type);
	extern void detach_pid(task_s *task);
	// extern void change_pid(task_s *task, enum pid_type,
	// 			pid_s *pid);
	// extern void exchange_tids(task_s *task, task_s *old);
	// extern void transfer_pid(task_s *old, task_s *new,
	// 			enum pid_type);

	// struct pid_namespace;
	// extern struct pid_namespace init_pid_ns;

	// extern int pid_max;
	// extern int pid_max_min, pid_max_max;

	// /*
	// * look up a PID in the hash table. Must be called with the tasklist_lock
	// * or rcu_read_lock() held.
	// *
	// * find_pid_ns() finds the pid in the namespace specified
	// * find_vpid() finds the pid by its virtual id, i.e. in the current namespace
	// *
	// * see also find_task_by_vpid() set in include/linux/sched.h
	// */
	// extern pid_s *find_pid_ns(int nr, struct pid_namespace *ns);
	// extern pid_s *find_vpid(int nr);

	// /*
	// * Lookup a PID in the hash table, and return with it's count elevated.
	// */
	// extern pid_s *find_get_pid(int nr);
	// extern pid_s *find_ge_pid(int nr, struct pid_namespace *);

	// extern pid_s *alloc_pid(struct pid_namespace *ns, pid_t *set_tid,
	// 				size_t set_tid_size);
	// extern void free_pid(pid_s *pid);
	// extern void disable_pid_allocation(struct pid_namespace *ns);

	// /*
	// * ns_of_pid() returns the pid namespace in which the specified pid was
	// * allocated.
	// *
	// * NOTE:
	// * 	ns_of_pid() is expected to be called for a process (task) that has
	// * 	an attached 'pid_s' (see attach_pid(), detach_pid()) i.e @pid
	// * 	is expected to be non-NULL. If @pid is NULL, caller should handle
	// * 	the resulting NULL pid-ns.
	// */
	// static inline struct pid_namespace *ns_of_pid(pid_s *pid)
	// {
	// 	struct pid_namespace *ns = NULL;
	// 	if (pid)
	// 		ns = pid->numbers[pid->level].ns;
	// 	return ns;
	// }

	// /*
	// * is_child_reaper returns true if the pid is the init process
	// * of the current namespace. As this one could be checked before
	// * pid_ns->child_reaper is assigned in copy_process, we check
	// * with the pid number.
	// */
	// static inline bool is_child_reaper(pid_s *pid)
	// {
	// 	return pid->numbers[pid->level].nr == 1;
	// }

	// /*
	// * the helpers to get the pid's id seen from different namespaces
	// *
	// * pid_nr()    : global id, i.e. the id seen from the init namespace;
	// * pid_vnr()   : virtual id, i.e. the id seen from the pid namespace of
	// *               current.
	// * pid_nr_ns() : id seen from the ns specified.
	// *
	// * see also task_xid_nr() etc in include/linux/sched.h
	// */

	// static inline pid_t pid_nr(pid_s *pid)
	// {
	// 	pid_t nr = 0;
	// 	if (pid)
	// 		nr = pid->numbers[0].nr;
	// 	return nr;
	// }

	// pid_t pid_nr_ns(pid_s *pid, struct pid_namespace *ns);
	// pid_t pid_vnr(pid_s *pid);

	// #define do_each_pid_task(pid, type, task)				\
	// 	do {								\
	// 		if ((pid) != NULL)					\
	// 			hlist_for_each_entry_rcu((task),		\
	// 				&(pid)->tasks[type], pid_links[type]) {

	// 			/*
	// 			* Both old and new leaders may be attached to
	// 			* the same pid in the middle of de_thread().
	// 			*/
	// #define while_each_pid_task(pid, type, task)				\
	// 				if (type == PIDTYPE_PID)		\
	// 					break;				\
	// 			}						\
	// 	} while (0)

	// #define do_each_pid_thread(pid, type, task)				\
	// 	do_each_pid_task(pid, type, task) {				\
	// 		task_s *tg___ = task;			\
	// 		for_each_thread(tg___, task) {

	// #define while_each_pid_thread(pid, type, task)				\
	// 		}							\
	// 		task = tg___;						\
	// 	} while_each_pid_task(pid, type, task)

#endif /* _LINUX_PID_H */
