/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_TYPES_H_
#define _LINUX_SCHEDULER_TYPES_H_

	#include <linux/kernel/sched.h>
	#include <linux/kernel/lock_ipc.h>


	typedef struct sched_entity {
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

	typedef struct sched_rt_entity {
		List_s			run_list;
		// unsigned long timeout;
		// unsigned long watchdog_stamp;
		unsigned int	time_slice;
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


	typedef struct sched_class {
	// #ifdef CONFIG_UCLAMP_TASK
		// int uclamp_enabled;
	// #endif

		void	(*enqueue_task) (rq_s *rq, task_s *p, int flags);
		void	(*dequeue_task) (rq_s *rq, task_s *p, int flags);
		// void	(*yield_task) (rq_s *rq);
		// bool	(*yield_to_task) (rq_s *rq, task_s *p);

		// void	(*check_preempt_curr) (rq_s *rq, task_s *p, int flags);

		task_s	*(*pick_next_task) (rq_s *rq);

		// void	(*put_prev_task) (rq_s *rq, task_s *p);
		// void	(*set_next_task) (rq_s *rq, task_s *p, bool first);

		// int		(*balance) (rq_s *rq, task_s *prev, struct rq_flags *rf);
		// int		(*select_task_rq) (task_s *p, int task_cpu, int flags);

		// task_s	*(*pick_task) (rq_s *rq);

		// void	(*migrate_task_rq) (task_s *p, int new_cpu);

		// void	(*task_woken) (rq_s *this_rq, task_s *task);

		// void	(*set_cpus_allowed) (task_s *p, struct affinity_context *ctx);

		// void	(*rq_online) (rq_s *rq);
		// void	(*rq_offline) (rq_s *rq);

		// rq_s	*(*find_lock_rq) (task_s *p, rq_s *rq);

		// void	(*task_tick) (rq_s *rq, task_s *p, int queued);
		// void	(*task_fork) (task_s *p);
		// void	(*task_dead) (task_s *p);

		// /*
		//  * The switched_from() call is allowed to drop rq->lock, therefore we
		//  * cannot assume the switched_from/switched_to pair is serialized by
		//  * rq->lock. They are however serialized by p->pi_lock.
		//  */
		// void	(*switched_from) (rq_s *this_rq, task_s *task);
		// void	(*switched_to) (rq_s *this_rq, task_s *task);
		// void	(*prio_changed) (rq_s *this_rq, task_s *task, int oldprio);

		// unsigned int	(*get_rr_interval) (rq_s *rq, task_s *task);

		// void	(*update_curr) (rq_s *rq);

		// void	(*task_change_group) (task_s *p);

	// #ifdef CONFIG_SCHED_CORE
		// int		(*task_is_throttled) (task_s *p, int cpu);
	// #endif
	} sched_class_s;

#endif /* _LINUX_SCHEDULER_TYPES_H_ */