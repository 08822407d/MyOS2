#include <linux/lib/string.h>
#include <asm/fsgsbase.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>
#include <asm/desc.h>
#include <asm/percpu.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <linux/kernel/sched.h>
#include <linux/mm/memblock.h>

#include <obsolete/arch_proto.h>
#include <obsolete/mutex.h>

extern phys_addr_t 		kernel_cr3;
extern u32	cr3_paddr;
extern struct cputopo	smp_topos[CONFIG_NR_CPUS];

void myos_early_init_smp(void)
{
	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apboot_len = &_APboot_etext - &_APboot_text;
	memcpy((void *)phys_to_virt((phys_addr_t)&_APboot_phys_start),
			(void *)&_APboot_text, apboot_len);
	cr3_paddr = kernel_cr3;
}

void myos_init_smp(size_t lcpu_nr)
{
	// init basic data for percpu
	// for (int i = 0; i < lcpu_nr; i++)
	// {
	// create percpu_data for current lcpu
	per_cpudata_s * cpudata_p = &(per_cpu(cpudata, 0).data);
	cpudata_p->cpu_idx = 0;
	cpudata_p->time_slice = cpudata_p->curr_task->rt.time_slice;

	// fill architechture part
	arch_cpudata_s * arch_cpuinfo = &(cpudata_p->arch_info);
	arch_cpuinfo->lcpu_addr = apicid_to_cpunr[0];
	arch_cpuinfo->lcpu_topo_flag[0] = smp_topos[0].thd_id;
	arch_cpuinfo->lcpu_topo_flag[1] = smp_topos[0].core_id;
	arch_cpuinfo->lcpu_topo_flag[2] = smp_topos[0].pack_id;
	arch_cpuinfo->lcpu_topo_flag[3] = smp_topos[0].not_use;
	arch_cpuinfo->tss = &(per_cpu(cpu_tss_rw, 0));
	// }
}

void myos_percpu_self_config(size_t cpu_idx)
{
	cpudata_u *cpudata_u_p = &(per_cpu(cpudata, cpu_idx));
	per_cpudata_s * cpudata_p = &(cpudata_u_p->data);
	task_s *	current_task = current;
	cpudata_p->curr_task =
	cpudata_p->idle_task = current_task;
	wrgsbase((unsigned long)cpudata_u_p);

	__flush_tlb_all();
}

void myos_startup_smp()
{
	wrmsrl(0x830,0xc4500);	//INIT IPI
	wrmsrl(0x830,0xc4620);	//Start-up IPI
}

inline __always_inline per_cpudata_s * get_current_cpu()
{
	cpudata_u * cpudata_u_p = (cpudata_u *)rdgsbase();
	return &(cpudata_u_p->data);
}