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



	/* <linux/sched/prio.h> */

	#define MAX_NICE				19
	#define MIN_NICE				-20
	#define NICE_WIDTH				(MAX_NICE - MIN_NICE + 1)

	/*
	 * Priority of a process goes from 0..MAX_PRIO-1, valid RT
	 * priority is 0..MAX_RT_PRIO-1, and SCHED_NORMAL/SCHED_BATCH
	 * tasks are in the range MAX_RT_PRIO..MAX_PRIO-1. Priority
	 * values are inverted: lower p->prio value means higher priority.
	 */

	#define MAX_RT_PRIO				100

	#define MAX_PRIO				(MAX_RT_PRIO + NICE_WIDTH)
	#define DEFAULT_PRIO			(MAX_RT_PRIO + NICE_WIDTH / 2)

#endif /* _LINUX_RUNQUEUE_CONST_H_ */