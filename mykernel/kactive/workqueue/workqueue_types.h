#ifndef _WORKQUEUE_TYPES_H_
#define _WORKQUEUE_TYPES_H_

	#include "../kactive_type_declaration.h"


	struct work_struct {
		atomic_long_t   data;
		List_s          entry;
		work_func_t     func;
	#ifdef CONFIG_LOCKDEP
		struct lockdep_map lockdep_map;
	#endif
	};

	/*
	 * The poor guys doing the actual heavy lifting.  All on-duty workers are
	 * either serving the manager role, on idle list or on busy hash.  For
	 * details on the locking annotation (L, I, X...), refer to workqueue.c.
	 *
	 * Only to be used in workqueue and async.
	 */
	struct worker {
		/* on idle list while idle, on busy hash table while busy */
		union {
			List_s			entry;			/* L: while idle */
			HList_s			hentry;			/* L: while busy */
		};

		work_s				*current_work;	/* K: work being processed and its */
		work_func_t			current_func;	/* K: function */
		pool_workqueue_s	*current_pwq;	/* K: pwq */
		// u64					current_at;		/* K: runtime at start or last wakeup */
		// uint				current_color;	/* K: color */

		// int					sleeping;		/* S: is worker sleeping? */

		// /* used by the scheduler to determine a worker's last known identity */
		// work_func_t		last_func;	/* K: last work's fn */

		List_hdr_s			scheduled;		/* L: scheduled works */

		task_s				*task;			/* I: worker task */
		worker_pool_s		*pool;			/* A: the associated pool */
											/* L: for rescuers */
		List_s				node;			/* A: anchored at pool->workers */
											/* A: runs through worker->node */

		// ulong				last_active;	/* K: last active timestamp */
		uint				flags;			/* L: flags */
		int					id;				/* I: worker id */

		// /*
		//  * Opaque string set with work_set_desc().  Printed out with task
		//  * dump for debugging - WARN, BUG, panic or sysrq.
		//  */
		// char				desc[WORKER_DESC_LEN];

		// /* used only by rescuers to point to the target workqueue */
		// workqueue_s			*rescue_wq;		/* I: the workqueue to rescue */
	};

	/*
	 * Structure fields follow one of the following exclusion rules.
	 *
	 * I: Modifiable by initialization/destruction paths and read-only for
	 *    everyone else.
	 *
	 * P: Preemption protected.  Disabling preemption is enough and should
	 *    only be modified and accessed from the local cpu.
	 *
	 * L: pool->lock protected.  Access with pool->lock held.
	 *
	 * LN: pool->lock and wq_node_nr_active->lock protected for writes. Either for
	 *     reads.
	 *
	 * K: Only modified by worker while holding pool->lock. Can be safely read by
	 *    self, while holding pool->lock or from IRQ context if %current is the
	 *    kworker.
	 *
	 * S: Only modified by worker self.
	 *
	 * A: wq_pool_attach_mutex protected.
	 *
	 * PL: wq_pool_mutex protected.
	 *
	 * PR: wq_pool_mutex protected for writes.  RCU protected for reads.
	 *
	 * PW: wq_pool_mutex and wq->mutex protected for writes.  Either for reads.
	 *
	 * PWR: wq_pool_mutex and wq->mutex protected for writes.  Either or
	 *      RCU for reads.
	 *
	 * WQ: wq->mutex protected.
	 *
	 * WR: wq->mutex protected for writes.  RCU protected for reads.
	 *
	 * WO: wq->mutex protected for writes. Updated with WRITE_ONCE() and can be read
	 *     with READ_ONCE() without locking.
	 *
	 * MD: wq_mayday_lock protected.
	 *
	 * WD: Used internally by the watchdog.
	 */

	/* struct worker is defined in workqueue_internal.h */
	struct worker_pool {
		spinlock_t		lock;			/* the pool lock */
		int				cpu;			/* I: the associated cpu */
		int				node;			/* I: the associated node ID */
		int				id;				/* I: pool ID */
		uint			flags;			/* L: flags */

		// ulong			watchdog_ts;	/* L: watchdog timestamp */
		// bool			cpu_stall;		/* WD: stalled cpu bound pool */

		// /*
		//  * The counter is incremented in a process context on the associated CPU
		//  * w/ preemption disabled, and decremented or reset in the same context
		//  * but w/ pool->lock held. The readers grab pool->lock and are
		//  * guaranteed to see if the counter reached zero.
		//  */
		// int				nr_running;

		List_hdr_s		worklist;	/* L: list of pending works */

		int				nr_workers;	/* L: total number of workers */
		// int				nr_idle;	/* L: currently idle workers */

		List_hdr_s		idle_list;	/* L: list of idle workers */
		// struct timer_list	idle_timer;	/* L: worker idle timeout */
		// struct work_struct      idle_cull_work; /* L: worker idle cleanup */

		// struct timer_list	mayday_timer;	  /* L: SOS timer for workers */

		// /* a workers is either on busy_hash or idle_list, or the manager */
		// DECLARE_HASHTABLE(busy_hash, BUSY_WORKER_HASH_ORDER);
		// 					/* L: hash of busy workers */

		// struct worker		*manager;	/* L: purely informational */
		List_hdr_s		workers;	/* A: attached workers */

		// struct ida		worker_ida;	/* worker IDs for task name */

		// struct workqueue_attrs	*attrs;		/* I: worker attributes */
		// struct hlist_node	hash_node;	/* PL: unbound_pool_hash node */
		// int			refcnt;		/* PL: refcnt for unbound pools */

		// /*
		//  * Destruction of pool is RCU protected to allow dereferences
		//  * from get_work_pool().
		//  */
		// struct rcu_head		rcu;
	};

	/*
	 * The per-pool workqueue.  While queued, bits below WORK_PWQ_SHIFT
	 * of work_struct->data are used for flags and the remaining high bits
	 * point to the pwq; thus, pwqs need to be aligned at two's power of the
	 * number of flag bits.
	 */
	struct pool_workqueue {
		worker_pool_s	*pool;			/* I: the associated pool */
		workqueue_s		*wq;			/* I: the owning workqueue */
		// int				work_color;		/* L: current color */
		// int				flush_color;	/* L: flushing color */
		// int				refcnt;			/* L: reference count */
		// int				nr_in_flight[WORK_NR_COLORS];
										/* L: nr of in_flight works */
		// bool			plugged;		/* L: execution suspended */

		// /*
		//  * nr_active management and WORK_STRUCT_INACTIVE:
		//  *
		//  * When pwq->nr_active >= max_active, new work item is queued to
		//  * pwq->inactive_works instead of pool->worklist and marked with
		//  * WORK_STRUCT_INACTIVE.
		//  *
		//  * All work items marked with WORK_STRUCT_INACTIVE do not participate in
		//  * nr_active and all work items in pwq->inactive_works are marked with
		//  * WORK_STRUCT_INACTIVE. But not all WORK_STRUCT_INACTIVE work items are
		//  * in pwq->inactive_works. Some of them are ready to run in
		//  * pool->worklist or worker->scheduled. Those work itmes are only struct
		//  * wq_barrier which is used for flush_work() and should not participate
		//  * in nr_active. For non-barrier work item, it is marked with
		//  * WORK_STRUCT_INACTIVE iff it is in pwq->inactive_works.
		//  */
		// int				nr_active;	/* L: nr of active works */
		// struct list_head	inactive_works;	/* L: inactive works */
		// struct list_head	pending_node;	/* LN: node on wq_node_nr_active->pending_pwqs */
		List_s			pwqs_node;	/* WR: node on wq->pwqs */
		// struct list_head	mayday_node;	/* MD: node on wq->maydays */

		// u64				stats[PWQ_NR_STATS];

		// /*
		//  * Release of unbound pwq is punted to a kthread_worker. See put_pwq()
		//  * and pwq_release_workfn() for details. pool_workqueue itself is also
		//  * RCU protected so that the first pwq can be determined without
		//  * grabbing wq->mutex.
		//  */
		// struct kthread_work	release_work;
		// struct rcu_head		rcu;
	} __aligned(1 << WORK_STRUCT_PWQ_SHIFT);


	/*
	 * The externally visible workqueue.  It relays the issued work items to
	 * the appropriate worker_pool through its pool_workqueues.
	 */
	struct workqueue_struct {
		List_hdr_s		pwqs;		/* WR: all pwqs of this wq */
		List_s			list;		/* PR: list of all workqueues */

		// struct mutex		mutex;		/* protects this wq */
		// int			work_color;	/* WQ: current work color */
		// int			flush_color;	/* WQ: current flush color */
		// atomic_t		nr_pwqs_to_flush; /* flush in progress */
		// struct wq_flusher	*first_flusher;	/* WQ: first flusher */
		// struct list_head	flusher_queue;	/* WQ: flush waiters */
		// struct list_head	flusher_overflow; /* WQ: flush overflow list */

		// struct list_head	maydays;	/* MD: pwqs requesting rescue */
		// struct worker		*rescuer;	/* MD: rescue worker */

		// int			nr_drainers;	/* WQ: drain in progress */

		// /* See alloc_workqueue() function comment for info on min/max_active */
		// int			max_active;	/* WO: max active works */
		// int			min_active;	/* WO: min active works */
		// int			saved_max_active; /* WQ: saved max_active */
		// int			saved_min_active; /* WQ: saved min_active */

		// struct workqueue_attrs	*unbound_attrs;	/* PW: only for unbound wqs */
		// struct pool_workqueue __rcu *dfl_pwq;   /* PW: only for unbound wqs */

	// #ifdef CONFIG_SYSFS
	// 	struct wq_device	*wq_dev;	/* I: for sysfs interface */
	// #endif
	// #ifdef CONFIG_LOCKDEP
	// 	char			*lock_name;
	// 	struct lock_class_key	key;
	// 	struct lockdep_map	lockdep_map;
	// #endif
		char			name[WQ_NAME_LEN]; /* I: workqueue name */

		// /*
		//  * Destruction of workqueue_struct is RCU protected to allow walking
		//  * the workqueues list without grabbing wq_pool_mutex.
		//  * This is used to dump all workqueues from sysrq.
		//  */
		// struct rcu_head		rcu;

		/* hot fields used during command issue, aligned to cacheline */
		uint			flags ____cacheline_aligned; /* WQ: WQ_* flags */
		pool_workqueue_s __rcu * __percpu	*cpu_pwq; /* I: per-cpu pwqs */
		// struct wq_node_nr_active *node_nr_active[]; /* I: per-node nr_active */
	};


#endif /* _WORKQUEUE_TYPES_H_ */