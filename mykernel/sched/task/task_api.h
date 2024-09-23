/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_API_H_
#define _LINUX_TASK_API_H_

	#include "task.h"
	#include "pid.h"


	extern pid_s init_struct_pid;


	extern task_s *myos_find_task_by_pid(pid_t nr);
	extern int wake_up_process(task_s *tsk);
	extern void wake_up_new_task(task_s *tsk);
	extern void __set_task_comm(task_s *tsk, const char *from);
	extern char *__get_task_comm(char *to, size_t len, task_s *tsk);

	extern void myos_init_pid_allocator(void);
	extern unsigned long myos_idr_alloc(void);
	extern void myos_schedule(void);


	extern void put_pid(pid_s *pid);
	extern pid_s *get_task_pid(task_s *task, enum pid_type type);

	/*
	 * these helpers must be called with the tasklist_lock write-held.
	 */
	extern void attach_pid(task_s *task, enum pid_type);
	extern void detach_pid(task_s *task, enum pid_type);

	extern pid_s *alloc_pid(pid_t *set_tid, size_t set_tid_size);
	extern void free_pid(pid_s *pid);

	// pid_t pid_nr_ns(pid_s *pid, struct pid_namespace *ns);
	pid_t pid_vnr(pid_s *pid);

#endif /* _LINUX_TASK_API_H_ */