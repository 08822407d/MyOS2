/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMEKEEPING_H_
#define _LINUX_TIMEKEEPING_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern void
		tk_normalize_xtime(timekeeper_s *tk);

		extern timespec64_s
		tk_xtime(const timekeeper_s *tk);

		extern void
		tk_update_sleep_time(timekeeper_s *tk, ktime_t delta);

		extern u64
		tk_clock_read(const tk_readbase_s *tkr);

	#endif

	#include "timekeeping_macro.h"
	
	#if defined(TIMEKEEPING_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		tk_normalize_xtime(timekeeper_s *tk) {
			while (tk->tkr_mono.xtime_nsec >= ((u64)NSEC_PER_SEC << tk->tkr_mono.shift)) {
				tk->tkr_mono.xtime_nsec -= (u64)NSEC_PER_SEC << tk->tkr_mono.shift;
				tk->xtime_sec++;
			}
			while (tk->tkr_raw.xtime_nsec >= ((u64)NSEC_PER_SEC << tk->tkr_raw.shift)) {
				tk->tkr_raw.xtime_nsec -= (u64)NSEC_PER_SEC << tk->tkr_raw.shift;
				tk->raw_sec++;
			}
		}

		PREFIX_STATIC_INLINE
		timespec64_s
		tk_xtime(const timekeeper_s *tk) {
			timespec64_s ts;
			ts.tv_sec = tk->xtime_sec;
			ts.tv_nsec = (long)(tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift);
			return ts;
		}

		PREFIX_STATIC_INLINE
		void
		tk_update_sleep_time(timekeeper_s *tk, ktime_t delta) {
			// tk->offs_boot = ktime_add(tk->offs_boot, delta);
			// /*
			//  * Timespec representation for VDSO update to avoid 64bit division
			//  * on every update.
			//  */
			// tk->monotonic_to_boot = ktime_to_timespec64(tk->offs_boot);
		}

		/*
		 * tk_clock_read - atomic clocksource read() helper
		 *
		 * This helper is necessary to use in the read paths because, while the
		 * seqcount ensures we don't return a bad value while structures are updated,
		 * it doesn't protect from potential crashes. There is the possibility that
		 * the tkr's clocksource may change between the read reference, and the
		 * clock reference passed to the read function.  This can cause crashes if
		 * the wrong clocksource is passed to the wrong read function.
		 * This isn't necessary to use when holding the timekeeper_lock or doing
		 * a read of the fast-timekeeper tkrs (which is protected by its own locking
		 * and update logic).
		 */
		PREFIX_STATIC_INLINE
		u64
		tk_clock_read(const tk_readbase_s *tkr) {
			clocksrc_s *clock = READ_ONCE(tkr->clock);
			return clock->read(clock);
		}

	#endif

#endif /* _LINUX_TIMEKEEPING_H_ */