#ifndef __LINUX_SPINLOCK_TYPES_H
#define __LINUX_SPINLOCK_TYPES_H

	/*
	 * include/linux/spinlock_types.h - generic spinlock type definitions
	 *                                  and initializers
	 *
	 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
	 * Released under the General Public License (GPL).
	 */

	// #include <linux/kernel/spinlock_types_raw.h>

	#include <asm/spinlock_types.h>

	// #ifndef CONFIG_PREEMPT_RT

	/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
	typedef arch_spinlock_t spinlock_t;

	#define ___SPIN_LOCK_INITIALIZER(lockname) __ARCH_SPIN_LOCK_UNLOCKED
	#define __SPIN_LOCK_UNLOCKED(lockname) __ARCH_SPIN_LOCK_UNLOCKED
	#define DEFINE_SPINLOCK(x)	spinlock_t x = __SPIN_LOCK_UNLOCKED(x)

	// #else /* !CONFIG_PREEMPT_RT */

	// /* PREEMPT_RT kernels map spinlock to rt_mutex */
	// #include <linux/rtmutex.h>

	// typedef struct spinlock {
	// 	struct rt_mutex_base	lock;
	// #ifdef CONFIG_DEBUG_LOCK_ALLOC
	// 	struct lockdep_map	dep_map;
	// #endif
	// } spinlock_t;

	// #define __SPIN_LOCK_UNLOCKED(name)				\
	// 	{							\
	// 		.lock = __RT_MUTEX_BASE_INITIALIZER(name.lock),	\
	// 		SPIN_DEP_MAP_INIT(name)				\
	// 	}

	// #define __LOCAL_SPIN_LOCK_UNLOCKED(name)			\
	// 	{							\
	// 		.lock = __RT_MUTEX_BASE_INITIALIZER(name.lock),	\
	// 		LOCAL_SPIN_DEP_MAP_INIT(name)			\
	// 	}

	// #define DEFINE_SPINLOCK(name)					\
	// 	spinlock_t name = __SPIN_LOCK_UNLOCKED(name)

	// #endif /* CONFIG_PREEMPT_RT */

	// #include <linux/rwlock_types.h>

#endif /* __LINUX_SPINLOCK_TYPES_H */
