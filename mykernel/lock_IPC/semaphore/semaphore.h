#ifndef __LINUX_SEMAPHORE_H_
#define __LINUX_SEMAPHORE_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG


	#endif

	#include "semaphore_macro.h"
	
	#if defined(SEMAPHORE_DEFINATION) || !(DEBUG)


	#endif /* !DEBUG */

#endif /* __LINUX_SEMAPHORE_H_ */