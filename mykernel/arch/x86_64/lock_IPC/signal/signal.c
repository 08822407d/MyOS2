#include <linux/kernel/sched.h>
#include <linux/kernel/lock_ipc.h>
#include <asm/syscall.h>




/*
 * Set up a signal frame.
 */

/* x86 ABI requires 16-byte alignment */
#define FRAME_ALIGNMENT		16UL
#define MAX_FRAME_PADDING	(FRAME_ALIGNMENT - 1)

/*
 * There are four different struct types for signal frame: sigframe_ia32,
 * rt_sigframe_ia32, rt_sigframe_x32, and rt_sigframe. Use the worst case
 * -- the largest size. It means the size for 64-bit apps is a bit more
 * than needed, but this keeps the code simple.
 */
#define MAX_FRAME_SIGINFO_UCTXT_SIZE	sizeof(rt_sigframe_s)

/*
 * The FP state frame contains an XSAVE buffer which must be 64-byte aligned.
 * If a signal frame starts at an unaligned address, extra space is required.
 * This is the max alignment padding, conservatively.
 */
#define MAX_XSAVE_PADDING	63UL

/*
 * Determine which stack to use..
 */
void __user *
get_sigframe(ksignal_s *ksig, pt_regs_s *regs,
		size_t frame_size, void __user **fpstate) {

	k_sigaction_s *ka = &ksig->ka;
	// int ia32_frame = is_ia32_frame(ksig);
	/* Default to using normal stack */
	bool nested_altstack = on_sig_stack(regs->sp);
	bool entering_altstack = false;
	ulong math_size = 0;
	ulong sp = regs->sp;
	ulong buf_fx = 0;

	/* redzone */
	// if (!ia32_frame)
		sp -= 128;

	/* This is the X/Open sanctioned signal stack switching.  */
	if (ka->sa.sa_flags & SA_ONSTACK) {
		/*
		 * This checks nested_altstack via sas_ss_flags(). Sensible
		 * programs use SS_AUTODISARM, which disables that check, and
		 * programs that don't use SS_AUTODISARM get compatible.
		 */
		if (sas_ss_flags(sp) == 0) {
			sp = current->sas_ss_sp + current->sas_ss_size;
			entering_altstack = true;
		}
	// } else if (ia32_frame &&
	} else if (
			!nested_altstack &&
			regs->ss != __USER_DS &&
			!(ka->sa.sa_flags & SA_RESTORER) &&
			ka->sa.sa_restorer) {
		/* This is the legacy signal stack switching. */
		sp = (ulong) ka->sa.sa_restorer;
		entering_altstack = true;
	}

	// sp = fpu__alloc_mathframe(sp, ia32_frame, &buf_fx, &math_size);
	*fpstate = (void __user *)sp;

	sp -= frame_size;

	// if (ia32_frame)
	// 	/*
	// 	 * Align the stack pointer according to the i386 ABI,
	// 	 * i.e. so that on function entry ((sp + 4) & 15) == 0.
	// 	 */
	// 	sp = ((sp + 4) & -FRAME_ALIGNMENT) - 4;
	// else
		sp = round_down(sp, FRAME_ALIGNMENT) - 8;

	/*
	 * If we are on the alternate signal stack and would overflow it, don't.
	 * Return an always-bogus address instead so we will die with SIGSEGV.
	 */
	if (unlikely((nested_altstack || entering_altstack) &&
		     !__on_sig_stack(sp))) {

		// if (show_unhandled_signals && printk_ratelimit())
		// 	pr_info("%s[%d] overflowed sigaltstack\n",
		// 		current->comm, task_pid_nr(current));

		return (void __user *)-1L;
	}

	// /* save i387 and extended state */
	// if (!copy_fpstate_to_sigframe(*fpstate, (void __user *)buf_fx, math_size))
	// 	return (void __user *)-1L;

	return (void __user *)sp;
}


/*
 * The frame data is composed of the following areas and laid out as:
 *
 * -------------------------
 * | alignment padding     |
 * -------------------------
 * | (f)xsave frame        |
 * -------------------------
 * | fsave header          |
 * -------------------------
 * | alignment padding     |
 * -------------------------
 * | siginfo + ucontext    |
 * -------------------------
 */

/* max_frame_size tells userspace the worst case signal stack size. */
static unsigned long __ro_after_init max_frame_size;
static unsigned int __ro_after_init fpu_default_state_size;

int __init init_sigframe_size(void)
{
	// fpu_default_state_size = fpu__get_fpstate_size();
	fpu_default_state_size = 0;

	max_frame_size = MAX_FRAME_SIGINFO_UCTXT_SIZE + MAX_FRAME_PADDING;

	max_frame_size += fpu_default_state_size + MAX_XSAVE_PADDING;

	/* Userspace expects an aligned size. */
	max_frame_size = round_up(max_frame_size, FRAME_ALIGNMENT);

	pr_info("max sigframe size: %lu\n", max_frame_size);
	return 0;
}
early_initcall(init_sigframe_size);

static int
setup_rt_frame(ksignal_s *ksig, pt_regs_s *regs)
{
	// /* Perform fixup for the pre-signal frame. */
	// rseq_signal_deliver(ksig, regs);

	// /* Set up the stack frame */
	// if (is_ia32_frame(ksig)) {
	// 	if (ksig->ka.sa.sa_flags & SA_SIGINFO)
	// 		return ia32_setup_rt_frame(ksig, regs);
	// 	else
	// 		return ia32_setup_frame(ksig, regs);
	// } else if (is_x32_frame(ksig)) {
	// 	return x32_setup_rt_frame(ksig, regs);
	// } else {
		return x64_setup_rt_frame(ksig, regs);
	// }
}

static void
handle_signal(ksignal_s *ksig, pt_regs_s *regs) {
	bool stepping, failed;
	// struct fpu *fpu = &current->thread.fpu;

	// if (v8086_mode(regs))
	// 	save_v86_state((struct kernel_vm86_regs *) regs, VM86_SIGNAL);

	/* Are we from a system call? */
	if (syscall_get_nr(current, regs) != -1) {
		/* If so, check system call restarting.. */
		switch (syscall_get_error(current, regs)) {
		case -ERESTART_RESTARTBLOCK:
		case -ERESTARTNOHAND:
			regs->ax = -EINTR;
			break;

		case -ERESTARTSYS:
			if (!(ksig->ka.sa.sa_flags & SA_RESTART)) {
				regs->ax = -EINTR;
				break;
			}
			fallthrough;
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;
		}
	}

	// /*
	//  * If TF is set due to a debugger (TIF_FORCED_TF), clear TF now
	//  * so that register information in the sigcontext is correct and
	//  * then notify the tracer before entering the signal handler.
	//  */
	// stepping = test_thread_flag(TIF_SINGLESTEP);
	// if (stepping)
	// 	user_disable_single_step(current);

	failed = (setup_rt_frame(ksig, regs) < 0);
	if (!failed) {
		/*
		 * Clear the direction flag as per the ABI for function entry.
		 *
		 * Clear RF when entering the signal handler, because
		 * it might disable possible debug exception from the
		 * signal handler.
		 *
		 * Clear TF for the case when it wasn't set by debugger to
		 * avoid the recursive send_sigtrap() in SIGTRAP handler.
		 */
		regs->flags &= ~(X86_EFLAGS_DF|X86_EFLAGS_RF|X86_EFLAGS_TF);
		/*
		 * Ensure the signal handler starts with the new fpu state.
		 */
		// fpu__clear_user_states(fpu);
	}
	// signal_setup_done(failed, ksig, stepping);
}

static inline ulong
get_nr_restart_syscall(const pt_regs_s *regs) {
#ifdef CONFIG_IA32_EMULATION
	if (current->restart_block.arch_data & TS_COMPAT)
		return __NR_ia32_restart_syscall;
#endif
#ifdef CONFIG_X86_X32_ABI
	return __NR_restart_syscall | (regs->orig_ax & __X32_SYSCALL_BIT);
#else
	return __NR_restart_syscall;
#endif
}

/*
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 */
void arch_do_signal_or_restart(pt_regs_s *regs)
{
	task_s *curr = current;
	ksignal_s ksig;

	if (get_signal(&ksig)) {
		/* Whee! Actually deliver the signal.  */
		handle_signal(&ksig, regs);
		return;
	}

	/* Did we come from a system call? */
	if (syscall_get_nr(current, regs) != -1) {
		/* Restart the system call - no handlers present */
		switch (syscall_get_error(current, regs)) {
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;

		case -ERESTART_RESTARTBLOCK:
			regs->ax = get_nr_restart_syscall(regs);
			regs->ip -= 2;
			break;
		}
	}

	// /*
	//  * If there's no signal to deliver, we just put the saved sigmask
	//  * back.
	//  */
	// restore_saved_sigmask();
}