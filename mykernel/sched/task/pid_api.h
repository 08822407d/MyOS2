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

	// pid_t pid_nr_ns(pid_s *pid, struct pid_namespace *ns);
	pid_t pid_vnr(pid_s *pid);


	extern pid_ns_s *task_active_pid_ns(task_s *tsk);
	void pidhash_init(void);
	void pid_idr_init(void);

#endif /* _LINUX_PID_API_H_ */