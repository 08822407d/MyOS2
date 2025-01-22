#ifndef _LOCK_IPC_TYPE_DECLARATIONS_H_
#define _LOCK_IPC_TYPE_DECLARATIONS_H_

	struct pid;
	typedef struct pid pid_s;


	#include <asm/lock_ipc_types.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/sched_type_declaration.h>
	#include <linux/kernel/lib_types.h>

	#include <linux/kernel/resource.h>
	#include <linux/kernel/sched_const.h>

	#include <uapi/linux/time.h>
	#include <uapi/linux/futex.h>

	#include <asm/signal.h>


	/* atomic */
	typedef atomic64_t atomic_long_t;

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
	struct sigqueue;
	typedef struct sigqueue sigqueue_s;
	struct sigpending;
	typedef struct sigpending sigpending_s;
	struct sigaction;
	typedef struct sigaction sigaction_s;
	struct k_sigaction;
	typedef struct k_sigaction k_sigaction_s;
	struct ksignal;
	typedef struct ksignal ksignal_s;
	struct signal_struct;
	typedef struct signal_struct signal_s;
	struct sighand_struct;
	typedef struct sighand_struct sighand_s;

	/* refcoount.c */
	struct refcount_struct;
	typedef struct refcount_struct refcount_t;

#endif /* _LOCK_IPC_TYPE_DECLARATIONS_H_ */