#include <linux/kernel/sched.h>
#include <linux/kernel/lock_ipc.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/ptrace.h>
#include <linux/kernel/uaccess.h>
#include <asm/syscall.h>




#define FIX_EFLAGS	(						\
			X86_EFLAGS_AC | X86_EFLAGS_OF |	\
			X86_EFLAGS_DF | X86_EFLAGS_TF |	\
			X86_EFLAGS_SF |	X86_EFLAGS_ZF |	\
			X86_EFLAGS_AF | X86_EFLAGS_PF |	\
			X86_EFLAGS_CF | X86_EFLAGS_RF)

/*
 * If regs->ss will cause an IRET fault, change it.  Otherwise leave it
 * alone.  Using this generally makes no sense unless
 * user_64bit_mode(regs) would return true.
 */
static void
force_valid_ss(pt_regs_s *regs) {
	u32 ar;
	asm volatile (	"lar	%[old_ss],	%[ar]		\n\t"
					"jz		1f						\n\t"	/* If invalid: */
					"xorl	%[ar],		%[ar]		\n\t"	/* set ar = 0 */
					"1:"
				:	[ar] "=r" (ar)
				:	[old_ss] "rm" ((u16)regs->ss)
				:
				);

	/*
	 * For a valid 64-bit user context, we need DPL 3, type
	 * read-write data or read-write exp-down data, and S and P
	 * set.  We can't use VERW because VERW doesn't check the
	 * P bit.
	 */
	ar &= AR_DPL_MASK | AR_S | AR_P | AR_TYPE_MASK;
	if (ar != (AR_DPL3 | AR_S | AR_P | AR_TYPE_RWDATA) &&
			ar != (AR_DPL3 | AR_S | AR_P | AR_TYPE_RWDATA_EXPDOWN))
		regs->ss = __USER_DS;
}

static bool
restore_sigcontext(pt_regs_s *regs,
		sigctx_s __user *usc, ulong uc_flags) {

	sigctx_s sc;

	// /* Always make any pending restarted system calls return -EINTR */
	// current->restart_block.fn = do_no_restart_syscall;

	if (copy_from_user(&sc, usc, offsetof(sigctx_s, reserved1)))
		return false;

	regs->bx = sc.bx;
	regs->cx = sc.cx;
	regs->dx = sc.dx;
	regs->si = sc.si;
	regs->di = sc.di;
	regs->bp = sc.bp;
	regs->ax = sc.ax;
	regs->sp = sc.sp;
	regs->ip = sc.ip;
	regs->r8 = sc.r8;
	regs->r9 = sc.r9;
	regs->r10 = sc.r10;
	regs->r11 = sc.r11;
	regs->r12 = sc.r12;
	regs->r13 = sc.r13;
	regs->r14 = sc.r14;
	regs->r15 = sc.r15;

	/* Get CS/SS and force CPL3 */
	regs->cs = sc.cs | 0x03;
	regs->ss = sc.ss | 0x03;

	regs->flags = (regs->flags & ~FIX_EFLAGS) | (sc.flags & FIX_EFLAGS);
	/* disable syscall checks */
	regs->orig_ax = -1;

	// /*
	//  * Fix up SS if needed for the benefit of old DOSEMU and
	//  * CRIU.
	//  */
	// if (unlikely(!(uc_flags & UC_STRICT_RESTORE_SS) && user_64bit_mode(regs)))
	// 	force_valid_ss(regs);

	// return fpu__restore_sig((void __user *)sc.fpstate, 0);
	return true;
}


static __always_inline int
__unsafe_setup_sigcontext(struct sigcontext __user *sc,
		void __user *fpstate, pt_regs_s *regs, ulong mask) {

	unsafe_put_user(regs->di, &sc->di, Efault);
	unsafe_put_user(regs->si, &sc->si, Efault);
	unsafe_put_user(regs->bp, &sc->bp, Efault);
	unsafe_put_user(regs->sp, &sc->sp, Efault);
	unsafe_put_user(regs->bx, &sc->bx, Efault);
	unsafe_put_user(regs->dx, &sc->dx, Efault);
	unsafe_put_user(regs->cx, &sc->cx, Efault);
	unsafe_put_user(regs->ax, &sc->ax, Efault);
	unsafe_put_user(regs->r8, &sc->r8, Efault);
	unsafe_put_user(regs->r9, &sc->r9, Efault);
	unsafe_put_user(regs->r10, &sc->r10, Efault);
	unsafe_put_user(regs->r11, &sc->r11, Efault);
	unsafe_put_user(regs->r12, &sc->r12, Efault);
	unsafe_put_user(regs->r13, &sc->r13, Efault);
	unsafe_put_user(regs->r14, &sc->r14, Efault);
	unsafe_put_user(regs->r15, &sc->r15, Efault);

	unsafe_put_user(current->thread.trap_nr, &sc->trapno, Efault);
	unsafe_put_user(current->thread.error_code, &sc->err, Efault);
	unsafe_put_user(regs->ip, &sc->ip, Efault);
	unsafe_put_user(regs->flags, &sc->flags, Efault);
	unsafe_put_user(regs->cs, &sc->cs, Efault);
	unsafe_put_user(0, &sc->gs, Efault);
	unsafe_put_user(0, &sc->fs, Efault);
	unsafe_put_user(regs->ss, &sc->ss, Efault);

	unsafe_put_user(fpstate, (unsigned long __user *)&sc->fpstate, Efault);

	/* non-iBCS2 extensions.. */
	unsafe_put_user(mask, &sc->oldmask, Efault);
	unsafe_put_user(current->thread.cr2, &sc->cr2, Efault);
	return 0;
Efault:
	return -EFAULT;
}

#define unsafe_put_sigcontext(sc, fp, regs, set, label)	\
		do {											\
			if (__unsafe_setup_sigcontext(sc,			\
					fp, regs, set->sig[0]))				\
				goto label;								\
		} while(0);

#define unsafe_put_sigmask(set, frame, label)			\
		unsafe_put_user(*(__u64 *)(set),				\
			(__u64 __user *)&(frame)->uc.uc_sigmask,	\
			label)

#define unsafe_save_altstack(uss, sp, label) do {			\
			stack_t __user *__uss = uss;					\
			task_s *t = current;							\
			unsafe_put_user((void __user *)t->sas_ss_sp,	\
				&__uss->ss_sp, label);						\
			unsafe_put_user(t->sas_ss_flags,				\
				&__uss->ss_flags, label);					\
			unsafe_put_user(t->sas_ss_size,					\
				&__uss->ss_size, label);					\
		} while (0);

int x64_setup_rt_frame(ksignal_s *ksig, pt_regs_s *regs)
{
	sigset_t *set = sigmask_to_save();
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

	/* Create the ucontext.  */
	unsafe_put_user(uc_flags, &frame->uc.uc_flags, Efault);
	unsafe_put_user(0, &frame->uc.uc_link, Efault);
	unsafe_save_altstack(&frame->uc.uc_stack, regs->sp, Efault);

	/* Set up to return from userspace.  If provided, use a stub
	   already in userspace.  */
	unsafe_put_user(ksig->ka.sa.sa_restorer, &frame->pretcode, Efault);
	unsafe_put_sigcontext(&frame->uc.uc_mcontext, fp, regs, set, Efault);
	unsafe_put_sigmask(set, frame, Efault);
	// user_access_end();

	if (ksig->ka.sa.sa_flags & SA_SIGINFO) {
		if (copy_siginfo_to_user(&frame->info, &ksig->info))
			return -EFAULT;
	}

	// if (setup_signal_shadow_stack(ksig))
	// 	return -EFAULT;

	/* Set up registers for signal handler */
	regs->di = ksig->sig;
	/* In case the signal handler was declared without prototypes */
	regs->ax = 0;

	/* This also works for non SA_SIGINFO handlers because they expect the
	   next argument after the signal number on the stack. */
	regs->si = (unsigned long)&frame->info;
	regs->dx = (unsigned long)&frame->uc;
	regs->ip = (unsigned long) ksig->ka.sa.sa_handler;

	regs->sp = (unsigned long) frame;

	/*
	 * Set up the CS and SS registers to run signal handlers in
	 * 64-bit mode, even if the handler happens to be interrupting
	 * 32-bit or 16-bit code.
	 *
	 * SS is subtle.  In 64-bit mode, we don't need any particular
	 * SS descriptor, but we do need SS to be valid.  It's possible
	 * that the old SS is entirely bogus -- this can happen if the
	 * signal we're trying to deliver is #GP or #SS caused by a bad
	 * SS value.  We also have a compatibility issue here: DOSEMU
	 * relies on the contents of the SS register indicating the
	 * SS value at the time of the signal, even though that code in
	 * DOSEMU predates sigreturn's ability to restore SS.  (DOSEMU
	 * avoids relying on sigreturn to restore SS; instead it uses
	 * a trampoline.)  So we do our best: if the old SS was valid,
	 * we keep it.  Otherwise we replace it.
	 */
	regs->cs = __USER_CS;

	if (unlikely(regs->ss != __USER_DS))
		force_valid_ss(regs);

	return 0;

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
	pt_regs_s *regs = current_pt_regs();
	rt_sigframe_s __user *frame;
	sigset_t set;
	ulong uc_flags;

	// while (1);

	frame = (rt_sigframe_s __user *)(regs->sp - sizeof(long));
	if (!access_ok(frame, sizeof(*frame)))
		goto badframe;
	if (get_user(*(__u64 *)&set, (__u64 __user *)&frame->uc.uc_sigmask))
		goto badframe;
	if (get_user(uc_flags, &frame->uc.uc_flags))
		goto badframe;

	set_current_blocked(&set);

	if (!restore_sigcontext(regs, &frame->uc.uc_mcontext, uc_flags))
		goto badframe;

	// if (restore_signal_shadow_stack())
	// 	goto badframe;

	// if (restore_altstack(&frame->uc.uc_stack))
	// 	goto badframe;

	return regs->ax;

badframe:
	// signal_fault(regs, frame, "rt_sigreturn");
	return 0;
}