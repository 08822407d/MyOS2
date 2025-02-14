#ifndef _LINUX_SOFTIRQ_H_
#define _LINUX_SOFTIRQ_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../kactive_const.h"
	#include "../kactive_types.h"
	#include "../kactive_api.h"

	#ifdef DEBUG


	#endif

	#include "softirq_macro.h"

	#if defined(SOFTIRQ_DEFINATION) || !(DEBUG)

		// PREFIX_STATIC_INLINE


	#endif /* !DEBUG */

#endif /* _LINUX_SOFTIRQ_H_ */