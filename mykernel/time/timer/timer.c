#define TIMER_DEFINATION
#include "timer.h"

// SPDX-License-Identifier: GPL-2.0
/*
 *  Kernel internal timers
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  1997-01-28  Modified by Finn Arne Gangstad to make timers scale better.
 *
 *  1997-09-10  Updated NTP code according to technical memorandum Jan '96
 *              "A Kernel Model for Precision Timekeeping" by Dave Mills
 *  1998-12-24  Fixed a xtime SMP race (we need the xtime_lock rw spinlock to
 *              serialize accesses to xtime/lost_ticks).
 *                              Copyright (C) 1998  Andrea Arcangeli
 *  1999-03-10  Improved NTP compatibility by Ulrich Windl
 *  2002-05-31	Move sys_sysinfo here and make its locking sane, Robert Love
 *  2000-10-05  Implemented scalable SMP per-CPU timer handling.
 *                              Copyright (C) 2000, 2001, 2002  Ingo Molnar
 *              Designed by David S. Miller, Alexey Kuznetsov and Ingo Molnar
 */

#include <linux/sched/debug.h>
#include <linux/kernel/sched_api.h>


/*
 * Since schedule_timeout()'s timer is defined on the stack, it must store
 * the target task on the stack as well.
 */
typedef struct process_timer {
	timer_list_s	timer;
	task_s			*task;
} proc_timer_s;


__visible u64 jiffies_64 __cacheline_aligned_in_smp = INITIAL_JIFFIES;


static void
process_timeout(timer_list_s *t) {
	proc_timer_s *timeout = from_timer(timeout, t, timer);
	wake_up_process(timeout->task);
}

static int
calc_wheel_index(ulong expires,
		ulong clk, ulong *bucket_expiry) {

	ulong delta = expires - clk;
	uint idx;

	if (delta < LVL_START(1)) {
		idx = calc_index(expires, 0, bucket_expiry);
	} else if (delta < LVL_START(2)) {
		idx = calc_index(expires, 1, bucket_expiry);
	} else if (delta < LVL_START(3)) {
		idx = calc_index(expires, 2, bucket_expiry);
	} else if (delta < LVL_START(4)) {
		idx = calc_index(expires, 3, bucket_expiry);
	} else if (delta < LVL_START(5)) {
		idx = calc_index(expires, 4, bucket_expiry);
	} else if (delta < LVL_START(6)) {
		idx = calc_index(expires, 5, bucket_expiry);
	} else if (delta < LVL_START(7)) {
		idx = calc_index(expires, 6, bucket_expiry);
	} else if (LVL_DEPTH > 8 && delta < LVL_START(8)) {
		idx = calc_index(expires, 7, bucket_expiry);
	} else if ((long) delta < 0) {
		idx = clk & LVL_MASK;
		*bucket_expiry = clk;
	} else {
		/*
		 * Force expire obscene large timeouts to expire at the
		 * capacity limit of the wheel.
		 */
		if (delta >= WHEEL_TIMEOUT_CUTOFF)
			expires = clk + WHEEL_TIMEOUT_MAX;

		idx = calc_index(expires, LVL_DEPTH - 1, bucket_expiry);
	}
	return idx;
}


static void
trigger_dyntick_cpu(timer_base_s *base, timer_list_s *timer) {
	// /*
	//  * Deferrable timers do not prevent the CPU from entering dynticks and
	//  * are not taken into account on the idle/nohz_full path. An IPI when a
	//  * new deferrable timer is enqueued will wake up the remote CPU but
	//  * nothing will be done with the deferrable timer base. Therefore skip
	//  * the remote IPI for deferrable timers completely.
	//  */
	// if (!is_timers_nohz_active() || timer->flags & TIMER_DEFERRABLE)
	// 	return;

	// /*
	//  * We might have to IPI the remote CPU if the base is idle and the
	//  * timer is pinned. If it is a non pinned timer, it is only queued
	//  * on the remote CPU, when timer was running during queueing. Then
	//  * everything is handled by remote CPU anyway. If the other CPU is
	//  * on the way to idle then it can't set base->is_idle as we hold
	//  * the base lock:
	//  */
	// if (base->is_idle) {
	// 	WARN_ON_ONCE(!(timer->flags & TIMER_PINNED ||
	// 		    tick_nohz_full_cpu(base->cpu)));
	// 	wake_up_nohz_cpu(base->cpu);
	// }
}

/*
 * Enqueue the timer into the hash bucket, mark it pending in
 * the bitmap, store the index in the timer flags then wake up
 * the target CPU if needed.
 */
static void
enqueue_timer(timer_base_s *base, timer_list_s *timer,
		uint idx, ulong bucket_expiry) {

	hlist_add_head(&timer->entry, base->vectors + idx);
	// __set_bit(idx, base->pending_map);
	timer_set_idx(timer, idx);

	// trace_timer_start(timer, bucket_expiry);

	/*
	 * Check whether this is the new first expiring timer. The
	 * effective expiry time of the timer is required here
	 * (bucket_expiry) instead of timer->expires.
	 */
	if (time_before(bucket_expiry, base->next_expiry)) {
		/*
		 * Set the next expiry time and kick the CPU so it
		 * can reevaluate the wheel:
		 */
		WRITE_ONCE(base->next_expiry, bucket_expiry);
		base->timers_pending = true;
		base->next_expiry_recalc = false;
		trigger_dyntick_cpu(base, timer);
	}
}

static void
internal_add_timer(timer_base_s *base, timer_list_s *timer) {
	ulong bucket_expiry;
	uint idx = calc_wheel_index(timer->expires,
					base->clk, &bucket_expiry);
	enqueue_timer(base, timer, idx, bucket_expiry);
}


static int
detach_if_pending(timer_list_s *timer,
		timer_base_s *base, bool clear_pending) {

	uint idx = timer_get_idx(timer);

	if (!timer_pending(timer))
		return 0;

	if (hlist_is_singular_node(&timer->entry, base->vectors + idx)) {
		// __clear_bit(idx, base->pending_map);
		base->next_expiry_recalc = true;
	}

	detach_timer(timer, clear_pending);
	return 1;
}

/*
 * We are using hashed locking: Holding per_cpu(timer_bases[x]).lock means
 * that all timers which are tied to this base are locked, and the base itself
 * is locked too.
 *
 * So __run_timers/migrate_timers can safely modify all timers which could
 * be found in the base->vectors array.
 *
 * When a timer is migrating then the TIMER_MIGRATING flag is set and we need
 * to wait until the migration is done.
 */
static timer_base_s
*lock_timer_base(timer_list_s *timer, ulong *flags)
	__acquires(timer->base->lock)
{
	for (;;) {
		timer_base_s *base;
		u32 tf;

		/*
		 * We need to use READ_ONCE() here, otherwise the compiler
		 * might re-read @tf between the check for TIMER_MIGRATING
		 * and spin_lock().
		 */
		tf = READ_ONCE(timer->flags);

		if (!(tf & TIMER_MIGRATING)) {
			base = get_timer_base(tf);
			// raw_spin_lock_irqsave(&base->lock, *flags);
			spin_lock_irqsave(&base->lock, *flags);
			if (timer->flags == tf)
				return base;
			// raw_spin_unlock_irqrestore(&base->lock, *flags);
			spin_unlock_irqrestore_no_resched(&base->lock, *flags);
		}
		cpu_relax();
	}
}


static inline int
__mod_timer(timer_list_s *timer, ulong expires, uint options) {
	ulong clk = 0, flags, bucket_expiry;
	timer_base_s *base, *new_base;
	uint idx = UINT_MAX;
	int ret = 0;

	// debug_assert_init(timer);

	/*
	 * This is a common optimization triggered by the networking code - if
	 * the timer is re-modified to have the same timeout or ends up in the
	 * same array bucket then just return:
	 */
	if (!(options & MOD_TIMER_NOTPENDING) && timer_pending(timer)) {
		/*
		 * The downside of this optimization is that it can result in
		 * larger granularity than you would get from adding a new
		 * timer with this expiry.
		 */
		long diff = timer->expires - expires;

		if (!diff)
			return 1;
		if (options & MOD_TIMER_REDUCE && diff <= 0)
			return 1;

		/*
		 * We lock timer base and calculate the bucket index right
		 * here. If the timer ends up in the same bucket, then we
		 * just update the expiry time and avoid the whole
		 * dequeue/enqueue dance.
		 */
		base = lock_timer_base(timer, &flags);
		/*
		 * Has @timer been shutdown? This needs to be evaluated
		 * while holding base lock to prevent a race against the
		 * shutdown code.
		 */
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);

		if (timer_pending(timer) && (options & MOD_TIMER_REDUCE) &&
				time_before_eq(timer->expires, expires)) {
			ret = 1;
			goto out_unlock;
		}

		clk = base->clk;
		idx = calc_wheel_index(expires, clk, &bucket_expiry);

		/*
		 * Retrieve and compare the array index of the pending
		 * timer. If it matches set the expiry to the new value so a
		 * subsequent call will exit in the expires check above.
		 */
		if (idx == timer_get_idx(timer)) {
			if (!(options & MOD_TIMER_REDUCE))
				timer->expires = expires;
			else if (time_after(timer->expires, expires))
				timer->expires = expires;
			ret = 1;
			goto out_unlock;
		}
	} else {
		base = lock_timer_base(timer, &flags);
		/*
		 * Has @timer been shutdown? This needs to be evaluated
		 * while holding base lock to prevent a race against the
		 * shutdown code.
		 */
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
	}

	ret = detach_if_pending(timer, base, false);
	if (!ret && (options & MOD_TIMER_PENDING_ONLY))
		goto out_unlock;

	new_base = get_timer_this_cpu_base(timer->flags);

	if (base != new_base) {
		/*
		 * We are trying to schedule the timer on the new base.
		 * However we can't change timer's base while it is running,
		 * otherwise timer_delete_sync() can't detect that the timer's
		 * handler yet has not finished. This also guarantees that the
		 * timer is serialized wrt itself.
		 */
		if (likely(base->running_timer != timer)) {
			/* See the comment in lock_timer_base() */
			timer->flags |= TIMER_MIGRATING;

			// raw_spin_unlock(&base->lock);
			spin_unlock_no_resched(&base->lock);
			base = new_base;
			// raw_spin_lock(&base->lock);
			spin_lock(&base->lock);
			WRITE_ONCE(timer->flags,
				(timer->flags & ~TIMER_BASEMASK) | base->cpu);
			forward_timer_base(base);
		}
	}

	// debug_timer_activate(timer);

	timer->expires = expires;
	/*
	 * If 'idx' was calculated above and the base time did not advance
	 * between calculating 'idx' and possibly switching the base, only
	 * enqueue_timer() is required. Otherwise we need to (re)calculate
	 * the wheel index via internal_add_timer().
	 */
	if (idx != UINT_MAX && clk == base->clk)
		enqueue_timer(base, timer, idx, bucket_expiry);
	else
		internal_add_timer(base, timer);

out_unlock:
	// raw_spin_unlock_irqrestore(&base->lock, flags);
	spin_unlock_irqrestore(&base->lock, flags);

	return ret;
}

/**
 * mod_timer_pending - Modify a pending timer's timeout
 * @timer:	The pending timer to be modified
 * @expires:	New absolute timeout in jiffies
 *
 * mod_timer_pending() is the same for pending timers as mod_timer(), but
 * will not activate inactive timers.
 *
 * If @timer->function == NULL then the start operation is silently
 * discarded.
 *
 * Return:
 * * %0 - The timer was inactive and not modified or was in
 *	  shutdown state and the operation was discarded
 * * %1 - The timer was active and requeued to expire at @expires
 */
int mod_timer_pending(timer_list_s *timer, ulong expires)
{
	return __mod_timer(timer, expires, MOD_TIMER_PENDING_ONLY);
}
EXPORT_SYMBOL(mod_timer_pending);

/**
 * mod_timer - Modify a timer's timeout
 * @timer:	The timer to be modified
 * @expires:	New absolute timeout in jiffies
 *
 * mod_timer(timer, expires) is equivalent to:
 *
 *     del_timer(timer); timer->expires = expires; add_timer(timer);
 *
 * mod_timer() is more efficient than the above open coded sequence. In
 * case that the timer is inactive, the del_timer() part is a NOP. The
 * timer is in any case activated with the new expiry time @expires.
 *
 * Note that if there are multiple unserialized concurrent users of the
 * same timer, then mod_timer() is the only safe way to modify the timeout,
 * since add_timer() cannot modify an already running timer.
 *
 * If @timer->function == NULL then the start operation is silently
 * discarded. In this case the return value is 0 and meaningless.
 *
 * Return:
 * * %0 - The timer was inactive and started or was in shutdown
 *	  state and the operation was discarded
 * * %1 - The timer was active and requeued to expire at @expires or
 *	  the timer was active and not modified because @expires did
 *	  not change the effective expiry time
 */
int mod_timer(timer_list_s *timer, ulong expires)
{
	return __mod_timer(timer, expires, 0);
}
EXPORT_SYMBOL(mod_timer);

/**
 * timer_reduce - Modify a timer's timeout if it would reduce the timeout
 * @timer:	The timer to be modified
 * @expires:	New absolute timeout in jiffies
 *
 * timer_reduce() is very similar to mod_timer(), except that it will only
 * modify an enqueued timer if that would reduce the expiration time. If
 * @timer is not enqueued it starts the timer.
 *
 * If @timer->function == NULL then the start operation is silently
 * discarded.
 *
 * Return:
 * * %0 - The timer was inactive and started or was in shutdown
 *	  state and the operation was discarded
 * * %1 - The timer was active and requeued to expire at @expires or
 *	  the timer was active and not modified because @expires
 *	  did not change the effective expiry time such that the
 *	  timer would expire earlier than already scheduled
 */
int timer_reduce(timer_list_s *timer, ulong expires)
{
	return __mod_timer(timer, expires, MOD_TIMER_REDUCE);
}
EXPORT_SYMBOL(timer_reduce);

/**
 * add_timer - Start a timer
 * @timer:	The timer to be started
 *
 * Start @timer to expire at @timer->expires in the future. @timer->expires
 * is the absolute expiry time measured in 'jiffies'. When the timer expires
 * timer->function(timer) will be invoked from soft interrupt context.
 *
 * The @timer->expires and @timer->function fields must be set prior
 * to calling this function.
 *
 * If @timer->function == NULL then the start operation is silently
 * discarded.
 *
 * If @timer->expires is already in the past @timer will be queued to
 * expire at the next timer tick.
 *
 * This can only operate on an inactive timer. Attempts to invoke this on
 * an active timer are rejected with a warning.
 */
void add_timer(timer_list_s *timer)
{
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
}
EXPORT_SYMBOL(add_timer);

/**
 * add_timer_local() - Start a timer on the local CPU
 * @timer:	The timer to be started
 *
 * Same as add_timer() except that the timer flag TIMER_PINNED is set.
 *
 * See add_timer() for further details.
 */
void add_timer_local(timer_list_s *timer)
{
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
	timer->flags |= TIMER_PINNED;
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
}
EXPORT_SYMBOL(add_timer_local);

/**
 * add_timer_global() - Start a timer without TIMER_PINNED flag set
 * @timer:	The timer to be started
 *
 * Same as add_timer() except that the timer flag TIMER_PINNED is unset.
 *
 * See add_timer() for further details.
 */
void add_timer_global(timer_list_s *timer)
{
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
	timer->flags &= ~TIMER_PINNED;
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
}
EXPORT_SYMBOL(add_timer_global);



/**
 * __try_to_del_timer_sync - Internal function: Try to deactivate a timer
 * @timer:	Timer to deactivate
 * @shutdown:	If true, this indicates that the timer is about to be
 *		shutdown permanently.
 *
 * If @shutdown is true then @timer->function is set to NULL under the
 * timer base lock which prevents further rearming of the timer. Any
 * attempt to rearm @timer after this function returns will be silently
 * ignored.
 *
 * This function cannot guarantee that the timer cannot be rearmed
 * right after dropping the base lock if @shutdown is false. That
 * needs to be prevented by the calling code if necessary.
 *
 * Return:
 * * %0  - The timer was not pending
 * * %1  - The timer was pending and deactivated
 * * %-1 - The timer callback function is running on a different CPU
 */
static int
__try_to_del_timer_sync(timer_list_s *timer, bool shutdown) {
	timer_base_s *base;
	ulong flags;
	int ret = -1;

	// debug_assert_init(timer);

	base = lock_timer_base(timer, &flags);

	if (base->running_timer != timer)
		ret = detach_if_pending(timer, base, true);
	if (shutdown)
		timer->function = NULL;

	// raw_spin_unlock_irqrestore(&base->lock, flags);
	spin_unlock_irqrestore(&base->lock, flags);

	return ret;
}

/**
 * try_to_del_timer_sync - Try to deactivate a timer
 * @timer:	Timer to deactivate
 *
 * This function tries to deactivate a timer. On success the timer is not
 * queued and the timer callback function is not running on any CPU.
 *
 * This function does not guarantee that the timer cannot be rearmed right
 * after dropping the base lock. That needs to be prevented by the calling
 * code if necessary.
 *
 * Return:
 * * %0  - The timer was not pending
 * * %1  - The timer was pending and deactivated
 * * %-1 - The timer callback function is running on a different CPU
 */
int try_to_del_timer_sync(timer_list_s *timer)
{
	return __try_to_del_timer_sync(timer, false);
}
EXPORT_SYMBOL(try_to_del_timer_sync);

/**
 * __timer_delete_sync - Internal function: Deactivate a timer and wait
 *			 for the handler to finish.
 * @timer:	The timer to be deactivated
 * @shutdown:	If true, @timer->function will be set to NULL under the
 *		timer base lock which prevents rearming of @timer
 *
 * If @shutdown is not set the timer can be rearmed later. If the timer can
 * be rearmed concurrently, i.e. after dropping the base lock then the
 * return value is meaningless.
 *
 * If @shutdown is set then @timer->function is set to NULL under timer
 * base lock which prevents rearming of the timer. Any attempt to rearm
 * a shutdown timer is silently ignored.
 *
 * If the timer should be reused after shutdown it has to be initialized
 * again.
 *
 * Return:
 * * %0	- The timer was not pending
 * * %1	- The timer was pending and deactivated
 */
static int
__timer_delete_sync(timer_list_s *timer, bool shutdown) {
	int ret;

	// /*
	//  * don't use it in hardirq context, because it
	//  * could lead to deadlock.
	//  */
	// WARN_ON(in_hardirq() && !(timer->flags & TIMER_IRQSAFE));

	do {
		ret = __try_to_del_timer_sync(timer, shutdown);

		if (unlikely(ret < 0)) {
			// del_timer_wait_running(timer);
			cpu_relax();
		}
	} while (ret < 0);

	return ret;
}

/**
 * timer_delete_sync - Deactivate a timer and wait for the handler to finish.
 * @timer:	The timer to be deactivated
 *
 * Synchronization rules: Callers must prevent restarting of the timer,
 * otherwise this function is meaningless. It must not be called from
 * interrupt contexts unless the timer is an irqsafe one. The caller must
 * not hold locks which would prevent completion of the timer's callback
 * function. The timer's handler must not call add_timer_on(). Upon exit
 * the timer is not queued and the handler is not running on any CPU.
 *
 * For !irqsafe timers, the caller must not hold locks that are held in
 * interrupt context. Even if the lock has nothing to do with the timer in
 * question.  Here's why::
 *
 *    CPU0                             CPU1
 *    ----                             ----
 *                                     <SOFTIRQ>
 *                                       call_timer_fn();
 *                                       base->running_timer = mytimer;
 *    spin_lock_irq(somelock);
 *                                     <IRQ>
 *                                        spin_lock(somelock);
 *    timer_delete_sync(mytimer);
 *    while (base->running_timer == mytimer);
 *
 * Now timer_delete_sync() will never return and never release somelock.
 * The interrupt on the other CPU is waiting to grab somelock but it has
 * interrupted the softirq that CPU0 is waiting to finish.
 *
 * This function cannot guarantee that the timer is not rearmed again by
 * some concurrent or preempting code, right after it dropped the base
 * lock. If there is the possibility of a concurrent rearm then the return
 * value of the function is meaningless.
 *
 * If such a guarantee is needed, e.g. for teardown situations then use
 * timer_shutdown_sync() instead.
 *
 * Return:
 * * %0	- The timer was not pending
 * * %1	- The timer was pending and deactivated
 */
int timer_delete_sync(timer_list_s *timer)
{
	return __timer_delete_sync(timer, false);
}
EXPORT_SYMBOL(timer_delete_sync);



/**
 * init_timer_key - initialize a timer
 * @timer: the timer to be initialized
 * @func: timer callback function
 * @flags: timer flags
 * @name: name of the timer
 * @key: lockdep class key of the fake lock used for tracking timer
 *       sync lock dependencies
 *
 * init_timer_key() must be done to a timer prior to calling *any* of the
 * other timer functions.
 */
void simple_init_timer_key(timer_list_s *timer,
		void (*func)(timer_list_s *), uint flags)
{
	timer->entry.pprev = NULL;
	timer->function = func;
	if (WARN_ON_ONCE(flags & ~TIMER_INIT_FLAGS))
		flags &= TIMER_INIT_FLAGS;
	timer->flags = flags | raw_smp_processor_id();
	// lockdep_init_map(&timer->lockdep_map, name, key, 0);
}





/**
 * schedule_timeout - sleep until timeout
 * @timeout: timeout value in jiffies
 *
 * Make the current task sleep until @timeout jiffies have elapsed.
 * The function behavior depends on the current task state
 * (see also set_current_state() description):
 *
 * %TASK_RUNNING - the scheduler is called, but the task does not sleep
 * at all. That happens because sched_submit_work() does nothing for
 * tasks in %TASK_RUNNING state.
 *
 * %TASK_UNINTERRUPTIBLE - at least @timeout jiffies are guaranteed to
 * pass before the routine returns unless the current task is explicitly
 * woken up, (e.g. by wake_up_process()).
 *
 * %TASK_INTERRUPTIBLE - the routine may return early if a signal is
 * delivered to the current task or the current task is explicitly woken
 * up.
 *
 * The current task state is guaranteed to be %TASK_RUNNING when this
 * routine returns.
 *
 * Specifying a @timeout value of %MAX_SCHEDULE_TIMEOUT will schedule
 * the CPU away without a bound on the timeout. In this case the return
 * value will be %MAX_SCHEDULE_TIMEOUT.
 *
 * Returns 0 when the timer has expired otherwise the remaining time in
 * jiffies will be returned. In all cases the return value is guaranteed
 * to be non-negative.
 */
long __sched schedule_timeout(long timeout)
{
	proc_timer_s timer;
	ulong expire;

	switch (timeout)
	{
	case MAX_SCHEDULE_TIMEOUT:
		/*
		 * These two special cases are useful to be comfortable
		 * in the caller. Nothing more. We could take
		 * MAX_SCHEDULE_TIMEOUT from one of the negative value
		 * but I' d like to return a valid offset (>=0) to allow
		 * the caller to do everything it want with the retval.
		 */
		schedule();
		goto out;
	default:
		/*
		 * Another bit of PARANOID. Note that the retval will be
		 * 0 since no piece of kernel is supposed to do a check
		 * for a negative retval of schedule_timeout() (since it
		 * should never happens anyway). You just have the printk()
		 * that will tell you if something is gone wrong and where.
		 */
		if (timeout < 0) {
			// printk(KERN_ERR "schedule_timeout: wrong timeout "
			// 	"value %lx\n", timeout);
			// dump_stack();
			__set_current_state(TASK_RUNNING);
			goto out;
		}
	}

	expire = timeout + jiffies;

	timer.task = current;
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);

	/* Remove the timer from the object tracker */
	destroy_timer_on_stack(&timer.timer);

 out:
	return timeout < 0 ? 0 : timeout;
}

/*
 * We can use __set_current_state() here because schedule_timeout() calls
 * schedule() unconditionally.
 */
long __sched schedule_timeout_interruptible(long timeout)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	return schedule_timeout(timeout);
}
EXPORT_SYMBOL(schedule_timeout_interruptible);

long __sched schedule_timeout_killable(long timeout)
{
	__set_current_state(TASK_KILLABLE);
	return schedule_timeout(timeout);
}
EXPORT_SYMBOL(schedule_timeout_killable);

long __sched schedule_timeout_uninterruptible(long timeout)
{
	__set_current_state(TASK_UNINTERRUPTIBLE);
	return schedule_timeout(timeout);
}
EXPORT_SYMBOL(schedule_timeout_uninterruptible);

/*
 * Like schedule_timeout_uninterruptible(), except this task will not contribute
 * to load average.
 */
long __sched schedule_timeout_idle(long timeout)
{
	__set_current_state(TASK_IDLE);
	return schedule_timeout(timeout);
}
EXPORT_SYMBOL(schedule_timeout_idle);





/**
 * msleep - sleep safely even with waitqueue interruptions
 * @msecs: Time in milliseconds to sleep for
 */
void msleep(uint msecs)
{
	ulong timeout = msecs_to_jiffies(msecs);

	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}
EXPORT_SYMBOL(msleep);

/**
 * msleep_interruptible - sleep waiting for signals
 * @msecs: Time in milliseconds to sleep for
 */
ulong msleep_interruptible(uint msecs)
{
	ulong timeout = msecs_to_jiffies(msecs);

	while (timeout && !signal_pending(current))
		timeout = schedule_timeout_interruptible(timeout);
	return jiffies_to_msecs(timeout);
}
EXPORT_SYMBOL(msleep_interruptible);