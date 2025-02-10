/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_API_H_
#define _LINUX_TIMER_API_H_

	#include "timer.h"
	#include "hrtimer.h"


	extern timer_base_s timer_bases[NR_BASES];


	const k_clock_s *clockid_to_kclock(const clockid_t id);

	/* timer */
	void simple_init_timer_key(timer_list_s *timer,
		void (*func)(timer_list_s *), uint flags);

	/* hrtimer */
	ktime_t ktime_add_safe(const ktime_t lhs, const ktime_t rhs);
	
	int try_to_del_timer_sync(timer_list_s *timer);
	int timer_delete_sync(timer_list_s *timer);

#endif /* _LINUX_TIMER_API_H_ */