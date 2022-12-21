/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_TIME_TYPES_H
#define _UAPI_LINUX_TIME_TYPES_H

	#include <linux/kernel/types.h>

	typedef struct __kernel_timespec {
		__kernel_time64_t       tv_sec;                 /* seconds */
		long long               tv_nsec;                /* nanoseconds */
	} __kernel_timespec_s;

	typedef struct __kernel_itimerspec {
		struct __kernel_timespec it_interval;    /* timer period */
		struct __kernel_timespec it_value;       /* timer expiration */
	} __kernel_itimerspec_s;

	/*
	* legacy timeval structure, only embedded in structures that
	* traditionally used 'timeval' to pass time intervals (not absolute
	* times). Do not add new users. If user space fails to compile
	* here, this is probably because it is not y2038 safe and needs to
	* be changed to use another interface.
	*/
	#ifndef __kernel_old_timeval
		typedef struct __kernel_old_timeval {
			__kernel_long_t tv_sec;
			__kernel_long_t tv_usec;
		} __kernel_old_timeval_s;
	#endif

	typedef struct __kernel_old_timespec {
		__kernel_old_time_t	tv_sec;		/* seconds */
		long			tv_nsec;	/* nanoseconds */
	} __kernel_old_timespec_s;

	typedef struct __kernel_old_itimerval {
		struct __kernel_old_timeval it_interval;/* timer interval */
		struct __kernel_old_timeval it_value;	/* current value */
	} __kernel_old_itimerval_s;

	typedef struct __kernel_sock_timeval {
		__s64 tv_sec;
		__s64 tv_usec;
	} __kernel_sock_timeval_s;

#endif /* _UAPI_LINUX_TIME_TYPES_H */