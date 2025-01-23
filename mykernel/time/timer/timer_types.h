/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_TYPES_H_
#define _LINUX_TIMER_TYPES_H_

	#include "../time_type_declaration.h"


	struct k_clock {
		int	(*clock_getres)(const clockid_t which_clock,
				timespec64_s *tp);
		int	(*clock_set)(const clockid_t which_clock,
				const timespec64_s *tp);
		/* Returns the clock value in the current time namespace. */
		int	(*clock_get_timespec)(const clockid_t which_clock,
				timespec64_s *tp);
		// /* Returns the clock value in the root time namespace. */
		// ktime_t	(*clock_get_ktime)(const clockid_t which_clock);
		// int	(*clock_adj)(const clockid_t which_clock, struct __kernel_timex *tx);
		// int	(*timer_create)(struct k_itimer *timer);
		// int	(*nsleep)(const clockid_t which_clock, int flags,
		// 		const struct timespec64 *);
		// int	(*timer_set)(struct k_itimer *timr, int flags,
		// 			struct itimerspec64 *new_setting,
		// 			struct itimerspec64 *old_setting);
		// int	(*timer_del)(struct k_itimer *timr);
		// void	(*timer_get)(struct k_itimer *timr,
		// 			struct itimerspec64 *cur_setting);
		// void	(*timer_rearm)(struct k_itimer *timr);
		// s64	(*timer_forward)(struct k_itimer *timr, ktime_t now);
		// ktime_t	(*timer_remaining)(struct k_itimer *timr, ktime_t now);
		// int	(*timer_try_to_cancel)(struct k_itimer *timr);
		// void	(*timer_arm)(struct k_itimer *timr, ktime_t expires,
		// 			bool absolute, bool sigev_none);
		// void	(*timer_wait_running)(struct k_itimer *timr);
	};

#endif /* _LINUX_TIMER_TYPES_H_ */