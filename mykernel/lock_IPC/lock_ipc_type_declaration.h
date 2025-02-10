#ifndef _LOCK_IPC_TYPE_DECLARATIONS_H_
#define _LOCK_IPC_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>
	#include <uapi/asm-generic/resource.h>
	#include <uapi/asm-generic/signal-defs.h>
	#include <uapi/asm-generic/siginfo.h>
	#include <uapi/linux/resource.h>
	#include <uapi/linux/time.h>
	#include <uapi/linux/futex.h>

	#include <asm/lock_ipc_types.h>


	/* atomic */
	typedef atomic64_t atomic_long_t;

	/* semaphore */
	struct semaphore;
	typedef struct semaphore sema_t;
	struct rw_semaphore;
	typedef struct rw_semaphore rwsem_t;

	/* spinlock */
	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	// typedef arch_spinlock_t spinlock_t;

	/* signal */
	union sigval;
	typedef union sigval sigval_t;
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


	#include <linux/kernel/sched_const.h>
	#include <linux/kernel/sched_type_declaration.h>
	#include <linux/kernel/time_type_declaration.h>
	#include <linux/kernel/lib_type_declaration.h>

#endif /* _LOCK_IPC_TYPE_DECLARATIONS_H_ */