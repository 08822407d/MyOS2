/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_H_
#define _LINUX_SCHEDULER_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern uint
		task_cpu(const task_s *p);

		extern void
		sched_submit_work(task_s *tsk);

		extern void
		__schedule_loop(int sched_mode);


		extern void
		__block_task(rq_s *rq, task_s *p);

	#endif

	#include "scheduler_macro.h"
	
	#if defined(SCHEDULER_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		uint
		task_cpu(const task_s *p) {
			return READ_ONCE(task_thread_info(p)->cpu);
		}

		PREFIX_STATIC_INLINE
		void
		sched_submit_work(task_s *tsk) {
			// static DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
			// uint task_flags;

			// /*
			//  * Establish LD_WAIT_CONFIG context to ensure none of the code called
			//  * will use a blocking primitive -- which would lead to recursion.
			//  */
			// lock_map_acquire_try(&sched_map);

			// task_flags = tsk->flags;
			// /*
			//  * If a worker goes to sleep, notify and ask workqueue whether it
			//  * wants to wake up a task to maintain concurrency.
			//  */
			// if (task_flags & PF_WQ_WORKER)
			// 	wq_worker_sleeping(tsk);
			// else if (task_flags & PF_IO_WORKER)
			// 	io_wq_worker_sleeping(tsk);

			// /*
			//  * spinlock and rwlock must not flush block requests.  This will
			//  * deadlock if the callback attempts to acquire a lock which is
			//  * already acquired.
			//  */
			// SCHED_WARN_ON(current->__state & TASK_RTLOCK_WAIT);

			// /*
			//  * If we are going to sleep and we have plugged IO queued,
			//  * make sure to submit it to avoid deadlocks.
			//  */
			// blk_flush_plug(tsk->plug, true);

			// lock_map_release(&sched_map);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__schedule_loop(int sched_mode) {
			do {
				preempt_disable();
				__schedule(sched_mode);
				sched_preempt_enable_no_resched();
			} while (need_resched());
		}


		PREFIX_STATIC_INLINE
		void
		__block_task(rq_s *rq, task_s *p) {
			if (p->sched_contributes_to_load)
				rq->nr_uninterruptible++;

			if (p->in_iowait) {
				atomic_inc(&rq->nr_iowait);
				// delayacct_blkio_start();
			}

			// ASSERT_EXCLUSIVE_WRITER(p->on_rq);

			// /*
			//  * The moment this write goes through, ttwu() can swoop in and migrate
			//  * this task, rendering our rq->__lock ineffective.
			//  *
			//  * __schedule()				try_to_wake_up()
			//  *   LOCK rq->__lock			  LOCK p->pi_lock
			//  *   pick_next_task()
			//  *     pick_next_task_fair()
			//  *       pick_next_entity()
			//  *         dequeue_entities()
			//  *           __block_task()
			//  *             RELEASE p->on_rq = 0	  if (p->on_rq && ...)
			//  *					    break;
			//  *
			//  *					  ACQUIRE (after ctrl-dep)
			//  *
			//  *					  cpu = select_task_rq();
			//  *					  set_task_cpu(p, cpu);
			//  *					  ttwu_queue()
			//  *					    ttwu_do_activate()
			//  *					      LOCK rq->__lock
			//  *					      activate_task()
			//  *					        STORE p->on_rq = 1
			//  *   UNLOCK rq->__lock
			//  *
			//  * Callers must ensure to not reference @p after this -- we no longer
			//  * own it.
			//  */
			// smp_store_release(&p->on_rq, 0);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SCHEDULER_H_ */