/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMEKEEPING_H_
#define _LINUX_TIMEKEEPING_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG



	#endif

	#include "timekeeping_macro.h"
	
	#if defined(TIMEKEEPING_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_TIMEKEEPING_H_ */