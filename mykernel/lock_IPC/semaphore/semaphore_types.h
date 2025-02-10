#ifndef __LINUX_SEMAPHORE_TYPES_H_
#define __LINUX_SEMAPHORE_TYPES_H_

	#include "../lock_ipc_type_declaration.h"

	/* Please don't access any members of this structure directly */
	struct semaphore {
		spinlock_t	lock;
		uint		count;
		List_s		wait_list;
	};

	struct semaphore_waiter {
		List_s  list;
		task_s  *task;
		bool    up;
	};

#endif /* __LINUX_SEMAPHORE_TYPES_H_ */
