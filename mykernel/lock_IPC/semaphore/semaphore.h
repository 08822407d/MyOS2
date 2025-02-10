#ifndef __LINUX_SEMAPHORE_H_
#define __LINUX_SEMAPHORE_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern void
		sema_init(sema_t *sem, int val);

		extern int
		___down_common(sema_t *sem, long state, long timeout);
	
		extern int
		__down_common(sema_t *sem, long state, long timeout);

	#endif

	#include "semaphore_macro.h"
	
	#if defined(SEMAPHORE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		sema_init(sema_t *sem, int val) {
			// static struct lock_class_key __key;
			*sem = (sema_t) __SEMAPHORE_INITIALIZER(*sem, val);
			// lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
		}

		/*
		 * Because this function is inlined, the 'state' parameter will be
		 * constant, and thus optimised away by the compiler.  Likewise the
		 * 'timeout' parameter for the cases without timeouts.
		 */
		PREFIX_STATIC_INLINE
		int __sched
		___down_common(sema_t *sem, long state, long timeout) {
			struct semaphore_waiter waiter;

			list_add_tail(&waiter.list, &sem->wait_list);
			waiter.task = current;
			waiter.up = false;

			for (;;) {
				if (signal_pending_state(state, current))
					goto interrupted;
				if (unlikely(timeout <= 0))
					goto timed_out;
				__set_current_state(state);
				// raw_spin_unlock_irq(&sem->lock);
				spin_unlock_irq_no_resched(&sem->lock);
				timeout = schedule_timeout(timeout);
				// raw_spin_lock_irq(&sem->lock);
				spin_lock_irq(&sem->lock);
				if (waiter.up)
					return 0;
			}

		timed_out:
			list_del(&waiter.list);
			return -ETIME;

		interrupted:
			list_del(&waiter.list);
			return -EINTR;
		}

		PREFIX_STATIC_INLINE
		int __sched
		__down_common(sema_t *sem, long state, long timeout) {
			int ret;

			// trace_contention_begin(sem, 0);
			ret = ___down_common(sem, state, timeout);
			// trace_contention_end(sem, ret);

			return ret;
		}

	#endif /* !DEBUG */

#endif /* __LINUX_SEMAPHORE_H_ */