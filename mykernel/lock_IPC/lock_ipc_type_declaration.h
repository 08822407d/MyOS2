#ifndef _LOCK_IPC_TYPE_DECLARATIONS_H_
#define _LOCK_IPC_TYPE_DECLARATIONS_H_

	struct tspinlock;
	typedef struct tspinlock arch_spinlock_t;

	#include <linux/kernel/types.h>

	#include <asm/lock_ipc_types.h>


	/* atomic */
	struct atomic;
	typedef struct atomic atomic_t;
	struct atomic64;
	typedef struct atomic64 atomic64_t;
	typedef atomic64_t atomic_long_t;

	/* spinlock */
	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	typedef arch_spinlock_t spinlock_t;

#endif /* _LOCK_IPC_TYPE_DECLARATIONS_H_ */