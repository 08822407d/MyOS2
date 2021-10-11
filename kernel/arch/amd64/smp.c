#include <lib/string.h>
#include <lib/stddef.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/arch_glo.h"
#include "include/mutex.h"

#include "../../include/glo.h"
#include "../../include/printk.h"
#include "../../include/proto.h"
#include "../../include/const.h"
#include "../../include/task.h"
#include "../../klib/data_structure.h"

extern PCB_u	task0_PCB;

cpudata_u **	percpu_data;

void create_percpu_idle(size_t cpu_idx);
void init_percpu_data(size_t cpu_idx);
void init_percpu_arch_data(size_t cpu_idx);

void init_smp_env()
{
	#ifdef DEBUG
		// make sure have get smp information
	#endif

	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apbbot_len = &_APboot_etext - &_APboot_text;
	memcpy(phys2virt((phys_addr)&_APboot_phys_start), (virt_addr)&_APboot_text, apbbot_len);

	// init basic data for percpu
	unsigned nr_lcpu = kparam.nr_lcpu;

	idle_tasks	= (PCB_u **)kmalloc(kparam.nr_lcpu * sizeof(PCB_u *));
	tss_ptr_arr	= (tss64_T **)kmalloc(nr_lcpu * sizeof(tss64_T *));
	percpu_data	= (cpudata_u **)kmalloc(kparam.nr_lcpu * sizeof(cpudata_u *));

	for (int i = 0; i < kparam.nr_lcpu; i++)
	{
		create_percpu_idle(i);
		init_percpu_arch_data(i);
		init_percpu_data(i);
	}
	kfree(idle_tasks[0]);
	idle_tasks[0] = &task0_PCB;

	idle_queue.nr_max = kparam.nr_lcpu;
	idle_queue.nr_curr = 0;
	idle_queue.head = 0;
	idle_queue.tail = 0;
	idle_queue.queue = (task_s **)idle_tasks;
	idle_queue.sched_task = &task0_PCB.task;
}

void create_percpu_idle(size_t cpu_idx)
{
	idle_tasks[cpu_idx] = (PCB_u *)kmalloc(sizeof(PCB_u));
	PCB_u * idle_pcb = idle_tasks[cpu_idx];
	task_s * idletask = &(idle_pcb->task);
	memset(idle_pcb, 0, sizeof(PCB_u));
	memcpy(idletask, &task0_PCB.task, sizeof(task_s));
	m_list_init(idletask);
	list_init(&idletask->schedule_list, idletask);
	list_hdr_init(&idletask->child_lhdr);
	idletask->pid = gen_newpid();
}

void init_percpu_arch_data(size_t cpu_idx)
{
	PCB_u * idle_pcb = idle_tasks[cpu_idx];
	tss_ptr_arr[cpu_idx] = (tss64_T *)kmalloc(sizeof(tss64_T));
	tss64_T * tss_p = tss_ptr_arr[cpu_idx];
	memset(tss_p, 0, sizeof(tss64_T));	// init tss's ists
	tss_p->rsp0 = (size_t)idle_pcb + TASK_KSTACK_SIZE;
	idle_pcb->task.arch_struct.tss_rsp0 = tss_p->rsp0;
	idle_pcb->task.mm_struct = task0_PCB.task.mm_struct;
	idle_pcb->task.mm_struct->start_stack = (reg_t)idle_pcb + TASK_KSTACK_SIZE;
}

void init_percpu_data(size_t cpu_idx)
{
	// create percpu_data for current lcpu
	percpu_data[cpu_idx] = (cpudata_u *)kmalloc(sizeof(cpudata_u));
	cpudata_u * cpudata_u_p = percpu_data[cpu_idx];
	per_cpudata_s * cpudata_p = &(cpudata_u_p->cpudata);
	memset(cpudata_u_p, 0, sizeof(cpudata_u));
	cpudata_p->cpu_idx = cpu_idx;
	// fill architechture part
	arch_cpudata_s * arch_cpuinfo = &(cpudata_p->arch_info);
	arch_cpuinfo->lcpu_addr = apic_id[cpu_idx];
	arch_cpuinfo->lcpu_topo_flag[0] = smp_topos[cpu_idx].thd_id;
	arch_cpuinfo->lcpu_topo_flag[1] = smp_topos[cpu_idx].core_id;
	arch_cpuinfo->lcpu_topo_flag[2] = smp_topos[cpu_idx].pack_id;
	arch_cpuinfo->lcpu_topo_flag[3] = smp_topos[cpu_idx].not_use;
	arch_cpuinfo->tss = tss_ptr_arr[cpu_idx];
	// set percpu_stack to ist
	tss64_T * tss_p = tss_ptr_arr[cpu_idx];
	tss_p->rsp1 =
	tss_p->rsp2 =
	tss_p->ist1 =
	tss_p->ist2 =
	tss_p->ist3 =
	tss_p->ist4 =
	tss_p->ist5 =
	tss_p->ist6 =
	tss_p->ist7 = (reg_t)cpudata_u_p + CONFIG_CPUSTACK_SIZE;
}

void percpu_self_config(size_t cpu_idx)
{
	cpudata_u * cpudata_u_p = percpu_data[cpu_idx];
	per_cpudata_s * cpudata_p = &(cpudata_u_p->cpudata);
	task_s *	current_task = get_current_task();
	wrgsbase((reg_t)cpudata_u_p);
	// tasks
	cpudata_p->idle_task = current_task;
	cpudata_p->curr_task = current_task;
	cpudata_p->ready_tasks.count =
	cpudata_p->finished_tasks.count = 0;
	cpudata_p->ready_tasks.head_p =
	cpudata_p->finished_tasks.head_p = NULL;
	cpudata_p->last_jiffies = 0;
	cpudata_p->time_slice = cpudata_p->curr_task->time_slice;
	cpudata_p->is_idle_flag = 1;
	cpudata_p->scheduleing_flag = 0;
	cpudata_p->cpustack_p = (reg_t)cpudata_u_p + CONFIG_CPUSTACK_SIZE;
	list_hdr_init(&cpudata_p->ruuning_lhdr);

	current_task->arch_struct.tss_rsp0 = (reg_t)current_task + TASK_KSTACK_SIZE;
	current_task->vruntime = -1;
	current_task->semaphore_count =
	current_task->spin_count = 0;
	current_task->state = PS_RUNNING;

	unmap_kernel_lowhalf();
	refresh_arch_page();
}

void startup_smp()
{
	wrmsr(0x830,0xc4500);	//INIT IPI
	wrmsr(0x830,0xc4620);	//Start-up IPI
}

inline __always_inline per_cpudata_s * get_current_cpu()
{
	cpudata_u * cpudata_u_p = (cpudata_u *)rdgsbase();
	return &(cpudata_u_p->cpudata);
}