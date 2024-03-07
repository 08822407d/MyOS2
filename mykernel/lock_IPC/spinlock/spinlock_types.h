#ifndef __LINUX_SPINLOCK_TYPES_H_
#define __LINUX_SPINLOCK_TYPES_H_

	/*
	 * include/linux/spinlock_types.h - generic spinlock type definitions
	 *                                  and initializers
	 *
	 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
	 * Released under the General Public License (GPL).
	 */
	#include <asm/lock_ipc.h>

	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	typedef arch_spinlock_t spinlock_t;

	#define __SPIN_LOCK_UNLOCKED(lockname) __ARCH_SPIN_LOCK_UNLOCKED
	#define DEFINE_SPINLOCK(x)	spinlock_t x = __SPIN_LOCK_UNLOCKED(x)

#endif /* __LINUX_SPINLOCK_TYPES_H_ */
