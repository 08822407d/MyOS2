#ifndef _LINUX_FUTEX_H_
#define _LINUX_FUTEX_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern uint
		futex_to_flags(uint op);

		extern bool
		futex_cmd_has_timeout(u32 cmd);

	#endif

	#include "futex_macro.h"
	
	#if defined(FUTEX_DEFINATION) || !(DEBUG)

		/* FUTEX_ to FLAGS_ */
		PREFIX_STATIC_INLINE
		uint
		futex_to_flags(uint op) {
			uint flags = FLAGS_SIZE_32;

			if (!(op & FUTEX_PRIVATE_FLAG))
				flags |= FLAGS_SHARED;

			if (op & FUTEX_CLOCK_REALTIME)
				flags |= FLAGS_CLOCKRT;

			return flags;
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		futex_cmd_has_timeout(u32 cmd) {
			switch (cmd) {
			case FUTEX_WAIT:
			case FUTEX_LOCK_PI:
			case FUTEX_LOCK_PI2:
			case FUTEX_WAIT_BITSET:
			case FUTEX_WAIT_REQUEUE_PI:
				return true;
			}
			return false;
		}

	#endif /* !DEBUG */

#endif /* _LINUX_FUTEX_H_ */