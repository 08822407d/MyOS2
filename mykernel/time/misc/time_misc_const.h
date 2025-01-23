#ifndef _TIME_MISC_CONST_H_
#define _TIME_MISC_CONST_H_


	/* Since jiffies uses a simple TICK_NSEC multiplier
	 * conversion, the .shift value could be zero. However
	 * this would make NTP adjustments impossible as they are
	 * in units of 1/2^.shift. Thus we use JIFFIES_SHIFT to
	 * shift both the nominator and denominator the same
	 * amount, and give ntp adjustments in units of 1/2^8
	 *
	 * The value 8 is somewhat carefully chosen, as anything
	 * larger can result in overflows. TICK_NSEC grows as HZ
	 * shrinks, so values greater than 8 overflow 32bits when
	 * HZ=100.
	 */
	#if HZ < 34
	#  define JIFFIES_SHIFT		6
	#elif HZ < 67
	#  define JIFFIES_SHIFT		7
	#else
	#  define JIFFIES_SHIFT		8
	#endif


	/* Parameters used to convert the timespec values: */
	#define MSEC_PER_SEC			1000L
	#define USEC_PER_MSEC			1000L
	#define NSEC_PER_USEC			1000L
	#define NSEC_PER_MSEC			1000000L
	#define USEC_PER_SEC			1000000L
	#define NSEC_PER_SEC			1000000000L
	#define PSEC_PER_SEC			1000000000000LL
	#define FSEC_PER_SEC			1000000000000000LL

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

#endif /* _TIME_MISC_CONST_H_ */
