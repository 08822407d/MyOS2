#ifndef _LOCK_IPC_TYPE_DECLARATIONS_H_
#define _LOCK_IPC_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>
	#include <asm/lock_ipc_types.h>


	/* atomic */
	typedef atomic64_t atomic_long_t;

	/* spinlock */
	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	typedef arch_spinlock_t spinlock_t;

#endif /* _LOCK_IPC_TYPE_DECLARATIONS_H_ */