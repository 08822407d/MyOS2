// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel scheduler code and related syscalls
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 */
// #define CREATE_TRACE_POINTS
// #include <trace/events/sched.h>
// #undef CREATE_TRACE_POINTS

#include <linux/sched/sched.h>

// #include <linux/nospec.h>
#include <linux/block/blkdev.h>
// #include <linux/kcov.h>
// #include <linux/scs.h>

// #include <asm/switch_to.h>
// #include <asm/tlb.h>

// #include "../workqueue_internal.h"
// #include "../../fs/io-wq.h"
// #include "../smpboot.h"

// #include "pelt.h"
// #include "smp.h"


#include <linux/sched/myos_percpu.h>
#include <obsolete/glo.h>

/*
 * Notes on Program-Order guarantees on SMP systems.
 *
 *  MIGRATION
 *
 * The basic program-order guarantee on SMP systems is that when a task [t]
 * migrates, all its activity on its old CPU [c0] happens-before any subsequent
 * execution on its new CPU [c1].
 *
 * For migration (of runnable tasks) this is provided by the following means:
 *
 *  A) UNLOCK of the rq(c0)->lock scheduling out task t
 *  B) migration for t is required to synchronize *both* rq(c0)->lock and
 *     rq(c1)->lock (if not at the same time, then in that order).
 *  C) LOCK of the rq(c1)->lock scheduling in task
 *
 * Release/acquire chaining guarantees that B happens after A and C after B.
 * Note: the CPU doing B need not be c0 or c1
 *
 * Example:
 *
 *   CPU0            CPU1            CPU2
 *
 *   LOCK rq(0)->lock
 *   sched-out X
 *   sched-in Y
 *   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(0)->lock // orders against CPU0
 *                                   dequeue X
 *                                   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(1)->lock
 *                                   enqueue X
 *                                   UNLOCK rq(1)->lock
 *
 *                   LOCK rq(1)->lock // orders against CPU2
 *                   sched-out Z
 *                   sched-in X
 *                   UNLOCK rq(1)->lock
 *
 *
 *  BLOCKING -- aka. SLEEP + WAKEUP
 *
 * For blocking we (obviously) need to provide the same guarantee as for
 * migration. However the means are completely different as there is no lock
 * chain to provide order. Instead we do:
 *
 *   1) smp_store_release(X->on_cpu, 0)   -- finish_task()
 *   2) smp_cond_load_acquire(!X->on_cpu) -- try_to_wake_up()
 *
 * Example:
 *
 *   CPU0 (schedule)  CPU1 (try_to_wake_up) CPU2 (schedule)
 *
 *   LOCK rq(0)->lock LOCK X->pi_lock
 *   dequeue X
 *   sched-out X
 *   smp_store_release(X->on_cpu, 0);
 *
 *                    smp_cond_load_acquire(&X->on_cpu, !VAL);
 *                    X->state = WAKING
 *                    set_task_cpu(X,2)
 *
 *                    LOCK rq(2)->lock
 *                    enqueue X
 *                    X->state = RUNNING
 *                    UNLOCK rq(2)->lock
 *
 *                                          LOCK rq(2)->lock // orders against CPU1
 *                                          sched-out Z
 *                                          sched-in X
 *                                          UNLOCK rq(2)->lock
 *
 *                    UNLOCK X->pi_lock
 *   UNLOCK rq(0)->lock
 *
 *
 * However, for wakeups there is a second guarantee we must provide, namely we
 * must ensure that CONDITION=1 done by the caller can not be reordered with
 * accesses to the task state; see try_to_wake_up() and set_current_state().
 */

/**
 * try_to_wake_up - wake up a thread
 * @p: the thread to be awakened
 * @state: the mask of task states that can be woken
 * @wake_flags: wake modifier flags (WF_*)
 *
 * Conceptually does:
 *
 *   If (@state & @p->state) @p->state = TASK_RUNNING.
 *
 * If the task was not queued/runnable, also place it back on a runqueue.
 *
 * This function is atomic against schedule() which would dequeue the task.
 *
 * It issues a full memory barrier before accessing @p->state, see the comment
 * with set_current_state().
 *
 * Uses p->pi_lock to serialize against concurrent wake-ups.
 *
 * Relies on p->pi_lock stabilizing:
 *  - p->sched_class
 *  - p->cpus_ptr
 *  - p->sched_task_group
 * in order to do migration, see its use of select_task_rq()/set_task_cpu().
 *
 * Tries really hard to only take one task_rq(p)->lock for performance.
 * Takes rq->lock in:
 *  - ttwu_runnable()    -- old rq, unavoidable, see comment there;
 *  - ttwu_queue()       -- new rq, for enqueue of the task;
 *  - psi_ttwu_dequeue() -- much sadness :-( accounting will kill us.
 *
 * As a consequence we race really badly with just about everything. See the
 * many memory barriers and their comments for details.
 *
 * Return: %true if @p->state changes (an actual wakeup was done),
 *	   %false otherwise.
 */
static int
try_to_wake_up(task_s *p, unsigned int state, int wake_flags)
{
	extern int myos_load_balance();
	int target_cpu_idx = myos_load_balance();
	per_cpudata_s * target_cpu_p = &percpu_data[target_cpu_idx].cpudata;
	if (p->__state != TASK_RUNNING)
	{
		p->__state = TASK_RUNNING;
		list_hdr_push(&target_cpu_p->running_lhdr, &p->tasks);
	}


// 	unsigned long flags;
// 	int cpu, success = 0;

// 	preempt_disable();
// 	if (p == current) {
// 		/*
// 		 * We're waking current, this means 'p->on_rq' and 'task_cpu(p)
// 		 * == smp_processor_id()'. Together this means we can special
// 		 * case the whole 'p->on_rq && ttwu_runnable()' case below
// 		 * without taking any locks.
// 		 *
// 		 * In particular:
// 		 *  - we rely on Program-Order guarantees for all the ordering,
// 		 *  - we're serialized against set_special_state() by virtue of
// 		 *    it disabling IRQs (this allows not taking ->pi_lock).
// 		 */
// 		if (!ttwu_state_match(p, state, &success))
// 			goto out;

// 		trace_sched_waking(p);
// 		WRITE_ONCE(p->__state, TASK_RUNNING);
// 		trace_sched_wakeup(p);
// 		goto out;
// 	}

// 	/*
// 	 * If we are going to wake up a thread waiting for CONDITION we
// 	 * need to ensure that CONDITION=1 done by the caller can not be
// 	 * reordered with p->state check below. This pairs with smp_store_mb()
// 	 * in set_current_state() that the waiting thread does.
// 	 */
// 	raw_spin_lock_irqsave(&p->pi_lock, flags);
// 	smp_mb__after_spinlock();
// 	if (!ttwu_state_match(p, state, &success))
// 		goto unlock;

// 	trace_sched_waking(p);

// 	/*
// 	 * Ensure we load p->on_rq _after_ p->state, otherwise it would
// 	 * be possible to, falsely, observe p->on_rq == 0 and get stuck
// 	 * in smp_cond_load_acquire() below.
// 	 *
// 	 * sched_ttwu_pending()			try_to_wake_up()
// 	 *   STORE p->on_rq = 1			  LOAD p->state
// 	 *   UNLOCK rq->lock
// 	 *
// 	 * __schedule() (switch to task 'p')
// 	 *   LOCK rq->lock			  smp_rmb();
// 	 *   smp_mb__after_spinlock();
// 	 *   UNLOCK rq->lock
// 	 *
// 	 * [task p]
// 	 *   STORE p->state = UNINTERRUPTIBLE	  LOAD p->on_rq
// 	 *
// 	 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
// 	 * __schedule().  See the comment for smp_mb__after_spinlock().
// 	 *
// 	 * A similar smb_rmb() lives in try_invoke_on_locked_down_task().
// 	 */
// 	smp_rmb();
// 	if (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
// 		goto unlock;

// 	/*
// 	 * Ensure we load p->on_cpu _after_ p->on_rq, otherwise it would be
// 	 * possible to, falsely, observe p->on_cpu == 0.
// 	 *
// 	 * One must be running (->on_cpu == 1) in order to remove oneself
// 	 * from the runqueue.
// 	 *
// 	 * __schedule() (switch to task 'p')	try_to_wake_up()
// 	 *   STORE p->on_cpu = 1		  LOAD p->on_rq
// 	 *   UNLOCK rq->lock
// 	 *
// 	 * __schedule() (put 'p' to sleep)
// 	 *   LOCK rq->lock			  smp_rmb();
// 	 *   smp_mb__after_spinlock();
// 	 *   STORE p->on_rq = 0			  LOAD p->on_cpu
// 	 *
// 	 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
// 	 * __schedule().  See the comment for smp_mb__after_spinlock().
// 	 *
// 	 * Form a control-dep-acquire with p->on_rq == 0 above, to ensure
// 	 * schedule()'s deactivate_task() has 'happened' and p will no longer
// 	 * care about it's own p->state. See the comment in __schedule().
// 	 */
// 	smp_acquire__after_ctrl_dep();

// 	/*
// 	 * We're doing the wakeup (@success == 1), they did a dequeue (p->on_rq
// 	 * == 0), which means we need to do an enqueue, change p->state to
// 	 * TASK_WAKING such that we can unlock p->pi_lock before doing the
// 	 * enqueue, such as ttwu_queue_wakelist().
// 	 */
// 	WRITE_ONCE(p->__state, TASK_WAKING);

// 	/*
// 	 * If the owning (remote) CPU is still in the middle of schedule() with
// 	 * this task as prev, considering queueing p on the remote CPUs wake_list
// 	 * which potentially sends an IPI instead of spinning on p->on_cpu to
// 	 * let the waker make forward progress. This is safe because IRQs are
// 	 * disabled and the IPI will deliver after on_cpu is cleared.
// 	 *
// 	 * Ensure we load task_cpu(p) after p->on_cpu:
// 	 *
// 	 * set_task_cpu(p, cpu);
// 	 *   STORE p->cpu = @cpu
// 	 * __schedule() (switch to task 'p')
// 	 *   LOCK rq->lock
// 	 *   smp_mb__after_spin_lock()		smp_cond_load_acquire(&p->on_cpu)
// 	 *   STORE p->on_cpu = 1		LOAD p->cpu
// 	 *
// 	 * to ensure we observe the correct CPU on which the task is currently
// 	 * scheduling.
// 	 */
// 	if (smp_load_acquire(&p->on_cpu) &&
// 	    ttwu_queue_wakelist(p, task_cpu(p), wake_flags | WF_ON_CPU))
// 		goto unlock;

// 	/*
// 	 * If the owning (remote) CPU is still in the middle of schedule() with
// 	 * this task as prev, wait until it's done referencing the task.
// 	 *
// 	 * Pairs with the smp_store_release() in finish_task().
// 	 *
// 	 * This ensures that tasks getting woken will be fully ordered against
// 	 * their previous state and preserve Program Order.
// 	 */
// 	smp_cond_load_acquire(&p->on_cpu, !VAL);

// 	cpu = select_task_rq(p, p->wake_cpu, wake_flags | WF_TTWU);
// 	if (task_cpu(p) != cpu) {
// 		if (p->in_iowait) {
// 			delayacct_blkio_end(p);
// 			atomic_dec(&task_rq(p)->nr_iowait);
// 		}

// 		wake_flags |= WF_MIGRATED;
// 		psi_ttwu_dequeue(p);
// 		set_task_cpu(p, cpu);
// 	}

// 	ttwu_queue(p, cpu, wake_flags);
// unlock:
// 	raw_spin_unlock_irqrestore(&p->pi_lock, flags);
// out:
// 	if (success)
// 		ttwu_stat(p, task_cpu(p), wake_flags);
// 	preempt_enable();

// 	return success;
}

// /**
//  * task_call_func - Invoke a function on task in fixed state
//  * @p: Process for which the function is to be invoked, can be @current.
//  * @func: Function to invoke.
//  * @arg: Argument to function.
//  *
//  * Fix the task in it's current state by avoiding wakeups and or rq operations
//  * and call @func(@arg) on it.  This function can use ->on_rq and task_curr()
//  * to work out what the state is, if required.  Given that @func can be invoked
//  * with a runqueue lock held, it had better be quite lightweight.
//  *
//  * Returns:
//  *   Whatever @func returns
//  */
// int task_call_func(task_s *p, task_call_f func, void *arg)
// {
// 	struct rq *rq = NULL;
// 	unsigned int state;
// 	struct rq_flags rf;
// 	int ret;

// 	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);

// 	state = READ_ONCE(p->__state);

// 	/*
// 	 * Ensure we load p->on_rq after p->__state, otherwise it would be
// 	 * possible to, falsely, observe p->on_rq == 0.
// 	 *
// 	 * See try_to_wake_up() for a longer comment.
// 	 */
// 	smp_rmb();

// 	/*
// 	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
// 	 * the task is blocked. Make sure to check @state since ttwu() can drop
// 	 * locks at the end, see ttwu_queue_wakelist().
// 	 */
// 	if (state == TASK_RUNNING || state == TASK_WAKING || p->on_rq)
// 		rq = __task_rq_lock(p, &rf);

// 	/*
// 	 * At this point the task is pinned; either:
// 	 *  - blocked and we're holding off wakeups	 (pi->lock)
// 	 *  - woken, and we're holding off enqueue	 (rq->lock)
// 	 *  - queued, and we're holding off schedule	 (rq->lock)
// 	 *  - running, and we're holding off de-schedule (rq->lock)
// 	 *
// 	 * The called function (@func) can use: task_curr(), p->on_rq and
// 	 * p->__state to differentiate between these states.
// 	 */
// 	ret = func(p, arg);

// 	if (rq)
// 		rq_unlock(rq, &rf);

// 	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
// 	return ret;
// }

/**
 * wake_up_process - Wake up a specific process
 * @p: The process to be woken up.
 *
 * Attempt to wake up the nominated process and move it to the set of runnable
 * processes.
 *
 * Return: 1 if the process was woken up, 0 if it was already running.
 *
 * This function executes a full memory barrier before accessing the task state.
 */
int wake_up_process(task_s *p) {
	return try_to_wake_up(p, TASK_NORMAL, 0);
}



int myos_load_balance()
{
	int i;
	int min_load = 99999999;
	int min_idx = 0;
	for (i = 0; i < kparam.nr_lcpu; i++)
	{
		per_cpudata_s * cpu_p = &percpu_data[i].cpudata;
		if (cpu_p->running_lhdr.count < min_load)
		{
			min_load = cpu_p->running_lhdr.count;
			min_idx = i;
		}
	}
	return min_idx;
}