#include <string.h>

#include <include/glo.h>
#include <include/proto.h>
#include <include/printk.h>
#include <linux/kernel/sched/task.h>
#include <linux/fs/fs.h>

#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/archtypes.h>
#include <arch/amd64/include/apic.h>
#include <arch/amd64/include/device.h>

atomic_T boot_counter;

void start_kernel()
{
	size_t cpu_idx = 0;

	pre_init_sytem();

	prepare_init_task(kparam.nr_lcpu);
	prepare_init_arch_data(kparam.nr_lcpu);
	prepare_init_smp(kparam.nr_lcpu);

	init_arch(cpu_idx);
	init_arch_page();

	init_video();

	prepare_init_mm();
	init_mm();

	init_task(kparam.nr_lcpu);
	init_smp(kparam.nr_lcpu);
	
	// enable bsp's apic
	init_bsp_intr();

	// post init
	softirq_init();
	timer_init();
	devices_init();

	// color_printk(BLACK, BLUE, "BSP env initiated.\n");

	startup_smp();
	atomic_set(&boot_counter, 0);
}

void idle(size_t cpu_idx)
{	
	reload_arch_data(cpu_idx);
	init_percpu_intr();
	percpu_self_config(cpu_idx);
	arch_system_call_init();

	atomic_inc(&boot_counter);
	while (boot_counter.value != kparam.nr_lcpu);
	unmap_kernel_lowhalf();
	refresh_arch_page();

	sti();

	if (cpu_idx == 0)
	{
		// kernel_thread(module_test, 0, 0);
		kernel_thread(init, 0, 0);
	}

	while (1)
	{
		hlt();
	}
}