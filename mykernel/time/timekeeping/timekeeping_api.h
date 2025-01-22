/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMEKEEPING_API_H_
#define _LINUX_TIMEKEEPING_API_H_

	#include "timekeeping.h"
	#include "../misc/time64.h"

	extern int timekeeping_suspended;


	void timekeeping_init(void);

	extern void do_timer(ulong ticks);

	extern void read_persistent_clock64(timespec64_s *ts);

#endif /* _LINUX_TIMEKEEPING_API_H_ */