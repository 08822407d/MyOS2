#define RUNQUEUE_DEFINATION
#include "runqueue.h"

/* SPDX-License-Identifier: GPL-2.0 */

void set_user_nice(task_s *p, long nice)
{
	bool queued, running;
	rq_s *rq;
	int old_prio;

	// if (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
	// 	return;
	// /*
	//  * We have to be careful, if called from sys_setpriority(),
	//  * the task might be in the middle of scheduling on another CPU.
	//  */
	// CLASS(task_rq_lock, rq_guard)(p);
	// rq = rq_guard.rq;

	// update_rq_clock(rq);

	// /*
	//  * The RT priorities are set via sched_setscheduler(), but we still
	//  * allow the 'normal' nice value to be set - but as expected
	//  * it won't have any effect on scheduling until the task is
	//  * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	//  */
	// if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
	// 	p->static_prio = NICE_TO_PRIO(nice);
	// 	return;
	// }

	// queued = task_on_rq_queued(p);
	// running = task_current(rq, p);
	// if (queued)
	// 	dequeue_task(rq, p, DEQUEUE_SAVE | DEQUEUE_NOCLOCK);
	// if (running)
	// 	put_prev_task(rq, p);

	// p->static_prio = NICE_TO_PRIO(nice);
	// set_load_weight(p, true);
	// old_prio = p->prio;
	// p->prio = effective_prio(p);

	// if (queued)
	// 	enqueue_task(rq, p, ENQUEUE_RESTORE | ENQUEUE_NOCLOCK);
	// if (running)
	// 	set_next_task(rq, p);

	// /*
	//  * If the task increased its priority or is running and
	//  * lowered its priority, then reschedule its CPU:
	//  */
	// p->sched_class->prio_changed(rq, p, old_prio);
}
EXPORT_SYMBOL(set_user_nice);