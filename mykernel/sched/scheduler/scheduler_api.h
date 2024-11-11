/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_API_H_
#define _LINUX_SCHEDULER_API_H_

	#include "scheduler.h"
	#include "preempt.h"


	/* Defined in include/asm-generic/vmlinux.lds.h */
	extern sched_class_s __sched_class_highest[];
	extern sched_class_s __sched_class_lowest[];


	extern void kick_process(task_s *p);

	int wake_up_process(task_s *p);
	int wake_up_state(task_s *p, uint state);

	extern void schedule_idle(void);
	extern long schedule_timeout(long timeout);
	asmlinkage void schedule(void);
	extern void schedule_preempt_disabled(void);
	extern void preempt_schedule(void);

	extern void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait);

	extern void preempt_schedule(void);


	/*
	 * Scheduler init related prototypes:
	 */

	extern void sched_init(void);
	extern void sched_init_smp(void);

#endif /* _LINUX_SCHEDULER_API_H_ */