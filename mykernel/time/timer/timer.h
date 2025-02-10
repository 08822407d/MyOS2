/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_H_
#define _LINUX_TIMER_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern int
		timer_pending(const timer_list_s * timer);

		extern void
		detach_timer(timer_list_s *timer, bool clear_pending);

		extern uint
		timer_get_idx(timer_list_s *timer);

		extern void
		timer_set_idx(timer_list_s *timer, uint idx);

		extern uint
		calc_index(ulong expires, uint lvl, ulong *bucket_expiry);

		extern timer_base_s
		*get_timer_cpu_base(u32 tflags, u32 cpu);

		extern timer_base_s
		*get_timer_this_cpu_base(u32 tflags);

		extern timer_base_s
		*get_timer_base(u32 tflags);

		extern void
		__forward_timer_base(timer_base_s *base, ulong basej);

		extern void
		forward_timer_base(timer_base_s *base);

	#endif

	#include "timer_macro.h"
	
	#if defined(TIMER_DEFINATION) || !(DEBUG)

		/**
		 * timer_pending - is a timer pending?
		 * @timer: the timer in question
		 *
		 * timer_pending will tell whether a given timer is currently pending,
		 * or not. Callers must ensure serialization wrt. other operations done
		 * to this timer, eg. interrupt contexts, or other CPUs on SMP.
		 *
		 * Returns: 1 if the timer is pending, 0 if not.
		 */
		PREFIX_STATIC_INLINE
		int
		timer_pending(const timer_list_s * timer) {
			return !hlist_unhashed_lockless(&timer->entry);
		}


		PREFIX_STATIC_INLINE
		void
		detach_timer(timer_list_s *timer, bool clear_pending) {
			HList_s *entry = &timer->entry;

			// debug_deactivate(timer);

			__hlist_del(entry);
			if (clear_pending)
				entry->pprev = NULL;
			entry->next = LIST_POISON2;
		}


		PREFIX_STATIC_INLINE
		uint
		timer_get_idx(timer_list_s *timer) {
			return (timer->flags & TIMER_ARRAYMASK) >> TIMER_ARRAYSHIFT;
		}

		PREFIX_STATIC_INLINE
		void
		timer_set_idx(timer_list_s *timer, uint idx) {
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
		}

		/*
		 * Helper function to calculate the array index for a given expiry
		 * time.
		 */
		PREFIX_STATIC_INLINE
		uint
		calc_index(ulong expires, uint lvl, ulong *bucket_expiry) {
			/*
			 * The timer wheel has to guarantee that a timer does not fire
			 * early. Early expiry can happen due to:
			 * - Timer is armed at the edge of a tick
			 * - Truncation of the expiry time in the outer wheel levels
			 *
			 * Round up with level granularity to prevent this.
			 */
			expires = (expires >> LVL_SHIFT(lvl)) + 1;
			*bucket_expiry = expires << LVL_SHIFT(lvl);
			return LVL_OFFS(lvl) + (expires & LVL_MASK);
		}

		PREFIX_STATIC_INLINE
		timer_base_s
		*get_timer_cpu_base(u32 tflags, u32 cpu) {
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = per_cpu_ptr(&timer_bases[index], cpu);
			// /*
			//  * If the timer is deferrable and NO_HZ_COMMON is set then we need
			//  * to use the deferrable base.
			//  */
			// if (IS_ENABLED(CONFIG_NO_HZ_COMMON) && (tflags & TIMER_DEFERRABLE))
			// 	base = per_cpu_ptr(&timer_bases[BASE_DEF], cpu);
			return base;
		}

		PREFIX_STATIC_INLINE
		timer_base_s
		*get_timer_this_cpu_base(u32 tflags) {
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = this_cpu_ptr(&timer_bases[index]);
			// /*
			//  * If the timer is deferrable and NO_HZ_COMMON is set then we need
			//  * to use the deferrable base.
			//  */
			// if (IS_ENABLED(CONFIG_NO_HZ_COMMON) && (tflags & TIMER_DEFERRABLE))
			// 	base = this_cpu_ptr(&timer_bases[BASE_DEF]);
			return base;
		}

		PREFIX_STATIC_INLINE
		timer_base_s
		*get_timer_base(u32 tflags) {
			return get_timer_cpu_base(tflags, tflags & TIMER_CPUMASK);
		}

		PREFIX_STATIC_INLINE
		void
		__forward_timer_base(timer_base_s *base, ulong basej) {
			/*
			 * Check whether we can forward the base. We can only do that when
			 * @basej is past base->clk otherwise we might rewind base->clk.
			 */
			if (time_before_eq(basej, base->clk))
				return;

			/*
			 * If the next expiry value is > jiffies, then we fast forward to
			 * jiffies otherwise we forward to the next expiry value.
			 */
			if (time_after(base->next_expiry, basej)) {
				base->clk = basej;
			} else {
				if (WARN_ON_ONCE(time_before(base->next_expiry, base->clk)))
					return;
				base->clk = base->next_expiry;
			}
		}

		PREFIX_STATIC_INLINE
		void
		forward_timer_base(timer_base_s *base) {
			__forward_timer_base(base, READ_ONCE(jiffies));
		}

	#endif

#endif /* _LINUX_TIMER_H_ */