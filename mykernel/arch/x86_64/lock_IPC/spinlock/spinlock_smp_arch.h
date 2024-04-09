/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_H_
#define _ASM_X86_SPINLOCK_H_

	#include <linux/compiler/compiler.h>
	#include <linux/compiler/myos_debug_option.h>
	#include <linux/kernel/lock_ipc.h>

	#include "spinlock_types_arch.h"


	#ifdef DEBUG

		extern void
		arch_spin_init(arch_spinlock_t *lock);

		extern int
		arch_spin_is_locked(arch_spinlock_t *lock);

		extern bool
		arch_spin_trylock(arch_spinlock_t *lock);

		extern void
		arch_spin_lock(arch_spinlock_t *lock);

		extern void
		arch_spin_unlock(arch_spinlock_t *lock);

	#endif
	
	#if defined(ARCH_SPINLOCK_SMP_DEFINATION) || !(DEBUG)

		/* Function prototypes */
		PREFIX_STATIC_INLINE
		void
		arch_spin_init(arch_spinlock_t *lock) {
			*lock = (arch_spinlock_t)__ARCH_SPIN_LOCK_UNLOCKED;
		}

		PREFIX_STATIC_INLINE
		int
		arch_spin_is_locked(arch_spinlock_t *lock) {
			u64 old = atomic_long_read(&lock->val);
			return ((old >> 32) != (old & 0xffffffff));
		}

		PREFIX_STATIC_INLINE
		bool
		arch_spin_trylock(arch_spinlock_t *lock) {
			u64 old = atomic_long_read(&lock->val);
			if ((old >> 32) != (old & 0xffffffff))
				return false;
			else
				return true;
		}

		PREFIX_STATIC_INLINE
		void
		arch_spin_lock(arch_spinlock_t *lock) {
			/* The lock function atomically increments and exchanges the head
			 * counter of the queue. If the old head of the queue is equal to the
			 * tail, we have locked the spinlock. Otherwise we have to wait.
			 */
			asm volatile(	"	movl	$0x1,	%%eax	\n"
						LOCK_PREFIX
							"	xaddl	%%eax,	%[head]	\n"
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

		PREFIX_STATIC_INLINE
		void
		arch_spin_unlock(arch_spinlock_t *lock) {
			/* Increment tail of queue */
			asm volatile(LOCK_PREFIX
							"incl	%[tail]			\n"
						:
						:	[tail] "m"(lock->tail)
						:	"cc", "memory"
						);
		}

	#endif

#endif /* _ASM_X86_SPINLOCK_H_ */
