#ifndef __LINUX_LOCKDEP_H_
#define __LINUX_LOCKDEP_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG


	#endif

	#include "lockdep_macro.h"
	
	#if defined(LOCKDEP_DEFINATION) || !(DEBUG)


	#endif /* !DEBUG */

#endif /* __LINUX_LOCKDEP_H_ */