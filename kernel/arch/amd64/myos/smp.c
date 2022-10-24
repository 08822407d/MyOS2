#include <linux/lib/string.h>
#include <asm/setup.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <linux/kernel/sched.h>
#include <linux/mm/memblock.h>

#include <obsolete/arch_proto.h>
#include <obsolete/archconst.h>
#include <obsolete/arch_glo.h>
#include <obsolete/mutex.h>

cpudata_u *	percpu_data;
extern u32	cr3_paddr;

void myos_early_init_smp(size_t lcpu_nr)
{
	percpu_data	= myos_memblock_alloc_normal(lcpu_nr * sizeof(cpudata_u), sizeof(void *));
}

static void init_percpu_data(size_t cpu_idx)
{
	// create percpu_data for current lcpu
	per_cpudata_s * cpudata_p = &(percpu_data + cpu_idx)->cpudata;
	cpudata_p->cpu_idx = cpu_idx;
	cpudata_p->last_jiffies = 0;
	cpudata_p->is_idle_flag = 1;
	cpudata_p->scheduleing_flag = 0;
	cpudata_p->curr_task =
	cpudata_p->idle_task = &(idle_tasks[cpu_idx]->task);
	cpudata_p->time_slice = cpudata_p->curr_task->time_slice;
	cpudata_p->cpustack_p = (reg_t)(percpu_data + cpu_idx) + PAGE_SIZE;
	list_hdr_init(&cpudata_p->running_lhdr);

	// fill architechture part
	arch_cpudata_s * arch_cpuinfo = &(cpudata_p->arch_info);
	arch_cpuinfo->lcpu_addr = apic_id[cpu_idx];
	arch_cpuinfo->lcpu_topo_flag[0] = smp_topos[cpu_idx].thd_id;
	arch_cpuinfo->lcpu_topo_flag[1] = smp_topos[cpu_idx].core_id;
	arch_cpuinfo->lcpu_topo_flag[2] = smp_topos[cpu_idx].pack_id;
	arch_cpuinfo->lcpu_topo_flag[3] = smp_topos[cpu_idx].not_use;
	arch_cpuinfo->tss = tss_ptr_arr + cpu_idx;
}

void myos_init_smp(size_t lcpu_nr)
{
	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apboot_len = &_APboot_etext - &_APboot_text;
	memcpy((void *)myos_phys2virt((phys_addr_t)&_APboot_phys_start),
			(void *)&_APboot_text, apboot_len);

	cr3_paddr = kernel_cr3;
	// init basic data for percpu
	for (int i = 0; i < lcpu_nr; i++)
	{
		init_percpu_data(i);
	}
}

void myos_percpu_self_config(size_t cpu_idx)
{
	cpudata_u * cpudata_u_p = percpu_data + cpu_idx;
	per_cpudata_s * cpudata_p = &(cpudata_u_p->cpudata);
	task_s *	current_task = get_current_task();
	wrgsbase((reg_t)cpudata_u_p);
	// tasks

	myos_refresh_arch_page();
}

void myos_startup_smp()
{
	wrmsr(0x830,0xc4500);	//INIT IPI
	wrmsr(0x830,0xc4620);	//Start-up IPI
}

inline __always_inline per_cpudata_s * get_current_cpu()
{
	cpudata_u * cpudata_u_p = (cpudata_u *)rdgsbase();
	return &(cpudata_u_p->cpudata);
}