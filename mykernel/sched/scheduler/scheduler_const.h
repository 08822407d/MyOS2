/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_CONST_H_
#define _LINUX_SCHEDULER_CONST_H_


	/*
	 * Constants for the sched_mode argument of __schedule().
	 *
	 * The mode argument allows RT enabled kernels to differentiate a
	 * preemption from blocking on an 'sleeping' spin/rwlock.
	 */
	#define SM_IDLE				(-1)
	#define SM_NONE				0
	#define SM_PREEMPT			1
	#define SM_RTLOCK_WAIT		2


	#define MAX_NICE			19
	#define MIN_NICE			-20
	#define NICE_WIDTH			(MAX_NICE - MIN_NICE + 1)

	/*
	 * rq::clock_update_flags bits
	 *
	 * %RQCF_REQ_SKIP - will request skipping of clock update on the next
	 *  call to __schedule(). This is an optimisation to avoid
	 *  neighbouring rq clock updates.
	 *
	 * %RQCF_ACT_SKIP - is set from inside of __schedule() when skipping is
	 *  in effect and calls to update_rq_clock() are being ignored.
	 *
	 * %RQCF_UPDATED - is a debug flag that indicates whether a call has been
	 *  made to update_rq_clock() since the last time rq::lock was pinned.
	 *
	 * If inside of __schedule(), clock_update_flags will have been
	 * shifted left (a left shift is a cheap operation for the fast path
	 * to promote %RQCF_REQ_SKIP to %RQCF_ACT_SKIP), so you must use,
	 *
	 *	if (rq-clock_update_flags >= RQCF_UPDATED)
	 *
	 * to check if %RQCF_UPDATED is set. It'll never be shifted more than
	 * one position though, because the next rq_unpin_lock() will shift it
	 * back.
	 */
	#define RQCF_REQ_SKIP		0x01
	#define RQCF_ACT_SKIP		0x02
	#define RQCF_UPDATED		0x04

	/* Wake flags. The first three directly map to some SD flag value */
	#define WF_EXEC				0x02	/* Wakeup after exec; maps to SD_BALANCE_EXEC */
	#define WF_FORK				0x04	/* Wakeup after fork; maps to SD_BALANCE_FORK */
	#define WF_TTWU				0x08	/* Wakeup;            maps to SD_BALANCE_WAKE */

	#define WF_SYNC				0x10	/* Waker goes to sleep after wakeup */
	#define WF_MIGRATED			0x20	/* Internal use, task got migrated */
	#define WF_CURRENT_CPU		0x40	/* Prefer to move the wakee to the current CPU. */
	#define WF_RQ_SELECTED		0x80	/* ->select_task_rq() was called */

	#ifdef CONFIG_SMP
		static_assert(WF_EXEC == SD_BALANCE_EXEC);
		static_assert(WF_FORK == SD_BALANCE_FORK);
		static_assert(WF_TTWU == SD_BALANCE_WAKE);
	#endif

	/*
	 * To aid in avoiding the subversion of "niceness" due to uneven distribution
	 * of tasks with abnormal "nice" values across CPUs the contribution that
	 * each task makes to its run queue's load is weighted according to its
	 * scheduling class and "nice" value. For SCHED_NORMAL tasks this is just a
	 * scaled version of the new time slice allocation that they receive on time
	 * slice expiry etc.
	 */

	#define WEIGHT_IDLEPRIO		3
	#define WMULT_IDLEPRIO		1431655765

	/*
	 * {de,en}queue flags:
	 *
	 * DEQUEUE_SLEEP  - task is no longer runnable
	 * ENQUEUE_WAKEUP - task just became runnable
	 *
	 * SAVE/RESTORE - an otherwise spurious dequeue/enqueue, done to ensure tasks
	 *                are in a known state which allows modification. Such pairs
	 *                should preserve as much state as possible.
	 *
	 * MOVE - paired with SAVE/RESTORE, explicitly does not preserve the location
	 *        in the runqueue.
	 *
	 * NOCLOCK - skip the update_rq_clock() (avoids double updates)
	 *
	 * MIGRATION - p->on_rq == TASK_ON_RQ_MIGRATING (used for DEADLINE)
	 *
	 * ENQUEUE_HEAD      - place at front of runqueue (tail if not specified)
	 * ENQUEUE_REPLENISH - CBS (replenish runtime and postpone deadline)
	 * ENQUEUE_MIGRATED  - the task was migrated during wakeup
	 * ENQUEUE_RQ_SELECTED - ->select_task_rq() was called
	 *
	 */

	#define DEQUEUE_SLEEP			0x01	/* Matches ENQUEUE_WAKEUP */
	#define DEQUEUE_SAVE			0x02	/* Matches ENQUEUE_RESTORE */
	#define DEQUEUE_MOVE			0x04	/* Matches ENQUEUE_MOVE */
	#define DEQUEUE_NOCLOCK			0x08	/* Matches ENQUEUE_NOCLOCK */
	#define DEQUEUE_SPECIAL			0x10
	#define DEQUEUE_MIGRATING		0x100	/* Matches ENQUEUE_MIGRATING */
	#define DEQUEUE_DELAYED			0x200	/* Matches ENQUEUE_DELAYED */

	#define ENQUEUE_WAKEUP			0x01
	#define ENQUEUE_RESTORE			0x02
	#define ENQUEUE_MOVE			0x04
	#define ENQUEUE_NOCLOCK			0x08

	#define ENQUEUE_HEAD			0x10
	#define ENQUEUE_REPLENISH		0x20
	#ifdef CONFIG_SMP
	#  define ENQUEUE_MIGRATED		0x40
	#else
	#  define ENQUEUE_MIGRATED		0x00
	#endif
	#define ENQUEUE_INITIAL			0x80
	#define ENQUEUE_MIGRATING		0x100
	#define ENQUEUE_DELAYED			0x200
	#define ENQUEUE_RQ_SELECTED		0x400

	#define RETRY_TASK				((void *)-1UL)

#endif /* _LINUX_SCHEDULER_CONST_H_ */