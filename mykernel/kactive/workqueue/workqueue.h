#ifndef _LINUX_WORKQUEUE_H_
#define _LINUX_WORKQUEUE_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../kactive_const.h"
	#include "../kactive_types.h"
	#include "../kactive_api.h"

	#ifdef DEBUG

		extern pool_workqueue_s
		*work_struct_pwq(ulong data);

		extern pool_workqueue_s
		*get_work_pwq(work_s *work);

		extern bool
		need_more_worker(worker_pool_s *pool);

		extern bool
		may_start_working(worker_pool_s *pool);

		extern bool
		keep_working(worker_pool_s *pool);

		extern bool
		need_to_create_worker(worker_pool_s *pool);

		extern bool
		too_many_workers(worker_pool_s *pool);

		extern void
		worker_set_flags(worker_s *worker, uint flags);

		extern void
		worker_clr_flags(worker_s *worker, uint flags);

		extern worker_s
		*first_idle_worker(worker_pool_s *pool);

		extern void
		worker_enter_idle(worker_s *worker);

		extern void
		worker_leave_idle(worker_s *worker);



		extern bool
		queue_work(workqueue_s*wq, work_s*work);

	#endif

	#include "workqueue_macro.h"

	#if defined(WORKQUEUE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pool_workqueue_s
		*work_struct_pwq(ulong data) {
			return (pool_workqueue_s *)(data & WORK_STRUCT_PWQ_MASK);
		}

		PREFIX_STATIC_INLINE
		pool_workqueue_s
		*get_work_pwq(work_s *work) {
			ulong data = atomic_long_read(&work->data);
			if (data & WORK_STRUCT_PWQ)
				return work_struct_pwq(data);
			else
				return NULL;
		}


		/*
		 * Policy functions.  These define the policies on how the global worker
		 * pools are managed.  Unless noted otherwise, these functions assume that
		 * they're being called with pool->lock held.
		 */

		/*
		 * Need to wake up a worker?  Called from anything but currently
		 * running workers.
		 *
		 * Note that, because unbound workers never contribute to nr_running, this
		 * function will always return %true for unbound pools as long as the
		 * worklist isn't empty.
		 */
		PREFIX_STATIC_INLINE
		bool
		need_more_worker(worker_pool_s *pool) {
			// return !list_empty(&pool->worklist) && !pool->nr_running;
			return !list_header_is_empty(&pool->worklist);
		}

		/* Can I start working?  Called from busy but !running workers. */
		PREFIX_STATIC_INLINE
		bool
		may_start_working(worker_pool_s *pool) {
			// return pool->nr_idle;
		}

		/* Do I need to keep working?  Called from currently running workers. */
		PREFIX_STATIC_INLINE
		bool
		keep_working(worker_pool_s *pool) {
			// return !list_empty(&pool->worklist) && (pool->nr_running <= 1);
			return !list_header_is_empty(&pool->worklist);
		}

		/* Do we need a new worker?  Called from manager. */
		PREFIX_STATIC_INLINE
		bool
		need_to_create_worker(worker_pool_s *pool) {
			return need_more_worker(pool) && !may_start_working(pool);
		}

		/* Do we have too many workers and should some go away? */
		PREFIX_STATIC_INLINE
		bool
		too_many_workers(worker_pool_s *pool) {
			bool managing = pool->flags & POOL_MANAGER_ACTIVE;
			// int nr_idle = pool->nr_idle + managing; /* manager is considered idle */
			// int nr_busy = pool->nr_workers - nr_idle;

			// return nr_idle > 2 && (nr_idle - 2) * MAX_IDLE_WORKERS_RATIO >= nr_busy;
		}

		/**
		 * worker_set_flags - set worker flags and adjust nr_running accordingly
		 * @worker: self
		 * @flags: flags to set
		 *
		 * Set @flags in @worker->flags and adjust nr_running accordingly.
		 */
		PREFIX_STATIC_INLINE
		void
		worker_set_flags(worker_s *worker, uint flags) {
			worker_pool_s *pool = worker->pool;

			// lockdep_assert_held(&pool->lock);

			// /* If transitioning into NOT_RUNNING, adjust nr_running. */
			// if ((flags & WORKER_NOT_RUNNING) &&
			// 		!(worker->flags & WORKER_NOT_RUNNING)) {
			// 	pool->nr_running--;
			// }

			worker->flags |= flags;
		}

		/**
		 * worker_clr_flags - clear worker flags and adjust nr_running accordingly
		 * @worker: self
		 * @flags: flags to clear
		 *
		 * Clear @flags in @worker->flags and adjust nr_running accordingly.
		 */
		PREFIX_STATIC_INLINE
		void
		worker_clr_flags(worker_s *worker, uint flags) {
			worker_pool_s *pool = worker->pool;
			uint oflags = worker->flags;

			// lockdep_assert_held(&pool->lock);

			worker->flags &= ~flags;

			// /*
			//  * If transitioning out of NOT_RUNNING, increment nr_running.  Note
			//  * that the nested NOT_RUNNING is not a noop.  NOT_RUNNING is mask
			//  * of multiple flags, not a single flag.
			//  */
			// if ((flags & WORKER_NOT_RUNNING) && (oflags & WORKER_NOT_RUNNING))
			// 	if (!(worker->flags & WORKER_NOT_RUNNING))
			// 		pool->nr_running++;
		}

		/* Return the first idle worker.  Called with pool->lock held. */
		PREFIX_STATIC_INLINE
		worker_s
		*first_idle_worker(worker_pool_s *pool) {
			if (unlikely(list_header_is_empty(&pool->idle_list)))
				return NULL;
			return list_headr_first_container(&pool->idle_list, worker_s, entry);
		}

		/**
		 * worker_enter_idle - enter idle state
		 * @worker: worker which is entering idle state
		 *
		 * @worker is entering idle state.  Update stats and idle timer if
		 * necessary.
		 *
		 * LOCKING:
		 * raw_spin_lock_irq(pool->lock).
		 */
		PREFIX_STATIC_INLINE
		void
		worker_enter_idle(worker_s *worker) {
			worker_pool_s *pool = worker->pool;

			// if (WARN_ON_ONCE(worker->flags & WORKER_IDLE) ||
			// 		WARN_ON_ONCE(!list_empty(&worker->entry) &&
			// 		(worker->hentry.next || worker->hentry.pprev)))
			// 	return;

			/* can't use worker_set_flags(), also called from create_worker() */
			worker->flags |= WORKER_IDLE;
			// pool->nr_idle++;
			// worker->last_active = jiffies;

			/* idle_list is LIFO */
			list_header_add_to_tail(&pool->idle_list, &worker->entry);

			// if (too_many_workers(pool) && !timer_pending(&pool->idle_timer))
			// 	mod_timer(&pool->idle_timer, jiffies + IDLE_WORKER_TIMEOUT);

			// /* Sanity check nr_running. */
			// WARN_ON_ONCE(pool->nr_workers == pool->nr_idle && pool->nr_running);
		}

		/**
		 * worker_leave_idle - leave idle state
		 * @worker: worker which is leaving idle state
		 *
		 * @worker is leaving idle state.  Update stats.
		 *
		 * LOCKING:
		 * raw_spin_lock_irq(pool->lock).
		 */
		PREFIX_STATIC_INLINE
		void
		worker_leave_idle(worker_s *worker) {
			worker_pool_s *pool = worker->pool;

			if (WARN_ON_ONCE(!(worker->flags & WORKER_IDLE)))
				return;
			// worker_clr_flags(worker, WORKER_IDLE);
			list_header_delete_node(&pool->idle_list, &worker->entry);
		}



		/**
		 * queue_work - queue work on a workqueue
		 * @wq: workqueue to use
		 * @work: work to queue
		 *
		 * Returns %false if @work was already on a queue, %true otherwise.
		 *
		 * We queue the work to the CPU on which it was submitted, but if the CPU dies
		 * it can be processed by another CPU.
		 *
		 * Memory-ordering properties:  If it returns %true, guarantees that all stores
		 * preceding the call to queue_work() in the program order will be visible from
		 * the CPU which will execute @work by the time such work executes, e.g.,
		 *
		 * { x is initially 0 }
		 *
		 *   CPU0				CPU1
		 *
		 *   WRITE_ONCE(x, 1);			[ @work is being executed ]
		 *   r0 = queue_work(wq, work);		  r1 = READ_ONCE(x);
		 *
		 * Forbids: r0 == true && r1 == 0
		 */
		PREFIX_STATIC_INLINE
		bool
		queue_work(workqueue_s*wq, work_s*work) {
			return queue_work_on(WORK_CPU_UNBOUND, wq, work);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_WORKQUEUE_H_ */