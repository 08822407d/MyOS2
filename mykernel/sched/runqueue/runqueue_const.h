/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_CONST_H_
#define _LINUX_RUNQUEUE_CONST_H_


	/*
	 * default timeslice is 100 msecs (used only for SCHED_RR tasks).
	 * Timeslices get refilled after they expire.
	 */
	#define RR_TIMESLICE			(100 * HZ / 1000)

	/* task_struct::on_rq states: */
	#define TASK_ON_RQ_QUEUED		1
	#define TASK_ON_RQ_MIGRATING	2

#endif /* _LINUX_RUNQUEUE_CONST_H_ */