// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/init/main.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  GK 2/5/95  -  Changed to support mounting root fs via NFS
 *  Added initrd & change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Moan early if gcc is old, avoiding bogus kernels - Paul Gortmaker, May '96
 *  Simplified starting of init:  Michael A. Griffith <grif@acm.org>
 */

// #define DEBUG		/* Enable initcall_debug */

#include <linux/kernel/types.h>
// #include <linux/extable.h>
// #include <linux/module.h>
// #include <linux/proc_fs.h>
// #include <linux/binfmts.h>
#include <linux/kernel/kernel.h>
// #include <linux/syscalls.h>
// #include <linux/stackprotector.h>
// #include <linux/string.h>
// #include <linux/ctype.h>
#include <linux/kernel/delay.h>
// #include <linux/ioport.h>
#include <linux/init/init.h>
// #include <linux/initrd.h>
#include <linux/mm/memblock.h>
// #include <linux/acpi.h>
// #include <linux/bootconfig.h>
// #include <linux/console.h>
// #include <linux/nmi.h>
// #include <linux/percpu.h>
// #include <linux/kmod.h>
// #include <linux/kprobes.h>
// #include <linux/vmalloc.h>
// #include <linux/kernel_stat.h>
// #include <linux/start_kernel.h>
// #include <linux/security.h>
// #include <linux/smp.h>
// #include <linux/profile.h>
// #include <linux/kfence.h>
// #include <linux/rcupdate.h>
// #include <linux/srcu.h>
// #include <linux/moduleparam.h>
// #include <linux/kallsyms.h>
// #include <linux/buildid.h>
// #include <linux/writeback.h>
// #include <linux/cpu.h>
// #include <linux/cpuset.h>
// #include <linux/cgroup.h>
// #include <linux/efi.h>
// #include <linux/tick.h>
// #include <linux/sched/isolation.h>
// #include <linux/interrupt.h>
// #include <linux/taskstats_kern.h>
// #include <linux/delayacct.h>
// #include <linux/unistd.h>
// #include <linux/utsname.h>
// #include <linux/rmap.h>
// #include <linux/mempolicy.h>
// #include <linux/key.h>
// #include <linux/page_ext.h>
// #include <linux/debug_locks.h>
// #include <linux/debugobjects.h>
// #include <linux/lockdep.h>
// #include <linux/kmemleak.h>
// #include <linux/padata.h>
// #include <linux/pid_namespace.h>
// #include <linux/device/driver.h>
// #include <linux/kthread.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/init.h>
// #include <linux/signal.h>
// #include <linux/idr.h>
// #include <linux/kgdb.h>
// #include <linux/ftrace.h>
// #include <linux/async.h>
// #include <linux/shmem_fs.h>
#include <linux/kernel/slab.h>
// #include <linux/perf_event.h>
// #include <linux/ptrace.h>
// #include <linux/pti.h>
#include <linux/block/blkdev.h>
// #include <linux/sched/clock.h>
// #include <linux/sched/task.h>
// #include <linux/sched/task_stack.h>
// #include <linux/context_tracking.h>
// #include <linux/random.h>
#include <linux/lib/list.h>
// #include <linux/integrity.h>
// #include <linux/proc_ns.h>
// #include <linux/io.h>
#include <linux/kernel/cache.h>
// #include <linux/rodata_test.h>
// #include <linux/jump_label.h>
// #include <linux/mem_encrypt.h>
// #include <linux/kcsan.h>
// #include <linux/init_syscalls.h>
// #include <linux/stackdepot.h>
// #include <net/net_namespace.h>

// #include <asm/io.h>
#include <asm/bugs.h>
#include <asm/setup.h>
#include <asm/sections.h>
// #include <asm/cacheflush.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/initcall.h>

// #include <kunit/test.h>


#include <linux/kernel/sched.h>
#include <linux/lib/string.h>
#include <linux/mm/myos_slab.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>
#include <obsolete/mutex.h>
#include <obsolete/apic.h>
#include <obsolete/device.h>

/*
 * This should be approx 2 Bo*oMips to start (note initial shift), and will
 * still work even if initially too large, it will just take slightly longer
 */
unsigned long loops_per_jiffy = (1<<12);

/*
 * Set up kernel memory allocators
 */
static void __init mm_init(void)
{
	// /*
	//  * page_ext requires contiguous pages,
	//  * bigger than MAX_ORDER unless SPARSEMEM.
	//  */
	// page_ext_init_flatmem();
	// init_mem_debugging_and_hardening();
	// kfence_alloc_pool();
	// report_meminit();
	// stack_depot_early_init();
	mem_init();
	// mem_init_print_info();
	// kmem_cache_init();
	// /*
	//  * page_owner must be initialized after buddy is ready, and also after
	//  * slab is ready so that stack_depot_init() works properly
	//  */
	// page_ext_init_flatmem_late();
	// kmemleak_init();
	// pgtable_init();
	// debug_objects_mem_init();
	// vmalloc_init();
	// /* Should be run before the first non-init thread is created */
	// init_espfix_bsp();
	// /* Should be run after espfix64 is set up. */
	// pti_init();
}

myos_atomic_T lcpu_boot_count;
myos_atomic_T lower_half_unmapped;
asmlinkage void __init start_kernel(void)
{
	// char *command_line;
	// char *after_dashes;

	setup_arch(NULL);

	myos_preinit_slab();

	mm_init();

	myos_init_slab();

	myos_init_task(kparam.nr_lcpu);
	myos_init_smp(kparam.nr_lcpu);
	
	// enable bsp's apic
	myos_init_bsp_intr();

#ifdef START_WAIT
	size_t delay = 0;
	for (unsigned i = 0; i < -1; i++)
		delay++;
#endif
	
	// post init
	myos_softirq_init();
	myos_timer_init();
	myos_devices_init();

	myos_startup_smp();

	vfs_caches_init();

	check_bugs();

	lcpu_boot_count.value = 0;
}

/*==============================================================================================*
 *										task0 -- idle()											*
 *==============================================================================================*/
void idle(size_t cpu_idx)
{	
	myos_reload_arch_data(cpu_idx);
	myos_init_percpu_intr();
	myos_percpu_self_config(cpu_idx);
	myos_arch_system_call_init();

	myos_atomic_inc(&lcpu_boot_count);
	while (myos_atomic_read(&lcpu_boot_count) != kparam.nr_lcpu);

	if (cpu_idx == 0)
	{
		lower_half_unmapped.value = 0;
		myos_unmap_kernel_lowhalf(&lower_half_unmapped);
	}
	while (lower_half_unmapped.value == 0)
		myos_refresh_arch_page();

	if (cpu_idx == 0)
	{
		myos_kernel_thread(kernel_init, 0, 0, "init");
		myos_schedule();
	}
	
	sti();

	// module_test();

	while (1)
		hlt();
}

/*==============================================================================================*
 *										task2 -- init()											*
 *==============================================================================================*/
void register_diskfs(void);
int tty_class_init(void);
int chr_dev_init(void);
static void do_initcalls(void)
{
	register_diskfs();

	tty_class_init();

	chr_dev_init();
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
extern void init_ATArqd();
extern void kjmp_to_doexecve();
unsigned long kernel_init(unsigned long arg)
{
	do_basic_setup();
	// do_name();
	// ata_probe();
	init_ATArqd();

	get_ata_info();
	
	// color_printk(GREEN, BLACK, "Enter task init.\n");
	myos_switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");

	// color_printk(GREEN, BLACK, "Device files created.\n");
	kjmp_to_doexecve();

	while (true);
	
	return 1;
}