/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_H_
#define _LINUX_TIMER_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern int
		timer_pending(const timer_list_s * timer);


	#endif

	#include "timer_macro.h"
	
	#if defined(TIMER_DEFINATION) || !(DEBUG)

		/**
		 * timer_pending - is a timer pending?
		 * @timer: the timer in question
		 *
		 * timer_pending will tell whether a given timer is currently pending,
		 * or not. Callers must ensure serialization wrt. other operations done
		 * to this timer, eg. interrupt contexts, or other CPUs on SMP.
		 *
		 * Returns: 1 if the timer is pending, 0 if not.
		 */
		PREFIX_STATIC_INLINE
		int
		timer_pending(const timer_list_s * timer) {
			return !hlist_unhashed_lockless(&timer->entry);
		}

	#endif

#endif /* _LINUX_TIMER_H_ */