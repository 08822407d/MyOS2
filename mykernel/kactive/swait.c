// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#include <linux/kernel/sched.h>


void __init_swait_queue_head(swqueue_hdr_s *q)
{
	spin_lock_init(&q->lock);
	INIT_LIST_HDR_S(&q->task_list_hdr);
}

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
void swake_up_locked(swqueue_hdr_s *q) {
	swqueue_s *curr;

	if (q->task_list_hdr.count == 0)
		return;

	List_s *lp = list_hdr_dequeue(&q->task_list_hdr);
	curr = container_of(lp, swqueue_s, task_list);
	wake_up_process(curr->task);
}

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
void swake_up_all_locked(swqueue_hdr_s *q) {
	while (&q->task_list_hdr.count != 0)
		swake_up_locked(q);
}


void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	wait->task = current;
	if (list_node_empty(&wait->task_list))
		list_hdr_enqueue(&q->task_list_hdr, &wait->task_list);
}

void __finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	__set_current_state(TASK_RUNNING);
	if (!list_node_empty(&wait->task_list))
		list_hdr_delete(&q->task_list_hdr, &wait->task_list);
}

// void finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
// 	unsigned long flags;

// 	__set_current_state(TASK_RUNNING);

// 	// if (!list_node_empty_careful(&wait->task_list)) {
// 	// 	flags = raw_spin_lock_irqsave(&q->lock);
// 	// 	list_del_init(&wait->task_list);
// 	// 	raw_spin_unlock_irqrestore(&q->lock, flags);
// 	// }
// }