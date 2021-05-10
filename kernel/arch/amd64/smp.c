#include <lib/string.h>
#include <lib/pthread.h>
#include <lib/stddef.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/arch_glo.h"

#include "../../include/glo.h"
#include "../../include/printk.h"
#include "../../include/proto.h"
#include "../../include/const.h"
#include "../../include/task.h"
#include "../../klib/data_structure.h"

percpu_data_s ** smp_info;

void init_smp()
{
	#ifdef DEBUG
		// make sure have get smp information
		while (!kparam.arch_init_flags.smp_info);
	#endif

	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apbbot_len = &_APboot_etext - &_APboot_text;
	memcpy(phys2virt((phys_addr)&_APboot_phys_start), (virt_addr)&_APboot_text, apbbot_len);

	// create pointer array for all percpu_data
	unsigned nr_lcpu = kparam.nr_lcpu;
	tss_ptr_arr = (tss64_s **)kmalloc(nr_lcpu * sizeof(tss64_s *));
	smp_info = (percpu_data_s **)kmalloc(kparam.nr_lcpu * sizeof(percpu_data_s *));
}

void init_percpu_data(size_t cpu_idx)
{
	// create percpu_data for current lcpu
	smp_info[cpu_idx] = (percpu_data_s *)kmalloc(sizeof(percpu_data_s));
	percpu_data_s * cpudata_p = smp_info[cpu_idx];
	memset(cpudata_p, 0, sizeof(percpu_data_s));
	cpudata_p->cpu_stack_start = (char (*)[CONFIG_CPUSTACK_SIZE])kmalloc(sizeof(char [CONFIG_CPUSTACK_SIZE]));
	// create architechture part of percpu_data
	cpudata_p->arch_info = (arch_percpu_data_s *)kmalloc(sizeof(arch_percpu_data_s));
	memset(cpudata_p->arch_info, 0, sizeof(arch_percpu_data_s));
	// fill architechture part
	arch_percpu_data_s * arch_cpuinfo = cpudata_p->arch_info;
	arch_cpuinfo->lcpu_addr = apic_id[cpu_idx];
	arch_cpuinfo->lcpu_topo_flag[0] = smp_topos[cpu_idx].thd_id;
	arch_cpuinfo->lcpu_topo_flag[1] = smp_topos[cpu_idx].core_id;
	arch_cpuinfo->lcpu_topo_flag[2] = smp_topos[cpu_idx].pack_id;
	arch_cpuinfo->lcpu_topo_flag[3] = smp_topos[cpu_idx].not_use;
	arch_cpuinfo->tss = tss_ptr_arr[cpu_idx];
	// set percpu_stack to ist
	tss64_s * tss_p = tss_ptr_arr[cpu_idx];
	tss_p->rsp1 =
	tss_p->rsp2 =
	tss_p->ist1 =
	tss_p->ist2 =
	tss_p->ist3 =
	tss_p->ist4 =
	tss_p->ist5 =
	tss_p->ist6 =
	tss_p->ist7 = (reg_t)(cpudata_p->cpu_stack_start) + CONFIG_CPUSTACK_SIZE;
}

void init_percpu_arch_data(size_t cpu_idx)
{
	tss_ptr_arr[cpu_idx] = (tss64_s *)kmalloc(sizeof(tss64_s));
	tss64_s * tss_p = tss_ptr_arr[cpu_idx];
	percpu_data_s * cpudata_p = smp_info[cpu_idx];
	cpudata_p->cpu_idx = cpu_idx;
	memset(tss_p, 0, sizeof(tss64_s));	// init tss's ists
	tss_p->rsp0 = (size_t)get_current() + TASK_KSTACK_SIZE;
}

void percpu_self_config(size_t cpu_idx)
{
	percpu_data_s * cpudata_p = smp_info[cpu_idx];
	task_s *	current_task = get_current();
	wrgsbase((reg_t)cpudata_p);
	// tasks
	cpudata_p->curr_task = current_task;
	cpudata_p->waiting_count =
	cpudata_p->finished_count = 0;
	cpudata_p->waiting_task =
	cpudata_p->finished_task = NULL;
	cpudata_p->last_jiffies =
	cpudata_p->task_jiffies = cpudata_p->curr_task->task_jiffies;
	// arch data
}

void startup_smp()
{
	wrmsr(0x830,0xc4500);	//INIT IPI
	wrmsr(0x830,0xc4620);	//Start-up IPI
}