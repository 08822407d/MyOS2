/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PREEMPT_H
#define __LINUX_PREEMPT_H

	/*
	 * include/linux/preempt.h - macros for accessing and manipulating
	 * preempt_count (used for kernel preemption, interrupt count, etc.)
	 */

	#include <linux/kernel/linkage.h>
	#include <linux/lib/list.h>

	/*
	 * We put the hardirq and softirq counter into the preemption
	 * counter. The bitmask has the following meaning:
	 *
	 * - bits 0-7 are the preemption count (max preemption depth: 256)
	 * - bits 8-15 are the softirq count (max # of softirqs: 256)
	 *
	 * The hardirq count could in theory be the same as the number of
	 * interrupts in the system, but we run all interrupt handlers with
	 * interrupts disabled, so we cannot have nesting interrupts. Though
	 * there are a few palaeontologic drivers which reenable interrupts in
	 * the handler, so we need more than one bit here.
	 *
	 *         PREEMPT_MASK:	0x000000ff
	 *         SOFTIRQ_MASK:	0x0000ff00
	 *         HARDIRQ_MASK:	0x000f0000
	 *             NMI_MASK:	0x00f00000
	 * PREEMPT_NEED_RESCHED:	0x80000000
	 */
	// #define PREEMPT_BITS		8
	// #define SOFTIRQ_BITS		8
	// #define HARDIRQ_BITS		4
	// #define NMI_BITS			4

	#define PREEMPT_SHIFT		0
	// #define SOFTIRQ_SHIFT		(PREEMPT_SHIFT + PREEMPT_BITS)
	// #define HARDIRQ_SHIFT		(SOFTIRQ_SHIFT + SOFTIRQ_BITS)
	// #define NMI_SHIFT			(HARDIRQ_SHIFT + HARDIRQ_BITS)

	#define __IRQ_MASK(x)		((1UL << (x))-1)

	#define PREEMPT_MASK		(__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
	// #define SOFTIRQ_MASK		(__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
	// #define HARDIRQ_MASK		(__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)
	// #define NMI_MASK			(__IRQ_MASK(NMI_BITS)     << NMI_SHIFT)

	#define PREEMPT_OFFSET		(1UL << PREEMPT_SHIFT)
	// #define SOFTIRQ_OFFSET		(1UL << SOFTIRQ_SHIFT)
	// #define HARDIRQ_OFFSET		(1UL << HARDIRQ_SHIFT)
	// #define NMI_OFFSET			(1UL << NMI_SHIFT)

	// #define SOFTIRQ_DISABLE_OFFSET	(2 * SOFTIRQ_OFFSET)

	#define PREEMPT_DISABLED		(PREEMPT_DISABLE_OFFSET + PREEMPT_ENABLED)

	/*
	 * Disable preemption until the scheduler is running -- use an unconditional
	 * value so that it also works on !PREEMPT_COUNT kernels.
	 *
	 * Reset by start_kernel()->sched_init()->init_idle()->init_idle_preempt_count().
	 */
	#define INIT_PREEMPT_COUNT		PREEMPT_OFFSET

	/*
	 * Initial preempt_count value; reflects the preempt_count schedule invariant
	 * which states that during context switches:
	 *
	 *    preempt_count() == 2*PREEMPT_DISABLE_OFFSET
	 *
	 * Note: PREEMPT_DISABLE_OFFSET is 0 for !PREEMPT_COUNT kernels.
	 * Note: See finish_task_switch().
	 */
	#define FORK_PREEMPT_COUNT		(2*PREEMPT_DISABLE_OFFSET + PREEMPT_ENABLED)

	/* preempt_count() and related functions, depends on PREEMPT_NEED_RESCHED */
	#include <asm/preempt.h>

	// /**
	//  * interrupt_context_level - return interrupt context level
	//  *
	//  * Returns the current interrupt context level.
	//  *  0 - normal context
	//  *  1 - softirq context
	//  *  2 - hardirq context
	//  *  3 - NMI context
	//  */
	// static __always_inline unsigned char interrupt_context_level(void) {
	// 	unsigned long pc = preempt_count();
	// 	unsigned char level = 0;

	// 	level += !!(pc & (NMI_MASK));
	// 	level += !!(pc & (NMI_MASK | HARDIRQ_MASK));
	// 	level += !!(pc & (NMI_MASK | HARDIRQ_MASK | SOFTIRQ_OFFSET));

	// 	return level;
	// }

	// #define nmi_count()			(preempt_count() & NMI_MASK)
	// #define hardirq_count()		(preempt_count() & HARDIRQ_MASK)
	// #ifdef CONFIG_PREEMPT_RT
	// #	define softirq_count()	(current->softirq_disable_cnt & SOFTIRQ_MASK)
	// #else
	// #	define softirq_count()	(preempt_count() & SOFTIRQ_MASK)
	// #endif
	// #define irq_count()			(nmi_count() | hardirq_count() | softirq_count())

	// /*
	//  * Macros to retrieve the current execution context:
	//  *
	//  * in_nmi()		- We're in NMI context
	//  * in_hardirq()		- We're in hard IRQ context
	//  * in_serving_softirq()	- We're in softirq context
	//  * in_task()		- We're in task context
	//  */
	// #define in_nmi()				(nmi_count())
	// #define in_hardirq()			(hardirq_count())
	// #define in_serving_softirq()	(softirq_count() & SOFTIRQ_OFFSET)
	// #define in_task()				(!(in_nmi() | in_hardirq() | in_serving_softirq()))

	/*
	 * The following macros are deprecated and should not be used in new code:
	 * in_irq()       - Obsolete version of in_hardirq()
	 * in_softirq()   - We have BH disabled, or are processing softirqs
	 * in_interrupt() - We're in NMI,IRQ,SoftIRQ context or have BH disabled
	 */
	// #define in_irq()		(hardirq_count())
	// #define in_softirq()		(softirq_count())
	// #define in_interrupt()		(irq_count())

	/*
	 * The preempt_count offset after preempt_disable();
	 */
	#define PREEMPT_DISABLE_OFFSET	PREEMPT_OFFSET

	/*
	 * The preempt_count offset after spin_lock()
	 */
	// #if !defined(CONFIG_PREEMPT_RT)
	// #define PREEMPT_LOCK_OFFSET		PREEMPT_DISABLE_OFFSET
	// #else
	// /* Locks on RT do not disable preemption */
	// #define PREEMPT_LOCK_OFFSET		0
	// #endif

	// /*
	//  * The preempt_count offset needed for things like:
	//  *
	//  *  spin_lock_bh()
	//  *
	//  * Which need to disable both preemption (CONFIG_PREEMPT_COUNT) and
	//  * softirqs, such that unlock sequences of:
	//  *
	//  *  spin_unlock();
	//  *  local_bh_enable();
	//  *
	//  * Work as expected.
	//  */
	// #define SOFTIRQ_LOCK_OFFSET		(SOFTIRQ_DISABLE_OFFSET + PREEMPT_LOCK_OFFSET)

	/*
	 * Are we running in atomic context?  WARNING: this macro cannot
	 * always detect atomic context; in particular, it cannot know about
	 * held spinlocks in non-preemptible kernels.  Thus it should not be
	 * used in the general case to determine whether sleeping is possible.
	 * Do not use in_atomic() in driver code.
	 */
	#define in_atomic()		(preempt_count() != 0)

	/*
	 * Check whether we were atomic before we did preempt_disable():
	 * (used by the scheduler)
	 */
	#define in_atomic_preempt_off()	(preempt_count() != PREEMPT_DISABLE_OFFSET)

	extern void preempt_count_add(int val);
	extern void preempt_count_sub(int val);
	#define preempt_count_dec_and_test() ({	\
				preempt_count_sub(1);		\
				should_resched(0);			\
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
					__preempt_schedule();			\
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
	#	undef sched_preempt_enable_no_resched
	#	undef preempt_enable_no_resched
	#	undef preempt_enable_no_resched_notrace
	#	undef preempt_check_resched
	#endif


#endif /* __LINUX_PREEMPT_H */
