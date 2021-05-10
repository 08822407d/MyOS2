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
	init_percpu_data(cpu_idx);
	reload_percpu_arch_env(cpu_idx);

	if (IS_BSP)
	{
		init_intr();
		startup_smp();
	}

	percpu_self_config(cpu_idx);

	// post init
	if (IS_BSP)
	{
		schedule();
		softirq_init();
		timer_init();
		devices_init();
	}

	sti();

	int i = 1 / 0;

	while(1){
		hlt();
	};
}