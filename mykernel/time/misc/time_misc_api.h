#ifndef _TIME_MISC_API_H_
#define _TIME_MISC_API_H_

	#include "time_misc.h"


	extern ulong __msecs_to_jiffies(const uint m);
	extern ulong __usecs_to_jiffies(const uint u);
	extern ulong timespec64_to_jiffies(const timespec64_s *value);
	extern uint jiffies_to_msecs(const ulong j);
	extern uint jiffies_to_usecs(const ulong j);
	extern void jiffies_to_timespec64(const ulong jiffies, timespec64_s *value);
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