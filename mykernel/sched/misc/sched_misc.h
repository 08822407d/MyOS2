/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_MISC_H_
#define _LINUX_SCHED_MISC_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern void
		*task_stack_page(const task_s *task);

	#endif
	
	#if defined(SCHED_MISC_DEFINATION) || !(DEBUG)

		/*
		 * When accessing the stack of a non-current task that might exit, use
		 * try_get_task_stack() instead.  task_stack_page will return a pointer
		 * that could get freed out from under you.
		 */
		PREFIX_STATIC_INLINE
		void
		*task_stack_page(const task_s *task) {
			return task->stack;
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SCHED_MISC_H_ */