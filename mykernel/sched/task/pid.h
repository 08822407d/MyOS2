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

		extern pid_t
		pid_nr(pid_s *pid);

	#endif
	
	#if defined(PID_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pid_s
		*get_pid(pid_s *pid) {
			if (pid) atomic_inc(&pid->count);
			return pid;
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

	#endif /* !DEBUG */

#endif /* _LINUX_PID_H_ */
