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
#include <linux/fs/binfmts.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/syscalls.h>
// #include <linux/stackprotector.h>
#include <linux/lib/string.h>
#include <linux/kernel/ctype.h>
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
#include <linux/kernel/start_kernel.h>
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
#include <linux/kernel/cpu.h>
// #include <linux/cpuset.h>
// #include <linux/cgroup.h>
#include <linux/block/efi.h>
// #include <linux/tick.h>
// #include <linux/sched/isolation.h>
// #include <linux/interrupt.h>
// #include <linux/taskstats_kern.h>
// #include <linux/delayacct.h>
#include <uapi/linux/unistd.h>
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
#include <linux/device/driver.h>
#include <linux/kernel/kthread.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/init.h>
#include <linux/kernel/signal.h>
// #include <linux/idr.h>
// #include <linux/kgdb.h>
// #include <linux/ftrace.h>
// #include <linux/async.h>
// #include <linux/shmem_fs.h>
#include <linux/kernel/slab.h>
// #include <linux/perf_event.h>
#include <linux/kernel/ptrace.h>
// #include <linux/pti.h>
#include <linux/block/blkdev.h>
// #include <linux/sched/clock.h>
#include <linux/sched/task.h>
// #include <linux/sched/task_stack.h>
// #include <linux/context_tracking.h>
// #include <linux/random.h>
#include <linux/lib/list.h>
// #include <linux/integrity.h>
// #include <linux/proc_ns.h>
#include <linux/kernel/io.h>
#include <linux/kernel/cache.h>
// #include <linux/rodata_test.h>
// #include <linux/jump_label.h>
// #include <linux/mem_encrypt.h>
// #include <linux/kcsan.h>
// #include <linux/init_syscalls.h>
// #include <linux/stackdepot.h>
// #include <net/net_namespace.h>

#include <asm/io.h>
#include <asm/bugs.h>
#include <asm/setup.h>
#include <asm/sections.h>
// #include <asm/cacheflush.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/initcall.h>

// #include <kunit/test.h>


#include <linux/mm/myos_slab.h>
#include <linux/kernel/completion.h>
#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/printk.h>
#include <obsolete/apic.h>
#include <obsolete/device.h>

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
	pid = kernel_thread(kernel_init, NULL, CLONE_FS);
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
	set_task_comm(tsk, "kernel_init");
	// rcu_read_unlock();

	// numa_default_policy();
	pid = kernel_thread(kthreadd, NULL, CLONE_FS | CLONE_FILES);
	// rcu_read_lock();
	// kthreadd_task = find_task_by_pid_ns(pid, &init_pid_ns);
	kthreadd_task = myos_find_task_by_pid(pid);
	set_task_comm(kthreadd_task, "kthreadd");
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


atomic_t lcpu_boot_count;
atomic_t lower_half_unmapped;
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
	myos_delay_full_u32(1000);
#endif
	
	// post init
	myos_softirq_init();
	myos_timer_init();
	myos_devices_init();

	myos_startup_smp();

	vfs_caches_init();

	check_bugs();

	atomic_set(&lcpu_boot_count, 0);
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

	atomic_inc(&lcpu_boot_count);
	while (atomic_read(&lcpu_boot_count) != kparam.nr_lcpu);

	if (cpu_idx == 0)
	{
		atomic_set(&lower_half_unmapped, 0);
		myos_unmap_kernel_lowhalf(&lower_half_unmapped);
	}
	while (atomic_read(&lower_half_unmapped) == 0)
		myos_refresh_arch_page();

	if (cpu_idx == 0)
		rest_init();
	
	prevent_tail_call_optimization();
}

/*==============================================================================================*
 *										task2 -- init()											*
 *==============================================================================================*/
void register_diskfs(void);
int tty_class_init(void);
int chr_dev_init(void);
int init_elf_binfmt(void);
static void do_initcalls(void)
{
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


void myos_ata_probe();
extern void init_ATArqd();
extern void kjmp_to_doexecve();
int kernel_init(void *unused)
{
	int ret;

	/*
	 * Wait until kthreadd is all set-up.
	 */
	wait_for_completion(&kthreadd_done);

	kernel_init_freeable();

	// do_name();
	init_ATArqd();
	
	myos_ata_probe();

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
	// gfp_allowed_mask = __GFP_BITS_MASK;

	/*
	 * init can allocate pages on any node
	 */
	// set_mems_allowed(node_states[N_MEMORY]);

	// cad_pid = get_pid(task_pid(current));

	// smp_prepare_cpus(setup_max_cpus);

	workqueue_init();

	// init_mm_internals();

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