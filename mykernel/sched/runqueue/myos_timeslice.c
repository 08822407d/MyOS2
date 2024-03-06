#include <linux/sched/sched.h>

static task_s *pick_next_task_myos(rq_s *rq)
{
	myos_rq_s	*myos_rq = &rq->myos;
	task_s		*retval, *curr_task;
	retval = curr_task = current;

	unsigned long used_jiffies = jiffies - myos_rq->last_jiffies;

	// if running time out, make the need_schedule flag of current task
	if ((curr_task == rq->idle ||
		used_jiffies >= myos_rq->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
	{
		BUG_ON(curr_task->__state != TASK_RUNNING &&
				myos_rq->running_lhdr.count <= 0);

		// fetch a task from running_list
		List_s * next_lp = list_hdr_pop(&myos_rq->running_lhdr);
		while (next_lp == 0);

		// and insert curr_task back to running_list
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
			{
				// insert idle task to cpu's running-list tail
				list_hdr_enqueue(&myos_rq->running_lhdr, &curr_task->tasks);
			}
			else
			{
				List_s * tmp_list = myos_rq->running_lhdr.header.next;
				while ((curr_task->se.vruntime > container_of(tmp_list, task_s, tasks)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.header)
				{
					tmp_list = tmp_list->next;
				}
				list_insert_prev(tmp_list, &curr_task->tasks);
				myos_rq->running_lhdr.count++;
			}
		}

		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;

		retval = container_of(next_lp, task_s, tasks);
		myos_rq->time_slice = retval->rt.time_slice;
		if (used_jiffies >= myos_rq->time_slice)
			myos_rq->last_jiffies = jiffies;
	}
	return retval;
}

DEFINE_SCHED_CLASS(myos_timeslice) = {
	.pick_next_task = pick_next_task_myos,
};