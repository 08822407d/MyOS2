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

unsigned long test_task_a(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;

		color_printk(WHITE, BLACK, "-A- ");
	}
}

unsigned long test_task_b(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-B- ");
	}
}

unsigned long test_task_c(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-C- ");
	}
}

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

	// kernel_thread(test_task_a, 0, 0);
	// kernel_thread(test_task_b, 0, 0);
	// kernel_thread(test_task_c, 0, 0);

	// post init
	if (IS_BSP)
	{
		softirq_init();
		timer_init();
		devices_init();
	}
	sti();

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