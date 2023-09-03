// source: linux-6.4.9

/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2000, 2001, 2002 Andi Kleen, SuSE Labs
 *
 *  Pentium III FXSR, SSE support
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 */

/*
 * Handle hardware traps and faults.
 */

// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

// #include <linux/context_tracking.h>
// #include <linux/interrupt.h>
// #include <linux/kallsyms.h>
// #include <linux/kmsan.h>
// #include <linux/spinlock.h>
// #include <linux/kprobes.h>
// #include <linux/uaccess.h>
// #include <linux/kdebug.h>
// #include <linux/kgdb.h>
// #include <linux/kernel.h>
// #include <linux/export.h>
// #include <linux/ptrace.h>
// #include <linux/uprobes.h>
// #include <linux/string.h>
// #include <linux/delay.h>
// #include <linux/errno.h>
// #include <linux/kexec.h>
// #include <linux/sched.h>
// #include <linux/sched/task_stack.h>
// #include <linux/timer.h>
// #include <linux/init.h>
// #include <linux/bug.h>
#include <linux/kernel/nmi.h>
// #include <linux/mm.h>
#include <linux/smp/smp.h>
// #include <linux/io.h>
// #include <linux/hardirq.h>
// #include <linux/atomic.h>
// #include <linux/iommu.h>

// #include <asm/stacktrace.h>
// #include <asm/processor.h>
// #include <asm/debugreg.h>
// #include <asm/realmode.h>
// #include <asm/text-patching.h>
// #include <asm/ftrace.h>
// #include <asm/traps.h>
#include <asm/desc.h>
// #include <asm/fpu/api.h>
#include <asm/cpu.h>
// #include <asm/cpu_entry_area.h>
// #include <asm/mce.h>
// #include <asm/fixmap.h>
// #include <asm/mach_traps.h>
// #include <asm/alternative.h>
// #include <asm/fpu/xstate.h>
// #include <asm/vm86.h>
// #include <asm/umip.h>
// #include <asm/insn.h>
// #include <asm/insn-eval.h>
// #include <asm/vdso.h>
// #include <asm/tdx.h>
// #include <asm/cfi.h>

// #include <asm/x86_init.h>


void __init trap_init(void)
{
	// /* Init cpu_entry_area before IST entries are set up */
	// setup_cpu_entry_areas();

	// /* Init GHCB memory pages when running as an SEV-ES guest */
	// sev_es_init_vc_handling();

	/* Initialize TSS before setting up traps so ISTs work */
	cpu_init_exception_handling();
	// /* Setup traps as cpu_init() might #GP */
	// idt_setup_traps();
	cpu_init();
}