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
#include <linux/sched/sched.h>

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