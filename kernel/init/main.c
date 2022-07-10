#include <string.h>

#include <include/glo.h>
#include <include/proto.h>
#include <include/printk.h>
#include <linux/kernel/sched/sched.h>
#include <linux/kernel/device.h>
#include <linux/fs/fs.h>

#include <arch/amd64/include/arch_proto.h>
#include <arch/amd64/include/archtypes.h>
#include <arch/amd64/include/apic.h>
#include <arch/amd64/include/device.h>

atomic_T boot_counter;

extern void kjmp_to_doexecve();
extern void do_name(void);


void start_kernel()
{
	size_t cpu_idx = 0;

	early_init_sytem();

	early_init_task(kparam.nr_lcpu);
	early_init_arch_data(kparam.nr_lcpu);
	early_init_smp(kparam.nr_lcpu);

	init_arch(cpu_idx);
	init_arch_page();

	init_video();

	early_init_mm();
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

	vfs_caches_init();

	atomic_set(&boot_counter, 0);
}

/*==============================================================================================*
 *										task0 -- idle()											*
 *==============================================================================================*/
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

	if (cpu_idx == 0)
		kernel_thread(kernel_init, 0, 0);
	
	schedule();

	while (1)
		hlt();
}

/*==============================================================================================*
 *										task2 -- init()											*
 *==============================================================================================*/
static void do_initcalls(void)
{
	register_diskfs();
}

/*
 * Ok, the machine is now initialized. None of the devices
 * have been touched yet, but the CPU subsystem is up and
 * running, and memory and process management works.
 *
 * Now we can finally start doing some real work..
 */
static void do_basic_setup(void)
{
	// cpuset_init_smp();
	driver_init();
	// init_irq_proc();
	// do_ctors();
	do_initcalls();
}


extern int ata_probe();
extern void get_ata_info(void);
unsigned long ATArq_deamon(unsigned long);
unsigned long kernel_init(unsigned long arg)
{
	do_basic_setup();
	// do_name();
	ata_probe();
	kernel_thread(ATArq_deamon, 0, 0);

	sti();

	get_ata_info();
	// color_printk(GREEN, BLACK, "Enter task init.\n");
	switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");
	creat_dev_file();

	// color_printk(GREEN, BLACK, "Device files created.\n");
	kjmp_to_doexecve();

	while (true);
	
	return 1;
}