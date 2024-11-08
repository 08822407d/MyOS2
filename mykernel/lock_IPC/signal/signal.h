#ifndef _LINUX_SIGNAL_H_
#define _LINUX_SIGNAL_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern pid_s
		*task_pid_type(task_s *task, enum pid_type type);

		extern pid_s
		*task_tgid(task_s *task);

		extern pid_s
		*task_pgrp(task_s *task);

		extern pid_s
		*task_session(task_s *task);

		// extern int
		// get_nr_threads(task_s *task);

		extern bool
		thread_group_leader(task_s *p);

		extern bool
		same_thread_group(task_s *p1, task_s *p2);

	#endif

	#include "signal_macro.h"
	
	#if defined(SIGNAL_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pid_s
		*task_pid_type(task_s *task, enum pid_type type) {
			pid_s *pid;
			if (type == PIDTYPE_PID)
				pid = task_pid(task);
			else
				pid = task->signal->pids[type];
			return pid;
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_tgid(task_s *task) {
			return task->signal->pids[PIDTYPE_TGID];
		}

		/*
		 * Without tasklist or RCU lock it is not safe to dereference
		 * the result of task_pgrp/task_session even if task == current,
		 * we can race with another thread doing sys_setsid/sys_setpgid.
		 */
		PREFIX_STATIC_INLINE
		pid_s
		*task_pgrp(task_s *task) {
			return task->signal->pids[PIDTYPE_PGID];
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_session(task_s *task) {
			return task->signal->pids[PIDTYPE_SID];
		}

		// PREFIX_STATIC_INLINE
		// int
		// get_nr_threads(task_s *task) {
		// 	return task->signal->nr_threads;
		// }

		PREFIX_STATIC_INLINE
		bool
		thread_group_leader(task_s *p) {
			return p->exit_signal >= 0;
		}

		PREFIX_STATIC_INLINE
		bool
		same_thread_group(task_s *p1, task_s *p2) {
			return p1->signal == p2->signal;
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SIGNAL_H_ */