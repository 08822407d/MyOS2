#include <sys/types.h>
#include <sys/cdefs.h>
// #include <lib/utils.h>
#include <lib/string.h>

#include "include/glo.h"
#include "include/const.h"
#include "include/task.h"
#include "include/proto.h"
#include "klib/data_structure.h"

PCB_u **	idle_tasks;
task_queue_s idle_queue;
// de attention that before entering kmain, rsp had already point to stack of task0,
// in pre_init() .bss section will be set 0, so here arrange task0 in .data section
PCB_u		task0_PCB __aligned(TASK_KSTACK_SIZE) __attribute__((section(".data")));

task_list_s global_waiting_task;

void creat_idles(void);

void init_task()
{
	arch_init_task();

	task_s *task0	= &task0_PCB.task;
	memset(task0, 0, sizeof(task_s));
	m_list_init(task0);
	task0->task_jiffies = 2;
	task0->flags = PF_KTHREAD;
	task0->pid = get_newpid();

	// complete bsp's cpudata_p
	percpu_data_s * bsp_cpudata = smp_info[0];
	bsp_cpudata->waiting_tasks.count = 
	bsp_cpudata->waiting_tasks.count = 0;
	bsp_cpudata->waiting_tasks.head_ptr =
	bsp_cpudata->finished_tasks.head_ptr = NULL;
	bsp_cpudata->curr_task = task0;
	bsp_cpudata->task_jiffies = task0->task_jiffies;

	creat_idles();
}

void creat_idles()
{
	idle_tasks = kmalloc(kparam.nr_lcpu * sizeof(PCB_u *));
	idle_tasks[0] = &task0_PCB;
	for (int i = 1; i < kparam.nr_lcpu; i++)
	{
		idle_tasks[i] = kmalloc(sizeof(PCB_u));
		memset(idle_tasks[i], 0, sizeof(PCB_u));
		task_s * procidle = &idle_tasks[i]->task;
		memcpy(procidle, &task0_PCB.task, sizeof(task_s));
		procidle->pid = get_newpid();
	}

	idle_queue.nr_max = kparam.nr_lcpu;
	idle_queue.nr_curr = 0;
	idle_queue.head = 0;
	idle_queue.tail = 0;
	idle_queue.queue = (task_s **)idle_tasks;
	idle_queue.sched_task = &task0_PCB.task;
}

/*==============================================================================================*
 *									schedule related functions							 		*                        
 *==============================================================================================*/
void idle_enqueue(task_s * idle)
{
	if (idle_queue.nr_curr >= idle_queue.nr_max)
	{
		return;
	}
	// make sure the schedule task always the first in queue
	else if (idle == idle_queue.sched_task)
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
}

task_s * idle_dequeue()
{
	task_s * ret_val = NULL;
	if (idle_queue.nr_curr > 0)
	{
		ret_val = idle_queue.queue[idle_queue.head];
		idle_queue.head = (idle_queue.head + 1) % idle_queue.nr_max;
	}
	idle_queue.nr_curr--;

	return ret_val;
}

void task_list_push(task_list_s * list, task_s * task)
{
	if (list == NULL || task == NULL)
		return;
	
	if (list->count == 0)
		list->head_ptr = task;
	else
		m_list_insert_front(task, list->head_ptr);

	list->head_ptr = list->head_ptr->prev;
	list->count++;
}

task_s * task_list_pop(task_list_s * list)
{
	task_s * ret_val = NULL;
	if (list != NULL)
	{
		ret_val = list->head_ptr;
		list->head_ptr = list->head_ptr->next;

		if (list->count == 1)
			list->head_ptr = NULL;
		else
			m_list_delete(ret_val);

		list->count--;
	}
	return ret_val;
}