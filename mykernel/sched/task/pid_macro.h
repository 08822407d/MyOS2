/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_MACRO_H_
#define _LINUX_PID_MACRO_H_


	#define do_each_pid_task(pid, type, task)					\
			do {												\
				if ((pid) != NULL)								\
					hlist_for_each_entry_rcu((task),			\
						&(pid)->tasks[type], pid_links[type]) {

					/*
					 * Both old and new leaders may be attached to
					 * the same pid in the middle of de_thread().
					 */
	#define while_each_pid_task(pid, type, task)				\
					if (type == PIDTYPE_PID)					\
						break;									\
				}												\
			} while (0)

#endif /* _LINUX_PID_MACRO_H_ */