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
#include <linux/fs/binfmts.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/syscalls.h>
#include <linux/lib/string.h>
#include <linux/kernel/ctype.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/ioport.h>
#include <linux/init/init.h>
#include <linux/device/console.h>
#include <linux/kernel/nmi.h>
#include <linux/smp/percpu.h>
#include <linux/kernel/start_kernel.h>
#include <linux/smp/smp.h>
#include <linux/kernel/cpu.h>
#include <linux/block/efi.h>
#include <uapi/linux/unistd.h>
#include <linux/device/driver.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/signal.h>
#include <linux/debug/ftrace.h>
#include <linux/kernel/ptrace.h>
#include <linux/block/blkdev.h>
#include <linux/sched/clock.h>
#include <linux/kernel/lib.h>
#include <linux/kernel/cache.h>


#include <asm/insns.h>
#include <asm/bugs.h>
#include <asm/setup.h>
#include <asm/sections.h>


#include <linux/kernel/mm.h>
#include <linux/kernel/completion.h>
#include <linux/kernel/timekeeping.h>
#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>
#include <obsolete/device.h>


static int kernel_init(void *);

extern void init_IRQ(void);


extern void time_init(void);
/* Default late time init is NULL. archs can override this later. */
void (*__initdata late_time_init)(void);


/*
 * This should be approx 2 Bo*oMips to start (note initial shift), and will
 * still work even if initially too large, it will just take slightly longer
 */
// unsigned long loops_per_jiffy = (1<<12);
unsigned long loops_per_jiffy = 10371364;


/*
 * We need to finalize in a non-__init function or else race conditions
 * between the root thread and the init thread may cause start_kernel to
 * be reaped by free_initmem before the root thread has proceeded to
 * cpu_idle.
 *
 * gcc-3.4 accidentally inlines this function, so use noinline.
 */

static __initdata DECLARE_COMPLETION(kthreadd_done);

noinline void __ref rest_init(void)
{
	task_s *tsk;
	int pid;

	// rcu_scheduler_starting();
	/*
	 * We need to spawn init first so that it obtains pid 1, however
	 * the init task will end up wanting to create kthreads, which, if
	 * we schedule it before we create kthreadd, will OOPS.
	 */
	pid = kernel_thread(kernel_init, NULL, "kthd: kernel_init", CLONE_FS);
	/*
	 * Pin init on the boot CPU. Task migration is not properly working
	 * until sched_init_smp() has been run. It will set the allowed
	 * CPUs for init to the non isolated CPUs.
	 */
	// rcu_read_lock();
	// tsk = find_task_by_pid_ns(pid, &init_pid_ns);
	tsk = myos_find_task_by_pid(pid);
	tsk->flags |= PF_NO_SETAFFINITY;
	// set_cpus_allowed_ptr(tsk, cpumask_of(smp_processor_id()));
	// rcu_read_unlock();

	// numa_default_policy();
	pid = kernel_thread(kthreadd, NULL, "kthreadd", CLONE_FS | CLONE_FILES);
	// rcu_read_lock();
	// kthreadd_task = find_task_by_pid_ns(pid, &init_pid_ns);
	kthreadd_task = myos_find_task_by_pid(pid);
	// rcu_read_unlock();

	/*
	 * Enable might_sleep() and smp_processor_id() checks.
	 * They cannot be enabled earlier because with CONFIG_PREEMPTION=y
	 * kernel_thread() would trigger might_sleep() splats. With
	 * CONFIG_PREEMPT_VOLUNTARY=y the init task might have scheduled
	 * already, but it's stuck on the kthreadd_done completion.
	 */
	// system_state = SYSTEM_SCHEDULING;

	complete(&kthreadd_done);

	/*
	 * The boot idle thread must execute schedule()
	 * at least once to get things moving:
	 */
	schedule_preempt_disabled();
	/* Call into cpu_idle with preempt disabled */
	cpu_startup_entry();
}


asmlinkage void __init start_kernel(void)
{
	char *command_line;
	char *after_dashes;

	setup_arch(NULL);

	setup_per_cpu_areas();


	myos_preinit_slab();


	trap_init();
	mm_core_init();

	init_IRQ();
	timekeeping_init();
	time_init();

	myos_init_slab();

	// enable bsp's apic
	myos_init_bsp_intr();

	// myos_startup_smp();


	/*
	 * HACK ALERT! This is early. We're enabling the console before
	 * we've done PCI setups etc, and console_init() must be aware of
	 * this. But we do want output early, in case something goes wrong.
	 */
	console_init();

	if (late_time_init)
		late_time_init();

	myos_devices_init();

	vfs_caches_init();

	check_bugs();

	myos_init_percpu_intr();

	myos_unmap_kernel_lowhalf();

	rest_init();

	prevent_tail_call_optimization();
}

/*==============================================================================================*
 *										task2 -- init()											*
 *==============================================================================================*/
void pci_arch_init(void);
void myos_scan_pci_devices(void);
void register_diskfs(void);
int tty_class_init(void);
int chr_dev_init(void);
int init_elf_binfmt(void);
static void do_initcalls(void)
{
	// pci_arch_init();
	// myos_scan_pci_devices();

	register_diskfs();

	tty_class_init();

	chr_dev_init();

	init_elf_binfmt();
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


static noinline void __init kernel_init_freeable(void);


extern void scan_PCI_devices(void);
extern void scan_XHCI_devices();
void myos_ata_probe();
extern void init_ATArqd();
extern void init_NVMErqd();
extern void init_XHCIrqd();
extern void kjmp_to_doexecve();
extern void NVMe_IOqueue_init();
extern void USB_Keyborad_init();
int kernel_init(void *unused)
{
	int ret;

	/*
	 * Wait until kthreadd is all set-up.
	 */
	wait_for_completion(&kthreadd_done);

extern void do_name(void);
	do_name();
	kernel_init_freeable();


	init_NVMErqd();
	init_ATArqd();
	init_XHCIrqd();

	scan_PCI_devices();
	myos_ata_probe();
	// scan_XHCI_devices();

	NVMe_IOqueue_init();
	// USB_Keyborad_init();

	// color_printk(GREEN, BLACK, "Enter task init.\n");
	myos_switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");

	kjmp_to_doexecve();

	while (true);
	
	return 1;
}

static noinline void __init kernel_init_freeable(void)
{
	/* Now the scheduler is fully set up and can do blocking allocations */
	gfp_allowed_mask = __GFP_BITS_MASK;

	/*
	 * init can allocate pages on any node
	 */
	// set_mems_allowed(node_states[N_MEMORY]);

	// cad_pid = get_pid(task_pid(current));

	// smp_prepare_cpus(setup_max_cpus);

	workqueue_init();

	// rcu_init_tasks_generic();
	// do_pre_smp_initcalls();
	// lockup_detector_init();

	// smp_init();
	// sched_init_smp();

	// padata_init();
	// page_alloc_init_late();
	// /* Initialize page ext after all struct pages are initialized. */
	// page_ext_init();

	do_basic_setup();

	// kunit_run_all_tests();

	// wait_for_initramfs();
	// console_on_rootfs();

	// /*
	//  * check if there is an early userspace init.  If yes, let it do all
	//  * the work
	//  */
	// if (init_eaccess(ramdisk_execute_command) != 0) {
	// 	ramdisk_execute_command = NULL;
	// 	prepare_namespace();
	// }

	// /*
	//  * Ok, we have completed the initial bootup, and
	//  * we're essentially up and running. Get rid of the
	//  * initmem segments and start the user-mode stuff..
	//  *
	//  * rootfs is available now, try loading the public keys
	//  * and default modules
	//  */

	// integrity_load_keys();
}