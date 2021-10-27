#include <include/glo.h>
#include <include/proto.h>
#include <include/task.h>
#include <include/printk.h>
#include <include/vfs.h>

#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/archtypes.h>
#include <arch/amd64/include/apic.h>
#include <arch/amd64/include/device.h>

atomic_T boot_counter;

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
	reload_percpu_arch_data(cpu_idx);
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