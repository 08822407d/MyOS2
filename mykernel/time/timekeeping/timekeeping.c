#define TIMEKEEPING_DEFINATION
#include "timekeeping.h"


#include <linux/sched/clock.h>


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

static timekeeper_s shadow_timekeeper;

/* flag for if timekeeping is suspended */
int __read_mostly timekeeping_suspended;



static void
tk_set_xtime(timekeeper_s *tk, const timespec64_s *ts) {
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec = (u64)ts->tv_nsec << tk->tkr_mono.shift;
}

static void
tk_xtime_add(timekeeper_s *tk, const timespec64_s *ts) {
	tk->xtime_sec += ts->tv_sec;
	tk->tkr_mono.xtime_nsec += (u64)ts->tv_nsec << tk->tkr_mono.shift;
	tk_normalize_xtime(tk);
}

static void
tk_set_wall_to_mono(timekeeper_s *tk, timespec64_s wtm) {
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


/* must hold timekeeper_lock */
static void
timekeeping_update(timekeeper_s *tk, uint action) {
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
static void
tk_setup_internals(timekeeper_s *tk, clocksrc_s *clock) {
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


extern u64 jiffies_64;
/*
 * Must hold jiffies_lock
 */
void do_timer(ulong ticks)
{
	jiffies_64 += ticks;
	// calc_global_load();
}