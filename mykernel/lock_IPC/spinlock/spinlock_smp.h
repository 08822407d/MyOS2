#ifndef __LINUX_SPINLOCK_SMP_H_
#define __LINUX_SPINLOCK_SMP_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern void
		raw_spin_lock_irqsave(arch_spinlock_t *lock, ulong *flags);

		extern void
		raw_spin_lock_irq(arch_spinlock_t *lock);

		extern void
		raw_spin_lock_bh(arch_spinlock_t *lock);

		extern void
		raw_spin_lock(arch_spinlock_t *lock);

		extern void
		raw_spin_unlock(arch_spinlock_t *lock);

		extern void
		raw_spin_unlock_irqrestore(arch_spinlock_t *lock, ulong *flags);

		extern void
		raw_spin_unlock_irq(arch_spinlock_t *lock);

		extern void
		raw_spin_unlock_bh(arch_spinlock_t *lock);

		// int
		// raw_spin_trylock_bh(arch_spinlock_t *lock);

	#endif

	#include "spinlock_smp_macro.h"
	
	#if defined(SPINLOCK_SMP_DEFINATION) || !(DEBUG)

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
		// static inline unsigned long
		// __raw_spin_lock_irqsave(raw_spinlock_t *lock)
		PREFIX_STATIC_INLINE
		void
		raw_spin_lock_irqsave(arch_spinlock_t *lock, ulong *flags) {
			local_irq_save(*flags);
			preempt_disable();
			arch_spin_lock(lock);
		}

		// static inline void __raw_spin_lock_irq(raw_spinlock_t *lock)
		PREFIX_STATIC_INLINE
		void
		raw_spin_lock_irq(arch_spinlock_t *lock) {
			local_irq_disable();
			preempt_disable();
			arch_spin_lock(lock);
		}

		// static inline void __raw_spin_lock_bh(raw_spinlock_t *lock) {
		PREFIX_STATIC_INLINE
		void
		raw_spin_lock_bh(arch_spinlock_t *lock) {
			arch_spin_lock(lock);
		}

		// static inline void __raw_spin_lock(raw_spinlock_t *lock)
		PREFIX_STATIC_INLINE
		void
		raw_spin_lock(arch_spinlock_t *lock) {
			preempt_disable();
			arch_spin_lock(lock);
		}


		// static inline void __raw_spin_unlock(raw_spinlock_t *lock) {
		PREFIX_STATIC_INLINE
		void
		raw_spin_unlock(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
			preempt_enable();
		}

		// static inline void __raw_spin_unlock_irqrestore(
		// 		raw_spinlock_t *lock, unsigned long flags)
		PREFIX_STATIC_INLINE
		void
		raw_spin_unlock_irqrestore(arch_spinlock_t *lock, ulong *flags) {
			arch_spin_unlock(lock);
			local_irq_restore(*flags);
			preempt_enable();
		}

		// static inline void __raw_spin_unlock_irq(raw_spinlock_t *lock) {
		PREFIX_STATIC_INLINE
		void
		raw_spin_unlock_irq(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
			local_irq_enable();
			preempt_enable();
		}

		// static inline void __raw_spin_unlock_bh(raw_spinlock_t *lock) {
		PREFIX_STATIC_INLINE
		void
		raw_spin_unlock_bh(arch_spinlock_t *lock) {
			arch_spin_unlock(lock);
		}

		// static inline int __raw_spin_trylock_bh(raw_spinlock_t *lock) {
		// 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// 	if (do_raw_spin_trylock(lock)) {
		// 		spin_acquire(&lock->dep_map, 0, 1, _RET_IP_);
		// 		return 1;
		// 	}
		// 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
		// 	return 0;
		// }

	#endif /* !DEBUG */

#endif /* __LINUX_SPINLOCK_SMP_H_ */