// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 1995  Linus Torvalds
 *
 *  Pentium III FXSR, SSE support
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 *
 *  X86-64 port
 *	Andi Kleen.
 *
 *	CPU hotplug support - ashok.raj@intel.com
 */

/*
 * This file handles the architecture-dependent parts of process handling..
 */

#include <linux/kernel/cpu.h>
#include <linux/lib/errno.h>
#include <linux/kernel/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
// #include <linux/elfcore.h>
#include <linux/smp/smp.h>
#include <linux/kernel/slab.h>
// #include <linux/user.h>
// #include <linux/interrupt.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/export.h>
#include <linux/kernel/ptrace.h>
// #include <linux/notifier.h>
// #include <linux/kprobes.h>
// #include <linux/kdebug.h>
// #include <linux/prctl.h>
#include <linux/kernel/uaccess.h>
#include <linux/kernel/io.h>
// #include <linux/ftrace.h>
#include <linux/kernel/syscalls.h>

#include <asm/processor.h>
// #include <asm/pkru.h>
// #include <asm/fpu/sched.h>
#include <asm/mmu_context.h>
// #include <asm/prctl.h>
#include <asm/desc.h>
// #include <asm/proto.h>
// #include <asm/ia32.h>
// #include <asm/debugreg.h>
#include <asm/switch_to.h>
// #include <asm/xen/hypervisor.h>
// #include <asm/vdso.h>
// #include <asm/resctrl.h>
// #include <asm/unistd.h>
#include <asm/fsgsbase.h>
// #ifdef CONFIG_IA32_EMULATION
// /* Not included via unistd.h */
// #	include <asm/unistd_32_ia32.h>
// #endif

// #include "process.h"

#include <linux/sched/myos_percpu.h>


void x86_fsbase_write_task(task_s* task, unsigned long fsbase)
{
	// WARN_ON_ONCE(task == current);
	task->thread.fsbase = fsbase;
}

void x86_gsbase_write_task(task_s *task, unsigned long gsbase)
{
	// WARN_ON_ONCE(task == current);
	task->thread.gsbase = gsbase;
}

static void
start_thread_common(pt_regs_s *regs,
		unsigned long new_ip, unsigned long new_sp,
		unsigned int _cs, unsigned int _ss, unsigned int _ds)
{
	// WARN_ON_ONCE(regs != current_pt_regs());

	// if (static_cpu_has(X86_BUG_NULL_SEG)) {
	// 	/* Loading zero below won't clear the base. */
	// 	loadsegment(fs, __USER_DS);
	// 	load_gs_index(__USER_DS);
	// }

	// loadsegment(fs, 0);
	loadsegment(es, _ds);
	loadsegment(ds, _ds);
	// load_gs_index(0);

	regs->ip		= new_ip;
	regs->sp		= new_sp;
	regs->cs		= _cs;
	regs->ss		= _ss;
	regs->flags		= X86_EFLAGS_IF;
}

void
start_thread(pt_regs_s *regs, unsigned long new_ip, unsigned long new_sp)
{
	start_thread_common(regs, new_ip, new_sp, __USER_CS, __USER_DS, 0);
}

/*
 *	switch_to(x,y) should switch tasks from x to y.
 *
 * This could still be optimized:
 * - fold all the options into a flag word and test it with a single test.
 * - could test fs/gs bitsliced
 *
 * Kprobes not supported here. Set the probe on schedule instead.
 * Function graph tracer not supported too.
 */
__visible notrace void __switch_to(task_s *prev_p, task_s *next_p)
{
	// struct thread_struct *prev = &prev_p->thread;
	// struct thread_struct *next = &next_p->thread;
	// struct fpu *prev_fpu = &prev->fpu;
	// int cpu = smp_processor_id();

	// WARN_ON_ONCE(IS_ENABLED(CONFIG_DEBUG_ENTRY) &&
	// 	     this_cpu_read(pcpu_hot.hardirq_stack_inuse));

	// if (!test_thread_flag(TIF_NEED_FPU_LOAD))
	// 	switch_fpu_prepare(prev_fpu, cpu);

	// /* We must save %fs and %gs before load_TLS() because
	//  * %fs and %gs may be cleared by load_TLS().
	//  *
	//  * (e.g. xen_load_tls())
	//  */
	// save_fsgs(prev_p);

	// /*
	//  * Load TLS before restoring any segments so that segment loads
	//  * reference the correct GDT entries.
	//  */
	// load_TLS(next, cpu);

	// /*
	//  * Leave lazy mode, flushing any hypercalls made here.  This
	//  * must be done after loading TLS entries in the GDT but before
	//  * loading segments that might reference them.
	//  */
	// arch_end_context_switch(next_p);

	// /* Switch DS and ES.
	//  *
	//  * Reading them only returns the selectors, but writing them (if
	//  * nonzero) loads the full descriptor from the GDT or LDT.  The
	//  * LDT for next is loaded in switch_mm, and the GDT is loaded
	//  * above.
	//  *
	//  * We therefore need to write new values to the segment
	//  * registers on every context switch unless both the new and old
	//  * values are zero.
	//  *
	//  * Note that we don't need to do anything for CS and SS, as
	//  * those are saved and restored as part of pt_regs.
	//  */
	// savesegment(es, prev->es);
	// if (unlikely(next->es | prev->es))
	// 	loadsegment(es, next->es);

	// savesegment(ds, prev->ds);
	// if (unlikely(next->ds | prev->ds))
	// 	loadsegment(ds, next->ds);

	// x86_fsgsbase_load(prev, next);

	// x86_pkru_load(prev, next);

	/*
	 * Switch the PDA and FPU contexts.
	 */
	*this_cpu_ptr(&pcpu_hot.current_task) = next_p;
	*this_cpu_ptr(&pcpu_hot.top_of_stack) = task_top_of_stack(next_p);

	// switch_fpu_finish();

	/* Reload sp0. */
	update_task_stack(next_p);

	// switch_to_extra(prev_p, next_p);

	// if (static_cpu_has_bug(X86_BUG_SYSRET_SS_ATTRS)) {
	// 	/*
	// 	 * AMD CPUs have a misfeature: SYSRET sets the SS selector but
	// 	 * does not update the cached descriptor.  As a result, if we
	// 	 * do SYSRET while SS is NULL, we'll end up in user mode with
	// 	 * SS apparently equal to __USER_DS but actually unusable.
	// 	 *
	// 	 * The straightforward workaround would be to fix it up just
	// 	 * before SYSRET, but that would slow down the system call
	// 	 * fast paths.  Instead, we ensure that SS is never NULL in
	// 	 * system call context.  We do this by replacing NULL SS
	// 	 * selectors at every context switch.  SYSCALL sets up a valid
	// 	 * SS, so the only way to get NULL is to re-enter the kernel
	// 	 * from CPL 3 through an interrupt.  Since that can't happen
	// 	 * in the same task as a running syscall, we are guaranteed to
	// 	 * context switch between every interrupt vector entry and a
	// 	 * subsequent SYSRET.
	// 	 *
	// 	 * We read SS first because SS reads are much faster than
	// 	 * writes.  Out of caution, we force SS to __KERNEL_DS even if
	// 	 * it previously had a different non-NULL value.
	// 	 */
	// 	unsigned short ss_sel;
	// 	savesegment(ss, ss_sel);
	// 	if (ss_sel != __KERNEL_DS)
	// 		loadsegment(ss, __KERNEL_DS);
	// }

	// /* Load the Intel cache allocation PQR MSR. */
	// resctrl_sched_in(next_p);

	// return prev_p;
}