/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_API_H_
#define _LINUX_SCHEDULER_API_H_

	#include "scheduler.h"
	#include "preempt.h"


	/* Defined in include/asm-generic/vmlinux.lds.h */
	extern sched_class_s __sched_class_highest[];
	extern sched_class_s __sched_class_lowest[];

	extern const sched_class_s stop_sched_class;
	extern const sched_class_s dl_sched_class;
	extern const sched_class_s rt_sched_class;
	extern const sched_class_s fair_sched_class;
	extern const sched_class_s idle_sched_class;
	extern const sched_class_s myos_rt_sched_class;


	extern void kick_process(task_s *p);

	void resched_curr(rq_s *rq);
	void resched_cpu(int cpu);

	rq_s *finish_task_switch(task_s *prev) __releases(rq->lock);

	int try_to_wake_up(task_s *p, uint state, int wake_flags);
	int wake_up_process(task_s *p);
	int wake_up_state(task_s *p, uint state);

	void __schedule(int sched_mode);

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