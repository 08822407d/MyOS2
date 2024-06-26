/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIME64_H
#define _LINUX_TIME64_H

	#include <linux/kernel/math64.h>
	// #include <vdso/time64.h>
	/* Parameters used to convert the timespec values: */
	#define MSEC_PER_SEC	1000L
	#define USEC_PER_MSEC	1000L
	#define NSEC_PER_USEC	1000L
	#define NSEC_PER_MSEC	1000000L
	#define USEC_PER_SEC	1000000L
	#define NSEC_PER_SEC	1000000000L
	#define PSEC_PER_SEC	1000000000000LL
	#define FSEC_PER_SEC	1000000000000000LL


	typedef __s64 time64_t;
	typedef __u64 timeu64_t;

	#include <uapi/linux/time.h>

	typedef struct timespec64 {
		time64_t	tv_sec;			/* seconds */
		long		tv_nsec;		/* nanoseconds */
	} timespec64_s;

	typedef struct itimerspec64 {
		timespec64_s it_interval;
		timespec64_s it_value;
	} itimerspec64_s;

	/* Parameters used to convert the timespec values: */
	#define PSEC_PER_NSEC			1000L

	/* Located here for timespec[64]_valid_strict */
	#define TIME64_MAX				((s64)~((u64)1 << 63))
	#define TIME64_MIN				(-TIME64_MAX - 1)

	#define KTIME_MAX				((s64)~((u64)1 << 63))
	#define KTIME_MIN				(-KTIME_MAX - 1)
	#define KTIME_SEC_MAX			(KTIME_MAX / NSEC_PER_SEC)
	#define KTIME_SEC_MIN			(KTIME_MIN / NSEC_PER_SEC)

	/*
	 * Limits for settimeofday():
	 *
	 * To prevent setting the time close to the wraparound point time setting
	 * is limited so a reasonable uptime can be accomodated. Uptime of 30 years
	 * should be really sufficient, which means the cutoff is 2232. At that
	 * point the cutoff is just a small part of the larger problem.
	 */
	#define TIME_UPTIME_SEC_MAX		(30LL * 365 * 24 *3600)
	#define TIME_SETTOD_SEC_MAX		(KTIME_SEC_MAX - TIME_UPTIME_SEC_MAX)

	static inline int
	timespec64_equal(const timespec64_s *a, const timespec64_s *b) {
		return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
	}

	/*
	 * lhs < rhs:  return <0
	 * lhs == rhs: return 0
	 * lhs > rhs:  return >0
	 */
	static inline int
	timespec64_compare(const timespec64_s *lhs, const timespec64_s *rhs) {
		if (lhs->tv_sec < rhs->tv_sec)
			return -1;
		if (lhs->tv_sec > rhs->tv_sec)
			return 1;
		return lhs->tv_nsec - rhs->tv_nsec;
	}

	extern void
	set_normalized_timespec64(timespec64_s *ts, time64_t sec, s64 nsec);

	static inline timespec64_s
	timespec64_add(timespec64_s lhs, timespec64_s rhs) {
		timespec64_s ts_delta;
		set_normalized_timespec64(&ts_delta, lhs.tv_sec + rhs.tv_sec,
				lhs.tv_nsec + rhs.tv_nsec);
		return ts_delta;
	}

	/*
	 * sub = lhs - rhs, in normalized form
	 */
	static inline timespec64_s
	timespec64_sub(timespec64_s lhs, timespec64_s rhs) {
		timespec64_s ts_delta;
		set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
				lhs.tv_nsec - rhs.tv_nsec);
		return ts_delta;
	}

	/*
	 * Returns true if the timespec64 is norm, false if denorm:
	 */
	static inline bool
	timespec64_valid(const timespec64_s *ts) {
		/* Dates before 1970 are bogus */
		if (ts->tv_sec < 0)
			return false;
		/* Can't have more nanoseconds then a second */
		if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
			return false;
		return true;
	}

	static inline bool
	timespec64_valid_strict(const timespec64_s *ts) {
		if (!timespec64_valid(ts))
			return false;
		/* Disallow values that could overflow ktime_t */
		if ((unsigned long long)ts->tv_sec >= KTIME_SEC_MAX)
			return false;
		return true;
	}

	static inline bool
	timespec64_valid_settod(const timespec64_s *ts) {
		if (!timespec64_valid(ts))
			return false;
		/* Disallow values which cause overflow issues vs. CLOCK_REALTIME */
		if ((unsigned long long)ts->tv_sec >= TIME_SETTOD_SEC_MAX)
			return false;
		return true;
	}

	/**
	 * timespec64_to_ns - Convert timespec64 to nanoseconds
	 * @ts:		pointer to the timespec64 variable to be converted
	 *
	 * Returns the scalar nanosecond representation of the timespec64
	 * parameter.
	 */
	static inline s64
	timespec64_to_ns(const timespec64_s *ts) {
		/* Prevent multiplication overflow / underflow */
		if (ts->tv_sec >= KTIME_SEC_MAX)
			return KTIME_MAX;

		if (ts->tv_sec <= KTIME_SEC_MIN)
			return KTIME_MIN;

		return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
	}

	/**
	 * ns_to_timespec64 - Convert nanoseconds to timespec64
	 * @nsec:	the nanoseconds value to be converted
	 *
	 * Returns the timespec64 representation of the nsec parameter.
	 */
	extern timespec64_s ns_to_timespec64(s64 nsec);

	/**
	 * timespec64_add_ns - Adds nanoseconds to a timespec64
	 * @a:		pointer to timespec64 to be incremented
	 * @ns:		unsigned nanoseconds value to be added
	 *
	 * This must always be inlined because its used from the x86-64 vdso,
	 * which cannot call other kernel functions.
	 */
	static __always_inline void
	timespec64_add_ns(timespec64_s *a, u64 ns) {
		a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
		a->tv_nsec = ns;
	}

	/*
	 * timespec64_add_safe assumes both values are positive and checks for
	 * overflow. It will return TIME64_MAX in case of overflow.
	 */
	extern timespec64_s
	timespec64_add_safe(const timespec64_s lhs, const timespec64_s rhs);


	#include <linux/kernel/ktime.h>

#endif /* _LINUX_TIME64_H */
