#include <linux/kernel/sched.h>
#include <obsolete/wait_queue.h>
#include <obsolete/arch_proto.h>

void wq_init(wait_queue_T * wq, task_s * tsk)
{
	list_init(&wq->wq_list, wq);
	wq->task = tsk;
}

void wq_sleep_on(List_hdr_s * wqhdr)
{
	task_s * curr = current;
	wait_queue_T wq;
	wq_init(&wq, curr);
	curr->__state = TASK_UNINTERRUPTIBLE;
	list_hdr_append(wqhdr, &wq.wq_list);

	schedule();
}

void wq_sleep_on_intrable(List_hdr_s * wqhdr)
{
	task_s * curr = current;
	wait_queue_T wq;
	wq_init(&wq, curr);
	curr->__state = TASK_INTERRUPTIBLE;
	list_hdr_append(wqhdr, &wq.wq_list);

	schedule();
}

void wq_wakeup(List_hdr_s * wqhdr, unsigned long pstate)
{
	if (wqhdr->count == 0)
		return;

	List_s * wq_lp = list_hdr_pop(wqhdr);
	while (!wq_lp);

	wait_queue_T * the_wait = container_of(wq_lp, wait_queue_T, wq_list);

	if (the_wait->task->__state & pstate)
	{
		wake_up_process(the_wait->task);
	}
	else
	{
		list_hdr_push(wqhdr, wq_lp);
	}
}