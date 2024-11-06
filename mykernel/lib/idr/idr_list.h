/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_H_
#define _LINUX_IDR_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../lib_const.h"
	#include "../lib_types.h"
	#include "../lib_api.h"


	#ifdef DEBUG



	#endif

	#include "idr_macro.h"
	
	#if defined(IDR_DEFINATION) || !(DEBUG)



	#endif

#endif /* _LINUX_IDR_H_ */