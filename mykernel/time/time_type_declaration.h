#ifndef _TIME_TYPE_DECLARATION_H_
#define _TIME_TYPE_DECLARATION_H_

	#include <linux/kernel/types.h>
	#include <linux/kernel/cache.h>
	#include <uapi/linux/time.h>

	typedef s64	ktime_t;
	typedef __s64 time64_t;
	typedef __u64 timeu64_t;


	#include <linux/kernel/lib_types.h>


	/* time misc */
	struct timespec64;
	typedef struct timespec64 timespec64_s;
	struct itimerspec64;
	typedef struct itimerspec64 itimerspec64_s;

	/* clocksource */
	struct clocksource;
	typedef struct clocksource clocksrc_s;

	/* timekeeping */
	struct tk_read_base;
	typedef struct tk_read_base tk_readbase_s;
	struct timekeeper;
	typedef struct timekeeper timekeeper_s;

	/* timer */
	struct timer_base;
	typedef struct timer_base timer_base_s;
	struct timer_list;
	typedef struct timer_list timer_list_s;
	struct k_clock;
	typedef struct k_clock k_clock_s;

#endif /* _TIME_TYPE_DECLARATION_H_ */