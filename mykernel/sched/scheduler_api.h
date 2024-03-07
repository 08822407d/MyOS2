/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_API_H_
#define _LINUX_SCHEDULER_API_H_

    #include "scheduler/scheduler_const.h"
    #include "scheduler/scheduler_types.h"

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
	extern long schedule_timeout(long timeout);
	asmlinkage void schedule(void);
	extern void schedule_preempt_disabled(void);
	extern void preempt_schedule(void);

	extern void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait);


#endif /* _LINUX_SCHEDULER_API_H_ */