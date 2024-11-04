/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_API_H_
#define _LINUX_PID_API_H_

	#include "task.h"
	#include "pid.h"


	extern pid_s	init_struct_pid;
	extern pid_ns_s	init_pid_ns;


	extern void put_pid(pid_s *pid);
	extern pid_s *get_task_pid(task_s *task, enum pid_type type);

	/*
	 * these helpers must be called with the tasklist_lock write-held.
	 */
	extern void attach_pid(task_s *task, enum pid_type);
	extern void detach_pid(task_s *task, enum pid_type);

	extern pid_s *alloc_pid(pid_ns_s *ns, pid_t *set_tid, size_t set_tid_size);
	extern void free_pid(pid_s *pid);

	/*
	 * the helpers to get the task's different pids as they are seen
	 * from various namespaces
	 *
	 * task_xid_nr()     : global id, i.e. the id seen from the init namespace;
	 * task_xid_vnr()    : virtual id, i.e. the id seen from the pid namespace of
	 *                     current.
	 * task_xid_nr_ns()  : id seen from the ns specified;
	 *
	 * see also pid_nr() etc in include/linux/pid.h
	 */
	pid_t __task_pid_nr_ns(task_s *task, enum pid_type type, pid_ns_s *ns);

	// pid_t pid_nr_ns(pid_s *pid, struct pid_namespace *ns);
	pid_t pid_vnr(pid_s *pid);


	extern pid_ns_s *task_active_pid_ns(task_s *tsk);
	void pidhash_init(void);
	void pid_idr_init(void);

#endif /* _LINUX_PID_API_H_ */