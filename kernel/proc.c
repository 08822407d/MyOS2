#include <sys/types.h>
#include <sys/cdefs.h>
// #include <lib/utils.h>
#include <lib/string.h>

#include "include/glo.h"
#include "include/const.h"
#include "include/proc.h"
#include "include/proto.h"
#include "klib/data_structure.h"

PCB_u ** idle_procs;

PCB_u proc0_PCB __aligned(PROC_KSTACK_SIZE);

proc_s * proc_waiting_list = NULL;

void creat_idles(void);

void proc_init()
{
	arch_init_proc();

	proc_s *proc0	= &proc0_PCB.proc;
	memset(proc0, 0, sizeof(proc_s));
	m_list_init(proc0);
	proc0->proc_jiffies = 5;
	proc0->flags = PF_KTHREAD;
	proc0->pid = get_newpid();

	// complete bsp's cpudata
	percpu_data_s * bsp_cpudata = smp_info[0];
	bsp_cpudata->waiting_count = 
	bsp_cpudata->finished_count = 0;
	bsp_cpudata->waiting_proc =
	bsp_cpudata->finished_proc = NULL;
	bsp_cpudata->curr_proc = proc0;
	bsp_cpudata->proc_jiffies = proc0->proc_jiffies;

	creat_idles();
}


void creat_idles()
{
	idle_procs = kmalloc(kparam.lcpu_nr * sizeof(PCB_u *));
	idle_procs[0] = &proc0_PCB;
	for (int i = 1; i < kparam.lcpu_nr; i++)
	{
		idle_procs[i] = kmalloc(sizeof(PCB_u));
		memset(idle_procs[i], 0, sizeof(PCB_u));
		proc_s * proci = &idle_procs[i]->proc;
		memcpy(proci, &proc0_PCB.proc, sizeof(proc_s));
		proci->pid = get_newpid();
	}
}