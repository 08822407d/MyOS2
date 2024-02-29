/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_SPINLOCK_H
#define __LINUX_SPINLOCK_H

	/*
	 * include/linux/spinlock.h - generic spinlock/rwlock declarations
	 *
	 * here's the role of the various spinlock/rwlock related include files:
	 *
	 * on SMP builds:
	 *
	 *  asm/spinlock_types.h: contains the arch_spinlock_t/arch_rwlock_t and the
	 *                        initializers
	 *
	 *  linux/spinlock_types_raw:
	 *			  The raw types and initializers
	 *  linux/spinlock_types.h:
	 *                        defines the generic type and initializers
	 *
	 *  asm/spinlock.h:       contains the arch_spin_*()/etc. lowlevel
	 *                        implementations, mostly inline assembly code
	 *
	 *   (also included on UP-debug builds:)
	 *
	 *  linux/spinlock_api_smp.h:
	 *                        contains the prototypes for the _spin_*() APIs.
	 *
	 *  linux/spinlock.h:     builds the final spin_*() APIs.
	 *
	 * on UP builds:
	 *
	 *  linux/spinlock_type_up.h:
	 *                        contains the generic, simplified UP spinlock type.
	 *                        (which is an empty structure on non-debug builds)
	 *
	 *  linux/spinlock_types_raw:
	 *			  The raw RT types and initializers
	 *  linux/spinlock_types.h:
	 *                        defines the generic type and initializers
	 *
	 *  linux/spinlock_up.h:
	 *                        contains the arch_spin_*()/etc. version of UP
	 *                        builds. (which are NOPs on non-debug, non-preempt
	 *                        builds)
	 *
	 *   (included on UP-non-debug builds:)
	 *
	 *  linux/spinlock_api_up.h:
	 *                        builds the _spin_*() APIs.
	 *
	 *  linux/spinlock.h:     builds the final spin_*() APIs.
	 */
	#include <asm/lock_ipc.h>

	#include "spinlock/spinlock_types.h"
	#include "spinlock/spinlock_smp.h"


	/*
	 * Pull the arch_spin*() functions/declarations (UP-nondebug doesn't need them):
	 */
	#define raw_spin_lock_init(lock)	arch_spin_init(lock)
	#define raw_spin_is_locked(lock)	arch_spin_is_locked(lock)

	/*
	 * smp_mb__after_spinlock() provides the equivalent of a full memory barrier
	 * between program-order earlier lock acquisitions and program-order later
	 * memory accesses.
	 *
	 * This guarantees that the following two properties hold:
	 *
	 *   1) Given the snippet:
	 *
	 *	  { X = 0;  Y = 0; }
	 *
	 *	  CPU0				CPU1
	 *
	 *	  WRITE_ONCE(X, 1);		WRITE_ONCE(Y, 1);
	 *	  spin_lock(S);			smp_mb();
	 *	  smp_mb__after_spinlock();	r1 = READ_ONCE(X);
	 *	  r0 = READ_ONCE(Y);
	 *	  spin_unlock(S);
	 *
	 *      it is forbidden that CPU0 does not observe CPU1's store to Y (r0 = 0)
	 *      and CPU1 does not observe CPU0's store to X (r1 = 0); see the comments
	 *      preceding the call to smp_mb__after_spinlock() in __schedule() and in
	 *      try_to_wake_up().
	 *
	 *   2) Given the snippet:
	 *
	 *  { X = 0;  Y = 0; }
	 *
	 *  CPU0		CPU1				CPU2
	 *
	 *  spin_lock(S);	spin_lock(S);			r1 = READ_ONCE(Y);
	 *  WRITE_ONCE(X, 1);	smp_mb__after_spinlock();	smp_rmb();
	 *  spin_unlock(S);	r0 = READ_ONCE(X);		r2 = READ_ONCE(X);
	 *			WRITE_ONCE(Y, 1);
	 *			spin_unlock(S);
	 *
	 *      it is forbidden that CPU0's critical section executes before CPU1's
	 *      critical section (r0 = 1), CPU2 observes CPU1's store to Y (r1 = 1)
	 *      and CPU2 does not observe CPU0's store to X (r2 = 0); see the comments
	 *      preceding the calls to smp_rmb() in try_to_wake_up() for similar
	 *      snippets but "projected" onto two CPUs.
	 *
	 * Property (2) upgrades the lock to an RCsc lock.
	 *
	 * Since most load-store architectures implement ACQUIRE with an smp_mb() after
	 * the LL/SC loop, they need no further barriers. Similarly all our TSO
	 * architectures imply an smp_mb() for each atomic instruction and equally don't
	 * need more.
	 *
	 * Architectures that can implement ACQUIRE better need to take care.
	 */

	/* Non PREEMPT_RT kernel, map to raw spinlocks: */

	#define spin_lock_init(lock)	\
				raw_spin_lock_init(lock)

	#define spin_lock(lock)	\
				raw_spin_lock(lock)

	#define spin_lock_bh(lock)	\
				raw_spin_lock_bh(lock)

	#define spin_trylock(lock)	\
				raw_spin_trylock(lock)

	// #define spin_lock_nested(lock, subclass)			\
	// 		do {										\
	// 			raw_spin_lock_nested(					\
	// 				spinlock_check(lock), subclass);	\
	// 		} while (0)

	// #define spin_lock_nest_lock(lock, nest_lock)		\
	// 		do {										\
	// 			raw_spin_lock_nest_lock(				\
	// 				spinlock_check(lock), nest_lock);	\
	// 		} while (0)

	#define spin_lock_irq(lock)	\
				raw_spin_lock_irq(lock)

	#define spin_lock_irqsave(lock, flags)	\
				raw_spin_lock_irqsave(lock, flags)

	// #define spin_lock_irqsave_nested(lock, flags, subclass)	\
	// 		do {												\
	// 			raw_spin_lock_irqsave_nested(					\
	// 				spinlock_check(lock), flags, subclass);		\
	// 		} while (0)

	#define spin_unlock(lock)	\
				raw_spin_unlock(lock)

	#define spin_unlock_bh(lock)	\
				raw_spin_unlock_bh(lock)

	#define spin_unlock_irq(lock)	\
				raw_spin_unlock_irq(lock)

	#define spin_unlock_irqrestore(lock)	\
				raw_spin_unlock_irqrestore(lock)

	// static __always_inline int spin_trylock_bh(spinlock_t *lock) {
	// 	return raw_spin_trylock_bh(&lock->rlock);
	// }

	// static __always_inline int spin_trylock_irq(spinlock_t *lock) {
	// 	return raw_spin_trylock_irq(&lock->rlock);
	// }

	// #define spin_trylock_irqsave(lock, flags) ({							\
	// 			raw_spin_trylock_irqsave(spinlock_check(lock), flags);	\
	// 		})


#endif /* __LINUX_SPINLOCK_H */
