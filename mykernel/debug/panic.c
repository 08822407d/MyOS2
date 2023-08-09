// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/kernel/panic.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * This function is used through-out the kernel (including mm and fs)
 * to indicate a major problem.
 */
// #include <linux/debug_locks.h>
#include <linux/sched/debug.h>
// #include <linux/interrupt.h>
// #include <linux/kgdb.h>
// #include <linux/kmsg_dump.h>
// #include <linux/kallsyms.h>
// #include <linux/notifier.h>
// #include <linux/vt_kern.h>
// #include <linux/module.h>
// #include <linux/random.h>
// #include <linux/ftrace.h>
// #include <linux/reboot.h>
#include <linux/kernel/delay.h>
// #include <linux/kexec.h>
// #include <linux/panic_notifier.h>
#include <linux/kernel/sched.h>
// #include <linux/string_helpers.h>
// #include <linux/sysrq.h>
#include <linux/init/init.h>
// #include <linux/nmi.h>
// #include <linux/console.h>
#include <linux/debug/bug.h>
// #include <linux/ratelimit.h>
// #include <linux/debugfs.h>
// #include <linux/sysfs.h>
// #include <linux/context_tracking.h>
// #include <trace/events/error_report.h>
#include <asm/sections.h>

#define PANIC_TIMER_STEP 100
#define PANIC_BLINK_SPD 18


/**
 *	panic - halt the system
 *	@fmt: The text string to print
 *
 *	Display a message, then perform cleanups.
 *
 *	This function never returns.
 */
void panic(const char *fmt, ...)
{
	static char buf[1024];
	va_list args;
	long i, i_next = 0, len;
	int state = 0;
	// int old_cpu, this_cpu;
	// bool _crash_kexec_post_notifiers = crash_kexec_post_notifiers;

	// if (panic_on_warn) {
	// 	/*
	// 	 * This thread may hit another WARN() in the panic path.
	// 	 * Resetting this prevents additional WARN() from panicking the
	// 	 * system on this thread.  Other threads are blocked by the
	// 	 * panic_mutex in panic().
	// 	 */
	// 	panic_on_warn = 0;
	// }

	/*
	 * Disable local interrupts. This will prevent panic_smp_self_stop
	 * from deadlocking the first cpu that invokes the panic, since
	 * there is nothing to prevent an interrupt handler (that runs
	 * after setting panic_cpu) from invoking panic() again.
	 */
	// local_irq_disable();
	// preempt_disable_notrace();

	/*
	 * It's possible to come here directly from a panic-assertion and
	 * not have preempt disabled. Some functions called from here want
	 * preempt to be disabled. No point enabling it later though...
	 *
	 * Only one CPU is allowed to execute the panic code from here. For
	 * multiple parallel invocations of panic, all other CPUs either
	 * stop themself or will wait until they are stopped by the 1st CPU
	 * with smp_send_stop().
	 *
	 * `old_cpu == PANIC_CPU_INVALID' means this is the 1st CPU which
	 * comes here, so go ahead.
	 * `old_cpu == this_cpu' means we came from nmi_panic() which sets
	 * panic_cpu to this CPU.  In this case, this is also the 1st CPU.
	 */
	// this_cpu = raw_smp_processor_id();
	// old_cpu  = atomic_cmpxchg(&panic_cpu, PANIC_CPU_INVALID, this_cpu);

	// if (old_cpu != PANIC_CPU_INVALID && old_cpu != this_cpu)
	// 	panic_smp_self_stop();

	// console_verbose();
	// bust_spinlocks(1);
	va_start(args, fmt);
	len = vscnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	// if (len && buf[len - 1] == '\n')
	// 	buf[len - 1] = '\0';

	pr_emerg("Kernel panic - not syncing: %s\n", buf);
// #ifdef CONFIG_DEBUG_BUGVERBOSE
	// /*
	//  * Avoid nested stack-dumping if a panic occurs during oops processing
	//  */
	// if (!test_taint(TAINT_DIE) && oops_in_progress <= 1)
	// 	dump_stack();
// #endif

	// /*
	//  * If kgdb is enabled, give it a chance to run before we stop all
	//  * the other CPUs or else we won't be able to debug processes left
	//  * running on them.
	//  */
	// kgdb_panic(buf);

	// /*
	//  * If we have crashed and we have a crash kernel loaded let it handle
	//  * everything else.
	//  * If we want to run this after calling panic_notifiers, pass
	//  * the "crash_kexec_post_notifiers" option to the kernel.
	//  *
	//  * Bypass the panic_cpu check and call __crash_kexec directly.
	//  */
	// if (!_crash_kexec_post_notifiers)
	// 	__crash_kexec(NULL);

	// panic_other_cpus_shutdown(_crash_kexec_post_notifiers);

	// /*
	//  * Run any panic handlers, including those that might need to
	//  * add information to the kmsg dump output.
	//  */
	// atomic_notifier_call_chain(&panic_notifier_list, 0, buf);

	// panic_print_sys_info(false);

	// kmsg_dump(KMSG_DUMP_PANIC);

	// /*
	//  * If you doubt kdump always works fine in any situation,
	//  * "crash_kexec_post_notifiers" offers you a chance to run
	//  * panic_notifiers and dumping kmsg before kdump.
	//  * Note: since some panic_notifiers can make crashed kernel
	//  * more unstable, it can increase risks of the kdump failure too.
	//  *
	//  * Bypass the panic_cpu check and call __crash_kexec directly.
	//  */
	// if (_crash_kexec_post_notifiers)
	// 	__crash_kexec(NULL);

	// console_unblank();

	// /*
	//  * We may have ended up stopping the CPU holding the lock (in
	//  * smp_send_stop()) while still having some valuable data in the console
	//  * buffer.  Try to acquire the lock then release it regardless of the
	//  * result.  The release will also print the buffers out.  Locks debug
	//  * should be disabled to avoid reporting bad unlock balance when
	//  * panic() is not being callled from OOPS.
	//  */
	// debug_locks_off();
	// console_flush_on_panic(CONSOLE_FLUSH_PENDING);

	// panic_print_sys_info(true);

	// if (!panic_blink)
	// 	panic_blink = no_blink;

	// if (panic_timeout > 0) {
	// 	/*
	// 	 * Delay timeout seconds before rebooting the machine.
	// 	 * We can't use the "normal" timers since we just panicked.
	// 	 */
	// 	pr_emerg("Rebooting in %d seconds..\n", panic_timeout);

	// 	for (i = 0; i < panic_timeout * 1000; i += PANIC_TIMER_STEP) {
	// 		touch_nmi_watchdog();
	// 		if (i >= i_next) {
	// 			i += panic_blink(state ^= 1);
	// 			i_next = i + 3600 / PANIC_BLINK_SPD;
	// 		}
	// 		mdelay(PANIC_TIMER_STEP);
	// 	}
	// }
	// if (panic_timeout != 0) {
	// 	/*
	// 	 * This will not be a clean reboot, with everything
	// 	 * shutting down.  But if there is a chance of
	// 	 * rebooting the system it will be rebooted.
	// 	 */
	// 	if (panic_reboot_mode != REBOOT_UNDEFINED)
	// 		reboot_mode = panic_reboot_mode;
	// 	emergency_restart();
	// }
// #ifdef __sparc__
	// {
	// 	extern int stop_a_enabled;
	// 	/* Make sure the user can actually press Stop-A (L1-A) */
	// 	stop_a_enabled = 1;
	// 	pr_emerg("Press Stop-A (L1-A) from sun keyboard or send break\n"
	// 		 "twice on console to return to the boot prom\n");
	// }
// #endif
// #if defined(CONFIG_S390)
	// disabled_wait();
// #endif
	pr_emerg("---[ end Kernel panic - not syncing: %s ]---\n", buf);

	// /* Do not scroll important messages printed above */
	// suppress_printk = 1;
	// local_irq_enable();
	// for (i = 0; ; i += PANIC_TIMER_STEP) {
	// 	touch_softlockup_watchdog();
	// 	if (i >= i_next) {
	// 		i += panic_blink(state ^= 1);
	// 		i_next = i + 3600 / PANIC_BLINK_SPD;
	// 	}
	// 	mdelay(PANIC_TIMER_STEP);
	// }
}