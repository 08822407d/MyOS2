/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_TYPES_H_
#define _LINUX_RUNQUEUE_TYPES_H_

	#include <linux/kernel/types.h>

	typedef struct sched_entity
	{
		// /* For load-balancing: */
		// struct load_weight load;
		// struct rb_node run_node;
		// List_s group_node;
		// unsigned int on_rq;

		// u64				exec_start;
		// u64				sum_exec_runtime;
		u64				vruntime;
		// u64				prev_sum_exec_runtime;
	
		// u64				nr_migrations;

	// #ifdef CONFIG_FAIR_GROUP_SCHED
		// int depth;
		// struct sched_entity *parent;
		// /* rq on which this entity is (to be) queued: */
		// struct cfs_rq *cfs_rq;
		// /* rq "owned" by this entity/group: */
		// struct cfs_rq *my_q;
		// /* cached value of my_q->h_nr_running */
		// unsigned long runnable_weight;
	// #endif

		/*
		 * Per entity load average tracking.
		 *
		 * Put into separate cache line so it does not
		 * collide with read-mostly values above.
		 */
		// struct sched_avg avg;
	} sched_entity_s;

	typedef struct sched_rt_entity
	{
		// List_s run_list;
		// unsigned long timeout;
		// unsigned long watchdog_stamp;
		unsigned int time_slice;
		// unsigned short on_rq;
		// unsigned short on_list;

		// struct sched_rt_entity *back;
	// #ifdef CONFIG_RT_GROUP_SCHED
		// struct sched_rt_entity *parent;
		// /* rq on which this entity is (to be) queued: */
		// struct rt_rq *rt_rq;
		// /* rq "owned" by this entity/group: */
		// struct rt_rq *my_q;
	// #endif
	} sched_rt_entity_s;

	typedef struct sched_dl_entity
	{
		// struct rb_node rb_node;

		// /*
		// * Original scheduling parameters. Copied here from sched_attr
		// * during sched_setattr(), they will remain the same until
		// * the next sched_setattr().
		// */
		// u64 dl_runtime;	 /* Maximum runtime for each instance	*/
		// u64 dl_deadline; /* Relative deadline of each instance	*/
		// u64 dl_period;	 /* Separation of two instances (period) */
		// u64 dl_bw;		 /* dl_runtime / dl_period		*/
		// u64 dl_density;	 /* dl_runtime / dl_deadline		*/

		// /*
		// * Actual scheduling parameters. Initialized with the values above,
		// * they are continuously updated during task execution. Note that
		// * the remaining runtime could be < 0 in case we are in overrun.
		// */
		// s64 runtime;		/* Remaining runtime for this instance	*/
		// u64 deadline;		/* Absolute deadline for this instance	*/
		// unsigned int flags; /* Specifying the scheduler behaviour	*/

		// /*
		// * Some bool flags:
		// *
		// * @dl_throttled tells if we exhausted the runtime. If so, the
		// * task has to wait for a replenishment to be performed at the
		// * next firing of dl_timer.
		// *
		// * @dl_yielded tells if task gave up the CPU before consuming
		// * all its available runtime during the last job.
		// *
		// * @dl_non_contending tells if the task is inactive while still
		// * contributing to the active utilization. In other words, it
		// * indicates if the inactive timer has been armed and its handler
		// * has not been executed yet. This flag is useful to avoid race
		// * conditions between the inactive timer handler and the wakeup
		// * code.
		// *
		// * @dl_overrun tells if the task asked to be informed about runtime
		// * overruns.
		// */
		// unsigned int dl_throttled : 1;
		// unsigned int dl_yielded : 1;
		// unsigned int dl_non_contending : 1;
		// unsigned int dl_overrun : 1;

		// /*
		// * Bandwidth enforcement timer. Each -deadline task has its
		// * own bandwidth to be enforced, thus we need one timer per task.
		// */
		// struct hrtimer dl_timer;

		// /*
		// * Inactive timer, responsible for decreasing the active utilization
		// * at the "0-lag time". When a -deadline task blocks, it contributes
		// * to GRUB's active utilization until the "0-lag time", hence a
		// * timer is needed to decrease the active utilization at the correct
		// * time.
		// */
		// struct hrtimer inactive_timer;

	// #ifdef CONFIG_RT_MUTEXES
	// 	/*
	// 	* Priority Inheritance. When a DEADLINE scheduling entity is boosted
	// 	* pi_se points to the donor, otherwise points to the dl_se it belongs
	// 	* to (the original one/itself).
	// 	*/
	// 	struct sched_dl_entity *pi_se;
	// #endif
	} sched_dl_entity_s;

#endif /* _LINUX_RUNQUEUE_TYPES_H_ */