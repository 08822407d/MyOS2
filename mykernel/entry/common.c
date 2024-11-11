#include <linux/init/init.h>
#include <linux/compiler/compiler.h>
#include <linux/compiler/compiler_attributes.h>
#include <linux/kernel/ptrace.h>

#include <linux/kernel/lock_ipc_api.h>


/**
 * exit_to_user_mode_loop - do any pending work before leaving to user space
 * @regs:	Pointer to pt_regs on entry stack
 * @ti_work:	TIF work flags as read by the caller
 */
__always_inline ulong
exit_to_user_mode_loop(pt_regs_s *regs, ulong ti_work) {
	// /*
	//  * Before returning to user space ensure that all pending work
	//  * items have been completed.
	//  */
	// while (ti_work & EXIT_TO_USER_MODE_WORK) {

	// 	local_irq_enable_exit_to_user(ti_work);

	// 	if (ti_work & _TIF_NEED_RESCHED)
	// 		schedule();

	// 	if (ti_work & _TIF_UPROBE)
	// 		uprobe_notify_resume(regs);

	// 	if (ti_work & _TIF_PATCH_PENDING)
	// 		klp_update_patch_state(current);

	// 	if (ti_work & (_TIF_SIGPENDING | _TIF_NOTIFY_SIGNAL))
			arch_do_signal_or_restart(regs);

	// 	if (ti_work & _TIF_NOTIFY_RESUME)
	// 		resume_user_mode_work(regs);

	// 	/* Architecture specific TIF work */
	// 	arch_exit_to_user_mode_work(regs, ti_work);

	// 	/*
	// 	 * Disable interrupts and reevaluate the work flags as they
	// 	 * might have changed while interrupts and preemption was
	// 	 * enabled above.
	// 	 */
	// 	local_irq_disable_exit_to_user();

	// 	/* Check if any of the above work has queued a deferred wakeup */
	// 	tick_nohz_user_enter_prepare();

	// 	ti_work = read_thread_flags();
	// }

	/* Return the latest work state for arch_exit_to_user_mode() */
	return ti_work;
}



static __always_inline void
__syscall_exit_to_user_mode_work(pt_regs_s *regs) {
	// syscall_exit_to_user_mode_prepare(regs);
	// local_irq_disable_exit_to_user();
	// exit_to_user_mode_prepare(regs);
	exit_to_user_mode_loop(regs, 0);
}

__visible noinstr void
syscall_exit_to_user_mode(pt_regs_s *regs) {
	// instrumentation_begin();
	__syscall_exit_to_user_mode_work(regs);
	// instrumentation_end();
	// exit_to_user_mode();
}