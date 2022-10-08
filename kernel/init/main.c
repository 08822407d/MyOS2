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
// #include <linux/delay.h>
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
// #include <linux/slab.h>
// #include <linux/perf_event.h>
// #include <linux/ptrace.h>
// #include <linux/pti.h>
// #include <linux/blkdev.h>
// #include <linux/sched/clock.h>
// #include <linux/sched/task.h>
// #include <linux/sched/task_stack.h>
// #include <linux/context_tracking.h>
// #include <linux/random.h>
#include <linux/lib/list.h>
// #include <linux/integrity.h>
// #include <linux/proc_ns.h>
// #include <linux/io.h>
// #include <linux/cache.h>
// #include <linux/rodata_test.h>
// #include <linux/jump_label.h>
// #include <linux/mem_encrypt.h>
// #include <linux/kcsan.h>
// #include <linux/init_syscalls.h>
// #include <linux/stackdepot.h>
// #include <net/net_namespace.h>

// #include <asm/io.h>
// #include <asm/bugs.h>
// #include <asm/setup.h>
// #include <asm/sections.h>
// #include <asm/cacheflush.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/initcall.h>

// #include <kunit/test.h>


#include <linux/kernel/sched.h>
#include <linux/device/driver.h>
#include <linux/fs/fs.h>
#include <linux/lib/string.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>
#include "../arch/amd64/include/arch_proto.h"
#include "../arch/amd64/include/archtypes.h"
#include "../arch/amd64/include/apic.h"
#include "../arch/amd64/include/device.h"

atomic_T boot_counter;

extern void kjmp_to_doexecve();
extern void do_name(void);

void zone_sizes_init(void)
{
	unsigned long max_zone_pfns[MAX_NR_ZONES];

	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

#ifdef CONFIG_ZONE_DMA
	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
#endif
#ifdef CONFIG_ZONE_DMA32
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
#endif
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;
#ifdef CONFIG_HIGHMEM
	max_zone_pfns[ZONE_HIGHMEM]	= max_pfn;
#endif

	free_area_init(max_zone_pfns);
}

void __init start_kernel(void)
{
	// char *command_line;
	// char *after_dashes;

	size_t cpu_idx = 0;

	early_init_sytem();
	// setup_arch(NULL);

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
		kernel_thread(kernel_init, 0, 0, "init");
	
	schedule();

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
unsigned long kernel_init(unsigned long arg)
{
	sti();

	do_basic_setup();
	// do_name();
	// ata_probe();
	init_ATArqd();

	get_ata_info();
	// color_printk(GREEN, BLACK, "Enter task init.\n");
	switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");

	// color_printk(GREEN, BLACK, "Device files created.\n");
	kjmp_to_doexecve();

	while (true);
	
	return 1;
}