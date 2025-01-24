/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KTIME_H_
#define _LINUX_KTIME_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG



	#endif

	#include "ktime_macro.h"
	
	#if defined(KTIME_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_KTIME_H_ */