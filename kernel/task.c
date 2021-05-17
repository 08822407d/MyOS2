#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/string.h>

#include "include/glo.h"
#include "include/const.h"
#include "include/task.h"
#include "include/proto.h"

#include "arch/amd64/include/exclusive.h"

// de attention that before entering kmain, rsp had already point to stack of task0,
// in pre_init() .bss section will be set 0, so here arrange task0 in .data section
PCB_u			task0_PCB __aligned(TASK_KSTACK_SIZE) __attribute__((section(".data")));

PCB_u **		idle_tasks;
task_queue_s	idle_queue;
spinlock_T		idle_queue_lock;

task_list_s		global_ready_task;
spinlock_T		global_ready_task_lock;
task_list_s		global_blocked_task;
spinlock_T		global_blocked_task_lock;


void init_task()
{
	arch_init_task();

	task_s *task0	= &task0_PCB.task;
	memset(task0, 0, sizeof(task_s));
	m_list_init(task0);
	task0->time_slice = 2;
	task0->vruntime = -1;
	task0->flags = PF_KTHREAD;
	task0->pid = get_newpid();

	// complete bsp's cpudata_p
	percpu_data_s * bsp_cpudata = percpu_data[0];
	bsp_cpudata->ready_tasks.count = 
	bsp_cpudata->ready_tasks.count = 0;
	bsp_cpudata->ready_tasks.head_p =
	bsp_cpudata->finished_tasks.head_p = NULL;
	bsp_cpudata->curr_task = task0;
	bsp_cpudata->time_slice = task0->time_slice;

	init_spinlock(&idle_queue_lock);
}

/*==============================================================================================*
 *									load_balance related functions							 		*                        
 *==============================================================================================*/
void idle_enqueue(task_s * idle)
{
	if (idle_queue.nr_curr >= idle_queue.nr_max)
		return;

	lock_spinlock(&idle_queue_lock);
	// make sure the load_balance task always the first in queue
	if (idle == idle_queue.sched_task)
	{
		idle_queue.head = (idle_queue.head - 1 + idle_queue.nr_max) % idle_queue.nr_max;
		idle_queue.queue[idle_queue.head] = idle;
	}
	else
	{
		idle_queue.queue[idle_queue.tail] = idle;
		idle_queue.tail = (idle_queue.tail + 1) % idle_queue.nr_max;
	}
	idle_queue.nr_curr++;
	unlock_spinlock(&idle_queue_lock);
}

task_s * idle_dequeue()
{
	task_s * ret_val = NULL;
	lock_spinlock(&idle_queue_lock);
	if (idle_queue.nr_curr > 0)
	{
		ret_val = idle_queue.queue[idle_queue.head];
		idle_queue.head = (idle_queue.head + 1) % idle_queue.nr_max;
	}
	idle_queue.nr_curr--;
	unlock_spinlock(&idle_queue_lock);

	return ret_val;
}

void task_list_push(task_list_s * list, task_s * task)
{
	if (list == NULL || task == NULL)
		return;
	
	if (list->count == 0)
		list->head_p = task;
	else
		__m_list_insert_front(task, list->head_p);

	list->head_p = list->head_p->prev;
	list->count++;
}

task_s * task_list_pop(task_list_s * list)
{
	task_s * ret_val = NULL;
	if (list != NULL)
	{
		ret_val = list->head_p;
		list->head_p = list->head_p->next;

		if (list->count == 1)
			list->head_p = NULL;
		else
			m_list_delete(ret_val);

		list->count--;
	}
	return ret_val;
}