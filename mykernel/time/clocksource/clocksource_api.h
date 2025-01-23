/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_API_H_
#define _LINUX_CLOCKSOURCE_API_H_

	#include "clocksource.h"
	#include <linux/init/init.h>


	// extern int clocksource_unregister(clocksrc_s*);
	// extern void clocksource_touch_watchdog(void);
	// extern void clocksource_change_rating(clocksrc_s *cs, int rating);
	// extern void clocksource_suspend(void);
	// extern void clocksource_resume(void);
	extern clocksrc_s * __init clocksource_default_clock(void);
	// extern void clocksource_mark_unstable(clocksrc_s *cs);
	// extern void
	// clocksource_start_suspend_timing(clocksrc_s *cs, u64 start_cycles);
	// extern u64 clocksource_stop_suspend_timing(clocksrc_s *cs, u64 now);

	extern u64 clocks_calc_max_nsecs(u32 mult, u32 shift, u32 maxadj, u64 mask, u64 *max_cycles);
	extern void clocks_calc_mult_shift(u32 *mult, u32 *shift, u32 from, u32 to, u32 minsec);

	/*
	 * Don't call __clocksource_register_scale directly, use
	 * clocksource_register_hz/khz
	 */
	extern int
	__clocksource_register_scale(clocksrc_s *cs, u32 scale, u32 freq);
	extern void
	__clocksource_update_freq_scale(clocksrc_s *cs, u32 scale, u32 freq);

#endif /* _LINUX_CLOCKSOURCE_API_H_ */