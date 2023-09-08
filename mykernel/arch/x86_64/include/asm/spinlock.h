/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_H
#define _ASM_X86_SPINLOCK_H

	// #include <linux/jump_label.h>
	#include <linux/kernel/atomic.h>
	#include <asm/page.h>
	#include <asm/processor.h>
	#include <linux/kernel/compiler.h>
	// #include <asm/paravirt.h>
	#include <asm/bitops.h>


	#ifdef USE_QSPINLOCK

	/*
	 * Your basic SMP spinlocks, allowing only a single CPU anywhere
	 *
	 * Simple spin lock operations.  There are two variants, one clears IRQ's
	 * on the local processor, one does not.
	 *
	 * These are fair FIFO ticket locks, which support up to 2^16 CPUs.
	 *
	 * (the type definitions are in asm/spinlock_types.h)
	 */
	/* How long a lock should spin before we consider blocking */
	#	define SPIN_THRESHOLD	(1 << 15)
	#	include <asm/qspinlock.h>
	/*
	 * Read-write spinlocks, allowing multiple readers
	 * but only one writer.
	 *
	 * NOTE! it is quite common to have readers in interrupts
	 * but no interrupt writers. For those circumstances we
	 * can "mix" irq-safe locks - any writer needs to get a
	 * irq-safe write-lock, but readers can get non-irqsafe
	 * read-locks.
	 *
	 * On x86, we implement read-write locks using the generic qrwlock with
	 * x86 specific optimization.
	 */
	#	include <asm/qrwlock.h>

	#else
	// #	include <asm/tspinlock.h>
	/* Function prototypes */
		static inline void spinlock_init(arch_spinlock_t *lock) {
			(void)memset(lock, 0U, sizeof(arch_spinlock_t));
		}

		static inline void spinlock_obtain(arch_spinlock_t *lock) {

			/* The lock function atomically increments and exchanges the head
				* counter of the queue. If the old head of the queue is equal to the
				* tail, we have locked the spinlock. Otherwise we have to wait.
				*/
			asm volatile(	"	movl	$0x1,	%%eax	\n"
							"lock xaddl	%%eax,	%[head]	\n"
							"	cmpl	%%eax,	%[tail]	\n"
							"	jz		1f				\n"
							"2: pause					\n"
							"	cmpl	%%eax,	%[tail]	\n"
							"	jnz		2b				\n"
							"1:							\n"
						:
						:	[head] "m"(lock->head),
							[tail] "m"(lock->tail)
						:	"cc", "memory", "eax");
		}

		static inline void spinlock_release(arch_spinlock_t *lock) {
			/* Increment tail of queue */
			asm volatile(	"lock incl	%[tail]			\n"
						:
						:	[tail] "m"(lock->tail)
						:	"cc", "memory");
		}

	#	define arch_spinlock_init(l)		spinlock_init(l)
	// #	define arch_spin_is_locked(l)		queued_spin_is_locked(l)
	// #	define arch_spin_is_contended(l)	queued_spin_is_contended(l)
	// #	define arch_spin_value_unlocked(l)	queued_spin_value_unlocked(l)
	#	define arch_spin_lock(l)			spinlock_obtain(l)
	// #	define arch_spin_trylock(l)		queued_spin_trylock(l)
	#	define arch_spin_unlock(l)			spinlock_release(l)

	#endif

#endif /* _ASM_X86_SPINLOCK_H */
