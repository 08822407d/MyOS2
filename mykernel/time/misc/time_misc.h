#ifndef _TIME_MISC_H_
#define _TIME_MISC_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern int
		timespec64_equal(const timespec64_s *a, const timespec64_s *b);

		extern int
		timespec64_compare(const timespec64_s *lhs, const timespec64_s *rhs);

		extern timespec64_s
		timespec64_add(timespec64_s lhs, timespec64_s rhs);

		extern timespec64_s
		timespec64_sub(timespec64_s lhs, timespec64_s rhs);

		extern bool
		timespec64_valid(const timespec64_s *ts);

		extern bool
		timespec64_valid_strict(const timespec64_s *ts);

		extern bool
		timespec64_valid_settod(const timespec64_s *ts);

		extern s64
		timespec64_to_ns(const timespec64_s *ts);

		extern void
		timespec64_add_ns(timespec64_s *a, u64 ns);

	#endif
	
	#if defined(TIME_MISC_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		int
		timespec64_equal(const timespec64_s *a, const timespec64_s *b) {
			return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
		}

		/*
		 * lhs < rhs:  return <0
		 * lhs == rhs: return 0
		 * lhs > rhs:  return >0
		 */
		PREFIX_STATIC_INLINE
		int
		timespec64_compare(const timespec64_s *lhs, const timespec64_s *rhs) {
			if (lhs->tv_sec < rhs->tv_sec)
				return -1;
			if (lhs->tv_sec > rhs->tv_sec)
				return 1;
			return lhs->tv_nsec - rhs->tv_nsec;
		}

		PREFIX_STATIC_INLINE
		timespec64_s
		timespec64_add(timespec64_s lhs, timespec64_s rhs) {
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec + rhs.tv_sec,
					lhs.tv_nsec + rhs.tv_nsec);
			return ts_delta;
		}

		/*
		 * sub = lhs - rhs, in normalized form
		 */
		PREFIX_STATIC_INLINE
		timespec64_s
		timespec64_sub(timespec64_s lhs, timespec64_s rhs) {
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
			return ts_delta;
		}

		/*
		 * Returns true if the timespec64 is norm, false if denorm:
		 */
		PREFIX_STATIC_INLINE
		bool
		timespec64_valid(const timespec64_s *ts) {
			/* Dates before 1970 are bogus */
			if (ts->tv_sec < 0)
				return false;
			/* Can't have more nanoseconds then a second */
			if ((ulong)ts->tv_nsec >= NSEC_PER_SEC)
				return false;
			return true;
		}

		PREFIX_STATIC_INLINE
		bool
		timespec64_valid_strict(const timespec64_s *ts) {
			if (!timespec64_valid(ts))
				return false;
			/* Disallow values that could overflow ktime_t */
			if ((ulonglong)ts->tv_sec >= KTIME_SEC_MAX)
				return false;
			return true;
		}

		PREFIX_STATIC_INLINE
		bool
		timespec64_valid_settod(const timespec64_s *ts) {
			if (!timespec64_valid(ts))
				return false;
			/* Disallow values which cause overflow issues vs. CLOCK_REALTIME */
			if ((ulonglong)ts->tv_sec >= TIME_SETTOD_SEC_MAX)
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
		PREFIX_STATIC_INLINE
		s64
		timespec64_to_ns(const timespec64_s *ts) {
			/* Prevent multiplication overflow / underflow */
			if (ts->tv_sec >= KTIME_SEC_MAX)
				return KTIME_MAX;

			if (ts->tv_sec <= KTIME_SEC_MIN)
				return KTIME_MIN;

			return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
		}

		/**
		 * timespec64_add_ns - Adds nanoseconds to a timespec64
		 * @a:		pointer to timespec64 to be incremented
		 * @ns:		unsigned nanoseconds value to be added
		 *
		 * This must always be inlined because its used from the x86-64 vdso,
		 * which cannot call other kernel functions.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		timespec64_add_ns(timespec64_s *a, u64 ns) {
			a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
			a->tv_nsec = ns;
		}

	#endif

#endif /* _TIME_MISC_H_ */