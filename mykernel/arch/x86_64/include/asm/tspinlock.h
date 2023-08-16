/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_TSPINLOCK_H
#define _ASM_X86_TSPINLOCK_H

	// #include <linux/jump_label.h>
	#include <asm/cpufeature.h>
	// #include <asm/paravirt.h>
	#include <asm/rmwcc.h>

	#include <asm-generic/tspinlock_types.h>
	#include <linux/lib/string.h>

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

	#define arch_spinlock_init(l)		spinlock_init(l)
	// #define arch_spin_is_locked(l)		queued_spin_is_locked(l)
	// #define arch_spin_is_contended(l)	queued_spin_is_contended(l)
	// #define arch_spin_value_unlocked(l)	queued_spin_value_unlocked(l)
	#define arch_spin_lock(l)			spinlock_obtain(l)
	// #define arch_spin_trylock(l)		queued_spin_trylock(l)
	#define arch_spin_unlock(l)			spinlock_release(l)
	
#endif /* _ASM_X86_TSPINLOCK_H */
