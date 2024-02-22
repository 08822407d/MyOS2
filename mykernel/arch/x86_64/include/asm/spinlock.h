/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_H
#define _ASM_X86_SPINLOCK_H

	// #include <linux/jump_label.h>
	#include <linux/kernel/atomic.h>
	#include <asm/page.h>
	#include <asm/processor.h>
	#include <linux/compiler/compiler.h>
	// #include <asm/paravirt.h>
	#include <asm/bitops.h>

	// #include <asm/tspinlock.h>
	/* Function prototypes */
	static inline void arch_spin_init(arch_spinlock_t *lock) {
		// (void)memset(lock, 0U, sizeof(arch_spinlock_t));
		*lock = (arch_spinlock_t)__ARCH_SPIN_LOCK_UNLOCKED;
	}

	static inline int arch_spin_is_locked(arch_spinlock_t *lock)
	{
		u64 old = atomic_long_read(&lock->val);
		return ((old >> 32) != (old & 0xffffffff));
	}

	// static inline bool arch_spin_trylock(arch_spinlock_t *lock)
	// {
	// 	u64 old = atomic_long_read(&lock->val);
	// 	if ((old >> 32) != (old & 0xffffffff))
	// 		return false;
	// 	else
	// 		return true;
	// }

	static inline void arch_spin_lock(arch_spinlock_t *lock) {
		/* The lock function atomically increments and exchanges the head
		 * counter of the queue. If the old head of the queue is equal to the
		 * tail, we have locked the spinlock. Otherwise we have to wait.
		 */
		asm volatile(	"	movl	$0x1,	%%eax	\n"
						"lock xaddl	%%eax,	%[head]	\n"
						"	cmpl	%%eax,	%[tail]	\n"
						"	jz		1f				\n"
						"2:	pause					\n"
						"	cmpl	%%eax,	%[tail]	\n"
						"	jnz		2b				\n"
						"1:							\n"
					:
					:	[head] "m"(lock->head),
						[tail] "m"(lock->tail)
					:	"cc", "memory", "eax"
					);
	}

	static inline void arch_spin_unlock(arch_spinlock_t *lock) {
		/* Increment tail of queue */
		asm volatile(	"lock incl	%[tail]			\n"
					:
					:	[tail] "m"(lock->tail)
					:	"cc", "memory"
					);
	}

#endif /* _ASM_X86_SPINLOCK_H */
