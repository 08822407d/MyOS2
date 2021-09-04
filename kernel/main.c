#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/task.h"
#include "include/printk.h"
#include "include/vfs.h"

#include "arch/amd64/include/arch_proto.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/apic.h"
#include "arch/amd64/include/device.h"

#include "klib/data_structure.h"


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
	init_percpu_intr();
	init_bsp_intr();

	// post init
	softirq_init();
	timer_init();
	devices_init();

	color_printk(BLACK, BLUE, "BSP env initiated.\n");

	startup_smp();
	atomic_set(&boot_counter, 0);

}

void idle(size_t cpu_idx)
{	
	reload_percpu_arch_data(cpu_idx);
	init_percpu_intr();
	percpu_self_config(cpu_idx);
	sti();

	// __asm__ __volatile__(	"movq	%0,				%%rax	\n\t"
	// 						"movq	%%gs:(%%rax),	%%rax	\n\t"
	// 					:
	// 					:	"m"(cpustack_off)
	// 					:	"rax");

	// kernel_thread(module_test, 0, 0);
	// module_test(cpu_idx);
	// if (cpu_idx == 0)
		// kernel_thread(init_vfs, 0, 0);
	userthd_test();

	while (1)
	{
		hlt();
	}
}