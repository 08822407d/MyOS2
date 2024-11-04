/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_H_
#define _LINUX_PID_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern pid_s
		*get_pid(pid_s *pid);

		extern pid_ns_s
		*ns_of_pid(pid_s *pid);

		extern bool
		is_child_reaper(pid_s *pid);

		extern pid_t
		pid_nr(pid_s *pid);

		extern pid_s
		*task_pid(task_s *task);

		extern pid_t
		task_pid_nr(task_s *tsk);

		extern pid_t
		task_tgid_vnr(task_s *tsk);

	#endif
	
	#if defined(PID_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pid_s
		*get_pid(pid_s *pid) {
			if (pid) atomic_inc(&pid->count);
			return pid;
		}

		/*
		 * ns_of_pid() returns the pid namespace in which the specified pid was
		 * allocated.
		 *
		 * NOTE:
		 * 	ns_of_pid() is expected to be called for a process (task) that has
		 * 	an attached 'struct pid' (see attach_pid(), detach_pid()) i.e @pid
		 * 	is expected to be non-NULL. If @pid is NULL, caller should handle
		 * 	the resulting NULL pid-ns.
		 */
		PREFIX_STATIC_INLINE
		pid_ns_s
		*ns_of_pid(pid_s *pid) {
			pid_ns_s *ns = NULL;
			if (pid)
				ns = pid->numbers[pid->level].ns;
			return ns;
		}

		/*
		 * is_child_reaper returns true if the pid is the init process
		 * of the current namespace. As this one could be checked before
		 * pid_ns->child_reaper is assigned in copy_process, we check
		 * with the pid number.
		 */
		PREFIX_STATIC_INLINE
		bool
		is_child_reaper(pid_s *pid) {
			return pid->numbers[pid->level].nr == 1;
		}

		/*
		 * the helpers to get the pid's id seen from different namespaces
		 *
		 * pid_nr()    : global id, i.e. the id seen from the init namespace;
		 * pid_vnr()   : virtual id, i.e. the id seen from the pid namespace of
		 *               current.
		 * pid_nr_ns() : id seen from the ns specified.
		 *
		 * see also task_xid_nr() etc in include/linux/sched.h
		 */
		PREFIX_STATIC_INLINE
		pid_t
		pid_nr(pid_s *pid) {
			pid_t nr = 0;
			if (pid)
				nr = pid->numbers[0].nr;
			return nr;
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_pid(task_s *task) {
			return task->thread_pid;
		}


		PREFIX_STATIC_INLINE
		pid_t
		task_pid_nr(task_s *tsk) {
			return tsk->pid;
		}

		PREFIX_STATIC_INLINE
		pid_t
		task_tgid_vnr(task_s *tsk) {
			return __task_pid_nr_ns(tsk, PIDTYPE_TGID, NULL);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_PID_H_ */
