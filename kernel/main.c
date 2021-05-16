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


void kmain()
{
	pre_init();

	init_bsp_arch_data();
	reload_bsp_arch_data();
	reload_arch_page();

	init_page_manage();
	init_video();
	init_slab();
	init_task();
	init_smp_env();
	
	// enable bsp's apic
	init_percpu_intr();
	init_bsp_intr();
	startup_smp();

	// post init
	softirq_init();
	timer_init();
	devices_init();
	sti();
}

void idle(size_t cpu_idx)
{	
	reload_percpu_arch_data(cpu_idx);
	if (cpu_idx != 0)
		init_percpu_intr();
	percpu_self_config(cpu_idx);

	module_test();
	while (1)
	{
		hlt();
	}
}