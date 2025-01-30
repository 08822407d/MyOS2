/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_API_H_
#define _LINUX_TIMER_API_H_

	#include "timer.h"
	#include "hrtimer.h"

	const k_clock_s *clockid_to_kclock(const clockid_t id);

	/* hrtimer */
	ktime_t ktime_add_safe(const ktime_t lhs, const ktime_t rhs);

#endif /* _LINUX_TIMER_API_H_ */