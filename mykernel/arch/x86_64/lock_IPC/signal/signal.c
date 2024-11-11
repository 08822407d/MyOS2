#include "../lock_ipc_api_arch.h"


/*
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 */
void arch_do_signal_or_restart(pt_regs_s *regs)
{
	// struct ksignal ksig;

	// if (get_signal(&ksig)) {
	// 	/* Whee! Actually deliver the signal.  */
	// 	handle_signal(&ksig, regs);
	// 	return;
	// }

	// /* Did we come from a system call? */
	// if (syscall_get_nr(current, regs) != -1) {
	// 	/* Restart the system call - no handlers present */
	// 	switch (syscall_get_error(current, regs)) {
	// 	case -ERESTARTNOHAND:
	// 	case -ERESTARTSYS:
	// 	case -ERESTARTNOINTR:
	// 		regs->ax = regs->orig_ax;
	// 		regs->ip -= 2;
	// 		break;

	// 	case -ERESTART_RESTARTBLOCK:
	// 		regs->ax = get_nr_restart_syscall(regs);
	// 		regs->ip -= 2;
	// 		break;
	// 	}
	// }

	// /*
	//  * If there's no signal to deliver, we just put the saved sigmask
	//  * back.
	//  */
	// restore_saved_sigmask();
}