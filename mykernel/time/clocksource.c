// SPDX-License-Identifier: GPL-2.0+
/*
 * This file contains the functions which manage clocksource drivers.
 *
 * Copyright (C) 2004, 2005 IBM, John Stultz (johnstul@us.ibm.com)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/device/device.h>
#include <linux/kernel/clocksource.h>
#include <linux/init/init.h>
// #include <linux/module.h>
#include <linux/kernel/sched.h> /* for spin_unlock_irq() using preempt_count() m68k */
// #include <linux/tick.h>
#include <linux/kernel/kthread.h>
// #include <linux/prandom.h>
#include <linux/kernel/cpu.h>

// #include "tick-internal.h"
#include "timekeeping_internal.h"


/**
 * __clocksource_update_freq_scale - Used update clocksource with new freq
 * @cs:		clocksource to be registered
 * @scale:	Scale factor multiplied against freq to get clocksource hz
 * @freq:	clocksource frequency (cycles per second) divided by scale
 *
 * This should only be called from the clocksource->enable() method.
 *
 * This *SHOULD NOT* be called directly! Please use the
 * __clocksource_update_freq_hz() or __clocksource_update_freq_khz() helper
 * functions.
 */
void __clocksource_update_freq_scale(clocksrc_s *cs, u32 scale, u32 freq)
{
	// u64 sec;

	// /*
	//  * Default clocksources are *special* and self-define their mult/shift.
	//  * But, you're not special, so you should specify a freq value.
	//  */
	// if (freq) {
	// 	/*
	// 	 * Calc the maximum number of seconds which we can run before
	// 	 * wrapping around. For clocksources which have a mask > 32-bit
	// 	 * we need to limit the max sleep time to have a good
	// 	 * conversion precision. 10 minutes is still a reasonable
	// 	 * amount. That results in a shift value of 24 for a
	// 	 * clocksource with mask >= 40-bit and f >= 4GHz. That maps to
	// 	 * ~ 0.06ppm granularity for NTP.
	// 	 */
	// 	sec = cs->mask;
	// 	do_div(sec, freq);
	// 	do_div(sec, scale);
	// 	if (!sec)
	// 		sec = 1;
	// 	else if (sec > 600 && cs->mask > UINT_MAX)
	// 		sec = 600;

	// 	clocks_calc_mult_shift(&cs->mult, &cs->shift, freq,
	// 			       NSEC_PER_SEC / scale, sec * scale);
	// }

	// /*
	//  * If the uncertainty margin is not specified, calculate it.
	//  * If both scale and freq are non-zero, calculate the clock
	//  * period, but bound below at 2*WATCHDOG_MAX_SKEW.  However,
	//  * if either of scale or freq is zero, be very conservative and
	//  * take the tens-of-milliseconds WATCHDOG_THRESHOLD value for the
	//  * uncertainty margin.  Allow stupidly small uncertainty margins
	//  * to be specified by the caller for testing purposes, but warn
	//  * to discourage production use of this capability.
	//  */
	// if (scale && freq && !cs->uncertainty_margin) {
	// 	cs->uncertainty_margin = NSEC_PER_SEC / (scale * freq);
	// 	if (cs->uncertainty_margin < 2 * WATCHDOG_MAX_SKEW)
	// 		cs->uncertainty_margin = 2 * WATCHDOG_MAX_SKEW;
	// } else if (!cs->uncertainty_margin) {
	// 	cs->uncertainty_margin = WATCHDOG_THRESHOLD;
	// }
	// WARN_ON_ONCE(cs->uncertainty_margin < 2 * WATCHDOG_MAX_SKEW);

	// /*
	//  * Ensure clocksources that have large 'mult' values don't overflow
	//  * when adjusted.
	//  */
	// cs->maxadj = clocksource_max_adjustment(cs);
	// while (freq && ((cs->mult + cs->maxadj < cs->mult)
	// 	|| (cs->mult - cs->maxadj > cs->mult))) {
	// 	cs->mult >>= 1;
	// 	cs->shift--;
	// 	cs->maxadj = clocksource_max_adjustment(cs);
	// }

	// /*
	//  * Only warn for *special* clocksources that self-define
	//  * their mult/shift values and don't specify a freq.
	//  */
	// WARN_ONCE(cs->mult + cs->maxadj < cs->mult,
	// 	"timekeeping: Clocksource %s might overflow on 11%% adjustment\n",
	// 	cs->name);

	// clocksource_update_max_deferment(cs);

	// pr_info("%s: mask: 0x%llx max_cycles: 0x%llx, max_idle_ns: %lld ns\n",
	// 	cs->name, cs->mask, cs->max_cycles, cs->max_idle_ns);
}

/**
 * __clocksource_register_scale - Used to install new clocksources
 * @cs:		clocksource to be registered
 * @scale:	Scale factor multiplied against freq to get clocksource hz
 * @freq:	clocksource frequency (cycles per second) divided by scale
 *
 * Returns -EBUSY if registration fails, zero otherwise.
 *
 * This *SHOULD NOT* be called directly! Please use the
 * clocksource_register_hz() or clocksource_register_khz helper functions.
 */
int __clocksource_register_scale(clocksrc_s *cs, u32 scale, u32 freq)
{
	// unsigned long flags;

	// clocksource_arch_init(cs);

	// if (WARN_ON_ONCE((unsigned int)cs->id >= CSID_MAX))
	// 	cs->id = CSID_GENERIC;
	// if (cs->vdso_clock_mode < 0 ||
	//     cs->vdso_clock_mode >= VDSO_CLOCKMODE_MAX) {
	// 	pr_warn("clocksource %s registered with invalid VDSO mode %d. Disabling VDSO support.\n",
	// 		cs->name, cs->vdso_clock_mode);
	// 	cs->vdso_clock_mode = VDSO_CLOCKMODE_NONE;
	// }

	/* Initialize mult/shift and max_idle_ns */
	__clocksource_update_freq_scale(cs, scale, freq);

	// /* Add clocksource to the clocksource list */
	// mutex_lock(&clocksource_mutex);

	// clocksource_watchdog_lock(&flags);
	// clocksource_enqueue(cs);
	// clocksource_enqueue_watchdog(cs);
	// clocksource_watchdog_unlock(&flags);

	// clocksource_select();
	// clocksource_select_watchdog(false);
	// __clocksource_suspend_select(cs);
	// mutex_unlock(&clocksource_mutex);
	// return 0;
}