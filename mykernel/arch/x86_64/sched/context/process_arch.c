// SPDX-License-Identifier: GPL-2.0
// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

// #include "../sched_api_arch.h"
#include <linux/kernel/sched.h>
#include <linux/kernel/ptrace.h>

#include <uapi/asm/prctl.h>
#include <asm/proto.h>


// /*
//  * this gets called so that we can store lazy state into memory and copy the
//  * current task into the new thread.
//  */
// int arch_dup_task_struct(task_s *dst, task_s *src)
// {
// 	memcpy(dst, src, arch_task_struct_size);
// #ifdef CONFIG_VM86
// 	dst->thread.vm86 = NULL;
// #endif
// 	/* Drop the copied pointer to current's fpstate */
// 	dst->thread.fpu.fpstate = NULL;

// 	return 0;
// }

// void arch_release_task_struct(task_s *tsk)
// {
// 	if (fpu_state_size_dynamic())
// 		fpstate_free(&tsk->thread.fpu);
// }

/*
 * Free thread data structures etc..
 */
void exit_thread(task_s *tsk)
{
	// struct thread_struct *t = &tsk->thread;
	// struct fpu *fpu = &t->fpu;

	// if (test_thread_flag(TIF_IO_BITMAP))
	// 	io_bitmap_exit(tsk);

	// free_vm86(t);

	// fpu__drop(fpu);
}

static int
set_new_tls(task_s *p, ulong tls) {
	// struct user_desc __user *utls = (struct user_desc __user *)tls;

	// if (in_ia32_syscall())
	// 	return do_set_thread_area(p, -1, utls, 0);
	// else
	// 	return do_set_thread_area_64(p, ARCH_SET_FS, tls);
		return do_arch_prctl_64(p, ARCH_SET_FS, tls);
}

__visible void
ret_from_fork(task_s *prev, pt_regs_s *regs,
		int (*fn)(void *), void *fn_arg)
{
	schedule_tail(prev);

	/* Is this a kernel thread? */
	if (unlikely(fn)) {
		fn(fn_arg);
		/*
		 * A kernel thread is allowed to return here after successfully
		 * calling kernel_execve().  Exit to userspace to complete the
		 * execve() syscall.
		 */
		regs->ax = 0;
	}

	// syscall_exit_to_user_mode(regs);
}

int copy_thread(task_s *p, const kclone_args_s *args)
{
	ulong clone_flags = args->flags;
	ulong sp = args->stack;
	ulong tls = args->tls;
	task_kframe_s *frame;
	fork_frame_s *fork_frame;
	pt_regs_s *childregs;
	int ret = 0;

	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, fork_frame_s, regs);
	frame = &fork_frame->frame;

	// frame->bp = (reg_t)encode_frame_pointer(childregs);
	frame->bp = (reg_t)childregs + 1;
	frame->ret_addr = (reg_t)ret_from_fork_asm;
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



ulong arch_align_stack(ulong sp)
{
	// if (!(current->personality & ADDR_NO_RANDOMIZE) &&
	// 		randomize_va_space)
	// 	sp -= get_random_u32_below(8192);
	return sp & ~0xf;
}