#ifndef __LINUX_READWRITE_SEMAPHORE_H_
#define __LINUX_READWRITE_SEMAPHORE_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG


	#endif

	#include "rwsem_macro.h"
	
	#if defined(READWRITE_SEMAPHORE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		int
		rwsem_is_locked(rwsem_t *sem) {
			return atomic_long_read(&sem->count) != RWSEM_UNLOCKED_VALUE;
		}

		PREFIX_STATIC_INLINE
		void
		rwsem_assert_held_nolockdep(const rwsem_t *sem) {
			WARN_ON(atomic_long_read(&sem->count) == RWSEM_UNLOCKED_VALUE);
		}

		PREFIX_STATIC_INLINE
		void
		rwsem_assert_held_write_nolockdep(const rwsem_t *sem) {
			WARN_ON(!(atomic_long_read(&sem->count) & RWSEM_WRITER_LOCKED));
		}

		/*
		 * This is the same regardless of which rwsem implementation that is being used.
		 * It is just a heuristic meant to be called by somebody already holding the
		 * rwsem to see if somebody from an incompatible type is wanting access to the
		 * lock.
		 */
		PREFIX_STATIC_INLINE
		int
		rwsem_is_contended(rwsem_t *sem) {
			return !list_empty(&sem->wait_list);
		}

		/*
		 * The functions below are the same for all rwsem implementations including
		 * the RT specific variant.
		 */

		PREFIX_STATIC_INLINE
		void
		rwsem_assert_held(const rwsem_t *sem) {
			// if (IS_ENABLED(CONFIG_LOCKDEP))
			// 	lockdep_assert_held(sem);
			// else
				rwsem_assert_held_nolockdep(sem);
		}

		PREFIX_STATIC_INLINE
		void
		rwsem_assert_held_write(const rwsem_t *sem) {
			// if (IS_ENABLED(CONFIG_LOCKDEP))
			// 	lockdep_assert_held_write(sem);
			// else
				rwsem_assert_held_write_nolockdep(sem);
		}

	#endif /* !DEBUG */

#endif /* __LINUX_SEMAPHORE_H_ */