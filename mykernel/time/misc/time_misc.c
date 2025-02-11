#define TIME_MISC_DEFINATION
#include "time_misc.h"

#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>


/**
 * __msecs_to_jiffies: - convert milliseconds to jiffies
 * @m:	time in milliseconds
 *
 * conversion is done as follows:
 *
 * - negative values mean 'infinite timeout' (MAX_JIFFY_OFFSET)
 *
 * - 'too large' values [that would result in larger than
 *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
 *
 * - all other values are converted to jiffies by either multiplying
 *   the input value by a factor or dividing it with a factor and
 *   handling any 32-bit overflows.
 *   for the details see __msecs_to_jiffies()
 *
 * __msecs_to_jiffies() checks for the passed in value being a constant
 * via __builtin_constant_p() allowing gcc to eliminate most of the
 * code, __msecs_to_jiffies() is called if the value passed does not
 * allow constant folding and the actual conversion must be done at
 * runtime.
 * The _msecs_to_jiffies helpers are the HZ dependent conversion
 * routines found in include/linux/jiffies.h
 *
 * Return: jiffies value
 */
ulong __msecs_to_jiffies(const uint m)
{
	/*
	 * Negative value, means infinite timeout:
	 */
	if ((int)m < 0)
		return MAX_JIFFY_OFFSET;
	return _msecs_to_jiffies(m);
}
EXPORT_SYMBOL(__msecs_to_jiffies);

/**
 * __usecs_to_jiffies: - convert microseconds to jiffies
 * @u:	time in milliseconds
 *
 * Return: jiffies value
 */
ulong __usecs_to_jiffies(const uint u)
{
	if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	return _usecs_to_jiffies(u);
}
EXPORT_SYMBOL(__usecs_to_jiffies);

/**
 * timespec64_to_jiffies - convert a timespec64 value to jiffies
 * @value: pointer to &struct timespec64
 *
 * The TICK_NSEC - 1 rounds up the value to the next resolution.  Note
 * that a remainder subtract here would not do the right thing as the
 * resolution values don't fall on second boundaries.  I.e. the line:
 * nsec -= nsec % TICK_NSEC; is NOT a correct resolution rounding.
 * Note that due to the small error in the multiplier here, this
 * rounding is incorrect for sufficiently large values of tv_nsec, but
 * well formed timespecs should have tv_nsec < NSEC_PER_SEC, so we're
 * OK.
 *
 * Rather, we just shift the bits off the right.
 *
 * The >> (NSEC_JIFFIE_SC - SEC_JIFFIE_SC) converts the scaled nsec
 * value to a scaled second value.
 *
 * Return: jiffies value
 */
ulong timespec64_to_jiffies(const timespec64_s *value)
{
	u64 sec = value->tv_sec;
	long nsec = value->tv_nsec + TICK_NSEC - 1;

	if (sec >= MAX_SEC_IN_JIFFIES){
		sec = MAX_SEC_IN_JIFFIES;
		nsec = 0;
	}
	return ((sec * SEC_CONVERSION) +
		(((u64)nsec * NSEC_CONVERSION) >>
		 (NSEC_JIFFIE_SC - SEC_JIFFIE_SC))) >> SEC_JIFFIE_SC;

}
EXPORT_SYMBOL(timespec64_to_jiffies);


/**
 * jiffies_to_msecs - Convert jiffies to milliseconds
 * @j: jiffies value
 *
 * Avoid unnecessary multiplications/divisions in the
 * two most common HZ cases.
 *
 * Return: milliseconds value
 */
uint jiffies_to_msecs(const ulong j)
{
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
	return (MSEC_PER_SEC / HZ) * j;
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
	return (j + (HZ / MSEC_PER_SEC) - 1)/(HZ / MSEC_PER_SEC);
#else
	return DIV_ROUND_UP(j * HZ_TO_MSEC_NUM, HZ_TO_MSEC_DEN);
#endif
}
EXPORT_SYMBOL(jiffies_to_msecs);

/**
 * jiffies_to_usecs - Convert jiffies to microseconds
 * @j: jiffies value
 *
 * Return: microseconds value
 */
uint jiffies_to_usecs(const ulong j)
{
	/*
	 * Hz usually doesn't go much further MSEC_PER_SEC.
	 * jiffies_to_usecs() and usecs_to_jiffies() depend on that.
	 */
	BUILD_BUG_ON(HZ > USEC_PER_SEC);

#if !(USEC_PER_SEC % HZ)
	return (USEC_PER_SEC / HZ) * j;
#else
	return (j * HZ_TO_USEC_NUM) / HZ_TO_USEC_DEN;
#endif
}
EXPORT_SYMBOL(jiffies_to_usecs);


/**
 * jiffies_to_timespec64 - convert jiffies value to &struct timespec64
 * @jiffies: jiffies value
 * @value: pointer to &struct timespec64
 */
void jiffies_to_timespec64(const ulong jiffies, timespec64_s *value)
{
	/*
	 * Convert jiffies to nanoseconds and separate with
	 * one divide.
	 */
	u32 rem;
	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
						NSEC_PER_SEC, &rem);
	value->tv_nsec = rem;
}
EXPORT_SYMBOL(jiffies_to_timespec64);

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
time64_t mktime64(const uint year0, const uint mon0,
		const uint day, const uint hour,
		const uint min, const uint sec)
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