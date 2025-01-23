/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ADJTIME_API_H_
#define _LINUX_ADJTIME_API_H_

	#include "adjtime.h"

	/*
	 * kernel variables
	 * Note: maximum error = NTP sync distance = dispersion + delay / 2;
	 * estimated error = NTP dispersion.
	 */
	extern ulong tick_usec;		/* USER_HZ period (usec) */
	extern ulong tick_nsec;		/* SHIFTED_HZ period (nsec) */


	int read_current_timer(ulong *timer_val);

#endif /* _LINUX_ADJTIME_API_H_ */