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

	// #include <linux/typecheck.h>
	#include <linux/kernel/preempt.h>
	#include <linux/kernel/linkage.h>
	#include <linux/compiler/compiler.h>
	#include <linux/kernel/irqflags.h>
	// #include <linux/sched/thread_info.h>
	#include <linux/kernel/stringify.h>
	// #include <linux/bottom_half.h>
	// #include <linux/lockdep.h>
	#include <asm/barrier.h>
	// #include <asm/mmiowb.h>


	/*
	 * Pull the arch_spinlock_t and arch_rwlock_t definitions:
	 */
	#include <linux/kernel/spinlock_types.h>

	/*
	 * Pull the arch_spin*() functions/declarations (UP-nondebug doesn't need them):
	 */
	#include <asm/spinlock.h>

	#define raw_spin_lock_init(lock)	arch_spin_init(lock)
	#define raw_spin_is_locked(lock)	arch_spin_is_locked(lock)

	// #ifdef arch_spin_is_contended
	// #	define raw_spin_is_contended(lock)	arch_spin_is_contended(&(lock)->raw_lock)
	// #else
	// #	define raw_spin_is_contended(lock)	(((void)(lock), 0))
	// #endif /*arch_spin_is_contended*/

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

	/*
	 * Pull the _spin_*()/_read_*()/_write_*() functions/declarations:
	 */
	// # include <linux/kernel/spinlock_api_smp.h>
	// {

		// static inline int __raw_spin_trylock(raw_spinlock_t *lock) {
		// static inline int raw_spin_trylock(arch_spinlock_t *lock) {
		// 	preempt_disable();
		// 	if (arch_spin_trylock(lock)) {
		// 		// spin_acquire(&lock->dep_map, 0, 1, _RET_IP_);
		// 		return 1;
		// 	}
		// 	preempt_enable();
		// 	return 0;
		// }

		/*
		 * If lockdep is enabled then we use the non-preemption spin-ops
		 * even on CONFIG_PREEMPTION, because lockdep assumes that interrupts are
		 * not re-enabled during lock-acquire (which the preempt-spin-ops do):
		 */
		// static inline void __raw_spin_lock_bh(raw_spinlock_t *lock) {
		// 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// 	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
		// 	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
		// }

		// static inline void __raw_spin_lock(raw_spinlock_t *lock)
		static inline void raw_spin_lock(arch_spinlock_t *lock) {
			preempt_disable();
			arch_spin_lock(lock);
		}

		// static inline void __raw_spin_lock_irq(raw_spinlock_t *lock)
		static inline void
		raw_spin_lock_irq(arch_spinlock_t *lock) {
			local_irq_disable();
			raw_spin_lock(lock);
		}

		// static inline unsigned long
		// __raw_spin_lock_irqsave(raw_spinlock_t *lock)
		static inline unsigned long
		raw_spin_lock_irqsave(arch_spinlock_t *lock) {
			unsigned long flags;

			local_irq_save(flags);
			raw_spin_lock(lock);
			return flags;
		}

		// static inline void __raw_spin_unlock(raw_spinlock_t *lock) {
		static inline void raw_spin_unlock(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
			preempt_enable();
		}

		static inline void
		raw_spin_unlock_no_resched(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
			preempt_enable_no_resched();
		}

		// static inline void __raw_spin_unlock_irqrestore(
		// 		raw_spinlock_t *lock, unsigned long flags)
		static inline void
		raw_spin_unlock_irqrestore(arch_spinlock_t *lock, unsigned long flags) {
			arch_spin_unlock(lock);
			local_irq_restore(flags);
			preempt_enable();
		}

		// static inline void __raw_spin_unlock_irq(raw_spinlock_t *lock) {
		static inline void raw_spin_unlock_irq(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
			local_irq_enable();
			preempt_enable();
		}

		// static inline void __raw_spin_unlock_bh(raw_spinlock_t *lock) {
		// 	spin_release(&lock->dep_map, _RET_IP_);
		// 	do_raw_spin_unlock(lock);
		// 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// }

		// static inline int __raw_spin_trylock_bh(raw_spinlock_t *lock) {
		// 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// 	if (do_raw_spin_trylock(lock)) {
		// 		spin_acquire(&lock->dep_map, 0, 1, _RET_IP_);
		// 		return 1;
		// 	}
		// 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// 	return 0;
		// }

	// }


	/* Non PREEMPT_RT kernel, map to raw spinlocks: */

	#define spin_lock_init(lock)	\
				raw_spin_lock_init(lock)

	#define spin_lock(lock)	\
				raw_spin_lock(lock)

	// static __always_inline void spin_lock_bh(spinlock_t *lock) {
	// 	raw_spin_lock_bh(&lock->rlock);
	// }

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

	// #define spin_lock_irqsave(lock, flags)		\
	// 		do {									\
	// 			raw_spin_lock_irqsave(				\
	// 				spinlock_check(lock), flags);	\
	// 		} while (0)

	// #define spin_lock_irqsave_nested(lock, flags, subclass)	\
	// 		do {												\
	// 			raw_spin_lock_irqsave_nested(					\
	// 				spinlock_check(lock), flags, subclass);		\
	// 		} while (0)

	#define spin_unlock(lock)	\
				raw_spin_unlock(lock)

	#define spin_unlock_no_resched(lock)	\
				raw_spin_unlock_no_resched(lock)

	// static __always_inline void spin_unlock_bh(spinlock_t *lock) {
	// 	raw_spin_unlock_bh(&lock->rlock);
	// }

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

	// /**
	//  * spin_is_locked() - Check whether a spinlock is locked.
	//  * @lock: Pointer to the spinlock.
	//  *
	//  * This function is NOT required to provide any memory ordering
	//  * guarantees; it could be used for debugging purposes or, when
	//  * additional synchronization is needed, accompanied with other
	//  * constructs (memory barriers) enforcing the synchronization.
	//  *
	//  * Returns: 1 if @lock is locked, 0 otherwise.
	//  *
	//  * Note that the function only tells you that the spinlock is
	//  * seen to be locked, not that it is locked on your CPU.
	//  *
	//  * Further, on CONFIG_SMP=n builds with CONFIG_DEBUG_SPINLOCK=n,
	//  * the return value is always 0 (see include/linux/spinlock_up.h).
	//  * Therefore you should not rely heavily on the return value.
	//  */
	// static __always_inline int spin_is_locked(spinlock_t *lock) {
	// 	return raw_spin_is_locked(&lock->rlock);
	// }

	// static __always_inline int spin_is_contended(spinlock_t *lock) {
	// 	return raw_spin_is_contended(&lock->rlock);
	// }

	// #define assert_spin_locked(lock)	assert_raw_spin_locked(&(lock)->rlock)

	/*
	 * Pull the atomic_t declaration:
	 * (asm-mips/atomic.h needs above definitions)
	 */
	#include <linux/kernel/atomic.h>
	// /**
	//  * atomic_dec_and_lock - lock on reaching reference count zero
	//  * @atomic: the atomic counter
	//  * @lock: the spinlock in question
	//  *
	//  * Decrements @atomic by 1.  If the result is 0, returns true and locks
	//  * @lock.  Returns false for all other cases.
	//  */
	// extern int _atomic_dec_and_lock(atomic_t *atomic, spinlock_t *lock);
	// #define atomic_dec_and_lock(atomic, lock) \
	// 			__cond_lock(lock, _atomic_dec_and_lock(atomic, lock))

	// extern int _atomic_dec_and_lock_irqsave(atomic_t *atomic,
	// 		spinlock_t *lock, unsigned long *flags);
	// #define atomic_dec_and_lock_irqsave(atomic, lock, flags) \
	// 			__cond_lock(lock, _atomic_dec_and_lock_irqsave(atomic, lock, &(flags)))

	// int __alloc_bucket_spinlocks(spinlock_t **locks, unsigned int *lock_mask,
	// 		size_t max_size, unsigned int cpu_mult, gfp_t gfp, const char *name,
	// 		struct lock_class_key *key);

	// #define alloc_bucket_spinlocks(locks, lock_mask, max_size, cpu_mult, gfp) ({	\
	// 			static struct lock_class_key key;									\
	// 			int ret;															\
	// 			ret = __alloc_bucket_spinlocks(locks, lock_mask, max_size,			\
	// 					cpu_mult, gfp, #locks, &key);								\
	// 			ret;																\
	// 		})

	// void free_bucket_spinlocks(spinlock_t *locks);

#endif /* __LINUX_SPINLOCK_H */
