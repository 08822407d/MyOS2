// SPDX-License-Identifier: GPL-2.0-only
/*
 * common.c - C code for kernel entry and exit
 * Copyright (c) 2015 Andrew Lutomirski
 *
 * Based on asm and ptrace code by many authors.  The code here originated
 * in ptrace.c and signal.c.
 */

#include <linux/compiler/compiler.h>
#include <linux/kernel/entry-common.h>
#include <asm/syscall.h>
#include <asm/unistd.h>
#include <asm/processor.h>

#include <obsolete/printk.h>


long __x64_sys_ni_syscall(const pt_regs_s *regs) {
	color_printk(RED, BLACK, "Syscall not implemented: %d\n", regs->orig_ax);
	while (1);
}

__visible noinstr bool do_syscall_64(pt_regs_s *regs, int nr)
{
	// task_s *curr = current;
	// pid_s *pid_struct = get_task_pid(curr, PIDTYPE_PID);
	// pid_t pid = pid_vnr(pid_struct);
	// color_printk(BLUE, BLACK, "Syscall: %d from pid-%d, %#018lx\n", nr, pid, regs->ip);

	// add_random_kstack_offset();
	// nr = syscall_enter_from_user_mode(regs, nr);

	// instrumentation_begin();

	// if (!do_syscall_x64(regs, nr) && !do_syscall_x32(regs, nr) && nr != -1) {
	// 	/* Invalid system call, but still a system call. */
	// 	regs->ax = __x64_sys_ni_syscall(regs);
	// }

	uint unr = nr;
	if (unr < __NR_syscalls) {
		// unr = array_index_nospec(unr, NR_syscalls);
		regs->ax = x64_sys_call(regs, unr);
		// return true;
	}

	// instrumentation_end();
	syscall_exit_to_user_mode(regs);

	/*
	 * Check that the register state is valid for using SYSRET to exit
	 * to userspace.  Otherwise use the slower but fully capable IRET
	 * exit path.
	 */

	/* SYSRET requires RCX == RIP and R11 == EFLAGS */
	if (unlikely(regs->cx != regs->ip || regs->r11 != regs->flags))
		return false;

	/* CS and SS must match the values set in MSR_STAR */
	if (unlikely(regs->cs != __USER_CS || regs->ss != __USER_DS))
		return false;

	/*
	 * On Intel CPUs, SYSRET with non-canonical RCX/RIP will #GP
	 * in kernel space.  This essentially lets the user take over
	 * the kernel, since userspace controls RSP.
	 *
	 * TASK_SIZE_MAX covers all user-accessible addresses other than
	 * the deprecated vsyscall page.
	 */
	if (unlikely(regs->ip >= TASK_SIZE_MAX))
		return false;

	/*
	 * SYSRET cannot restore RF.  It can restore TF, but unlike IRET,
	 * restoring TF results in a trap from userspace immediately after
	 * SYSRET.
	 */
	if (unlikely(regs->flags & (X86_EFLAGS_RF | X86_EFLAGS_TF)))
		return false;

	/* Use SYSRET to exit to userspace */
	return true;
}