/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PREEMPT_MACRO_H_
#define _LINUX_PREEMPT_MACRO_H_


	/*
	 * These macro definitions avoid redundant invocations of preempt_count()
	 * because such invocations would result in redundant loads given that
	 * preempt_count() is commonly implemented with READ_ONCE().
	 */

	#define nmi_count()	(						\
				preempt_count() & NMI_MASK		\
			)
	#define hardirq_count()	(					\
				preempt_count() & HARDIRQ_MASK	\
			)
	#define softirq_count()	(					\
				preempt_count() & SOFTIRQ_MASK	\
			)
	#define irq_count()	(							\
				preempt_count() & (NMI_MASK |		\
					HARDIRQ_MASK | SOFTIRQ_MASK)	\
			)

	/*
	 * Macros to retrieve the current execution context:
	 *
	 * in_nmi()		- We're in NMI context
	 * in_hardirq()		- We're in hard IRQ context
	 * in_serving_softirq()	- We're in softirq context
	 * in_task()		- We're in task context
	 */
	#define in_nmi()				(nmi_count())
	#define in_hardirq()			(hardirq_count())
	#define in_serving_softirq()	(softirq_count() & SOFTIRQ_OFFSET)

	#define in_task()	(							\
				!(preempt_count() & (NMI_MASK |		\
					HARDIRQ_MASK | SOFTIRQ_OFFSET))	\
			)

	/*
	 * The following macros are deprecated and should not be used in new code:
	 * in_irq()       - Obsolete version of in_hardirq()
	 * in_softirq()   - We have BH disabled, or are processing softirqs
	 * in_interrupt() - We're in NMI,IRQ,SoftIRQ context or have BH disabled
	 */
	#define in_irq()				(hardirq_count())
	#define in_softirq()			(softirq_count())
	#define in_interrupt()			(irq_count())



	/*
	 * Are we running in atomic context?  WARNING: this macro cannot
	 * always detect atomic context; in particular, it cannot know about
	 * held spinlocks in non-preemptible kernels.  Thus it should not be
	 * used in the general case to determine whether sleeping is possible.
	 * Do not use in_atomic() in driver code.
	 */
	#define in_atomic()	(				\
				preempt_count() != 0	\
			)

	/*
	 * Check whether we were atomic before we did preempt_disable():
	 * (used by the scheduler)
	 */
	#define in_atomic_preempt_off()	(						\
				preempt_count() != PREEMPT_DISABLE_OFFSET	\
			)

	extern void preempt_count_add(int val);
	extern void preempt_count_sub(int val);
	#define preempt_count_dec_and_test() ({		\
				preempt_count_sub(1);			\
				should_resched(0);				\
			})

	#define preempt_count_inc()		preempt_count_add(1)
	#define preempt_count_dec()		preempt_count_sub(1)

	#define preempt_disable()					\
			do {								\
				preempt_count_inc();			\
				barrier();						\
			} while (0)

	#define sched_preempt_enable_no_resched()	\
			do {								\
				barrier();						\
				preempt_count_dec();			\
			} while (0)

	#define preempt_enable_no_resched()	\
				sched_preempt_enable_no_resched()

	#define preemptible()	\
				(preempt_count() == 0 && !irqs_disabled())


	#define preempt_enable()						\
			do {									\
				barrier();							\
				if (preempt_count_dec_and_test())	\
					preempt_schedule();				\
			} while (0)

	// #define preempt_enable_notrace()				\
	// 		do {									\
	// 			barrier();							\
	// 			if (__preempt_count_dec_and_test())	\
	// 				__preempt_schedule_notrace();	\
	// 		} while (0)

	#define preempt_check_resched()				\
			do {								\
				if (should_resched(0))			\
					__preempt_schedule();		\
			} while (0)


	#define preempt_disable_notrace()			\
			do {								\
				preempt_count_inc();			\
				barrier();						\
			} while (0)

	#define preempt_enable_no_resched_notrace()	\
			do {								\
				barrier();						\
				preempt_count_dec();			\
			} while (0)

	#ifdef MODULE
	/*
	 * Modules have no business playing preemption tricks.
	 */
	#  undef sched_preempt_enable_no_resched
	#  undef preempt_enable_no_resched
	#  undef preempt_enable_no_resched_notrace
	#  undef preempt_check_resched
	#endif

#endif /* _LINUX_PREEMPT_MACRO_H_ */