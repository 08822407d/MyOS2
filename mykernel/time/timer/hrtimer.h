/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HRTIMER_H_
#define _LINUX_HRTIMER_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG



	#endif

	#include "timer_macro.h"
	
	#if defined(HRTIMER_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_HRTIMER_H_ */