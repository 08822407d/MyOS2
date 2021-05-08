#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/proc.h"
#include "include/printk.h"

#include "arch/amd64/include/arch_proto.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/apic.h"
#include "arch/amd64/include/device.h"

void kmain(size_t cpu_idx)
{
	// pre init only bsp do these works
	// prepare an environment and global system data
	if (cpu_idx == 0)
	{
		init_arch_env();
		init_video();
		init_slab();
		init_smp_env();
		init_smp();

		init_proc();
		startup_smp();
	}

	config_lcpu_self(cpu_idx);

	// post init
	if (cpu_idx == 0)
	{
		// schedule();

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