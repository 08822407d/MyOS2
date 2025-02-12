#ifndef _LINUX_COMPLETION_H_
#define _LINUX_COMPLETION_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../kactive_const.h"
	#include "../kactive_types.h"
	#include "../kactive_api.h"

	#ifdef DEBUG

		extern void
		init_completion(completion_s *x);
		
		extern long
		do_wait_for_common(completion_s *x,
				long (*action)(long), long timeout, int state);

		extern long
		wait_for_common(completion_s*x, long timeout, int state);

	#endif

	#include "completion_macro.h"

	#if defined(COMPLETION_DEFINATION) || !(DEBUG)

		/**
		 * init_completion - Initialize a dynamically allocated completion
		 * @x:  pointer to completion structure that is to be initialized
		 *
		 * This inline function will initialize a dynamically created completion
		 * structure.
		 */
		PREFIX_STATIC_INLINE
		void
		init_completion(completion_s *x) {
			x->done = 0;
			init_swait_queue_head(&x->wait);
		}

		PREFIX_STATIC_INLINE
		long
		do_wait_for_common(completion_s *x,
				long (*action)(long), long timeout, int state) {

			if (!x->done) {
				DECLARE_SWAITQUEUE(wait);

				do {
					if (signal_pending_state(state, current)) {
						timeout = -ERESTARTSYS;
						break;
					}
					__prepare_to_swait(&x->wait, &wait);
					__set_current_state(state);
					spin_unlock_irq(&x->wait.lock);
					timeout = action(timeout);
					spin_lock_irq(&x->wait.lock);
				} while (!x->done && timeout);
				__finish_swait(&x->wait, &wait);
				if (!x->done)
					return timeout;
			}
			if (x->done != UINT_MAX)
				x->done--;
			return timeout ?: 1;
		}

		PREFIX_STATIC_INLINE
		long
		wait_for_common(completion_s*x, long timeout, int state) {
			// might_sleep();

			spin_lock_irq(&x->wait.lock);
			timeout = do_wait_for_common(x, schedule_timeout, timeout, state);
			spin_unlock_irq(&x->wait.lock);

			return timeout;
		}

	#endif /* !DEBUG */

#endif /* _LINUX_COMPLETION_H_ */