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
#include <linux/kernel/sched.h>
#include <linux/kernel/ptrace.h>
#include <linux/debug/kdebug.h>
#include <linux/debug/ftrace.h>

#include <asm/insns.h>
#include <asm/prctl.h>


/* Prints also some state that isn't saved in the pt_regs */
void __show_regs(pt_regs_s *regs,
		enum show_regs_mode mode, const char *log_lvl)
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
static noinstr ulong
__rdgsbase_inactive(void) {
	ulong gsbase;

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
__wrgsbase_inactive(ulong gsbase) {
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


static __always_inline void
save_fsgs(task_s *task) {
	savesegment(fs, task->thread.fsindex);
	savesegment(gs, task->thread.gsindex);
	// if (static_cpu_has(X86_FEATURE_FSGSBASE)) {
	if (boot_cpu_data.x86_capa_bits.FSGSBASE) {
		/*
		 * If FSGSBASE is enabled, we can't make any useful guesses
		 * about the base, and user code expects us to save the current
		 * value.  Fortunately, reading the base directly is efficient.
		 */
		task->thread.fsbase = rdfsbase();
		task->thread.gsbase = __rdgsbase_inactive();
	} else {
		// save_base_legacy(task, task->thread.fsindex, FS);
		// save_base_legacy(task, task->thread.gsindex, GS);
	}
}

/*
 * While a process is running,current->thread.fsbase and current->thread.gsbase
 * may not match the corresponding CPU registers (see save_base_legacy()).
 */
void current_save_fsgs(void)
{
	ulong flags;

	/* Interrupts need to be off for FSGSBASE */
	local_irq_save(flags);
	save_fsgs(current);
	local_irq_restore(flags);
}

static __always_inline void 
loadseg(enum which_selector which, ushort sel) {
	if (which == FS)
		loadsegment(fs, sel);
	else
		load_gs_index(sel);
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


void x86_gsbase_write_cpu_inactive(ulong gsbase)
{
	// if (boot_cpu_has(X86_FEATURE_FSGSBASE)) {
	if (boot_cpu_data.x86_capa_bits.FSGSBASE) {
		ulong flags;

		local_irq_save(flags);
		__wrgsbase_inactive(gsbase);
		local_irq_restore(flags);
	} else {
		wrmsrl(MSR_KERNEL_GS_BASE, gsbase);
	}
}


void x86_fsbase_write_task(task_s* task, ulong fsbase)
{
	WARN_ON_ONCE(task == current);
	task->thread.fsbase = fsbase;
}

void x86_gsbase_write_task(task_s *task, ulong gsbase)
{
	WARN_ON_ONCE(task == current);
	task->thread.gsbase = gsbase;
}

static void
start_thread_common(pt_regs_s *regs, ulong new_ip,
		ulong new_sp, u16 _cs, u16 _ss, u16 _ds) {
	WARN_ON_ONCE(regs != current_pt_regs());

	// if (static_cpu_has(X86_BUG_NULL_SEG)) {
	// 	/* Loading zero below won't clear the base. */
		loadsegment(fs, __USER_DS);
		load_gs_index(__USER_DS);
	// }

	// reset_thread_features();

	loadsegment(fs, 0);
	loadsegment(es, _ds);
	loadsegment(ds, _ds);
	load_gs_index(0);

	regs->ip		= new_ip;
	regs->sp		= new_sp;
	regs->cs		= _cs;
	regs->ss		= _ss;
	// /*
	//  * Allow single-step trap and NMI when starting a new task, thus
	//  * once the new task enters user space, single-step trap and NMI
	//  * are both enabled immediately.
	//  *
	//  * Entering a new task is logically speaking a return from a
	//  * system call (exec, fork, clone, etc.). As such, if ptrace
	//  * enables single stepping a single step exception should be
	//  * allowed to trigger immediately upon entering user space.
	//  * This is not optional.
	//  *
	//  * NMI should *never* be disabled in user space. As such, this
	//  * is an optional, opportunistic way to catch errors.
	//  *
	//  * Paranoia: High-order 48 bits above the lowest 16 bit SS are
	//  * discarded by the legacy IRET instruction on all Intel, AMD,
	//  * and Cyrix/Centaur/VIA CPUs, thus can be set unconditionally,
	//  * even when FRED is not enabled. But we choose the safer side
	//  * to use these bits only when FRED is enabled.
	//  */
	// if (cpu_feature_enabled(X86_FEATURE_FRED)) {
	// 	regs->fred_ss.swevent	= true;
	// 	regs->fred_ss.nmi	= true;
	// }

	regs->flags		= X86_EFLAGS_IF | X86_EFLAGS_FIXED;
}

void
start_thread(pt_regs_s *regs, ulong new_ip, ulong new_sp)
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
	int cpu = smp_processor_id();

	// WARN_ON_ONCE(IS_ENABLED(CONFIG_DEBUG_ENTRY) &&
	// 	     this_cpu_read(pcpu_hot.hardirq_stack_inuse));

	// if (!test_tsk_thread_flag(prev_p, TIF_NEED_FPU_LOAD))
	// 	switch_fpu_prepare(prev_p, cpu);

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

	/* Switch DS and ES.
	 *
	 * Reading them only returns the selectors, but writing them (if
	 * nonzero) loads the full descriptor from the GDT or LDT.  The
	 * LDT for next is loaded in switch_mm, and the GDT is loaded
	 * above.
	 *
	 * We therefore need to write new values to the segment
	 * registers on every context switch unless both the new and old
	 * values are zero.
	 *
	 * Note that we don't need to do anything for CS and SS, as
	 * those are saved and restored as part of pt_regs.
	 */
	savesegment(es, prev->es);
	if (unlikely(next->es | prev->es))
		loadsegment(es, next->es);

	savesegment(ds, prev->ds);
	if (unlikely(next->ds | prev->ds))
		loadsegment(ds, next->ds);

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



long do_arch_prctl_64(task_s *task, int option, ulong arg2)
{
	int ret = 0;

	switch (option) {
	case ARCH_SET_GS: {
		if (unlikely(arg2 >= TASK_SIZE_MAX))
			return -EPERM;

		preempt_disable();
		/*
		 * ARCH_SET_GS has always overwritten the index
		 * and the base. Zero is the most sensible value
		 * to put in the index, and is the only value that
		 * makes any sense if FSGSBASE is unavailable.
		 */
		// if (task == current) {
			loadseg(GS, 0);
			x86_gsbase_write_cpu_inactive(arg2);

			/*
			 * On non-FSGSBASE systems, save_base_legacy() expects
			 * that we also fill in thread.gsbase.
			 */
			task->thread.gsbase = arg2;
		// } else {
		// 	task->thread.gsindex = 0;
		// 	x86_gsbase_write_task(task, arg2);
		// }
		preempt_enable();
		break;
	}
	case ARCH_SET_FS: {
		/*
		 * Not strictly needed for %fs, but do it for symmetry
		 * with %gs
		 */
		if (unlikely(arg2 >= TASK_SIZE_MAX))
			return -EPERM;

		preempt_disable();
		/*
		 * Set the selector to 0 for the same reason
		 * as %gs above.
		 */
		// if (task == current) {
			loadseg(FS, 0);
			x86_fsbase_write_cpu(arg2);

			/*
			 * On non-FSGSBASE systems, save_base_legacy() expects
			 * that we also fill in thread.fsbase.
			 */
			task->thread.fsbase = arg2;
		// } else {
		// 	task->thread.fsindex = 0;
		// 	x86_fsbase_write_task(task, arg2);
		// }
		preempt_enable();
		break;
	}
	case ARCH_GET_FS: {
		// unsigned long base = x86_fsbase_read_task(task);

		// ret = put_user(base, (unsigned long __user *)arg2);
		break;
	}
	case ARCH_GET_GS: {
		// unsigned long base = x86_gsbase_read_task(task);

		// ret = put_user(base, (unsigned long __user *)arg2);
		break;
	}

// #ifdef CONFIG_CHECKPOINT_RESTORE
// # ifdef CONFIG_X86_X32_ABI
// 	case ARCH_MAP_VDSO_X32:
// 		return prctl_map_vdso(&vdso_image_x32, arg2);
// # endif
// # if defined CONFIG_X86_32 || defined CONFIG_IA32_EMULATION
// 	case ARCH_MAP_VDSO_32:
// 		return prctl_map_vdso(&vdso_image_32, arg2);
// # endif
// 	case ARCH_MAP_VDSO_64:
// 		return prctl_map_vdso(&vdso_image_64, arg2);
// #endif
// #ifdef CONFIG_ADDRESS_MASKING
// 	case ARCH_GET_UNTAG_MASK:
// 		return put_user(task->mm->context.untag_mask,
// 				(unsigned long __user *)arg2);
// 	case ARCH_ENABLE_TAGGED_ADDR:
// 		return prctl_enable_tagged_addr(task->mm, arg2);
// 	case ARCH_FORCE_TAGGED_SVA:
// 		if (current != task)
// 			return -EINVAL;
// 		set_bit(MM_CONTEXT_FORCE_TAGGED_SVA, &task->mm->context.flags);
// 		return 0;
// 	case ARCH_GET_MAX_TAG_BITS:
// 		if (!cpu_feature_enabled(X86_FEATURE_LAM))
// 			return put_user(0, (unsigned long __user *)arg2);
// 		else
// 			return put_user(LAM_U57_BITS, (unsigned long __user *)arg2);
// #endif
	case ARCH_SHSTK_ENABLE:
	case ARCH_SHSTK_DISABLE:
	case ARCH_SHSTK_LOCK:
	case ARCH_SHSTK_UNLOCK:
	case ARCH_SHSTK_STATUS:
		// return shstk_prctl(task, option, arg2);
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}