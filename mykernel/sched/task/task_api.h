/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_API_H_
#define _LINUX_TASK_API_H_

	#include "task.h"
	#include "pid.h"


	extern char init_stack[THREAD_SIZE] __page_aligned_data;
	extern task_s init_task;

	extern task_s *myos_find_task_by_pid(pid_t nr);
	extern int wake_up_process(task_s *tsk);
	extern void wake_up_new_task(task_s *tsk);
	extern void __set_task_comm(task_s *tsk, const char *from);
	extern char *__get_task_comm(char *to, size_t len, task_s *tsk);

	extern void myos_init_pid_allocator(void);
	extern unsigned long myos_idr_alloc(void);
	extern void myos_schedule(void);


	extern void init_idle(task_s *idle, int cpu);

#endif /* _LINUX_TASK_API_H_ */