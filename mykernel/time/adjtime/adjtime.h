/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ADJTIME_H_
#define _LINUX_ADJTIME_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG



	#endif

	#include "adjtime_macro.h"
	
	#if defined(ADJTIME_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_ADJTIME_H_ */