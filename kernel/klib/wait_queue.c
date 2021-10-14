#include "../include/task.h"

#include "../include/wait_queue.h"

void wq_init(wait_queue_T * wq, task_s * tsk)
{
	list_init(&wq->wq_list, wq);
	wq->task = tsk;
}

void wq_sleep_on(wait_queue_hdr_s * wqhdr)
{
	task_s * curr = curr_tsk;
	wait_queue_T wq;
	wq_init(&wq, curr);
	curr->state = PS_UNINTERRUPTIBLE;
	list_hdr_append(wqhdr, &wq.wq_list);

	schedule();
}

void wq_sleep_on_intrable(wait_queue_hdr_s * wqhdr)
{
	task_s * curr = curr_tsk;
	wait_queue_T wq;
	wq_init(&wq, curr);
	curr->state = PS_INTERRUPTIBLE;
	list_hdr_append(wqhdr, &wq.wq_list);

	schedule();
}

void wq_wakeup(wait_queue_hdr_s * wqhdr)
{
	if (wqhdr->count == 0)
		return;
}