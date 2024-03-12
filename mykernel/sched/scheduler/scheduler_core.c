// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel scheduler code and related syscalls
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 */
#include <linux/sched/clock.h>
#include <linux/sched/debug.h>
#include <linux/block/blkdev.h>

#include <asm/insns.h>

#include <linux/smp/smp.h>
#include <obsolete/glo.h>



void update_rq_clock(rq_s *rq)
{
	s64 delta;

	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
}



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

	rq_s *target_rq = &(per_cpu(runqueues, new_cpu));
	p->__state = TASK_RUNNING;
	if (!list_in_lhdr(&target_rq->myos.running_lhdr, &p->rt.run_list))
		list_hdr_push(&target_rq->myos.running_lhdr, &p->rt.run_list);
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
	int cpu = 0, success = 0;

	preempt_disable();
	if (p == current) {
		/*
		 * We're waking current, this means 'p->on_rq' and 'task_cpu(p)
		 * == smp_processor_id()'. Together this means we can special
		 * case the whole 'p->on_rq && ttwu_runnable()' case below
		 * without taking any locks.
		 *
		 * In particular:
		 *  - we rely on Program-Order guarantees for all the ordering,
		 *  - we're serialized against set_special_state() by virtue of
		 *    it disabling IRQs (this allows not taking ->pi_lock).
		 */
		// if (!ttwu_state_match(p, state, &success))
		// 	goto out;

		// trace_sched_waking(p);
		WRITE_ONCE(p->__state, TASK_RUNNING);
		// trace_sched_wakeup(p);
		goto out;
	}

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

	/*
	 * Ensure we load p->on_rq _after_ p->state, otherwise it would
	 * be possible to, falsely, observe p->on_rq == 0 and get stuck
	 * in smp_cond_load_acquire() below.
	 *
	 * sched_ttwu_pending()			try_to_wake_up()
	 *   STORE p->on_rq = 1			  LOAD p->state
	 *   UNLOCK rq->lock
	 *
	 * __schedule() (switch to task 'p')
	 *   LOCK rq->lock			  smp_rmb();
	 *   smp_mb__after_spinlock();
	 *   UNLOCK rq->lock
	 *
	 * [task p]
	 *   STORE p->state = UNINTERRUPTIBLE	  LOAD p->on_rq
	 *
	 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
	 * __schedule().  See the comment for smp_mb__after_spinlock().
	 *
	 * A similar smb_rmb() lives in try_invoke_on_locked_down_task().
	 */
	smp_rmb();
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
unlock:
	// raw_spin_unlock_irqrestore(&p->pi_lock, flags);
out:
	// if (success)
	// 	ttwu_stat(p, task_cpu(p), wake_flags);
	preempt_enable();

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
// 	rq_s *rq = NULL;
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
	return try_to_wake_up(p, TASK_NORMAL, 0);
}
EXPORT_SYMBOL(wake_up_process);

int wake_up_state(task_s *p, unsigned int state)
{
	return try_to_wake_up(p, state, 0);
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
	list_init(&p->rt.run_list, &p->rt);
	// p->rt.timeout		= 0;
	// p->rt.time_slice	= sched_rr_timeslice;
	// p->rt.on_rq		= 0;
	// p->rt.on_list		= 0;

// #ifdef CONFIG_PREEMPT_NOTIFIERS
	// INIT_HLIST_HEAD(&p->preempt_notifiers);
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
	p->on_cpu = 0;
	// plist_node_init(&p->pushable_tasks, MAX_PRIO);
	// RB_CLEAR_NODE(&p->pushable_dl_tasks);

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
	// rq_s *rq;
	rq_s *rq = &(per_cpu(runqueues, 0));

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

	list_hdr_push(&rq->myos.running_lhdr, &p->rt.run_list);
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
static inline void
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

/**
 * finish_task_switch - clean up after a task-switch
 * @prev: the thread we just switched away from.
 *
 * finish_task_switch must be called after the context switch, paired
 * with a prepare_task_switch call before the context switch.
 * finish_task_switch will reconcile locking set up by prepare_task_switch,
 * and do any other architecture-specific cleanup actions.
 *
 * Note that we may have delayed dropping an mm in context_switch(). If
 * so, we finish that here outside of the runqueue lock. (Doing it
 * with the lock held can cause deadlocks; see schedule() for
 * details.)
 *
 * The context switch have flipped the stack from under us and restored the
 * local variables which were saved when this task called schedule() in the
 * past. prev == current is still correct but we need to recalculate this_rq
 * because prev may have moved to another CPU.
 */
static rq_s *
finish_task_switch(task_s *prev) __releases(rq->lock) {
	rq_s *rq = this_rq();
	mm_s *mm = rq->prev_mm;
	unsigned int prev_state;

	// /*
	//  * The previous task will have left us with a preempt_count of 2
	//  * because it left us after:
	//  *
	//  *	schedule()
	//  *	  preempt_disable();			// 1
	//  *	  __schedule()
	//  *	    raw_spin_lock_irq(&rq->lock)	// 2
	//  *
	//  * Also, see FORK_PREEMPT_COUNT.
	//  */
	// if (WARN_ONCE(preempt_count() != 2*PREEMPT_DISABLE_OFFSET,
	// 		"corrupted preempt_count: %s/%d/0x%x\n",
	// 		current->comm, current->pid, preempt_count()))
	// 	preempt_count_set(FORK_PREEMPT_COUNT);

	rq->prev_mm = NULL;

	/*
	 * A task struct has one reference for the use as "current".
	 * If a task dies, then it sets TASK_DEAD in tsk->state and calls
	 * schedule one last time. The schedule call will never return, and
	 * the scheduled task must drop that reference.
	 *
	 * We must observe prev->state before clearing prev->on_cpu (in
	 * finish_task), otherwise a concurrent wakeup can get prev
	 * running on another CPU and we could rave with its RUNNING -> DEAD
	 * transition, resulting in a double drop.
	 */
	// prev_state = READ_ONCE(prev->__state);
	// vtime_task_switch(prev);
	// perf_event_task_sched_in(prev, current);
	// finish_task(prev);
	// tick_nohz_task_switch();
	// finish_lock_switch(rq);
	// finish_arch_post_lock_switch();
	// kcov_finish_switch(current);
	// /*
	//  * kmap_local_sched_out() is invoked with rq::lock held and
	//  * interrupts disabled. There is no requirement for that, but the
	//  * sched out code does not have an interrupt enabled section.
	//  * Restoring the maps on sched in does not require interrupts being
	//  * disabled either.
	//  */
	// kmap_local_sched_in();

	// fire_sched_in_preempt_notifiers(current);
	// /*
	//  * When switching through a kernel thread, the loop in
	//  * membarrier_{private,global}_expedited() may have observed that
	//  * kernel thread and not issued an IPI. It is therefore possible to
	//  * schedule between user->kernel->user threads without passing though
	//  * switch_mm(). Membarrier requires a barrier after storing to
	//  * rq->curr, before returning to userspace, so provide them here:
	//  *
	//  * - a full memory barrier for {PRIVATE,GLOBAL}_EXPEDITED, implicitly
	//  *   provided by mmdrop_lazy_tlb(),
	//  * - a sync_core for SYNC_CORE.
	//  */
	// if (mm) {
	// 	membarrier_mm_sync_core_before_usermode(mm);
	// 	mmdrop_lazy_tlb_sched(mm);
	// }

	// if (unlikely(prev_state == TASK_DEAD)) {
	// 	if (prev->sched_class->task_dead)
	// 		prev->sched_class->task_dead(prev);

	// 	/* Task is done with its stack. */
	// 	put_task_stack(prev);

	// 	put_task_struct_rcu_user(prev);
	// }

	return rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible void
schedule_tail(task_s *prev) __releases(rq->lock)
{
	/*
	 * New tasks start with FORK_PREEMPT_COUNT, see there and
	 * finish_task_switch() for details.
	 *
	 * finish_task_switch() will drop rq->lock() and lower preempt_count
	 * and the preempt_enable() will end up enabling preemption (on
	 * PREEMPT_COUNT kernels).
	 */

	// finish_task_switch(prev);
	preempt_enable();

	// if (current->set_child_tid)
	// 	put_user(task_pid_vnr(current), current->set_child_tid);

	// calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
static __always_inline rq_s *
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
	if (!next->mm) {                                // to kernel
		// enter_lazy_tlb(prev->active_mm, next);

		next->active_mm = prev->active_mm;
		// if (prev->mm)                           // from user
		// 	mmgrab_lazy_tlb(prev->active_mm);
		// else
			prev->active_mm = NULL;
	} else {                                        // to user
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

		if (!prev->mm) {                        // from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->active_mm;
			prev->active_mm = NULL;
		}
	}
#if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
	wrmsrl(MSR_IA32_SYSENTER_ESP, task_top_of_stack(next));
#endif

	// /* switch_mm_cid() requires the memory barriers above. */
	// switch_mm_cid(rq, prev, next);

	// rq->clock_update_flags &= ~(RQCF_ACT_SKIP|RQCF_REQ_SKIP);

	// prepare_lock_switch(rq, next, rf);
	rq->curr = next;

	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
	barrier();

	return finish_task_switch(prev);
}



/*
 * Print scheduling while atomic bug:
 */
static noinline void __schedule_bug(task_s *prev)
{
	// /* Save this before calling printk(), since that will clobber it */
	// unsigned long preempt_disable_ip = get_preempt_disable_ip(current);

	// if (oops_in_progress)
	// 	return;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_count());

	// debug_show_held_locks(prev);
	// print_modules();
	// if (irqs_disabled())
	// 	print_irqtrace_events(prev);
	// if (IS_ENABLED(CONFIG_DEBUG_PREEMPT)
	// 	&& in_atomic_preempt_off()) {
	// 	pr_err("Preemption disabled at:");
	// 	print_ip_sym(KERN_ERR, preempt_disable_ip);
	// }
	// check_panic_on_warn("scheduling while atomic");

	// dump_stack();
	// add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
inline void
schedule_debug(task_s *prev, bool preempt) {
// #ifdef CONFIG_SCHED_STACK_END_CHECK
// 	if (task_stack_end_corrupted(prev))
// 		panic("corrupted stack end detected inside scheduler\n");

// 	if (task_scs_end_corrupted(prev))
// 		panic("corrupted shadow stack detected inside scheduler\n");
// #endif

// #ifdef CONFIG_DEBUG_ATOMIC_SLEEP
// 	if (!preempt && READ_ONCE(prev->__state) && prev->non_block_count) {
// 		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
// 			prev->comm, prev->pid, prev->non_block_count);
// 		dump_stack();
// 		add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
// 	}
// #endif

	if (unlikely(in_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_count_set(PREEMPT_DISABLED);
	}
	// rcu_sleep_check();
	// SCHED_WARN_ON(ct_state() == CONTEXT_USER);

	// profile_hit(SCHED_PROFILING, __builtin_return_address(0));

	// schedstat_inc(this_rq()->sched_count);
}

static void put_prev_task_balance(rq_s *rq,
		task_s *prev, struct rq_flags *rf)
{
	// const sched_class_s *class;
	// /*
	//  * We must do the balancing pass before put_prev_task(), such
	//  * that when we release the rq->lock the task is in the same
	//  * state as before we took rq->lock.
	//  *
	//  * We can terminate the balance pass as soon as we know there is
	//  * a runnable task of @class priority or higher.
	//  */
	// for_class_range(class, prev->sched_class, &idle_sched_class) {
	// 	if (class->balance(rq, prev, rf))
	// 		break;
	// }

	// put_prev_task(rq, prev);
}

/*
 * Pick up the highest-prio task:
 */
static inline task_s *
__pick_next_task(rq_s *rq, task_s *prev, struct rq_flags *rf)
{
	const sched_class_s *class;
	task_s *p;

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
	// 		put_prev_task(rq, prev);
	// 		p = pick_next_task_idle(rq);
	// 	}

	// 	return p;
	// }

restart:
	// put_prev_task_balance(rq, prev, rf);

	for_each_class(class) {
		p = class->pick_next_task(rq);
		if (p)
			return p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}


#define pick_next_task	__pick_next_task

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock. Note that
 * SM_MASK_PREEMPT for !RT has all bits set, which allows the compiler to
 * optimize the AND operation out and just check for zero.
 */
#define SM_NONE				0x0
#define SM_PREEMPT			0x1
#define SM_RTLOCK_WAIT		0x2

#ifndef CONFIG_PREEMPT_RT
#	define SM_MASK_PREEMPT	(~0U)
#else
#	define SM_MASK_PREEMPT	SM_PREEMPT
#endif

/*
 * __schedule() is the main scheduler function.
 *
 * The main means of driving the scheduler and thus entering this function are:
 *
 *   1. Explicit blocking: mutex, semaphore, waitqueue, etc.
 *
 *   2. TIF_NEED_RESCHED flag is checked on interrupt and userspace return
 *      paths. For example, see arch/x86/entry_64.S.
 *
 *      To drive preemption between tasks, the scheduler sets the flag in timer
 *      interrupt handler scheduler_tick().
 *
 *   3. Wakeups don't really cause entry into schedule(). They add a
 *      task to the run-queue and that's it.
 *
 *      Now, if the new task added to the run-queue preempts the current
 *      task, then the wakeup sets TIF_NEED_RESCHED and schedule() gets
 *      called on the nearest possible occasion:
 *
 *       - If the kernel is preemptible (CONFIG_PREEMPTION=y):
 *
 *         - in syscall or exception context, at the next outmost
 *           preempt_enable(). (this might be as soon as the wake_up()'s
 *           spin_unlock()!)
 *
 *         - in IRQ context, return from interrupt-handler to
 *           preemptible context
 *
 *       - If the kernel is not preemptible (CONFIG_PREEMPTION is not set)
 *         then at the next:
 *
 *          - cond_resched() call
 *          - explicit schedule() call
 *          - return from syscall or exception to user-space
 *          - return from interrupt-handler to user-space
 *
 * WARNING: must be called with preemption disabled!
 */
static void __sched notrace
__schedule(unsigned int sched_mode) {
	task_s *prev, *next;
	unsigned long *switch_count;
	unsigned long prev_state;
	struct rq_flags rf;
	rq_s *rq;
	int cpu;

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	schedule_debug(prev, !!sched_mode);

	// if (sched_feat(HRTICK) || sched_feat(HRTICK_DL))
	// 	hrtick_clear(rq);

	local_irq_disable();
	// rcu_note_context_switch(!!sched_mode);

	/*
	 * Make sure that signal_pending_state()->signal_pending() below
	 * can't be reordered with __set_current_state(TASK_INTERRUPTIBLE)
	 * done by the caller to avoid the race with signal_wake_up():
	 *
	 * __set_current_state(@state)		signal_wake_up()
	 * schedule()				  set_tsk_thread_flag(p, TIF_SIGPENDING)
	 *					  wake_up_state(p, state)
	 *   LOCK rq->lock			    LOCK p->pi_state
	 *   smp_mb__after_spinlock()		    smp_mb__after_spinlock()
	 *     if (signal_pending_state())	    if (p->state & @state)
	 *
	 * Also, the membarrier system call requires a full memory barrier
	 * after coming from user-space, before storing to rq->curr.
	 */
	// rq_lock(rq, &rf);
	// smp_mb__after_spinlock();

	// /* Promote REQ to ACT */
	// rq->clock_update_flags <<= 1;
	update_rq_clock(rq);

	switch_count = &prev->nivcsw;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	// prev_state = READ_ONCE(prev->__state);
	// if (!(sched_mode & SM_MASK_PREEMPT) && prev_state) {
	// 	if (signal_pending_state(prev_state, prev)) {
	// 		WRITE_ONCE(prev->__state, TASK_RUNNING);
	// 	} else {
	// 		prev->sched_contributes_to_load =
	// 			(prev_state & TASK_UNINTERRUPTIBLE) &&
	// 			!(prev_state & TASK_NOLOAD) &&
	// 			!(prev_state & TASK_FROZEN);

	// 		if (prev->sched_contributes_to_load)
	// 			rq->nr_uninterruptible++;

	// 		/*
	// 		 * __schedule()			ttwu()
	// 		 *   prev_state = prev->state;    if (p->on_rq && ...)
	// 		 *   if (prev_state)		    goto out;
	// 		 *     p->on_rq = 0;		  smp_acquire__after_ctrl_dep();
	// 		 *				  p->state = TASK_WAKING
	// 		 *
	// 		 * Where __schedule() and ttwu() have matching control dependencies.
	// 		 *
	// 		 * After this, schedule() must not care about p->state any more.
	// 		 */
	// 		deactivate_task(rq, prev, DEQUEUE_SLEEP | DEQUEUE_NOCLOCK);

	// 		if (prev->in_iowait) {
	// 			atomic_inc(&rq->nr_iowait);
	// 			delayacct_blkio_start();
	// 		}
	// 	}
	// 	switch_count = &prev->nvcsw;
	// }

	next = pick_next_task(rq, prev, &rf);
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();

	if (likely(prev != next)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		// RCU_INIT_POINTER(rq->curr, next);
		/*
		 * The membarrier system call requires each architecture
		 * to have a full memory barrier after updating
		 * rq->curr, before returning to user-space.
		 *
		 * Here are the schemes providing that barrier on the
		 * various architectures:
		 * - mm ? switch_mm() : mmdrop() for x86, s390, sparc, PowerPC.
		 *   switch_mm() rely on membarrier_arch_switch_mm() on PowerPC.
		 * - finish_lock_switch() for weakly-ordered
		 *   architectures where spin_unlock is a full barrier,
		 * - switch_to() for arm64 (weakly-ordered, spin_unlock
		 *   is a RELEASE barrier),
		 */
		++*switch_count;

		// migrate_disable_switch(rq, prev);
		// psi_sched_switch(prev, next, !task_on_rq_queued(prev));

		// trace_sched_switch(sched_mode & SM_MASK_PREEMPT, prev, next, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, next, &rf);
	} else {
		// rq->clock_update_flags &= ~(RQCF_ACT_SKIP|RQCF_REQ_SKIP);

		// rq_unpin_lock(rq, &rf);
		// __balance_callbacks(rq);
		// raw_spin_rq_unlock_irq(rq);
	}

	local_irq_enable();
}

void __noreturn do_task_dead(void)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	set_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	// for (;;)
	// 	cpu_relax();
	while (1);
}

static inline void sched_submit_work(task_s *tsk)
{
	// unsigned int task_flags;

	// if (task_is_running(tsk))
	// 	return;

	// task_flags = tsk->flags;
	// /*
	//  * If a worker goes to sleep, notify and ask workqueue whether it
	//  * wants to wake up a task to maintain concurrency.
	//  */
	// if (task_flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
	// 	if (task_flags & PF_WQ_WORKER)
	// 		wq_worker_sleeping(tsk);
	// 	else
	// 		io_wq_worker_sleeping(tsk);
	// }

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
}

static void sched_update_worker(task_s *tsk)
{
	// if (tsk->flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
	// 	if (tsk->flags & PF_WQ_WORKER)
	// 		wq_worker_running(tsk);
	// 	else
	// 		io_wq_worker_running(tsk);
	// }
}

asmlinkage __visible void __sched schedule(void)
{
	task_s *tsk = current;

	sched_submit_work(tsk);
	do {
		preempt_disable();
		__schedule(SM_NONE);
		sched_preempt_enable_no_resched();
	} while (need_resched());
	sched_update_worker(tsk);
}
EXPORT_SYMBOL(schedule);

/*
 * synchronize_rcu_tasks() makes sure that no task is stuck in preempted
 * state (have scheduled out non-voluntarily) by making sure that all
 * tasks have either left the run queue or have gone into user space.
 * As idle tasks do not do either, they must not ever be preempted
 * (schedule out non-voluntarily).
 *
 * schedule_idle() is similar to schedule_preempt_disable() except that it
 * never enables preemption because it does not call sched_submit_work().
 */
void __sched schedule_idle(void)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a nop when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	do {
		__schedule(SM_NONE);
	} while (need_resched());
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

static void __sched notrace
preempt_schedule_common(void)
{
	do {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		// preempt_latency_start(1);
		__schedule(SM_PREEMPT);
		// preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		// /*
		//  * Check again in case we missed a preemption opportunity
		//  * between schedule and now.
		//  */
	} while (need_resched());
}

/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible void __sched notrace
preempt_schedule(void)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	if (likely(!preemptible()))
		return;
	preempt_schedule_common();
}
// NOKPROBE_SYMBOL(preempt_schedule);
EXPORT_SYMBOL(preempt_schedule);