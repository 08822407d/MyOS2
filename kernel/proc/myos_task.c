#include <linux/kernel/sched.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>
#include <linux/kernel/fdtable.h>

#include <linux/kernel/asm-generic/sections.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/arch_glo.h>


size_t			cpustack_off;
PCB_u **		idle_tasks;
// de attention that before entering start_kernel, rsp had already point to stack of task0,
// in myos_early_init_system() .bss section will be set 0, so here arrange task0 in .data section
files_struct_s	task0_files;
PCB_u			task0_PCB __aligned(TASK_KSTACK_SIZE) __section(".data");
taskfs_s		task0_fs =
{
	.in_exec		= 0,
	.umask			= 0,
	.users			= 0,
	.pwd.mnt		= NULL,
	.root.mnt		= NULL,
};

void compute_consts(void);

static void create_smp_idles(size_t cpu_idx)
{
	PCB_u * idle_pcb = idle_tasks[cpu_idx];
	task_s * idletask = &(idle_pcb->task);
	idletask->parent = idletask;
	memcpy(idletask, &task0_PCB.task, sizeof(task_s));
	list_init(&idletask->tasks, idletask);
	list_init(&idletask->sibling, idletask);
	list_hdr_init(&idletask->children);
	idletask->pid = myos_gen_newpid();
}

void myos_init_task(size_t lcpu_nr)
{
	compute_consts();

	myos_preinit_arch_task();

	task_s *task0 = &task0_PCB.task;
	
	// set arch struct in mm_s
	task0->rt.time_slice	= 2;
	task0->se.vruntime		= -1;
	task0->__state		= TASK_RUNNING;
	task0->flags		= PF_KTHREAD;
	task0->mm			= &init_mm;
	task0->fs			= &task0_fs;
	task0->files		= &task0_files;
	task0->name			= "cpu0_idel";

	for (int i = 0; i < lcpu_nr; i++)
	{
		create_smp_idles(i);
		myos_init_arch_task(i);
	}
}

void compute_consts()
{
	cpudata_u * base_p = 0;
	cpustack_off = (void *)&(base_p->cpudata.cpustack_p) - (void *)base_p;
}

void myos_early_init_task(size_t lcpu_nr)
{
	idle_tasks = myos_memblock_alloc_normal(lcpu_nr * sizeof(PCB_u *), sizeof(PCB_u *));
	idle_tasks[0] = &task0_PCB;
	for (int i = 1; i < lcpu_nr; i++)
	{
		idle_tasks[i] = myos_memblock_alloc_normal(sizeof(PCB_u), sizeof(PCB_u));
	}
}