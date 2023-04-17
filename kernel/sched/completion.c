// SPDX-License-Identifier: GPL-2.0
/*
 * Generic wait-for-completion handler;
 *
 * It differs from semaphores in that their default case is the opposite,
 * wait_for_completion default blocks whereas semaphore default non-block. The
 * interface also makes it easy to 'complete' multiple waiting threads,
 * something which isn't entirely natural for semaphores.
 *
 * But more importantly, the primitive documents the usage. Semaphores would
 * typically be used for exclusion which gives rise to priority inversion.
 * Waiting for completion is a typically sync point, but not an exclusion point.
 */
#include "sched.h"

/**
 * complete: - signals a single thread waiting on this completion
 * @x:  holds the state of this particular completion
 *
 * This will wake up a single thread waiting on this completion. Threads will be
 * awakened in the same order in which they were queued.
 *
 * See also complete_all(), wait_for_completion() and related routines.
 *
 * If this function wakes up a task, it executes a full memory barrier before
 * accessing the task state.
 */
void complete(completion_s *x)
{
	unsigned long flags;

	flags = raw_spin_lock_irqsave(&x->wait.lock);

	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}


static inline long
do_wait_for_common(completion_s *x, long timeout, int state) {
	if (!x->done) {
		DECLARE_SWAITQUEUE(wait);

		do {
			// if (signal_pending_state(state, current)) {
			// 	timeout = -ERESTARTSYS;
			// 	break;
			// }
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.lock);
			// timeout = action(timeout);
			schedule();
			raw_spin_lock_irq(&x->wait.lock);
		// } while (!x->done && timeout);
		} while (!x->done);
		__finish_swait(&x->wait, &wait);
		if (!x->done)
			return timeout;
	}
	if (x->done != UINT_MAX)
		x->done--;
	return timeout ?: 1;
}

static inline long
wait_for_common(completion_s*x, long timeout, int state) {
	// might_sleep();

	raw_spin_lock_irq(&x->wait.lock);
	timeout = do_wait_for_common(x, timeout, state);
	raw_spin_unlock_irq(&x->wait.lock);

	return timeout;
}

/**
 * wait_for_completion: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout.
 *
 * See also similar routines (i.e. wait_for_completion_timeout()) with timeout
 * and interrupt capability. Also see complete().
 */
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}