/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LOCK_IPC_API_H_
#define _LOCK_IPC_API_H_

	#include "lock_ipc_type_declaration.h"
	#include <asm/lock_ipc_api.h>


	#include "atomic/atomic_api.h"
	#include "spinlock/spinlock_api.h"
	#include "semaphore/semaphore_api.h"
	#include "semaphore/rwsem_api.h"
	#include "lockdep/lockdep_api.h"
	#include "signal/signal_api.h"
	#include "futex/futex_api.h"


	#include <linux/kernel/irqflags.h>
	#include <linux/kernel/sched_api.h>
	#include <linux/sched/debug.h>

#endif /* _LOCK_IPC_API_H_ */
