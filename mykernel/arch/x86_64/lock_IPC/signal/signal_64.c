#include <linux/kernel/sched.h>
#include <linux/kernel/lock_ipc.h>
#include <linux/kernel/syscalls.h>
#include <asm/syscall.h>



int x64_setup_rt_frame(ksignal_s *ksig, pt_regs_s *regs)
{
	// sigset_t *set = sigmask_to_save();
	rt_sigframe_s __user *frame;
	void __user *fp = NULL;
	ulong uc_flags;

	/* x86-64 should always use SA_RESTORER. */
	if (!(ksig->ka.sa.sa_flags & SA_RESTORER))
		return -EFAULT;

	frame = get_sigframe(ksig, regs, sizeof(rt_sigframe_s), &fp);
	// uc_flags = frame_uc_flags(regs);

	// if (!user_access_begin(frame, sizeof(*frame)))
	// 	return -EFAULT;

	// /* Create the ucontext.  */
	// unsafe_put_user(uc_flags, &frame->uc.uc_flags, Efault);
	// unsafe_put_user(0, &frame->uc.uc_link, Efault);
	// unsafe_save_altstack(&frame->uc.uc_stack, regs->sp, Efault);

	// /* Set up to return from userspace.  If provided, use a stub
	//    already in userspace.  */
	// unsafe_put_user(ksig->ka.sa.sa_restorer, &frame->pretcode, Efault);
	// unsafe_put_sigcontext(&frame->uc.uc_mcontext, fp, regs, set, Efault);
	// unsafe_put_sigmask(set, frame, Efault);
	// user_access_end();

	// if (ksig->ka.sa.sa_flags & SA_SIGINFO) {
	// 	if (copy_siginfo_to_user(&frame->info, &ksig->info))
	// 		return -EFAULT;
	// }

	// if (setup_signal_shadow_stack(ksig))
	// 	return -EFAULT;

	// /* Set up registers for signal handler */
	// regs->di = ksig->sig;
	// /* In case the signal handler was declared without prototypes */
	// regs->ax = 0;

	// /* This also works for non SA_SIGINFO handlers because they expect the
	//    next argument after the signal number on the stack. */
	// regs->si = (unsigned long)&frame->info;
	// regs->dx = (unsigned long)&frame->uc;
	// regs->ip = (unsigned long) ksig->ka.sa.sa_handler;

	// regs->sp = (unsigned long)frame;

	// /*
	//  * Set up the CS and SS registers to run signal handlers in
	//  * 64-bit mode, even if the handler happens to be interrupting
	//  * 32-bit or 16-bit code.
	//  *
	//  * SS is subtle.  In 64-bit mode, we don't need any particular
	//  * SS descriptor, but we do need SS to be valid.  It's possible
	//  * that the old SS is entirely bogus -- this can happen if the
	//  * signal we're trying to deliver is #GP or #SS caused by a bad
	//  * SS value.  We also have a compatibility issue here: DOSEMU
	//  * relies on the contents of the SS register indicating the
	//  * SS value at the time of the signal, even though that code in
	//  * DOSEMU predates sigreturn's ability to restore SS.  (DOSEMU
	//  * avoids relying on sigreturn to restore SS; instead it uses
	//  * a trampoline.)  So we do our best: if the old SS was valid,
	//  * we keep it.  Otherwise we replace it.
	//  */
	// regs->cs = __USER_CS;

	// if (unlikely(regs->ss != __USER_DS))
	// 	force_valid_ss(regs);

	// return 0;

Efault:
	// user_access_end();
	return -EFAULT;
}


// #define __NR_rt_sigreturn	15
/*
 * Do a signal return; undo the signal stack.
 */
MYOS_SYSCALL_DEFINE0(rt_sigreturn)
{
// 	struct pt_regs *regs = current_pt_regs();
// 	struct rt_sigframe __user *frame;
// 	sigset_t set;
// 	unsigned long uc_flags;

// 	frame = (struct rt_sigframe __user *)(regs->sp - sizeof(long));
// 	if (!access_ok(frame, sizeof(*frame)))
// 		goto badframe;
// 	if (__get_user(*(__u64 *)&set, (__u64 __user *)&frame->uc.uc_sigmask))
// 		goto badframe;
// 	if (__get_user(uc_flags, &frame->uc.uc_flags))
// 		goto badframe;

// 	set_current_blocked(&set);

// 	if (!restore_sigcontext(regs, &frame->uc.uc_mcontext, uc_flags))
// 		goto badframe;

// 	if (restore_signal_shadow_stack())
// 		goto badframe;

// 	if (restore_altstack(&frame->uc.uc_stack))
// 		goto badframe;

// 	return regs->ax;

// badframe:
// 	signal_fault(regs, frame, "rt_sigreturn");
// 	return 0;
}