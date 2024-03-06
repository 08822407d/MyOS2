/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_TASK_STACK_H
#define _LINUX_SCHED_TASK_STACK_H

	/*
	 * task->stack (kernel stack) handling interfaces:
	 */

	#include <linux/kernel/sched.h>
	#include <uapi/linux/magic.h>

	/*
	 * When accessing the stack of a non-current task that might exit, use
	 * try_get_task_stack() instead.  task_stack_page will return a pointer
	 * that could get freed out from under you.
	 */
	static inline void
	*task_stack_page(const task_s *task) {
		return task->stack;
	}


#endif /* _LINUX_SCHED_TASK_STACK_H */
