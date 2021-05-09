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

// de attention that before entering kmain, rsp had already point to stack of task0,
// in pre_init() .bss section will be set 0, so here arrange task0 in .data section
PCB_u		task0_PCB __aligned(TASK_KSTACK_SIZE) __attribute__((section(".data")));

task_s *	task_waiting_list = NULL;
unsigned long	waiting_task_count = 0;

void creat_idles(void);

void init_task()
{
	arch_init_task();

	task_s *task0	= &task0_PCB.task;
	memset(task0, 0, sizeof(task_s));
	m_list_init(task0);
	task0->task_jiffies = 5;
	task0->flags = PF_KTHREAD;
	task0->pid = get_newpid();

	// complete bsp's cpudata_p
	percpu_data_s * bsp_cpudata = smp_info[0];
	bsp_cpudata->waiting_count = 
	bsp_cpudata->finished_count = 0;
	bsp_cpudata->waiting_task =
	bsp_cpudata->finished_task = NULL;
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
}