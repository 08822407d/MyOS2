#ifndef _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_
#define _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>


	/* arch atomic */
	struct atomic;
	typedef struct atomic atomic_t;
	struct atomic64;
	typedef struct atomic64 atomic64_t;
	typedef struct atomic64 atomic_long_t;

	/* arch spinlock */
	struct tspinlock;
	typedef struct tspinlock arch_spinlock_t;
	typedef struct tspinlock spinlock_t;

#endif /* _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_ */