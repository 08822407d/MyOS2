#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>
#include <lib/string.h>

#include "include/const.h"
#include "include/proc.h"

PCB_u proc0_PCB __aligned(PROC_KSTACK_SIZE);
PCB_u proc1_PCB __aligned(PROC_KSTACK_SIZE);

void init_proc0()
{
	proc_s *proc0	= &proc0_PCB.proc;
	memset(proc0, 0, sizeof(proc_s));
	list_init(&proc0->PCB_list);
	proc0->counter	= 0;
	proc0->pid		= 0;

	proc_s *next	= container_of(&proc0->PCB_list, proc_s, PCB_list);

	memset(&proc1_PCB.proc, 0, sizeof(proc1_PCB.proc));

	arch_init_proc0();
}