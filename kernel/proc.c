#include <sys/types.h>
#include <sys/cdefs.h>

#include "include/const.h"
#include "include/proc.h"

PCB_u proc0_PCB __aligned(PSTACK_SIZE);

void init_proc0()
{
	memset(&proc0_PCB, 0, sizeof(PCB_u));
	proc_s *proc0	= &proc0_PCB.proc;
	list_init(&proc0->PCB_list);
	proc0->counter	= 0;
	proc0->pid		= 0;
}