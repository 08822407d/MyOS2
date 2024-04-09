#ifndef _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_
#define _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>


	/* arch atomic */
	struct atomic;
	typedef struct atomic atomic_t;
	struct atomic64;
	typedef struct atomic64 atomic64_t;

	/* arch spinlock */
	struct tspinlock;
	typedef struct tspinlock arch_spinlock_t;


	#include <linux/kernel/lock_ipc_type_declaration.h>

#endif /* _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_ */