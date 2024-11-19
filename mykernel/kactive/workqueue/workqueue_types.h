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
		bool			cpu_stall;		/* WD: stalled cpu bound pool */

		/*
		 * The counter is incremented in a process context on the associated CPU
		 * w/ preemption disabled, and decremented or reset in the same context
		 * but w/ pool->lock held. The readers grab pool->lock and are
		 * guaranteed to see if the counter reached zero.
		 */
		int				nr_running;

		List_hdr_s		worklist;	/* L: list of pending works */

		int				nr_workers;	/* L: total number of workers */
		int				nr_idle;	/* L: currently idle workers */

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


#endif /* _WORKQUEUE_TYPES_H_ */