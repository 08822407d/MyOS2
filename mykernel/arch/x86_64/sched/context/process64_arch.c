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
// #include "../sched_api_arch.h"
#include <linux/kernel/sched.h>
#include <linux/kernel/ptrace.h>
#include <linux/debug/kdebug.h>
#include <linux/debug/ftrace.h>

#include <asm/insns.h>

// #include "process.h"

/* Prints also some state that isn't saved in the pt_regs */
void __show_regs(pt_regs_s *regs, enum show_regs_mode mode,
		 const char *log_lvl)
{
	// unsigned long cr0 = 0L, cr2 = 0L, cr3 = 0L, cr4 = 0L, fs, gs, shadowgs;
	// unsigned long d0, d1, d2, d3, d6, d7;
	// unsigned int fsindex, gsindex;
	// unsigned int ds, es;

	// show_iret_regs(regs, log_lvl);

	// if (regs->orig_ax != -1)
	// 	pr_cont(" ORIG_RAX: %016lx\n", regs->orig_ax);
	// else
	// 	pr_cont("\n");

	// printk("%sRAX: %016lx RBX: %016lx RCX: %016lx\n",
	//        log_lvl, regs->ax, regs->bx, regs->cx);
	// printk("%sRDX: %016lx RSI: %016lx RDI: %016lx\n",
	//        log_lvl, regs->dx, regs->si, regs->di);
	// printk("%sRBP: %016lx R08: %016lx R09: %016lx\n",
	//        log_lvl, regs->bp, regs->r8, regs->r9);
	// printk("%sR10: %016lx R11: %016lx R12: %016lx\n",
	//        log_lvl, regs->r10, regs->r11, regs->r12);
	// printk("%sR13: %016lx R14: %016lx R15: %016lx\n",
	//        log_lvl, regs->r13, regs->r14, regs->r15);

	// if (mode == SHOW_REGS_SHORT)
	// 	return;

	// if (mode == SHOW_REGS_USER) {
	// 	rdmsrl(MSR_FS_BASE, &fs);
	// 	rdmsrl(MSR_KERNEL_GS_BASE, &shadowgs);
	// 	printk("%sFS:  %016lx GS:  %016lx\n",
	// 	       log_lvl, fs, shadowgs);
	// 	return;
	// }

	// asm("movl %%ds,%0" : "=r" (ds));
	// asm("movl %%es,%0" : "=r" (es));
	// asm("movl %%fs,%0" : "=r" (fsindex));
	// asm("movl %%gs,%0" : "=r" (gsindex));

	// rdmsrl(MSR_FS_BASE, &fs);
	// rdmsrl(MSR_GS_BASE, &gs);
	// rdmsrl(MSR_KERNEL_GS_BASE, &shadowgs);

	// cr0 = read_cr0();
	// cr2 = read_cr2();
	// cr3 = __read_cr3();
	// cr4 = __read_cr4();

	// printk("%sFS:  %016lx(%04x) GS:%016lx(%04x) knlGS:%016lx\n",
	//        log_lvl, fs, fsindex, gs, gsindex, shadowgs);
	// printk("%sCS:  %04lx DS: %04x ES: %04x CR0: %016lx\n",
	// 	log_lvl, regs->cs, ds, es, cr0);
	// printk("%sCR2: %016lx CR3: %016lx CR4: %016lx\n",
	// 	log_lvl, cr2, cr3, cr4);

	// get_debugreg(d0, 0);
	// get_debugreg(d1, 1);
	// get_debugreg(d2, 2);
	// get_debugreg(d3, 3);
	// get_debugreg(d6, 6);
	// get_debugreg(d7, 7);

	// /* Only print out debug registers if they are in their non-default state. */
	// if (!((d0 == 0) && (d1 == 0) && (d2 == 0) && (d3 == 0) &&
	//     (d6 == DR6_RESERVED) && (d7 == 0x400))) {
	// 	printk("%sDR0: %016lx DR1: %016lx DR2: %016lx\n",
	// 	       log_lvl, d0, d1, d2);
	// 	printk("%sDR3: %016lx DR6: %016lx DR7: %016lx\n",
	// 	       log_lvl, d3, d6, d7);
	// }

	// if (cpu_feature_enabled(X86_FEATURE_OSPKE))
	// 	printk("%sPKRU: %08x\n", log_lvl, read_pkru());
}


enum which_selector {
	FS,
	GS
};

/*
 * Out of line to be protected from kprobes and tracing. If this would be
 * traced or probed than any access to a per CPU variable happens with
 * the wrong GS.
 *
 * It is not used on Xen paravirt. When paravirt support is needed, it
 * needs to be renamed with native_ prefix.
 */
static noinstr unsigned long
__rdgsbase_inactive(void) {
	unsigned long gsbase;

	// lockdep_assert_irqs_disabled();

	// if (!cpu_feature_enabled(X86_FEATURE_XENPV)) {
		native_swapgs();
		gsbase = rdgsbase();
		native_swapgs();
	// } else {
	// 	instrumentation_begin();
	// 	rdmsrl(MSR_KERNEL_GS_BASE, gsbase);
	// 	instrumentation_end();
	// }

	return gsbase;
}

/*
 * Out of line to be protected from kprobes and tracing. If this would be
 * traced or probed than any access to a per CPU variable happens with
 * the wrong GS.
 *
 * It is not used on Xen paravirt. When paravirt support is needed, it
 * needs to be renamed with native_ prefix.
 */
static noinstr void
__wrgsbase_inactive(unsigned long gsbase) {
	// lockdep_assert_irqs_disabled();

	// if (!cpu_feature_enabled(X86_FEATURE_XENPV)) {
		native_swapgs();
		wrgsbase(gsbase);
		native_swapgs();
	// } else {
	// 	instrumentation_begin();
	// 	wrmsrl(MSR_KERNEL_GS_BASE, gsbase);
	// 	instrumentation_end();
	// }
}


static __always_inline void save_fsgs(task_s *task) {
	savesegment(fs, task->thread.fsindex);
	savesegment(gs, task->thread.gsindex);
	// if (static_cpu_has(X86_FEATURE_FSGSBASE)) {
		/*
		 * If FSGSBASE is enabled, we can't make any useful guesses
		 * about the base, and user code expects us to save the current
		 * value.  Fortunately, reading the base directly is efficient.
		 */
		task->thread.fsbase = rdfsbase();
		task->thread.gsbase = __rdgsbase_inactive();
	// } else {
	// 	save_base_legacy(task, task->thread.fsindex, FS);
	// 	save_base_legacy(task, task->thread.gsindex, GS);
	// }
}

/*
 * While a process is running,current->thread.fsbase and current->thread.gsbase
 * may not match the corresponding CPU registers (see save_base_legacy()).
 */
void current_save_fsgs(void)
{
	unsigned long flags;

	/* Interrupts need to be off for FSGSBASE */
	local_irq_save(flags);
	save_fsgs(current);
	local_irq_restore(flags);
}


static __always_inline void
x86_fsgsbase_load(thread_s *prev, thread_s *next) {
	// if (static_cpu_has(X86_FEATURE_FSGSBASE)) {
	// 	/* Update the FS and GS selectors if they could have changed. */
	// 	if (unlikely(prev->fsindex || next->fsindex))
	// 		loadseg(FS, next->fsindex);
	// 	if (unlikely(prev->gsindex || next->gsindex))
	// 		loadseg(GS, next->gsindex);

	// 	/* Update the bases. */
	// 	wrfsbase(next->fsbase);
	// 	__wrgsbase_inactive(next->gsbase);
	// } else {
	// 	load_seg_legacy(prev->fsindex, prev->fsbase,
	// 			next->fsindex, next->fsbase, FS);
	// 	load_seg_legacy(prev->gsindex, prev->gsbase,
	// 			next->gsindex, next->gsbase, GS);
	// }
}



void x86_fsbase_write_task(task_s* task, unsigned long fsbase)
{
	WARN_ON_ONCE(task == current);
	task->thread.fsbase = fsbase;
}

void x86_gsbase_write_task(task_s *task, unsigned long gsbase)
{
	WARN_ON_ONCE(task == current);
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
__no_kmsan_checks
__visible __notrace_funcgraph task_s
*__switch_to(task_s *prev_p, task_s *next_p)
{
	thread_s *prev = &prev_p->thread;
	thread_s *next = &next_p->thread;
	// struct fpu *prev_fpu = &prev->fpu;
	int cpu = smp_processor_id();

	// WARN_ON_ONCE(IS_ENABLED(CONFIG_DEBUG_ENTRY) &&
	// 	     this_cpu_read(pcpu_hot.hardirq_stack_inuse));

	// if (!test_thread_flag(TIF_NEED_FPU_LOAD))
	// 	switch_fpu_prepare(prev_fpu, cpu);

	/* We must save %fs and %gs before load_TLS() because
	 * %fs and %gs may be cleared by load_TLS().
	 *
	 * (e.g. xen_load_tls())
	 */
	save_fsgs(prev_p);

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

	x86_fsgsbase_load(prev, next);

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

	return prev_p;
}