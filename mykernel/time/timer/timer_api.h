/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_API_H_
#define _LINUX_TIMER_API_H_

	#include "timer.h"

	const k_clock_s *clockid_to_kclock(const clockid_t id);

#endif /* _LINUX_TIMER_API_H_ */