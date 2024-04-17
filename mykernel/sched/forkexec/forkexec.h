#ifndef _LINUX_FORK_EXEC_H_
#define _LINUX_FORK_EXEC_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG


	#endif

	#include "forkexec_macro.h"

	#if defined(FORKEXEC_DEFINATION) || !(DEBUG)


	#endif

#endif /* _LINUX_FORK_EXEC_H_ */