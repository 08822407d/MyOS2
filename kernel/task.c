#include <stddef.h>
#include <sys/types.h>
#include <sys/cdefs.h>

#include <string.h>

#include <include/glo.h>
#include <include/mm/mm.h>
#include <include/mm/memblock.h>
#include <include/task.h>
#include <include/proto.h>

#include <arch/amd64/include/arch_glo.h>
#include <arch/amd64/include/mutex.h>

extern char _text;
extern char _etext;
extern char _rodata;
extern char _erodata;
extern char _data;
extern char _edata;
extern char _bss;
extern char _ebss;


size_t			cpustack_off;
PCB_u **		idle_tasks;
// de attention that before entering start_kernel, rsp had already point to stack of task0,
// in pre_init_sytem() .bss section will be set 0, so here arrange task0 in .data section
PCB_u			task0_PCB __aligned(TASK_KSTACK_SIZE) __attribute__((section(".data")));
mm_s			task0_mm = 
{
	.start_code		= (reg_t)&_text,
	.end_code		= (reg_t)&_etext,
	.start_rodata	= (reg_t)&_rodata,
	.end_rodata		= (reg_t)&_erodata,
	.start_data		= (reg_t)&_data,
	.end_data		= (reg_t)&_edata,
	.start_bss		= (reg_t)&_bss,
	.end_bss		= (reg_t)&_bss,
	.start_stack	= 0,
};
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
	list_init(&idletask->schedule_list, idletask);
	list_init(&idletask->child_list, idletask);
	list_hdr_init(&idletask->child_lhdr);
	idletask->pid = gen_newpid();
}

void init_task(size_t lcpu_nr)
{
	compute_consts();

	preinit_arch_task();

	task_s *task0 = &task0_PCB.task;
	
	// set arch struct in mm_s
	task0_mm.cr3			= (reg_t)virt2phys(KERN_PML4);

	task0->time_slice = 2;
	task0->vruntime = -1;
	task0->semaphore_count =
	task0->spin_count = 0;
	task0->state = PS_RUNNING;
	task0->flags = PF_KTHREAD;
	task0->mm_struct = &task0_mm;
	task0->fs = &task0_fs;

	for (int i = 0; i < lcpu_nr; i++)
	{
		create_smp_idles(i);
		init_arch_task(i);
	}
}

void compute_consts()
{
	cpudata_u * base_p = 0;
	cpustack_off = (void *)&(base_p->cpudata.cpustack_p) - (void *)base_p;
}

void prepare_init_task(size_t lcpu_nr)
{
	idle_tasks = memblock_alloc_normal(lcpu_nr * sizeof(PCB_u *), sizeof(PCB_u *));
	idle_tasks[0] = &task0_PCB;
	for (int i = 1; i < lcpu_nr; i++)
	{
		idle_tasks[i] = memblock_alloc_normal(sizeof(PCB_u), sizeof(PCB_u));
	}
}