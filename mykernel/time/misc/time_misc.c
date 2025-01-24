#define TIME_MISC_DEFINATION
#include "time_misc.h"

#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>


/*
 * mktime64 - Converts date to seconds.
 * Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * A leap second can be indicated by calling this function with sec as
 * 60 (allowable under ISO 8601).  The leap second is treated the same
 * as the following second since they don't exist in UNIX time.
 *
 * An encoding of midnight at the end of the day as 24:00:00 - ie. midnight
 * tomorrow - (allowable under ISO 8601) is supported.
 */
time64_t mktime64(const uint year0, const uint mon0, const uint day,
		const uint hour, const uint min, const uint sec)
{
	uint mon = mon0, year = year0;

	/* 1..12 -> 11,12,1..10 */
	if (0 >= (int) (mon -= 2)) {
		mon += 12;	/* Puts Feb last since it has leap day */
		year -= 1;
	}

	return ((((time64_t)
				(year/4 - year/100 + year/400 + 367*mon/12 + day) +
				year*365 - 719499
			)*24 + hour /* now have hours - midnight tomorrow handled here */
		)*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
}


/**
 * ns_to_timespec64 - Convert nanoseconds to timespec64
 * @nsec:       the nanoseconds value to be converted
 *
 * Returns the timespec64 representation of the nsec parameter.
 */
timespec64_s ns_to_timespec64(s64 nsec)
{
	timespec64_s ts = { 0, 0 };
	s32 rem;

	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
	} else if (nsec < 0) {
		/*
		 * With negative times, tv_sec points to the earlier
		 * second, and tv_nsec counts the nanoseconds since
		 * then, so tv_nsec is always a positive number.
		 */
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
	}

	return ts;
}



/**
 * get_timespec64 - get user's time value into kernel space
 * @ts: destination &struct timespec64
 * @uts: user's time value as &struct __kernel_timespec
 *
 * Handles compat or 32-bit modes.
 *
 * Return: %0 on success or negative errno on error
 */
int get_timespec64(timespec64_s *ts,
		const __kernel_timespec_s __user *uts)
{
	__kernel_timespec_s kts;
	int ret;

	ret = copy_from_user(&kts, uts, sizeof(kts));
	if (ret)
		return -EFAULT;

	ts->tv_sec = kts.tv_sec;

	// /* Zero out the padding in compat mode */
	// if (in_compat_syscall())
	// 	kts.tv_nsec &= 0xFFFFFFFFUL;

	/* In 32-bit mode, this drops the padding */
	ts->tv_nsec = kts.tv_nsec;

	return 0;
}
EXPORT_SYMBOL_GPL(get_timespec64);

/**
 * put_timespec64 - convert timespec64 value to __kernel_timespec format and
 * 		    copy the latter to userspace
 * @ts: input &struct timespec64
 * @uts: user's &struct __kernel_timespec
 *
 * Return: %0 on success or negative errno on error
 */
int put_timespec64(const timespec64_s *ts,
		__kernel_timespec_s __user *uts)
{
	__kernel_timespec_s kts = {
		.tv_sec = ts->tv_sec,
		.tv_nsec = ts->tv_nsec
	};

	return copy_to_user(uts, &kts, sizeof(kts)) ? -EFAULT : 0;
}
EXPORT_SYMBOL_GPL(put_timespec64);