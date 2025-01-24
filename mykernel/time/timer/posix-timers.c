#include "timer.h"

#include <linux/kernel/kernel.h>


// clock_getres() implements
// static int
// posix_get_hrtimer_res(clockid_t which_clock, timespec64_s *tp) {
// 	tp->tv_sec = 0;
// 	tp->tv_nsec = hrtimer_resolution;
// 	return 0;
// }
// static int
// posix_get_coarse_res(const clockid_t which_clock, timespec64_s *tp) {
// 	*tp = ktime_to_timespec64(KTIME_LOW_RES);
// 	return 0;
// }


// clock_set() implements
// static int
// posix_clock_realtime_set(const clockid_t which_clock, const timespec64_s *tp) {
// 	return do_sys_settimeofday64(tp, NULL);
// }


// clock_get_timespec() implements
static int
posix_get_realtime_timespec(clockid_t which_clock, timespec64_s *tp) {
	ktime_get_real_ts64(tp);
	return 0;
}
// static int
// posix_get_monotonic_timespec(clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_ts64(tp);
// 	timens_add_monotonic(tp);
// 	return 0;
// }
// static int
// posix_get_monotonic_raw(clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_raw_ts64(tp);
// 	timens_add_monotonic(tp);
// 	return 0;
// }
// static int
// posix_get_realtime_coarse(clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_coarse_real_ts64(tp);
// 	return 0;
// }
// static int
// posix_get_monotonic_coarse(clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_coarse_ts64(tp);
// 	timens_add_monotonic(tp);
// 	return 0;
// }
// static int
// posix_get_tai_timespec(clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_clocktai_ts64(tp);
// 	return 0;
// }
// static int
// posix_get_boottime_timespec(const clockid_t which_clock, timespec64_s *tp) {
// 	ktime_get_boottime_ts64(tp);
// 	timens_add_boottime(tp);
// 	return 0;
// }


// clock_get_ktime() implements
// static ktime_t
// posix_get_realtime_ktime(clockid_t which_clock) {
// 	return ktime_get_real();
// }
// static ktime_t
// posix_get_monotonic_ktime(clockid_t which_clock) {
// 	return ktime_get();
// }
// static ktime_t
// posix_get_tai_ktime(clockid_t which_clock) {
// 	return ktime_get_clocktai();
// }
// static ktime_t
// posix_get_boottime_ktime(const clockid_t which_clock) {
// 	return ktime_get_boottime();
// }



static const k_clock_s clock_realtime = {
	// .clock_getres			= posix_get_hrtimer_res,
	// .clock_set				= posix_clock_realtime_set,
	.clock_get_timespec		= posix_get_realtime_timespec,
	// .clock_get_ktime		= posix_get_realtime_ktime,
	// .clock_adj				= posix_clock_realtime_adj,
	// .nsleep					= common_nsleep,
	// .timer_create			= common_timer_create,
	// .timer_set				= common_timer_set,
	// .timer_get				= common_timer_get,
	// .timer_del				= common_timer_del,
	// .timer_rearm			= common_hrtimer_rearm,
	// .timer_forward			= common_hrtimer_forward,
	// .timer_remaining		= common_hrtimer_remaining,
	// .timer_try_to_cancel	= common_hrtimer_try_to_cancel,
	// .timer_wait_running		= common_timer_wait_running,
	// .timer_arm				= common_hrtimer_arm,
};

static const k_clock_s clock_monotonic = {
	// .clock_getres			= posix_get_hrtimer_res,
	// .clock_get_timespec		= posix_get_monotonic_timespec,
	// .clock_get_ktime		= posix_get_monotonic_ktime,
	// .nsleep					= common_nsleep_timens,
	// .timer_create			= common_timer_create,
	// .timer_set				= common_timer_set,
	// .timer_get				= common_timer_get,
	// .timer_del				= common_timer_del,
	// .timer_rearm			= common_hrtimer_rearm,
	// .timer_forward			= common_hrtimer_forward,
	// .timer_remaining		= common_hrtimer_remaining,
	// .timer_try_to_cancel	= common_hrtimer_try_to_cancel,
	// .timer_wait_running		= common_timer_wait_running,
	// .timer_arm				= common_hrtimer_arm,
};

static const k_clock_s clock_monotonic_raw = {
	// .clock_getres			= posix_get_hrtimer_res,
	// .clock_get_timespec		= posix_get_monotonic_raw,
};

static const k_clock_s clock_realtime_coarse = {
	// .clock_getres			= posix_get_coarse_res,
	// .clock_get_timespec		= posix_get_realtime_coarse,
};

static const k_clock_s clock_monotonic_coarse = {
	// .clock_getres			= posix_get_coarse_res,
	// .clock_get_timespec		= posix_get_monotonic_coarse,
};

static const k_clock_s clock_tai = {
	// .clock_getres			= posix_get_hrtimer_res,
	// .clock_get_timespec		= posix_get_tai_timespec,
	// .clock_get_ktime		= posix_get_tai_ktime,
	// .nsleep					= common_nsleep,
	// .timer_create			= common_timer_create,
	// .timer_set				= common_timer_set,
	// .timer_get				= common_timer_get,
	// .timer_del				= common_timer_del,
	// .timer_rearm			= common_hrtimer_rearm,
	// .timer_forward			= common_hrtimer_forward,
	// .timer_remaining		= common_hrtimer_remaining,
	// .timer_try_to_cancel	= common_hrtimer_try_to_cancel,
	// .timer_wait_running		= common_timer_wait_running,
	// .timer_arm				= common_hrtimer_arm,
};

static const k_clock_s clock_boottime = {
	// .clock_getres			= posix_get_hrtimer_res,
	// .clock_get_timespec		= posix_get_boottime_timespec,
	// .clock_get_ktime		= posix_get_boottime_ktime,
	// .nsleep					= common_nsleep_timens,
	// .timer_create			= common_timer_create,
	// .timer_set				= common_timer_set,
	// .timer_get				= common_timer_get,
	// .timer_del				= common_timer_del,
	// .timer_rearm			= common_hrtimer_rearm,
	// .timer_forward			= common_hrtimer_forward,
	// .timer_remaining		= common_hrtimer_remaining,
	// .timer_try_to_cancel	= common_hrtimer_try_to_cancel,
	// .timer_wait_running		= common_timer_wait_running,
	// .timer_arm				= common_hrtimer_arm,
};

static const k_clock_s *const posix_clocks[] = {
	[CLOCK_REALTIME]		    = &clock_realtime,
	[CLOCK_MONOTONIC]		    = &clock_monotonic,
	// [CLOCK_PROCESS_CPUTIME_ID]	= &clock_process,
	// [CLOCK_THREAD_CPUTIME_ID]	= &clock_thread,
	[CLOCK_MONOTONIC_RAW]		= &clock_monotonic_raw,
	[CLOCK_REALTIME_COARSE]		= &clock_realtime_coarse,
	[CLOCK_MONOTONIC_COARSE]	= &clock_monotonic_coarse,
	[CLOCK_BOOTTIME]		    = &clock_boottime,
	// [CLOCK_REALTIME_ALARM]		= &alarm_clock,
	// [CLOCK_BOOTTIME_ALARM]		= &alarm_clock,
	[CLOCK_TAI]			        = &clock_tai,
};

const k_clock_s
*clockid_to_kclock(const clockid_t id) {
	clockid_t idx = id;

	if (id < 0) {
		pr_err("Dynamic timer currently Not supported.\n");
		while (1);
		// return (id & CLOCKFD_MASK) == CLOCKFD ?
		// 	&clock_posix_dynamic : &clock_posix_cpu;
	}

	if (id >= ARRAY_SIZE(posix_clocks))
		return NULL;

	// return posix_clocks[array_index_nospec(idx, ARRAY_SIZE(posix_clocks))];
	return posix_clocks[idx];
}