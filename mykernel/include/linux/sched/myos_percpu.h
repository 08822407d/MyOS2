#ifndef _MYOS_PERCPU_H_
#define _MYOS_PERCPU_H_

#include <asm/processor.h>
#include <linux/lib/list.h>
#include <linux/kernel/sizes.h>

	struct task_struct;
	typedef struct task_struct task_s;

	typedef struct arch_percpu_info
	{
		uint64_t	lcpu_addr;			// local apic_id
		uint16_t	lcpu_topo_flag[4];	// 3 = flag, 2 = package_id, 1 = core_id, 0 = thread_id
		struct tss_struct	*tss;
	} arch_cpudata_s;

	typedef struct per_cpudata {
		task_s *	curr_task;
		task_s *	idle_task;

		List_hdr_s	running_lhdr;

		unsigned long	is_idle_flag;
		unsigned long	scheduleing_flag;

		unsigned long	last_jiffies;	// abs jiffies when curr-task loaded
		unsigned long	time_slice;		// max jiffies for running of this task

		size_t			cpu_idx;
		arch_cpudata_s	arch_info;

		unsigned long	preempt_count;
	} per_cpudata_s;


	extern per_cpudata_s cpudata;

	// per_cpudata_s * get_current_cpu(void);
	// #define curr_cpu get_current_cpu()

#endif /* _MYOS_PERCPU_H_ */