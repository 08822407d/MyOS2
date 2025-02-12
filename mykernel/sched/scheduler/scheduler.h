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

		extern int
		select_task_rq(task_s *p, int cpu, int wake_flags);

		extern void
		prepare_task_switch(rq_s *rq, task_s *prev, task_s *next);

		extern rq_s *
		context_switch(rq_s *rq, task_s *prev,
				task_s *next, struct rq_flags *rf);

		extern task_s *
		__pick_next_task(rq_s *rq, task_s *prev,
				struct rq_flags *rf);

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

		/*
		 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
		 */
		PREFIX_STATIC_INLINE
		int
		select_task_rq(task_s *p, int cpu, int wake_flags) {
			// lockdep_assert_held(&p->pi_lock);

			// if (p->nr_cpus_allowed > 1 && !is_migration_disabled(p))
			// 	cpu = p->sched_class->select_task_rq(p, cpu, wake_flags);
			// else
			// 	cpu = cpumask_any(p->cpus_ptr);

			// /*
			//  * In order not to call set_task_cpu() on a blocking task we need
			//  * to rely on ttwu() to place the task on a valid ->cpus_ptr
			//  * CPU.
			//  *
			//  * Since this is common to all placement strategies, this lives here.
			//  *
			//  * [ this allows ->select_task() to simply return task_cpu(p) and
			//  *   not worry about this generic constraint ]
			//  */
			// if (unlikely(!is_cpu_allowed(p, cpu)))
			// 	cpu = select_fallback_rq(task_cpu(p), p);

			return cpu;
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

		/**
		 * prepare_task_switch - prepare to switch tasks
		 * @rq: the runqueue preparing to switch
		 * @prev: the current task that is being switched out
		 * @next: the task we are going to switch to.
		 *
		 * This is called with the rq lock held and interrupts off. It must
		 * be paired with a subsequent finish_task_switch after the context
		 * switch.
		 *
		 * prepare_task_switch sets up locking and calls architecture specific
		 * hooks.
		 */
		PREFIX_STATIC_INLINE
		void
		prepare_task_switch(rq_s *rq, task_s *prev, task_s *next) {
			// kcov_prepare_switch(prev);
			// sched_info_switch(rq, prev, next);
			// perf_event_task_sched_out(prev, next);
			// rseq_preempt(prev);
			// fire_sched_out_preempt_notifiers(prev, next);
			// kmap_local_sched_out();
			// prepare_task(next);
			// prepare_arch_switch(next);
		}

		/*
		 * context_switch - switch to the new MM and the new thread's register state.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		rq_s *
		context_switch(rq_s *rq, task_s *prev,
				task_s *next, struct rq_flags *rf) {

			prepare_task_switch(rq, prev, next);

			/*
			 * For paravirt, this is coupled with an exit in switch_to to
			 * combine the page table reload and the switch backend into
			 * one hypercall.
			 */
			// arch_start_context_switch(prev);

			/*
			 * kernel -> kernel   lazy + transfer active
			 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
			 *
			 * kernel ->   user   switch + mmdrop_lazy_tlb() active
			 *   user ->   user   switch
			 *
			 * switch_mm_cid() needs to be updated if the barriers provided
			 * by context_switch() are modified.
			 */
			if (!next->mm) {								// to kernel
				// enter_lazy_tlb(prev->active_mm, next);

				next->active_mm = prev->active_mm;
				if (prev->mm)								// from user
					// mmgrab_lazy_tlb(prev->active_mm);
					mmget(prev->active_mm);
				else
					prev->active_mm = NULL;
			} else {										// to user
				// membarrier_switch_mm(rq, prev->active_mm, next->mm);
				/*
				 * sys_membarrier() requires an smp_mb() between setting
				 * rq->curr / membarrier_switch_mm() and returning to userspace.
				 *
				 * The below provides this either through switch_mm(), or in
				 * case 'prev->active_mm == next->mm' through
				 * finish_task_switch()'s mmdrop().
				 */
				switch_mm_irqs_off(prev->active_mm, next->mm, next);
				// lru_gen_use_mm(next->mm);

				if (!prev->mm) {							// from kernel
					/* will mmdrop_lazy_tlb() in finish_task_switch(). */
					rq->prev_mm = prev->active_mm;
					prev->active_mm = NULL;
				}
			}
		// #if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
		// 	wrmsrl(MSR_IA32_SYSENTER_ESP, task_top_of_stack(next));
		// #endif

			// /* switch_mm_cid() requires the memory barriers above. */
			// switch_mm_cid(rq, prev, next);

			// prepare_lock_switch(rq, next, rf);

			/* Here we just switch the register state and the stack. */
			switch_to(prev, next, prev);
			barrier();

			return finish_task_switch(prev);
		}

		/*
		 * Pick up the highest-prio task:
		 */
		PREFIX_STATIC_INLINE
		task_s *
		__pick_next_task(rq_s *rq, task_s *prev,
				struct rq_flags *rf) {

			const sched_class_s *class;
			task_s *p;

			// rq->dl_server = NULL;

			// if (scx_enabled())
			// 	goto restart;

			// /*
			//  * Optimization: we know that if all tasks are in the fair class we can
			//  * call that function directly, but only if the @prev task wasn't of a
			//  * higher scheduling class, because otherwise those lose the
			//  * opportunity to pull in more work from other CPUs.
			//  */
			// if (likely(!sched_class_above(prev->sched_class, &fair_sched_class) &&
			// 	   rq->nr_running == rq->cfs.h_nr_running)) {

			// 	p = pick_next_task_fair(rq, prev, rf);
			// 	if (unlikely(p == RETRY_TASK))
			// 		goto restart;

			// 	/* Assume the next prioritized class is idle_sched_class */
			// 	if (!p) {
			// 		p = pick_task_idle(rq);
			// 		put_prev_set_next_task(rq, prev, p);
			// 	}

			// 	return p;
			// }

		restart:
			// put_prev_task_balance(rq, prev, rf);

			for_each_class(class) {
				// p = class->pick_next_task(rq, prev);
				// if (p)
				// 	return p;

				// if (class->pick_next_task) {
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
				// } else {
				// 	p = class->pick_task(rq);
				// 	if (p) {
				// 		put_prev_set_next_task(rq, prev, p);
				// 		return p;
				// 	}
				// }
			}

			BUG(); /* The idle class should always have a runnable task. */
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