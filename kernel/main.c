#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/task.h"
#include "include/printk.h"

#include "arch/amd64/include/arch_proto.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/apic.h"
#include "arch/amd64/include/device.h"

#include "klib/data_structure.h"


void kmain(size_t cpu_idx)
{
	// here an env of bsp had been setup
	// include the GDT, IDT and a tmp_tss for bsp
	// or to say kmain running in a simple but usable protect mode
	int IS_BSP = !cpu_idx;
	if (IS_BSP)
	{
		init_page_manage();
		init_video();
		init_slab();
		init_task();
		init_smp();
	}

	init_percpu_arch_data(cpu_idx);
	reload_percpu_arch_env(cpu_idx);
	init_percpu_data(cpu_idx);
	init_percpu_intr();

	if (IS_BSP)
	{
		init_bsp_intr();
		startup_smp();
	}

	percpu_self_config(cpu_idx);


	// post init
	if (IS_BSP)
	{
		softirq_init();
		timer_init();
		devices_init();
	}
	sti();

	module_test();

	while(1){
		// the idle-tasks run here repeatedly
		// the IS_BSP flag only conbined with task_0 but not the BSP
		// once they scheduled, idle will be pushed in idle-queue
		if (IS_BSP)
			schedule();
		else
			hlt();
	};
}
