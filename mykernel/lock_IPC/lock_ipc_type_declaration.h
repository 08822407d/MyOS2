#ifndef _LOCK_IPC_TYPE_DECLARATIONS_H_
#define _LOCK_IPC_TYPE_DECLARATIONS_H_

	struct pid;
	typedef struct pid pid_s;


	#include <asm/lock_ipc_types.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/lib_types.h>

	#include <linux/kernel/resource.h>
	#include <linux/kernel/sched_const.h>


	/* atomic */
	// typedef atomic64_t atomic_long_t;

	/* semaphore */
	struct rw_semaphore;
	typedef struct rw_semaphore rwsem_t;

	/* spinlock */
	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	// typedef arch_spinlock_t spinlock_t;

	/* signal */
	union sigval;
	typedef union sigval sigval_t;
	union __sifields;
	typedef union __sifields __sifields_u;
	struct kernel_siginfo;
	typedef struct kernel_siginfo kernel_siginfo_t;
	struct signal_struct;
	typedef struct signal_struct signal_s;

	/* refcoount.c */
	struct refcount_struct;
	typedef struct refcount_struct refcount_t;

#endif /* _LOCK_IPC_TYPE_DECLARATIONS_H_ */