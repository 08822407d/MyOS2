// SPDX-License-Identifier: GPL-2.0
/*
 *  Kernel timekeeping code and accessor functions. Based on code from
 *  timer.c, moved in commit 8524070b7982.
 */
#include <linux/kernel/timekeeper_internal.h>
// #include <linux/module.h>
// #include <linux/interrupt.h>
#include <linux/smp/percpu.h>
#include <linux/init/init.h>
#include <linux/kernel/mm.h>
#include <linux/kernel/nmi.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/loadavg.h>
#include <linux/sched/clock.h>
// #include <linux/syscore_ops.h>
#include <linux/kernel/clocksource.h>
#include <linux/kernel/jiffies.h>
#include <linux/kernel/time.h>
#include <linux/kernel/timex.h>
// #include <linux/tick.h>
// #include <linux/stop_machine.h>
// #include <linux/pvclock_gtod.h>
#include <linux/compiler/compiler.h>
// #include <linux/audit.h>
// #include <linux/random.h>

// #include "tick-internal.h"
// #include "ntp_internal.h"
#include "timekeeping_internal.h"


#include <linux/kernel/timekeeping.h>


#define TK_CLEAR_NTP		(1 << 0)
#define TK_MIRROR			(1 << 1)
#define TK_CLOCK_WAS_SET	(1 << 2)

enum timekeeping_adv_mode {
	/* Update timekeeper when a tick has passed */
	TK_ADV_TICK,
	/* Update timekeeper on a direct frequency change */
	TK_ADV_FREQ
};

/*
 * The most important data for readout fits into a single 64 byte
 * cache line.
 */
static struct {
	// seqcount_raw_spinlock_t	seq;
	timekeeper_s	timekeeper;
} tk_core ____cacheline_aligned = {
	// .seq = SEQCNT_RAW_SPINLOCK_ZERO(tk_core.seq, &timekeeper_lock),
};

static struct timekeeper shadow_timekeeper;

/* flag for if timekeeping is suspended */
int __read_mostly timekeeping_suspended;



static inline void
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

static inline timespec64_s
tk_xtime(const timekeeper_s *tk) {
	timespec64_s ts;

	ts.tv_sec = tk->xtime_sec;
	ts.tv_nsec = (long)(tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift);
	return ts;
}

static void tk_set_xtime(timekeeper_s *tk, const timespec64_s *ts)
{
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec = (u64)ts->tv_nsec << tk->tkr_mono.shift;
}

static void tk_xtime_add(timekeeper_s *tk, const timespec64_s *ts)
{
	tk->xtime_sec += ts->tv_sec;
	tk->tkr_mono.xtime_nsec += (u64)ts->tv_nsec << tk->tkr_mono.shift;
	tk_normalize_xtime(tk);
}

static void tk_set_wall_to_mono(timekeeper_s *tk, timespec64_s wtm)
{
	// timespec64_s tmp;

	// /*
	//  * Verify consistency of: offset_real = -wall_to_monotonic
	//  * before modifying anything
	//  */
	// set_normalized_timespec64(&tmp, -tk->wall_to_monotonic.tv_sec,
	// 				-tk->wall_to_monotonic.tv_nsec);
	// WARN_ON_ONCE(tk->offs_real != timespec64_to_ktime(tmp));
	// tk->wall_to_monotonic = wtm;
	// set_normalized_timespec64(&tmp, -wtm.tv_sec, -wtm.tv_nsec);
	// tk->offs_real = timespec64_to_ktime(tmp);
	// tk->offs_tai = ktime_add(tk->offs_real, ktime_set(tk->tai_offset, 0));
}

static inline void
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
static inline u64 tk_clock_read(const tk_readbase_s *tkr) {
	clocksrc_s *clock = READ_ONCE(tkr->clock);
	return clock->read(clock);
}



/* must hold timekeeper_lock */
static void timekeeping_update(timekeeper_s *tk, unsigned int action)
{
	// if (action & TK_CLEAR_NTP) {
	// 	tk->ntp_error = 0;
	// 	ntp_clear();
	// }

	// tk_update_leap_state(tk);
	// tk_update_ktime_data(tk);

	// update_vsyscall(tk);
	// update_pvclock_gtod(tk, action & TK_CLOCK_WAS_SET);

	// tk->tkr_mono.base_real = tk->tkr_mono.base + tk->offs_real;
	// update_fast_timekeeper(&tk->tkr_mono, &tk_fast_mono);
	// update_fast_timekeeper(&tk->tkr_raw,  &tk_fast_raw);

	// if (action & TK_CLOCK_WAS_SET)
	// 	tk->clock_was_set_seq++;
	// /*
	//  * The mirroring of the data to the shadow-timekeeper needs
	//  * to happen last here to ensure we don't over-write the
	//  * timekeeper structure on the next update with stale data
	//  */
	// if (action & TK_MIRROR)
	// 	memcpy(&shadow_timekeeper, &tk_core.timekeeper,
	// 	       sizeof(tk_core.timekeeper));
}



/**
 * tk_setup_internals - Set up internals to use clocksource clock.
 *
 * @tk:		The target timekeeper to setup.
 * @clock:		Pointer to clocksource.
 *
 * Calculates a fixed cycle/nsec interval for a given clocksource/adjustment
 * pair and interval request.
 *
 * Unless you're the timekeeping code, you should not be using this!
 */
static void tk_setup_internals(timekeeper_s *tk, clocksrc_s *clock)
{
	u64 interval;
	u64 tmp, ntpinterval;
	clocksrc_s *old_clock;

	++tk->cs_was_changed_seq;
	old_clock = tk->tkr_mono.clock;
	tk->tkr_mono.clock = clock;
	tk->tkr_mono.mask = clock->mask;
	tk->tkr_mono.cycle_last = tk_clock_read(&tk->tkr_mono);

	tk->tkr_raw.clock = clock;
	tk->tkr_raw.mask = clock->mask;
	tk->tkr_raw.cycle_last = tk->tkr_mono.cycle_last;

	/* Do the ns -> cycle conversion first, using original mult */
	tmp = NTP_INTERVAL_LENGTH;
	tmp <<= clock->shift;
	ntpinterval = tmp;
	tmp += clock->mult/2;
	do_div(tmp, clock->mult);
	if (tmp == 0)
		tmp = 1;

	interval = (u64) tmp;
	tk->cycle_interval = interval;

	/* Go back from cycles -> shifted ns */
	tk->xtime_interval = interval * clock->mult;
	tk->xtime_remainder = ntpinterval - tk->xtime_interval;
	tk->raw_interval = interval * clock->mult;

	 /* if changing clocks, convert xtime_nsec shift units */
	if (old_clock) {
		int shift_change = clock->shift - old_clock->shift;
		if (shift_change < 0) {
			tk->tkr_mono.xtime_nsec >>= -shift_change;
			tk->tkr_raw.xtime_nsec >>= -shift_change;
		} else {
			tk->tkr_mono.xtime_nsec <<= shift_change;
			tk->tkr_raw.xtime_nsec <<= shift_change;
		}
	}

	tk->tkr_mono.shift = clock->shift;
	tk->tkr_raw.shift = clock->shift;

	// tk->ntp_error = 0;
	// tk->ntp_error_shift = NTP_SCALE_SHIFT - clock->shift;
	// tk->ntp_tick = ntpinterval << tk->ntp_error_shift;

	/*
	 * The timekeeper keeps its own mult values for the currently
	 * active clocksource. These value will be adjusted via NTP
	 * to counteract clock drifting.
	 */
	tk->tkr_mono.mult = clock->mult;
	tk->tkr_raw.mult = clock->mult;
	// tk->ntp_err_mult = 0;
	// tk->skip_second_overflow = 0;
}




/*
 * timekeeping_init - Initializes the clocksource and common timekeeping values
 */
void __init timekeeping_init(void)
{
	timespec64_s wall_time, boot_offset, wall_to_mono;
	timekeeper_s *tk = &tk_core.timekeeper;
	clocksrc_s *clock;
	unsigned long flags;

	read_persistent_clock64(&wall_time);
	boot_offset = ns_to_timespec64(local_clock());


	clock = clocksource_default_clock();
	if (clock->enable)
		clock->enable(clock);	
	tk_setup_internals(tk, clock);

	tk_set_xtime(tk, &wall_time);
	tk->raw_sec = 0;

	tk_set_wall_to_mono(tk, wall_to_mono);

	timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);
}



/*
 * Must hold jiffies_lock
 */
void do_timer(unsigned long ticks)
{
	jiffies_64 += ticks;
	// calc_global_load();
}