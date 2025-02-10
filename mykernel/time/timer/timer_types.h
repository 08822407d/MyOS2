/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_TYPES_H_
#define _LINUX_TIMER_TYPES_H_

	#include "../time_type_declaration.h"


	/**
	 * struct timer_base - Per CPU timer base (number of base depends on config)
	 * @lock:		Lock protecting the timer_base
	 * @running_timer:	When expiring timers, the lock is dropped. To make
	 *			sure not to race against deleting/modifying a
	 *			currently running timer, the pointer is set to the
	 *			timer, which expires at the moment. If no timer is
	 *			running, the pointer is NULL.
	 * @expiry_lock:	PREEMPT_RT only: Lock is taken in softirq around
	 *			timer expiry callback execution and when trying to
	 *			delete a running timer and it wasn't successful in
	 *			the first glance. It prevents priority inversion
	 *			when callback was preempted on a remote CPU and a
	 *			caller tries to delete the running timer. It also
	 *			prevents a life lock, when the task which tries to
	 *			delete a timer preempted the softirq thread which
	 *			is running the timer callback function.
	 * @timer_waiters:	PREEMPT_RT only: Tells, if there is a waiter
	 *			waiting for the end of the timer callback function
	 *			execution.
	 * @clk:		clock of the timer base; is updated before enqueue
	 *			of a timer; during expiry, it is 1 offset ahead of
	 *			jiffies to avoid endless requeuing to current
	 *			jiffies
	 * @next_expiry:	expiry value of the first timer; it is updated when
	 *			finding the next timer and during enqueue; the
	 *			value is not valid, when next_expiry_recalc is set
	 * @cpu:		Number of CPU the timer base belongs to
	 * @next_expiry_recalc: States, whether a recalculation of next_expiry is
	 *			required. Value is set true, when a timer was
	 *			deleted.
	 * @is_idle:		Is set, when timer_base is idle. It is triggered by NOHZ
	 *			code. This state is only used in standard
	 *			base. Deferrable timers, which are enqueued remotely
	 *			never wake up an idle CPU. So no matter of supporting it
	 *			for this base.
	 * @timers_pending:	Is set, when a timer is pending in the base. It is only
	 *			reliable when next_expiry_recalc is not set.
	 * @pending_map:	bitmap of the timer wheel; each bit reflects a
	 *			bucket of the wheel. When a bit is set, at least a
	 *			single timer is enqueued in the related bucket.
	 * @vectors:		Array of lists; Each array member reflects a bucket
	 *			of the timer wheel. The list contains all timers
	 *			which are enqueued into a specific bucket.
	 */
	struct timer_base {
		spinlock_t		lock;
		timer_list_s	*running_timer;
	// #ifdef CONFIG_PREEMPT_RT
	// 	spinlock_t		expiry_lock;
	// 	atomic_t		timer_waiters;
	// #endif
		ulong			clk;
		ulong			next_expiry;
		uint			cpu;
		bool			next_expiry_recalc;
		bool			is_idle;
		bool			timers_pending;
		// DECLARE_BITMAP(pending_map, WHEEL_SIZE);
		// HList_s			vectors[WHEEL_SIZE];
	} ____cacheline_aligned;

	struct timer_list {
		/*
		 * All fields that change during normal runtime grouped to the
		 * same cacheline
		 */
		HList_s		entry;
		ulong		expires;
		void		(*function)(timer_list_s *);
		u32			flags;

	// #ifdef CONFIG_LOCKDEP
	// 	struct lockdep_map	lockdep_map;
	// #endif
	};

	struct k_clock {
		int	(*clock_getres)(const clockid_t which_clock,
				timespec64_s *tp);
		int	(*clock_set)(const clockid_t which_clock,
				const timespec64_s *tp);
		/* Returns the clock value in the current time namespace. */
		int	(*clock_get_timespec)(const clockid_t which_clock,
				timespec64_s *tp);
		/* Returns the clock value in the root time namespace. */
		ktime_t	(*clock_get_ktime)(const clockid_t which_clock);
		// int	(*clock_adj)(const clockid_t which_clock, struct __kernel_timex *tx);
		// int	(*timer_create)(struct k_itimer *timer);
		// int	(*nsleep)(const clockid_t which_clock, int flags,
		// 		const struct timespec64 *);
		// int	(*timer_set)(struct k_itimer *timr, int flags,
		// 			struct itimerspec64 *new_setting,
		// 			struct itimerspec64 *old_setting);
		// int	(*timer_del)(struct k_itimer *timr);
		// void	(*timer_get)(struct k_itimer *timr,
		// 			struct itimerspec64 *cur_setting);
		// void	(*timer_rearm)(struct k_itimer *timr);
		// s64	(*timer_forward)(struct k_itimer *timr, ktime_t now);
		// ktime_t	(*timer_remaining)(struct k_itimer *timr, ktime_t now);
		// int	(*timer_try_to_cancel)(struct k_itimer *timr);
		// void	(*timer_arm)(struct k_itimer *timr, ktime_t expires,
		// 			bool absolute, bool sigev_none);
		// void	(*timer_wait_running)(struct k_itimer *timr);
	};

#endif /* _LINUX_TIMER_TYPES_H_ */