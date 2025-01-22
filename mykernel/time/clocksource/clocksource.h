/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_H_
#define _LINUX_CLOCKSOURCE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG



	#endif

	#include "clocksource_macro.h"
	
	#if defined(CLOCKSOURCE_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_CLOCKSOURCE_H_ */