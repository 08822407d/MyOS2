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
	tk->tkr_mono.xtime_nsec =
		(u64)ts->tv_nsec << tk->tkr_mono.shift;
}

static void
tk_xtime_add(timekeeper_s *tk, const timespec64_s *ts) {
	tk->xtime_sec += ts->tv_sec;
	tk->tkr_mono.xtime_nsec +=
		(u64)ts->tv_nsec << tk->tkr_mono.shift;
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



/**
 * ktime_get_real_ts64 - Returns the time of day in a timespec64.
 * @ts:		pointer to the timespec to be set
 *
 * Returns the time of day in a timespec64 (WARN if suspended).
 */
void ktime_get_real_ts64(timespec64_s *ts)
{
	timekeeper_s *tk = &tk_core.timekeeper;
	uint seq;
	u64 nsecs;

	// WARN_ON(timekeeping_suspended);

	// do {
	// 	seq = read_seqcount_begin(&tk_core.seq);

		ts->tv_sec = tk->xtime_sec;
		nsecs = timekeeping_get_ns(&tk->tkr_mono);

	// } while (read_seqcount_retry(&tk_core.seq, seq));

	ts->tv_nsec = 0;
	timespec64_add_ns(ts, nsecs);
}
EXPORT_SYMBOL(ktime_get_real_ts64);


ktime_t ktime_get(void)
{
	timekeeper_s *tk = &tk_core.timekeeper;
	uint seq;
	ktime_t base;
	u64 nsecs;

	WARN_ON(timekeeping_suspended);

	// do {
		// seq = read_seqcount_begin(&tk_core.seq);
		base = tk->tkr_mono.base;
		// nsecs = timekeeping_get_ns(&tk->tkr_mono);
	// } while (read_seqcount_retry(&tk_core.seq, seq));

	return ktime_add_ns(base, nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get);



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

/* Timekeeper helper functions. */
static noinline u64
delta_to_ns_safe(const tk_readbase_s *tkr, u64 delta) {
	return mul_u64_u32_add_u64_shr(delta,
			tkr->mult, tkr->xtime_nsec, tkr->shift);
}

static inline u64
timekeeping_cycles_to_ns(const tk_readbase_s *tkr, u64 cycles) {
	/* Calculate the delta since the last update_wall_time() */
	u64 mask = tkr->mask, delta = (cycles - tkr->cycle_last) & mask;

	/*
	 * This detects both negative motion and the case where the delta
	 * overflows the multiplication with tkr->mult.
	 */
	if (unlikely(delta > tkr->clock->max_cycles)) {
		/*
		 * Handle clocksource inconsistency between CPUs to prevent
		 * time from going backwards by checking for the MSB of the
		 * mask being set in the delta.
		 */
		if (delta & ~(mask >> 1))
			return tkr->xtime_nsec >> tkr->shift;

		return delta_to_ns_safe(tkr, delta);
	}

	return ((delta * tkr->mult) + tkr->xtime_nsec) >> tkr->shift;
}

static __always_inline u64
__timekeeping_get_ns(const tk_readbase_s *tkr) {
	return timekeeping_cycles_to_ns(tkr, tk_clock_read(tkr));
}

u64 timekeeping_get_ns(const tk_readbase_s *tkr)
{
	// if (IS_ENABLED(CONFIG_DEBUG_TIMEKEEPING))
	// 	return timekeeping_debug_get_ns(tkr);

	return __timekeeping_get_ns(tkr);
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

	// timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);
}


/*
 * Must hold jiffies_lock
 */
void do_timer(ulong ticks)
{
	jiffies_64 += ticks;
	// calc_global_load();
}