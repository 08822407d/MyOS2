#include <stddef.h>
#include <sys/types.h>
#include <sys/cdefs.h>

#include <string.h>

#include <include/glo.h>
#include <include/mm.h>
#include <include/memblock.h>
#include <include/task.h>
#include <include/proto.h>

#include <arch/amd64/include/arch_glo.h>
#include <arch/amd64/include/mutex.h>

PCB_u **		idle_tasks;
// de attention that before entering kmain, rsp had already point to stack of task0,
// in pre_init() .bss section will be set 0, so here arrange task0 in .data section
PCB_u			task0_PCB __aligned(TASK_KSTACK_SIZE) __attribute__((section(".data")));
mm_s			task0_mm;

size_t			cpustack_off;

extern char _text;
extern char _etext;
extern char _rodata;
extern char _erodata;
extern char _data;
extern char _edata;
extern char _bss;
extern char _ebss;

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
	task0->time_slice = 2;
	task0->vruntime = -1;
	task0->flags = PF_KTHREAD;
	task0->mm_struct = &task0_mm;

	for (int i = 0; i < lcpu_nr; i++)
	{
		create_smp_idles(i);
		init_arch_task(i);
	}

	// set arch struct in mm_s
	mm_s * task0_mm_p = task0_PCB.task.mm_struct;
	task0_mm_p->cr3	= (reg_t)virt2phys(KERN_PML4);
	task0_mm_p->start_code		= (reg_t)&_text;
	task0_mm_p->end_code		= (reg_t)&_etext;
	task0_mm_p->start_rodata	= (reg_t)&_rodata;
	task0_mm_p->end_rodata		= (reg_t)&_erodata;
	task0_mm_p->start_data		= (reg_t)&_data;
	task0_mm_p->end_data		= (reg_t)&_edata;
	task0_mm_p->start_bss		= (reg_t)&_bss;
	task0_mm_p->end_bss			= (reg_t)&_bss;
	task0_mm_p->start_stack		= 0;

	// complete bsp's cpudata_p
	cpudata_u * bsp_cpudata_u_p = percpu_data[0];
	per_cpudata_s * bsp_cpudata_p = &(bsp_cpudata_u_p->cpudata);
	bsp_cpudata_p->curr_task = task0;
	bsp_cpudata_p->time_slice = task0->time_slice;
	list_hdr_init(&bsp_cpudata_p->ruuning_lhdr);
}

void compute_consts()
{
	cpudata_u * base_p = 0;
	cpustack_off = (void *)&(base_p->cpudata.cpustack_p) - (void *)base_p;
}

void preinit_task(size_t lcpu_nr)
{
	idle_tasks = memblock_alloc_normal(lcpu_nr * sizeof(PCB_u *), sizeof(PCB_u *));
	idle_tasks[0] = &task0_PCB;
	for (int i = 1; i < lcpu_nr; i++)
	{
		idle_tasks[i] = memblock_alloc_normal(sizeof(PCB_u), sizeof(PCB_u));
	}
}