/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_H_
#define _LINUX_SCHED_H_

	#include <uapi/linux/sched.h>

	/*
	 * Define 'task_s' and provide the main scheduler
	 * APIs (schedule(), wakeup variants, etc.)
	 */
	#include <sched/task_api.h>
	#include <sched/forkexec_api.h>
	#include <sched/runqueue_api.h>
	#include <sched/scheduler_api.h>
	#include <sched/preempt_api.h>
	#include <sched/sched_misc_api.h>

#endif
