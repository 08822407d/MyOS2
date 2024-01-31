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
 * clocks_calc_mult_shift - calculate mult/shift factors for scaled math of clocks
 * @mult:	pointer to mult variable
 * @shift:	pointer to shift variable
 * @from:	frequency to convert from
 * @to:		frequency to convert to
 * @maxsec:	guaranteed runtime conversion range in seconds
 *
 * The function evaluates the shift/mult pair for the scaled math
 * operations of clocksources and clockevents.
 *
 * @to and @from are frequency values in HZ. For clock sources @to is
 * NSEC_PER_SEC == 1GHz and @from is the counter frequency. For clock
 * event @to is the counter frequency and @from is NSEC_PER_SEC.
 *
 * The @maxsec conversion range argument controls the time frame in
 * seconds which must be covered by the runtime conversion with the
 * calculated mult and shift factors. This guarantees that no 64bit
 * overflow happens when the input value of the conversion is
 * multiplied with the calculated mult factor. Larger ranges may
 * reduce the conversion accuracy by choosing smaller mult and shift
 * factors.
 */
void clocks_calc_mult_shift(u32 *mult, u32 *shift, u32 from, u32 to, u32 maxsec)
{
	u64 tmp;
	u32 sft, sftacc= 32;

	/*
	 * Calculate the shift factor which is limiting the conversion
	 * range:
	 */
	tmp = ((u64)maxsec * from) >> 32;
	while (tmp) {
		tmp >>=1;
		sftacc--;
	}

	/*
	 * Find the conversion shift/mult pair which has the best
	 * accuracy and fits the maxsec conversion range:
	 */
	for (sft = 32; sft > 0; sft--) {
		tmp = (u64) to << sft;
		tmp += from / 2;
		do_div(tmp, from);
		if ((tmp >> sftacc) == 0)
			break;
	}
	*mult = tmp;
	*shift = sft;
}


/**
 * clocksource_max_adjustment- Returns max adjustment amount
 * @cs:         Pointer to clocksource
 *
 */
static u32 clocksource_max_adjustment(clocksrc_s *cs)
{
	u64 ret;
	/*
	 * We won't try to correct for more than 11% adjustments (110,000 ppm),
	 */
	ret = (u64)cs->mult * 11;
	do_div(ret,100);
	return (u32)ret;
}

/**
 * clocks_calc_max_nsecs - Returns maximum nanoseconds that can be converted
 * @mult:	cycle to nanosecond multiplier
 * @shift:	cycle to nanosecond divisor (power of two)
 * @maxadj:	maximum adjustment value to mult (~11%)
 * @mask:	bitmask for two's complement subtraction of non 64 bit counters
 * @max_cyc:	maximum cycle value before potential overflow (does not include
 *		any safety margin)
 *
 * NOTE: This function includes a safety margin of 50%, in other words, we
 * return half the number of nanoseconds the hardware counter can technically
 * cover. This is done so that we can potentially detect problems caused by
 * delayed timers or bad hardware, which might result in time intervals that
 * are larger than what the math used can handle without overflows.
 */
u64 clocks_calc_max_nsecs(u32 mult, u32 shift, u32 maxadj, u64 mask, u64 *max_cyc)
{
	u64 max_nsecs, max_cycles;

	/*
	 * Calculate the maximum number of cycles that we can pass to the
	 * cyc2ns() function without overflowing a 64-bit result.
	 */
	max_cycles = ULLONG_MAX;
	do_div(max_cycles, mult+maxadj);

	/*
	 * The actual maximum number of cycles we can defer the clocksource is
	 * determined by the minimum of max_cycles and mask.
	 * Note: Here we subtract the maxadj to make sure we don't sleep for
	 * too long if there's a large negative adjustment.
	 */
	max_cycles = min(max_cycles, mask);
	max_nsecs = clocksource_cyc2ns(max_cycles, mult - maxadj, shift);

	/* return the max_cycles value as well if requested */
	if (max_cyc)
		*max_cyc = max_cycles;

	/* Return 50% of the actual maximum, so we can detect bad values */
	max_nsecs >>= 1;

	return max_nsecs;
}

/**
 * clocksource_update_max_deferment - Updates the clocksource max_idle_ns & max_cycles
 * @cs:         Pointer to clocksource to be updated
 *
 */
static inline void
clocksource_update_max_deferment(struct clocksource *cs) {
	cs->max_idle_ns = clocks_calc_max_nsecs(cs->mult, cs->shift,
						cs->maxadj, cs->mask, &cs->max_cycles);
}

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
	u64 sec;

	/*
	 * Default clocksources are *special* and self-define their mult/shift.
	 * But, you're not special, so you should specify a freq value.
	 */
	if (freq) {
		/*
		 * Calc the maximum number of seconds which we can run before
		 * wrapping around. For clocksources which have a mask > 32-bit
		 * we need to limit the max sleep time to have a good
		 * conversion precision. 10 minutes is still a reasonable
		 * amount. That results in a shift value of 24 for a
		 * clocksource with mask >= 40-bit and f >= 4GHz. That maps to
		 * ~ 0.06ppm granularity for NTP.
		 */
		sec = cs->mask;
		do_div(sec, freq);
		do_div(sec, scale);
		if (!sec)
			sec = 1;
		else if (sec > 600 && cs->mask > UINT_MAX)
			sec = 600;

		clocks_calc_mult_shift(&cs->mult, &cs->shift, freq, NSEC_PER_SEC / scale, sec * scale);
	}

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

	/*
	 * Ensure clocksources that have large 'mult' values don't overflow
	 * when adjusted.
	 */
	cs->maxadj = clocksource_max_adjustment(cs);
	while (freq && ((cs->mult + cs->maxadj < cs->mult)
		|| (cs->mult - cs->maxadj > cs->mult))) {
		cs->mult >>= 1;
		cs->shift--;
		cs->maxadj = clocksource_max_adjustment(cs);
	}

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
	unsigned long flags;

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
	return 0;
}