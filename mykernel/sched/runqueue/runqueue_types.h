/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_TYPES_H_
#define _LINUX_RUNQUEUE_TYPES_H_

	#include "../sched_type_declaration.h"


	/* CFS-related fields in a runqueue */
	struct cfs_rq {
		// struct load_weight	load;
		// unsigned int		nr_running;
		// unsigned int		h_nr_running;      /* SCHED_{NORMAL,BATCH,IDLE} */
		// unsigned int		idle_nr_running;   /* SCHED_IDLE */
		// unsigned int		idle_h_nr_running; /* SCHED_IDLE */

		// u64			exec_clock;
		// u64			min_vruntime;

		// struct rb_root_cached	tasks_timeline;

		// /*
		//  * 'curr' points to currently running entity on this cfs_rq.
		//  * It is set to NULL otherwise (i.e when none are currently running).
		//  */
		// struct sched_entity	*curr;
		// struct sched_entity	*next;
		// struct sched_entity	*last;
		// struct sched_entity	*skip;

	// #ifdef	CONFIG_SCHED_DEBUG
		// unsigned int		nr_spread_over;
	// #endif

		// /*
		//  * CFS load tracking
		//  */
		// struct sched_avg	avg;
		// struct {
		// 	raw_spinlock_t	lock ____cacheline_aligned;
		// 	int		nr;
		// 	unsigned long	load_avg;
		// 	unsigned long	util_avg;
		// 	unsigned long	runnable_avg;
		// } removed;

	// #ifdef CONFIG_FAIR_GROUP_SCHED
		// unsigned long		tg_load_avg_contrib;
		// long			propagate;
		// long			prop_runnable_sum;

		// /*
		//  *   h_load = weight * f(tg)
		//  *
		//  * Where f(tg) is the recursive weight fraction assigned to
		//  * this group.
		//  */
		// unsigned long		h_load;
		// u64			last_h_load_update;
		// struct sched_entity	*h_load_next;

		rq_s		*rq;	/* CPU runqueue to which this cfs_rq is attached */

		// /*
		//  * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
		//  * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
		//  * (like users, containers etc.)
		//  *
		//  * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
		//  * This list is used during load balance.
		//  */
		// int			on_list;
		// struct list_head	leaf_cfs_rq_list;
		// struct task_group	*tg;	/* group that "owns" this runqueue */

		// /* Locally cached copy of our task_group's idle value */
		// int			idle;

	// #ifdef CONFIG_CFS_BANDWIDTH
		// int			runtime_enabled;
		// s64			runtime_remaining;

		// u64			throttled_pelt_idle;
		// u64			throttled_clock;
		// u64			throttled_clock_pelt;
		// u64			throttled_clock_pelt_time;
		// int			throttled;
		// int			throttle_count;
		// struct list_head	throttled_list;
		// struct list_head	throttled_csd_list;
	// #endif /* CONFIG_CFS_BANDWIDTH */
	// #endif /* CONFIG_FAIR_GROUP_SCHED */
	};

	// static inline int rt_bandwidth_enabled(void)
	// {
	// 	return sysctl_sched_rt_runtime >= 0;
	// }

	// /* RT IPI pull logic requires IRQ_WORK */
	// #if defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
	// # define HAVE_RT_PUSH_IPI
	// #endif

	/* Real-Time classes' related field in a runqueue: */
	struct rt_rq {
	// 	struct rt_prio_array	active;
	// 	unsigned int		rt_nr_running;
	// 	unsigned int		rr_nr_running;
	// 	struct {
	// 		int		curr; /* highest queued rt task prio */
	// 		int		next; /* next highest */
	// 	} highest_prio;
	// 	unsigned int		rt_nr_migratory;
	// 	unsigned int		rt_nr_total;
	// 	int			overloaded;
	// 	struct plist_head	pushable_tasks;

	// 	int			rt_queued;

	// 	int			rt_throttled;
	// 	u64			rt_time;
	// 	u64			rt_runtime;
	// 	/* Nests inside the rq lock: */
	// 	raw_spinlock_t		rt_runtime_lock;

	// #ifdef CONFIG_RT_GROUP_SCHED
	// 	unsigned int		rt_nr_boosted;

	// 	rq_s		*rq;
	// 	struct task_group	*tg;
	// #endif
	};

	// static inline bool rt_rq_is_runnable(struct rt_rq *rt_rq)
	// {
	// 	return rt_rq->rt_queued && rt_rq->rt_nr_running;
	// }

	/* Deadline class' related fields in a runqueue */
	struct dl_rq {
		// /* runqueue is an rbtree, ordered by deadline */
		// struct rb_root_cached	root;

		// unsigned int		dl_nr_running;

		// /*
		//  * Deadline values of the currently executing and the
		//  * earliest ready task on this rq. Caching these facilitates
		//  * the decision whether or not a ready but not running task
		//  * should migrate somewhere else.
		//  */
		// struct {
		// 	u64		curr;
		// 	u64		next;
		// } earliest_dl;

		// unsigned int		dl_nr_migratory;
		// int			overloaded;

		// /*
		//  * Tasks on this rq that can be pushed away. They are kept in
		//  * an rb-tree, ordered by tasks' deadlines, with caching
		//  * of the leftmost (earliest deadline) element.
		//  */
		// struct rb_root_cached	pushable_dl_tasks_root;

		// /*
		//  * "Active utilization" for this runqueue: increased when a
		//  * task wakes up (becomes TASK_RUNNING) and decreased when a
		//  * task blocks
		//  */
		// u64			running_bw;

		// /*
		//  * Utilization of the tasks "assigned" to this runqueue (including
		//  * the tasks that are in runqueue and the tasks that executed on this
		//  * CPU and blocked). Increased when a task moves to this runqueue, and
		//  * decreased when the task moves away (migrates, changes scheduling
		//  * policy, or terminates).
		//  * This is needed to compute the "inactive utilization" for the
		//  * runqueue (inactive utilization = this_bw - running_bw).
		//  */
		// u64			this_bw;
		// u64			extra_bw;

		// /*
		//  * Inverse of the fraction of CPU utilization that can be reclaimed
		//  * by the GRUB algorithm.
		//  */
		// u64			bw_ratio;
	};

	struct myos_rq {
		// MyOS2 variables
		List_hdr_s		running_lhdr;
		ulong			last_jiffies;	// abs jiffies when curr-task loaded
	};

	/*
	 * This is the main, per-CPU runqueue data structure.
	 *
	 * Locking rule: those places that want to lock multiple runqueues
	 * (such as the load balancing or the thread migration code), lock
	 * acquire operations must be ordered by ascending &runqueue.
	 */
	struct runqueue {
		/* runqueue lock: */
		spinlock_t		__lock;

	// 	/*
	// 	 * nr_running and cpu_load should be in the same cacheline because
	// 	 * remote CPUs use both these fields when doing load calculation.
	// 	 */
	// 	unsigned int		nr_running;
	// #ifdef CONFIG_NUMA_BALANCING
	// 	unsigned int		nr_numa_running;
	// 	unsigned int		nr_preferred_running;
	// 	unsigned int		numa_migrate_on;
	// #endif
	// #ifdef CONFIG_NO_HZ_COMMON
	// 	unsigned long		last_blocked_load_update_tick;
	// 	unsigned int		has_blocked_load;
	// 	call_single_data_t	nohz_csd;
	// 	unsigned int		nohz_tick_stopped;
	// 	atomic_t		nohz_flags;
	// #endif /* CONFIG_NO_HZ_COMMON */

		uint			ttwu_pending;
		u64				nr_switches;

	// #ifdef CONFIG_UCLAMP_TASK
	// 	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	// 	struct uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	// 	unsigned int		uclamp_flags;
	// #define UCLAMP_FLAG_IDLE 0x01
	// #endif

		cfs_rq_s		cfs;
		rt_rq_s			rt;
		dl_rq_s			dl;
		myos_rq_s		myos;

	// #ifdef CONFIG_FAIR_GROUP_SCHED
	// 	/* list of leaf cfs_rq on this CPU: */
	// 	struct list_head	leaf_cfs_rq_list;
	// 	struct list_head	*tmp_alone_branch;
	// #endif /* CONFIG_FAIR_GROUP_SCHED */

		/*
		 * This is part of a global counter where only the total sum
		 * over all CPUs matters. A task can increase this counter on
		 * one CPU and if it got migrated afterwards it may decrease
		 * it on another CPU. Always updated under the runqueue lock:
		 */
		uint			nr_uninterruptible;

		task_s __rcu	*curr;
		task_s			*idle;
		task_s			*stop;
		ulong			next_balance;
		mm_s			*prev_mm;

		uint			clock_update_flags;
		u64				clock;
		/* Ensure that all clocks are in the same cache line */
		u64				clock_task;
		u64				clock_pelt;
		ulong			lost_idle_time;
		u64				clock_pelt_idle;
		u64				clock_idle;

		atomic_t		nr_iowait;

	// #ifdef CONFIG_SCHED_DEBUG
	// 	u64 last_seen_need_resched_ns;
	// 	int ticks_without_resched;
	// #endif

	// #ifdef CONFIG_MEMBARRIER
	// 	int membarrier_state;
	// #endif

		// struct root_domain		*rd;
		// struct sched_domain __rcu	*sd;

		// unsigned long		cpu_capacity;
		// unsigned long		cpu_capacity_orig;

		// struct balance_callback *balance_callback;

		// unsigned char		nohz_idle_balance;
		// unsigned char		idle_balance;

		// unsigned long		misfit_task_load;

		// /* For active balancing */
		// int			active_balance;
		// int			push_cpu;
		// struct cpu_stop_work	active_balance_work;

		// /* CPU of this runqueue: */
		// int			cpu;
		// int			online;

		// struct list_head cfs_tasks;

		// struct sched_avg	avg_rt;
		// struct sched_avg	avg_dl;
	// #ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	// 	struct sched_avg	avg_irq;
	// #endif
	// #ifdef CONFIG_SCHED_THERMAL_PRESSURE
	// 	struct sched_avg	avg_thermal;
	// #endif
		// u64			idle_stamp;
		// u64			avg_idle;

		// unsigned long		wake_stamp;
		// u64			wake_avg_idle;

		// /* This is used to determine avg_idle's max value */
		// u64			max_idle_balance_cost;

	// #ifdef CONFIG_HOTPLUG_CPU
	// 	struct rcuwait		hotplug_wait;
	// #endif

	// #ifdef CONFIG_IRQ_TIME_ACCOUNTING
	// 	u64			prev_irq_time;
	// #endif
	// #ifdef CONFIG_PARAVIRT
	// 	u64			prev_steal_time;
	// #endif
	// #ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	// 	u64			prev_steal_time_rq;
	// #endif

		// /* calc_load related fields */
		// unsigned long		calc_load_update;
		// long			calc_load_active;

	// #ifdef CONFIG_SCHED_HRTICK
	// 	call_single_data_t	hrtick_csd;
	// 	struct hrtimer		hrtick_timer;
	// 	ktime_t 		hrtick_time;
	// #endif

	// #ifdef CONFIG_SCHEDSTATS
		// /* latency stats */
		// struct sched_info	rq_sched_info;
		// unsigned long long	rq_cpu_time;
		// /* could above be rq->cfs_rq.exec_clock + rq->rt_rq.rt_runtime ? */

		// /* sys_sched_yield() stats */
		// unsigned int		yld_count;

		// /* schedule() stats */
		// unsigned int		sched_count;
		// unsigned int		sched_goidle;

		// /* try_to_wake_up() stats */
		// unsigned int		ttwu_count;
		// unsigned int		ttwu_local;
	// #endif

	// #ifdef CONFIG_CPU_IDLE
		// /* Must be inspected within a rcu lock section */
		// struct cpuidle_state	*idle_state;
	// #endif

		// unsigned int		nr_pinned;
		// unsigned int		push_busy;
		// struct cpu_stop_work	push_work;

		// /* Scratch cpumask to be temporarily used under rq_lock */
		// cpumask_var_t		scratch_mask;

	// #if defined(CONFIG_CFS_BANDWIDTH) && defined(CONFIG_SMP)
		// call_single_data_t	cfsb_csd;
		// struct list_head	cfsb_csd_list;
	// #endif
	};

	struct rq_flags {
		ulong		flags;
		// struct pin_cookie cookie;
	// #ifdef CONFIG_SCHED_DEBUG
		// /*
		//  * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
		//  * current pin context is stashed here in case it needs to be
		//  * restored in rq_repin_lock().
		//  */
		// unsigned int clock_update_flags;
	// #endif
	};

#endif /* _LINUX_RUNQUEUE_TYPES_H_ */