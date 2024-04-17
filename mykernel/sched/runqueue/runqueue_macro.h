/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_MACRO_H_
#define _LINUX_RUNQUEUE_MACRO_H_

	/*
	 * Special states are those that do not use the normal wait-loop pattern. See
	 * the comment with set_special_state().
	 */
	#define is_special_task_state(state) (					\
				(state) & (__TASK_STOPPED | __TASK_TRACED |	\
				TASK_PARKED | TASK_DEAD)					\
			)

	/*
	 * set_current_state() includes a barrier so that the write of current->state
	 * is correctly serialised wrt the caller's subsequent test of whether to
	 * actually sleep:
	 *
	 *   for (;;) {
	 *	set_current_state(TASK_UNINTERRUPTIBLE);
	 *	if (CONDITION)
	 *	   break;
	 *
	 *	schedule();
	 *   }
	 *   __set_current_state(TASK_RUNNING);
	 *
	 * If the caller does not need such serialisation (because, for instance, the
	 * CONDITION test and condition change and wakeup are under the same lock) then
	 * use __set_current_state().
	 *
	 * The above is typically ordered against the wakeup, which does:
	 *
	 *   CONDITION = 1;
	 *   wake_up_state(p, TASK_UNINTERRUPTIBLE);
	 *
	 * where wake_up_state()/try_to_wake_up() executes a full memory barrier before
	 * accessing p->state.
	 *
	 * Wakeup will do: if (@state & p->state) p->state = TASK_RUNNING, that is,
	 * once it observes the TASK_UNINTERRUPTIBLE store the waking CPU can issue a
	 * TASK_RUNNING store which can collide with __set_current_state(TASK_RUNNING).
	 *
	 * However, with slightly different timing the wakeup TASK_RUNNING store can
	 * also collide with the TASK_UNINTERRUPTIBLE store. Losing that store is not
	 * a problem either because that will result in one extra go around the loop
	 * and our @cond test will save the day.
	 *
	 * Also see the comments of try_to_wake_up().
	 */
	#define __set_current_state(state_value)					\
			do {												\
				WRITE_ONCE(current->__state, (state_value));	\
			} while (0)

	#define set_current_state(val) __set_current_state(val)

	/*
	 * set_special_state() should be used for those states when the blocking task
	 * can not use the regular condition based wait-loop. In that case we must
	 * serialize against wakeups such that any possible in-flight TASK_RUNNING
	 * stores will not collide with our state change.
	 */
	// #define set_special_state(state_value)								\
	// 		do {														\
	// 			unsigned long flags; /* may shadow */					\
	// 			raw_spin_lock_irqsave(&current->pi_lock, flags);		\
	// 			debug_special_state_change((state_value));				\
	// 			WRITE_ONCE(current->__state, (state_value));			\
	// 			raw_spin_unlock_irqrestore(&current->pi_lock, flags);	\
	// 		} while (0)
	#define set_special_state(val) __set_current_state(val)


	#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
	#define this_rq()		this_cpu_ptr(&runqueues)
	// #define task_rq(p)		cpu_rq(task_cpu(p))
	#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
	// #define raw_rq()		raw_cpu_ptr(&runqueues)

#endif /* _LINUX_RUNQUEUE_MACRO_H_ */