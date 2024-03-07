/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_X86_PREEMPT_H_
#define __ASM_X86_PREEMPT_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "preempt_const_arch.h"

	#include <linux/kernel/cache.h>
	// #include <asm/rmwcc.h>
	#include <asm/insns.h>
	#include <asm/processor.h>


	#ifdef DEBUG

		extern int
		preempt_count(void);

		extern void
		preempt_count_set(int val);

		extern void
		set_preempt_need_resched(void);

		extern void
		clear_preempt_need_resched(void);

		extern bool
		test_preempt_need_resched(void);

		extern void
		preempt_count_add(int val);

		extern void
		preempt_count_sub(int val);

		extern bool
		should_resched(int preempt_offset);

	#endif

	#if defined(ARCH_PREEMPT_DEFINATION) || !(DEBUG)

    #  define __preempt_count_ref	(this_cpu_ptr(&pcpu_hot)->preempt_count)

		/*
		 * We mask the PREEMPT_NEED_RESCHED bit so as not to confuse all current users
		 * that think a non-zero value indicates we cannot preempt.
		 */
		PREFIX_STATIC_INLINE
		int
		preempt_count(void) {
			return (__preempt_count_ref & ~PREEMPT_NEED_RESCHED);
		}

		PREFIX_STATIC_INLINE
		void
		preempt_count_set(int val) {
			int old = __preempt_count_ref;
			__preempt_count_ref =
				((old & PREEMPT_NEED_RESCHED) | (val & ~PREEMPT_NEED_RESCHED));
		}

		/*
		 * must be macros to avoid header recursion hell
		 */
		// #define init_task_preempt_count(p) do { } while (0)

		// #define init_idle_preempt_count(p, cpu) do {					\
		// 			per_cpu(__preempt_count, (cpu)) = PREEMPT_DISABLED; \
		// 		} while (0)

		/*
		 * We fold the NEED_RESCHED bit into the preempt count such that
		 * preempt_enable() can decrement and test for needing to reschedule with a
		 * single instruction.
		 *
		 * We invert the actual bit, so that when the decrement hits 0 we know we both
		 * need to resched (the bit is cleared) and can resched (no preempt count).
		 */
		PREFIX_STATIC_INLINE
		void
		set_preempt_need_resched(void) {
			__preempt_count_ref &= ~PREEMPT_NEED_RESCHED;
		}

		PREFIX_STATIC_INLINE
		void
		clear_preempt_need_resched(void) {
			__preempt_count_ref |= PREEMPT_NEED_RESCHED;
		}

		PREFIX_STATIC_INLINE
		bool
		test_preempt_need_resched(void) {
			return !(__preempt_count_ref & PREEMPT_NEED_RESCHED);
		}

		/*
		 * The various preempt_count add/sub methods
		 */
		PREFIX_STATIC_INLINE
		void
		preempt_count_add(int val) {
			__preempt_count_ref += val;
		}

		PREFIX_STATIC_INLINE
		void
		preempt_count_sub(int val) {
			__preempt_count_ref -= val;
		}

		// /*
		//  * Because we keep PREEMPT_NEED_RESCHED set when we do _not_ need to reschedule
		//  * a decrement which hits zero means we have no preempt_count and should
		//  * reschedule.
		//  */
		// static __always_inline bool __preempt_count_dec_and_test(void) {
		// 	return GEN_UNARY_RMWcc("decl", __preempt_count, e, __percpu_arg([var]));
		// }

		/*
		 * Returns true when we need to resched and can (barring IRQ state).
		 */
		PREFIX_STATIC_INLINE
		bool
		should_resched(int preempt_offset) {
			return unlikely(__preempt_count_ref == preempt_offset);
		}

	#endif

#endif /* __ASM_X86_PREEMPT_H_ */
