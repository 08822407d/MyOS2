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

	List_s * wq_lp = list_hdr_pop(wqhdr);
	wait_queue_T * the_wait = container_of(wq_lp, wait_queue_T, wq_list);

	if (the_wait->task->state & PS_UNINTERRUPTIBLE)
	{
		wakeup_task(the_wait->task);
	}
	else
	{
		list_hdr_push(wqhdr, wq_lp);
	}
}