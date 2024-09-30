#ifndef __LINUX_READWRITE_SEMAPHORE_TYPES_H_
#define __LINUX_READWRITE_SEMAPHORE_TYPES_H_

	#include "../lock_ipc_type_declaration.h"


	/*
	 * For an uncontended rwsem, count and owner are the only fields a task
	 * needs to touch when acquiring the rwsem. So they are put next to each
	 * other to increase the chance that they will share the same cacheline.
	 *
	 * In a contended rwsem, the owner is likely the most frequently accessed
	 * field in the structure as the optimistic waiter that holds the osq lock
	 * will spin on owner. For an embedded rwsem, other hot fields in the
	 * containing structure should be moved further away from the rwsem to
	 * reduce the chance that they will share the same cacheline causing
	 * cacheline bouncing problem.
	 */
	typedef struct rw_semaphore {
		atomic_long_t	count;
		/*
		 * Write owner or one of the read owners as well flags regarding
		 * the current state of the rwsem. Can be used as a speculative
		 * check to see if the write owner is running on the cpu.
		 */
		atomic_long_t	owner;
		spinlock_t		wait_lock;
		List_s			wait_list;
	} rwsem_t;

#endif /* __LINUX_READWRITE_SEMAPHORE_TYPES_H_ */
