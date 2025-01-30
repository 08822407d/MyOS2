/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMEKEEPING_API_H_
#define _LINUX_TIMEKEEPING_API_H_

	#include "timekeeping.h"

	extern int timekeeping_suspended;


	void timekeeping_init(void);
	u64 timekeeping_get_ns(const tk_readbase_s *tkr);

	extern void do_timer(ulong ticks);

	extern void read_persistent_clock64(timespec64_s *ts);

	extern void ktime_get_real_ts64(timespec64_s *ts);

	extern ktime_t ktime_get(void);


	/*
	 * timekeeping debug functions
	 */
	#ifdef CONFIG_DEBUG_FS
		extern void tk_debug_account_sleep_time(const timespec64_s *t);
	#else
	#	define tk_debug_account_sleep_time(x)
	#endif

	#ifdef CONFIG_CLOCKSOURCE_VALIDATE_LAST_CYCLE
		static inline u64
		clocksource_delta(u64 now, u64 last, u64 mask) {
			u64 ret = (now - last) & mask;

			/*
			 * Prevent time going backwards by checking the MSB of mask in
			 * the result. If set, return 0.
			 */
			return ret & ~(mask >> 1) ? 0 : ret;
		}
	#else
		static inline u64
		clocksource_delta(u64 now, u64 last, u64 mask) {
			return (now - last) & mask;
		}
	#endif

#endif /* _LINUX_TIMEKEEPING_API_H_ */