/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_H_
#define _LINUX_SCHEDULER_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern uint
		task_cpu(const task_s *p);

	#endif

	#include "scheduler_macro.h"
	
	#if defined(SCHEDULER_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		uint
		task_cpu(const task_s *p) {
			return READ_ONCE(task_thread_info(p)->cpu);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SCHEDULER_H_ */