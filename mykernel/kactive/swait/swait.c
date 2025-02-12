#define SWAIT_DEFINATION
#include "swait.h"

// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#include <linux/kernel/sched.h>


void __init_swait_queue_head(swqueue_hdr_s *q)
{
	spin_lock_init(&q->lock);
	INIT_LIST_HEADER_S(&q->task_list_hdr);
}

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
void swake_up_locked(swqueue_hdr_s *q, int wake_flags)
{
	swqueue_s *curr;
	if (list_header_is_empty(&q->task_list_hdr))
		return;
	curr = list_headr_first_container(&q->task_list_hdr, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
void swake_up_all_locked(swqueue_hdr_s *q) {
	while (!list_header_is_empty(&q->task_list_hdr))
		swake_up_locked(q, 0);
}


void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	wait->task = current;
	if (list_is_empty_entry(&wait->task_list))
		list_header_add_to_tail(&q->task_list_hdr, &wait->task_list);
}

void __finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	__set_current_state(TASK_RUNNING);
	if (!list_is_empty_entry(&wait->task_list))
		list_header_delete_node(&q->task_list_hdr, &wait->task_list);
}

void finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	ulong flags;
	__set_current_state(TASK_RUNNING);
	if (!list_is_empty_entry_careful(&wait->task_list)) {
		spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		spin_unlock_irqrestore(&q->lock, flags);
	}
}