#ifndef _TIME_MISC_API_H_
#define _TIME_MISC_API_H_

	#include "time_misc.h"


	extern time64_t
	mktime64(const uint year, const uint mon, const uint day,
			const uint hour, const uint min, const uint sec);

	extern void
	set_normalized_timespec64(timespec64_s *ts, time64_t sec, s64 nsec);

	/**
	 * ns_to_timespec64 - Convert nanoseconds to timespec64
	 * @nsec:	the nanoseconds value to be converted
	 *
	 * Returns the timespec64 representation of the nsec parameter.
	 */
	extern timespec64_s ns_to_timespec64(s64 nsec);

	/*
	 * timespec64_add_safe assumes both values are positive and checks for
	 * overflow. It will return TIME64_MAX in case of overflow.
	 */
	extern timespec64_s
	timespec64_add_safe(const timespec64_s lhs, const timespec64_s rhs);


	extern int
	get_timespec64(timespec64_s *ts, const __kernel_timespec_s __user *uts);
	extern int
	put_timespec64(const timespec64_s *ts, __kernel_timespec_s __user *uts);

#endif /* _TIME_MISC_API_H_ */