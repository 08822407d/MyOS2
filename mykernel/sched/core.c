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

#include "sched.h"

// #include <linux/nospec.h>
#include <linux/block/blkdev.h>
// #include <linux/kcov.h>
// #include <linux/scs.h>

#include <asm/switch_to.h>
// #include <asm/tlb.h>

// #include "../workqueue_internal.h"
// #include "../../fs/io-wq.h"
// #include "../smpboot.h"

// #include "pelt.h"
// #include "smp.h"


#include <linux/sched/myos_percpu.h>
#include <obsolete/glo.h>


void set_task_cpu(task_s *p, unsigned int new_cpu)
{
// #ifdef CONFIG_SCHED_DEBUG
	// unsigned int state = READ_ONCE(p->__state);

	// /*
	//  * We should never call set_task_cpu() on a blocked task,
	//  * ttwu() will sort out the placement.
	//  */
	// WARN_ON_ONCE(state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq);

	// /*
	//  * Migrating fair class task must have p->on_rq = TASK_ON_RQ_MIGRATING,
	//  * because schedstat_wait_{start,end} rebase migrating task's wait_start
	//  * time relying on p->on_rq.
	//  */
	// WARN_ON_ONCE(state == TASK_RUNNING &&
	// 	     p->sched_class == &fair_sched_class &&
	// 	     (p->on_rq && !task_on_rq_migrating(p)));

// #ifdef CONFIG_LOCKDEP
	// /*
	//  * The caller should hold either p->pi_lock or rq->lock, when changing
	//  * a task's CPU. ->pi_lock for waking tasks, rq->lock for runnable tasks.
	//  *
	//  * sched_move_task() holds both and thus holding either pins the cgroup,
	//  * see task_group().
	//  *
	//  * Furthermore, all task_rq users should acquire both locks, see
	//  * task_rq_lock().
	//  */
	// WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_lock) ||
	// 			      lockdep_is_held(__rq_lockp(task_rq(p)))));
// #endif
	// /*
	//  * Clearly, migrating tasks to offline CPUs is a fairly daft thing.
	//  */
	// WARN_ON_ONCE(!cpu_online(new_cpu));

	// WARN_ON_ONCE(is_migration_disabled(p));
// #endif

	// trace_sched_migrate_task(p, new_cpu);

	// if (task_cpu(p) != new_cpu) {
	// 	if (p->sched_class->migrate_task_rq)
	// 		p->sched_class->migrate_task_rq(p, new_cpu);
	// 	p->se.nr_migrations++;
	// 	rseq_migrate(p);
	// 	perf_event_task_migrate(p);
	// }

	// __set_task_cpu(p, new_cpu);

	per_cpudata_s *target_cpu_p = &cpudata.data;
	p->__state = TASK_RUNNING;
	if (!list_in_lhdr(&target_cpu_p->running_lhdr, &p->tasks))
		list_hdr_push(&target_cpu_p->running_lhdr, &p->tasks);
}


/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
static inline int select_task_rq(task_s *p, int cpu, int wake_flags) {
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
	unsigned long flags;
	int cpu, success = 0;

	// if (p == current) {
	// 	/*
	// 	 * We're waking current, this means 'p->on_rq' and 'task_cpu(p)
	// 	 * == smp_processor_id()'. Together this means we can special
	// 	 * case the whole 'p->on_rq && ttwu_runnable()' case below
	// 	 * without taking any locks.
	// 	 *
	// 	 * In particular:
	// 	 *  - we rely on Program-Order guarantees for all the ordering,
	// 	 *  - we're serialized against set_special_state() by virtue of
	// 	 *    it disabling IRQs (this allows not taking ->pi_lock).
	// 	 */
	// 	if (!ttwu_state_match(p, state, &success))
	// 		goto out;

	// 	trace_sched_waking(p);
	// 	WRITE_ONCE(p->__state, TASK_RUNNING);
	// 	trace_sched_wakeup(p);
	// 	goto out;
	// }

	// /*
	//  * If we are going to wake up a thread waiting for CONDITION we
	//  * need to ensure that CONDITION=1 done by the caller can not be
	//  * reordered with p->state check below. This pairs with smp_store_mb()
	//  * in set_current_state() that the waiting thread does.
	//  */
	// raw_spin_lock_irqsave(&p->pi_lock, flags);
	// smp_mb__after_spinlock();
	// if (!ttwu_state_match(p, state, &success))
	// 	goto unlock;

	// trace_sched_waking(p);

	// /*
	//  * Ensure we load p->on_rq _after_ p->state, otherwise it would
	//  * be possible to, falsely, observe p->on_rq == 0 and get stuck
	//  * in smp_cond_load_acquire() below.
	//  *
	//  * sched_ttwu_pending()			try_to_wake_up()
	//  *   STORE p->on_rq = 1			  LOAD p->state
	//  *   UNLOCK rq->lock
	//  *
	//  * __schedule() (switch to task 'p')
	//  *   LOCK rq->lock			  smp_rmb();
	//  *   smp_mb__after_spinlock();
	//  *   UNLOCK rq->lock
	//  *
	//  * [task p]
	//  *   STORE p->state = UNINTERRUPTIBLE	  LOAD p->on_rq
	//  *
	//  * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
	//  * __schedule().  See the comment for smp_mb__after_spinlock().
	//  *
	//  * A similar smb_rmb() lives in try_invoke_on_locked_down_task().
	//  */
	// smp_rmb();
	// if (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
	// 	goto unlock;

	// /*
	//  * Ensure we load p->on_cpu _after_ p->on_rq, otherwise it would be
	//  * possible to, falsely, observe p->on_cpu == 0.
	//  *
	//  * One must be running (->on_cpu == 1) in order to remove oneself
	//  * from the runqueue.
	//  *
	//  * __schedule() (switch to task 'p')	try_to_wake_up()
	//  *   STORE p->on_cpu = 1		  LOAD p->on_rq
	//  *   UNLOCK rq->lock
	//  *
	//  * __schedule() (put 'p' to sleep)
	//  *   LOCK rq->lock			  smp_rmb();
	//  *   smp_mb__after_spinlock();
	//  *   STORE p->on_rq = 0			  LOAD p->on_cpu
	//  *
	//  * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
	//  * __schedule().  See the comment for smp_mb__after_spinlock().
	//  *
	//  * Form a control-dep-acquire with p->on_rq == 0 above, to ensure
	//  * schedule()'s deactivate_task() has 'happened' and p will no longer
	//  * care about it's own p->state. See the comment in __schedule().
	//  */
	// smp_acquire__after_ctrl_dep();

	/*
	 * We're doing the wakeup (@success == 1), they did a dequeue (p->on_rq
	 * == 0), which means we need to do an enqueue, change p->state to
	 * TASK_WAKING such that we can unlock p->pi_lock before doing the
	 * enqueue, such as ttwu_queue_wakelist().
	 */
	WRITE_ONCE(p->__state, TASK_WAKING);

	// /*
	//  * If the owning (remote) CPU is still in the middle of schedule() with
	//  * this task as prev, considering queueing p on the remote CPUs wake_list
	//  * which potentially sends an IPI instead of spinning on p->on_cpu to
	//  * let the waker make forward progress. This is safe because IRQs are
	//  * disabled and the IPI will deliver after on_cpu is cleared.
	//  *
	//  * Ensure we load task_cpu(p) after p->on_cpu:
	//  *
	//  * set_task_cpu(p, cpu);
	//  *   STORE p->cpu = @cpu
	//  * __schedule() (switch to task 'p')
	//  *   LOCK rq->lock
	//  *   smp_mb__after_spin_lock()		smp_cond_load_acquire(&p->on_cpu)
	//  *   STORE p->on_cpu = 1		LOAD p->cpu
	//  *
	//  * to ensure we observe the correct CPU on which the task is currently
	//  * scheduling.
	//  */
	// if (smp_load_acquire(&p->on_cpu) &&
	//     ttwu_queue_wakelist(p, task_cpu(p), wake_flags | WF_ON_CPU))
	// 	goto unlock;

	// /*
	//  * If the owning (remote) CPU is still in the middle of schedule() with
	//  * this task as prev, wait until it's done referencing the task.
	//  *
	//  * Pairs with the smp_store_release() in finish_task().
	//  *
	//  * This ensures that tasks getting woken will be fully ordered against
	//  * their previous state and preserve Program Order.
	//  */
	// smp_cond_load_acquire(&p->on_cpu, !VAL);

	// cpu = select_task_rq(p, p->wake_cpu, wake_flags | WF_TTWU);
	cpu = select_task_rq(p, cpu, wake_flags | WF_TTWU);
	// if (task_cpu(p) != cpu) {
	// 	if (p->in_iowait) {
	// 		delayacct_blkio_end(p);
	// 		atomic_dec(&task_rq(p)->nr_iowait);
	// 	}

	// 	wake_flags |= WF_MIGRATED;
	// 	psi_ttwu_dequeue(p);
		set_task_cpu(p, cpu);
	// }

	// ttwu_queue(p, cpu, wake_flags);
// unlock:
	// raw_spin_unlock_irqrestore(&p->pi_lock, flags);
// out:
	// if (success)
	// 	ttwu_stat(p, task_cpu(p), wake_flags);

	return success;
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
int wake_up_process(task_s *p)
{
	int retval = 0;

	preempt_disable();
	retval = try_to_wake_up(p, TASK_NORMAL, 0);
	preempt_enable();

	return retval;
}

int wake_up_state(task_s *p, unsigned int state)
{
	int retval = 0;

	preempt_disable();
	retval = try_to_wake_up(p, state, 0);
	preempt_enable();

	return retval;
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup used by init_idle() too:
 */
static void __sched_fork(unsigned long clone_flags, task_s *p)
{
	// p->on_rq			= 0;

	// p->se.on_rq			= 0;
	// p->se.exec_start		= 0;
	// p->se.sum_exec_runtime		= 0;
	// p->se.prev_sum_exec_runtime	= 0;
	// p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	// INIT_LIST_HEAD(&p->se.group_node);

// #ifdef CONFIG_FAIR_GROUP_SCHED
	// p->se.cfs_rq			= NULL;
// #endif

// #ifdef CONFIG_SCHEDSTATS
	// /* Even if schedstat is disabled, there should not be garbage */
	// memset(&p->stats, 0, sizeof(p->stats));
// #endif

	// RB_CLEAR_NODE(&p->dl.rb_node);
	// init_dl_task_timer(&p->dl);
	// init_dl_inactive_task_timer(&p->dl);
	// __dl_clear_params(p);

	// INIT_LIST_HEAD(&p->rt.run_list);
	// p->rt.timeout		= 0;
	// p->rt.time_slice	= sched_rr_timeslice;
	// p->rt.on_rq		= 0;
	// p->rt.on_list		= 0;

// #ifdef CONFIG_PREEMPT_NOTIFIERS
	// INIT_HLIST_HEAD(&p->preempt_notifiers);
// #endif

// #ifdef CONFIG_COMPACTION
	// p->capture_control = NULL;
// #endif
	// init_numa_balancing(clone_flags, p);
	// p->wake_entry.u_flags = CSD_TYPE_TTWU;
	// p->migration_pending = NULL;
}


/*
 * fork()/clone()-time setup:
 */
int sched_fork(unsigned long clone_flags, task_s *p)
{
	__sched_fork(clone_flags, p);
	/*
	 * We mark the process as NEW here. This guarantees that
	 * nobody will actually run it, and a signal or other external
	 * event cannot wake it up and insert it on the runqueue either.
	 */
	p->__state = TASK_NEW;

	// /*
	//  * Make sure we do not leak PI boosting priority to the child.
	//  */
	// p->prio = current->normal_prio;

	// uclamp_fork(p);

	// /*
	//  * Revert to default priority/policy on fork if requested.
	//  */
	// if (unlikely(p->sched_reset_on_fork)) {
	// 	if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
	// 		p->policy = SCHED_NORMAL;
	// 		p->static_prio = NICE_TO_PRIO(0);
	// 		p->rt_priority = 0;
	// 	} else if (PRIO_TO_NICE(p->static_prio) < 0)
	// 		p->static_prio = NICE_TO_PRIO(0);

	// 	p->prio = p->normal_prio = p->static_prio;
	// 	set_load_weight(p, false);

	// 	/*
	// 	 * We don't need the reset flag anymore after the fork. It has
	// 	 * fulfilled its duty:
	// 	 */
	// 	p->sched_reset_on_fork = 0;
	// }

	// if (dl_prio(p->prio))
	// 	return -EAGAIN;
	// else if (rt_prio(p->prio))
	// 	p->sched_class = &rt_sched_class;
	// else
	// 	p->sched_class = &fair_sched_class;

	// init_entity_runnable_average(&p->se);


// #ifdef CONFIG_SCHED_INFO
	// if (likely(sched_info_on()))
	// 	memset(&p->sched_info, 0, sizeof(p->sched_info));
// #endif
// #if defined(CONFIG_SMP)
	// p->on_cpu = 0;
// #endif
	init_task_preempt_count(p);
	// plist_node_init(&p->pushable_tasks, MAX_PRIO);
	// RB_CLEAR_NODE(&p->pushable_dl_tasks);

	per_cpudata_s * target_cpu_p = &(cpudata.data);
	list_hdr_push(&target_cpu_p->running_lhdr, &p->tasks);

	return 0;
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
void wake_up_new_task(task_s *p)
{
	// struct rq_flags rf;
	// struct rq *rq;

	// raw_spin_lock_irqsave(&p->pi_lock, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	// p->recent_used_cpu = task_cpu(p);
	// rseq_migrate(p);
	// __set_task_cpu(p, select_task_rq(p, task_cpu(p), WF_FORK));
	// rq = __task_rq_lock(p, &rf);
	// update_rq_clock(rq);
	// post_init_entity_util_avg(p);

	// activate_task(rq, p, ENQUEUE_NOCLOCK);
	// trace_sched_wakeup_new(p);
	// check_preempt_curr(rq, p, WF_FORK);
	// if (p->sched_class->task_woken) {
	// 	/*
	// 	 * Nothing relies on rq->lock after this, so it's fine to
	// 	 * drop it.
	// 	 */
	// 	rq_unpin_lock(rq, &rf);
	// 	p->sched_class->task_woken(rq, p);
	// 	rq_repin_lock(rq, &rf);
	// }
	// task_rq_unlock(rq, p, &rf);
}


/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
void schedule_preempt_disabled(void)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}