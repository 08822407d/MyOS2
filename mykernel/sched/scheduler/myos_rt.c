#include "../sched_api.h"
#include <linux/kernel/time_api.h>


static task_s *pick_next_task_myos(rq_s *rq, task_s *prev)
{
	myos_rq_s	*myos_rq = &rq->myos;
	task_s		*retval, *curr_task;
	retval = curr_task = current;
	sched_rt_entity_s *rt = &curr_task->rt;

	ulong used_jiffies = jiffies - myos_rq->last_jiffies;

	// if running time out, make the need_schedule flag of current task
	if ((curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
	{
		BUG_ON(curr_task->__state != TASK_RUNNING &&
				myos_rq->running_lhdr.count <= 0);

		// fetch a task from running_list
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);

		// and insert curr_task back to running_list
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
			{
				// insert idle task to cpu's running-list tail
				list_header_add_to_tail(&myos_rq->running_lhdr, &rt->run_list);
			}
			else
			{
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
				{
					tmp_list = tmp_list->next;
				}
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
			}
		}

		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;

		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
	}
	return retval;
}

DEFINE_SCHED_CLASS(myos_rt) = {
	.pick_next_task = pick_next_task_myos,
};