/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_H_
#define _LINUX_RUNQUEUE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern long
		nice_to_rlimit(long nice);

		extern long
		rlimit_to_nice(long prio);

		extern int
		task_nice(const task_s *p);

	#endif

	#include "runqueue_macro.h"
	
	#if defined(RUNQUEUE_DEFINATION) || !(DEBUG)

		/*
		 * Convert nice value [19,-20] to rlimit style value [1,40].
		 */
		PREFIX_STATIC_INLINE
		long
		nice_to_rlimit(long nice) {
			return (MAX_NICE - nice + 1);
		}

		/*
		 * Convert rlimit style value [1,40] to nice value [-20, 19].
		 */
		PREFIX_STATIC_INLINE
		long
		rlimit_to_nice(long prio) {
			return (MAX_NICE - prio + 1);
		}

		/**
		 * task_nice - return the nice value of a given task.
		 * @p: the task in question.
		 *
		 * Return: The nice value [ -20 ... 0 ... 19 ].
		 */
		PREFIX_STATIC_INLINE
		int
		task_nice(const task_s *p) {
			return PRIO_TO_NICE((p)->static_prio);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_RUNQUEUE_H_ */