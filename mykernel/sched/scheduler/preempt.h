/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PREEMPT_H_
#define _LINUX_PREEMPT_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG


	#endif

	#include "preempt_macro.h"
	
	#if defined(PREEMPT_DEFINATION) || !(DEBUG)


	#endif /* !DEBUG */

#endif /* _LINUX_PREEMPT_H_ */