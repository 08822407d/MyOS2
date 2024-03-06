/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_API_H_
#define _LINUX_RUNQUEUE_API_H_

	#include <linux/kernel/linkage.h>
	#include <linux/kernel/swait.h>

	#include "runqueue/runqueue_const.h"
	#include "runqueue/runqueue_types.h"

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

	#define set_current_state __set_current_state

	#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
	#define this_rq()		this_cpu_ptr(&runqueues)
	// #define task_rq(p)		cpu_rq(task_cpu(p))
	#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
	// #define raw_rq()		raw_cpu_ptr(&runqueues)


	extern long schedule_timeout(long timeout);
	asmlinkage void schedule(void);

	extern void schedule_preempt_disabled(void);

	extern void update_rq_clock(rq_s *rq);

	/*
	 * Helper to define a sched_class instance; each one is placed in a separate
	 * section which is ordered by the linker script:
	 *
	 *   include/asm-generic/vmlinux.lds.h
	 *
	 * *CAREFUL* they are laid out in *REVERSE* order!!!
	 *
	 * Also enforce alignment on the instance, not the type, to guarantee layout.
	 */
	#define DEFINE_SCHED_CLASS(name) \
	const sched_class_s name##_sched_class \
		__aligned(__alignof__(sched_class_s)) \
			__section(".data.sched_class")

	/* Defined in include/asm-generic/vmlinux.lds.h */
	extern sched_class_s __sched_class_highest[];
	extern sched_class_s __sched_class_lowest[];

	#define for_class_range(class, _from, _to) \
		for (class = (_from); class < (_to); class++)

	#define for_each_class(class) \
		for_class_range(class, __sched_class_highest, __sched_class_lowest)

	#define sched_class_above(_a, _b)	((_a) < (_b))

	extern void schedule_idle(void);

	extern void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait);

#endif /* _LINUX_RUNQUEUE_API_H_ */