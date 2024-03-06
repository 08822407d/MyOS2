/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_H_
#define _LINUX_SCHED_H_

	#include <uapi/linux/sched.h>

	/*
	 * Define 'task_s' and provide the main scheduler
	 * APIs (schedule(), wakeup variants, etc.)
	 */
	#include <sched/runqueue_api.h>
	#include <sched/task_api.h>

#endif
