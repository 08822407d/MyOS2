#define CLOCKSOURCE_DEFINATION
#include "clocksource.h"

#include <linux/kernel/limits.h>


/*[Clocksource internal variables]---------
 * curr_clocksource:
 *	currently selected clocksource.
 * suspend_clocksource:
 *	used to calculate the suspend time.
 * clocksource_list:
 *	linked list with the registered clocksources
 * clocksource_mutex:
 *	protects manipulations to curr_clocksource and the clocksource_list
 * override_name:
 *	Name of the user-specified clocksource.
 */
static clocksrc_s *curr_clocksource;
static clocksrc_s *suspend_clocksource;
static DECLARE_LIST_HDR_S(clocksource_list);


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
void clocks_calc_mult_shift(u32 *mult, u32 *shift,
		u32 from, u32 to, u32 maxsec)
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


static void
__clocksource_unstable(clocksrc_s *cs) {
	cs->flags &= ~(CLOCK_SOURCE_VALID_FOR_HRES | CLOCK_SOURCE_WATCHDOG);
	cs->flags |= CLOCK_SOURCE_UNSTABLE;

	/*
	 * If the clocksource is registered clocksource_watchdog_kthread() will
	 * re-rate and re-select.
	 */
	if (list_empty(&cs->list)) {
		cs->rating = 0;
		return;
	}

	// if (cs->mark_unstable)
	// 	cs->mark_unstable(cs);

	// /* kick clocksource_watchdog_kthread() */
	// if (finished_booting)
	// 	schedule_work(&watchdog_work);
}

/**
 * clocksource_mark_unstable - mark clocksource unstable via watchdog
 * @cs:		clocksource to be marked unstable
 *
 * This function is called by the x86 TSC code to mark clocksources as unstable;
 * it defers demotion and re-selection to a kthread.
 */
void clocksource_mark_unstable(clocksrc_s *cs)
{
	ulong flags;

	// spin_lock_irqsave(&watchdog_lock, flags);
	if (!(cs->flags & CLOCK_SOURCE_UNSTABLE)) {
		// if (!list_empty(&cs->list) && list_empty(&cs->wd_list))
		// 	list_add(&cs->wd_list, &watchdog_list);
		__clocksource_unstable(cs);
	}
	// spin_unlock_irqrestore(&watchdog_lock, flags);
}


/**
 * clocksource_max_adjustment- Returns max adjustment amount
 * @cs:         Pointer to clocksource
 *
 */
static u32
clocksource_max_adjustment(clocksrc_s *cs) {
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
u64 clocks_calc_max_nsecs(u32 mult, u32 shift,
		u32 maxadj, u64 mask, u64 *max_cyc)
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


static clocksrc_s
*clocksource_find_best(bool oneshot, bool skipcur) {
	clocksrc_s *cs;

	// if (!finished_booting || list_empty(&clocksource_list))
	if (list_header_is_empty(&clocksource_list))
		return NULL;

	/*
	 * We pick the clocksource with the highest rating. If oneshot
	 * mode is active, we pick the highres valid clocksource with
	 * the best rating.
	 */
	list_header_for_each_container(cs, &clocksource_list, list) {
		if (skipcur && cs == curr_clocksource)
			continue;
		if (oneshot && !(cs->flags & CLOCK_SOURCE_VALID_FOR_HRES))
			continue;
		return cs;
	}
	return NULL;
}

static void
__clocksource_select(bool skipcur) {
	// bool oneshot = tick_oneshot_mode_active();
	bool oneshot = false;
	clocksrc_s *best, *cs;

	/* Find the best suitable clocksource */
	best = clocksource_find_best(oneshot, skipcur);
	if (!best)
		return;

	// if (!strlen(override_name))
	// 	goto found;

	/* Check for the override clocksource. */
	list_header_for_each_container(cs, &clocksource_list, list) {
		if (skipcur && cs == curr_clocksource)
			continue;
		// if (strcmp(cs->name, override_name) != 0)
		// 	continue;
		/*
		 * Check to make sure we don't switch to a non-highres
		 * capable clocksource if the tick code is in oneshot
		 * mode (highres or nohz)
		 */
		if (!(cs->flags & CLOCK_SOURCE_VALID_FOR_HRES) && oneshot) {
			/* Override clocksource cannot be used. */
			if (cs->flags & CLOCK_SOURCE_UNSTABLE) {
				pr_warn("Override clocksource %s is unstable and not HRT compatible - cannot switch while in HRT/NOHZ mode\n",
					cs->name);
				// override_name[0] = 0;
			} else {
				/*
				 * The override cannot be currently verified.
				 * Deferring to let the watchdog check.
				 */
				pr_info("Override clocksource %s is not currently HRT compatible - deferring\n",
					cs->name);
			}
		} else
			/* Override clocksource can be used. */
			best = cs;
		break;
	}

// found:
	// if (curr_clocksource != best && !timekeeping_notify(best)) {
	// 	pr_info("Switched to clocksource %s\n", best->name);
	// 	curr_clocksource = best;
	// }
}

/*
 * clocksource_done_booting - Called near the end of core bootup
 *
 * Hack to avoid lots of clocksource churn at boot time.
 * We use fs_initcall because we want this to start before
 * device_initcall but after subsys_initcall.
 */
int __init
clocksource_done_booting(void) {
	// mutex_lock(&clocksource_mutex);
	curr_clocksource = clocksource_default_clock();
	// finished_booting = 1;
	// /*
	//  * Run the watchdog first to eliminate unstable clock sources
	//  */
	// __clocksource_watchdog_kthread();
	clocksource_select();
	// mutex_unlock(&clocksource_mutex);
	return 0;
}
fs_initcall(clocksource_done_booting);

/*
 * Enqueue the clocksource sorted by rating
 */
static void
clocksource_enqueue(clocksrc_s *cs) {
	List_s *entry = &clocksource_list.anchor;
	clocksrc_s *tmp;

	list_header_for_each_container(tmp, &clocksource_list, list) {
		/* Keep track of the place, where to insert */
		if (tmp->rating < cs->rating)
			break;
		entry = &tmp->list;
	}
	list_add(&cs->list, entry);
	clocksource_list.count++;
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
	ulong flags;

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
	clocksource_enqueue(cs);
	// clocksource_enqueue_watchdog(cs);
	// clocksource_watchdog_unlock(&flags);

	// clocksource_select();
	// clocksource_select_watchdog(false);
	// __clocksource_suspend_select(cs);
	// mutex_unlock(&clocksource_mutex);
	return 0;
}