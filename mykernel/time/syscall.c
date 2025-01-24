#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <uapi/linux/time_types.h>
#include <linux/kernel/uaccess.h>

#include "time_const.h"
#include "time_types.h"
#include "time_api.h"


// #define __NR_gettimeofday	96
MYOS_SYSCALL_DEFINE2(gettimeofday,
		struct __kernel_old_timeval __user *, tv,
		struct timezone __user *, tz)
{
	if (likely(tv != NULL)) {
		timespec64_s ts;

		ktime_get_real_ts64(&ts);
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
	}
	// if (unlikely(tz != NULL)) {
	// 	if (copy_to_user(tz, &sys_tz, sizeof(sys_tz)))
	// 		return -EFAULT;
	// }
	return 0;
}


// #define __NR_clock_gettime		228
MYOS_SYSCALL_DEFINE2(clock_gettime,
		const clockid_t, which_clock,
		__kernel_timespec_s __user *, tp)
{
	const k_clock_s *kc = clockid_to_kclock(which_clock);
	timespec64_s kernel_tp;
	int error;

	if (!kc)
		return -EINVAL;

	error = kc->clock_get_timespec(which_clock, &kernel_tp);

	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;

	return error;
}