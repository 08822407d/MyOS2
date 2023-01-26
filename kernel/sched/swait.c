// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#include <linux/sched/sched.h>


/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
void swake_up_locked(swqueue_hdr_s *q)
{
	swqueue_s *curr;

	if (list_empty(&q->task_list))
		return;

	// curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	// wake_up_process(curr->task);
	myos_wake_up_new_task(curr->task);
	// list_del_init(&curr->task_list);
	list_delete(&curr->task_list);
}