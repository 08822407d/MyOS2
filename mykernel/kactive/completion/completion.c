#define COMPLETION_DEFINATION
#include "completion.h"


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
#include <linux/kernel/sched.h>

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
	ulong flags;
	spin_lock_irqsave(&x->wait.lock, flags);
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
	spin_unlock_irqrestore(&x->wait.lock, flags);
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