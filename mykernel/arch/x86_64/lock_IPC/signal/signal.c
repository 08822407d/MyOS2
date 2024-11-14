#include <linux/kernel/sched.h>
#include <linux/kernel/lock_ipc.h>
#include <asm/syscall.h>



static void
handle_signal(ksignal_s *ksig, pt_regs_s *regs) {
	// bool stepping, failed;
	// struct fpu *fpu = &current->thread.fpu;

	// if (v8086_mode(regs))
	// 	save_v86_state((struct kernel_vm86_regs *) regs, VM86_SIGNAL);

	// /* Are we from a system call? */
	// if (syscall_get_nr(current, regs) != -1) {
	// 	/* If so, check system call restarting.. */
	// 	switch (syscall_get_error(current, regs)) {
	// 	case -ERESTART_RESTARTBLOCK:
	// 	case -ERESTARTNOHAND:
	// 		regs->ax = -EINTR;
	// 		break;

	// 	case -ERESTARTSYS:
	// 		if (!(ksig->ka.sa.sa_flags & SA_RESTART)) {
	// 			regs->ax = -EINTR;
	// 			break;
	// 		}
	// 		fallthrough;
	// 	case -ERESTARTNOINTR:
	// 		regs->ax = regs->orig_ax;
	// 		regs->ip -= 2;
	// 		break;
	// 	}
	// }

	// /*
	//  * If TF is set due to a debugger (TIF_FORCED_TF), clear TF now
	//  * so that register information in the sigcontext is correct and
	//  * then notify the tracer before entering the signal handler.
	//  */
	// stepping = test_thread_flag(TIF_SINGLESTEP);
	// if (stepping)
	// 	user_disable_single_step(current);

	// failed = (setup_rt_frame(ksig, regs) < 0);
	// if (!failed) {
	// 	/*
	// 	 * Clear the direction flag as per the ABI for function entry.
	// 	 *
	// 	 * Clear RF when entering the signal handler, because
	// 	 * it might disable possible debug exception from the
	// 	 * signal handler.
	// 	 *
	// 	 * Clear TF for the case when it wasn't set by debugger to
	// 	 * avoid the recursive send_sigtrap() in SIGTRAP handler.
	// 	 */
	// 	regs->flags &= ~(X86_EFLAGS_DF|X86_EFLAGS_RF|X86_EFLAGS_TF);
	// 	/*
	// 	 * Ensure the signal handler starts with the new fpu state.
	// 	 */
	// 	fpu__clear_user_states(fpu);
	// }
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