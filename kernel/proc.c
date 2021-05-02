#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>
#include <lib/string.h>

#include "include/glo.h"
#include "include/const.h"
#include "include/proc.h"
#include "include/proto.h"

PCB_u ** idle_procs;

PCB_u proc0_PCB __aligned(PROC_KSTACK_SIZE);
PCB_u proc1_PCB __aligned(PROC_KSTACK_SIZE);


void creat_idles(void);

void proc_init()
{
	proc_s *proc0	= &proc0_PCB.proc;
	memset(proc0, 0, sizeof(proc_s));
	list_init(&proc0->PCB_list);
	proc0->counter	= 0;
	proc0->pid		= 0;

	proc_s *next	= container_of(&proc0->PCB_list, proc_s, PCB_list);

	creat_idles();

	// memset(&proc1_PCB.proc, 0, sizeof(proc1_PCB.proc));

	// arch_init_proc();
}


void creat_idles()
{
	idle_procs = kmalloc(kparam.lcpu_nr * sizeof(PCB_u *));
	for (int i = 0; i < kparam.lcpu_nr; i++)
	{
		idle_procs[i] = kmalloc(sizeof(PCB_u));
		memset(idle_procs[i], 0, sizeof(PCB_u));
	}
}