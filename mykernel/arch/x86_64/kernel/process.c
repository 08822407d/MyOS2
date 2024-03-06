// SPDX-License-Identifier: GPL-2.0
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/lib/errno.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/mm.h>
#include <linux/smp/smp.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/uaccess.h>
#include <linux/sched/debug.h>
#include <linux/init/init.h>
#include <linux/kernel/export.h>
#include <asm/cpu.h>
#include <asm/apic.h>
#include <asm/tlbflush.h>
#include <asm/switch_to.h>
#include <asm/frame.h>
#include <asm/insns.h>

// #include "process.h"


#include <linux/kernel/ptrace.h>

int copy_thread(task_s *p, const kclone_args_s *args)
{
	unsigned long clone_flags = args->flags;
	unsigned long sp = args->stack;
	unsigned long tls = args->tls;
	task_kframe_s *frame;
	fork_frame_s *fork_frame;
	pt_regs_s *childregs;
	int ret = 0;

	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, fork_frame_s, regs);
	frame = &fork_frame->frame;

	// frame->bp = (reg_t)encode_frame_pointer(childregs);
	frame->ret_addr = (reg_t)ret_from_fork;
	p->thread.sp = (reg_t)fork_frame;
	// p->thread.io_bitmap = NULL;
	// p->thread.iopl_warn = 0;
	// memset(p->thread.ptrace_bps, 0, sizeof(p->thread.ptrace_bps));

	current_save_fsgs();
	p->thread.fsindex = current->thread.fsindex;
	p->thread.fsbase = current->thread.fsbase;
	p->thread.gsindex = current->thread.gsindex;
	p->thread.gsbase = current->thread.gsbase;

	savesegment(es, p->thread.es);
	savesegment(ds, p->thread.ds);

	// if (p->mm && (clone_flags & (CLONE_VM | CLONE_VFORK)) == CLONE_VM)
	// 	set_bit(MM_CONTEXT_LOCK_LAM, &p->mm->context.flags);


	// fpu_clone(p, clone_flags);

	/* Kernel thread ? */
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}

	// /*
	//  * Clone current's PKRU value from hardware. tsk->thread.pkru
	//  * is only valid when scheduled out.
	//  */
	// p->thread.pkru = read_pkru();

	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;

	if (unlikely(args->fn)) {
		/*
		 * A user space thread, but it doesn't return to
		 * ret_after_fork().
		 *
		 * In order to indicate that to tools like gdb,
		 * we reset the stack and instruction pointers.
		 *
		 * It does the same kernel frame setup to return to a kernel
		 * function that a kernel thread does.
		 */
		childregs->sp = 0;
		childregs->ip = 0;
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}

	// /* Set a new TLS for the child thread? */
	// if (clone_flags & CLONE_SETTLS)
	// 	ret = set_new_tls(p, tls);

	// if (!ret && unlikely(test_tsk_thread_flag(current, TIF_IO_BITMAP)))
	// 	io_bitmap_share(p);

	return ret;
}