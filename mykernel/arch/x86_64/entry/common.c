// SPDX-License-Identifier: GPL-2.0-only
/*
 * common.c - C code for kernel entry and exit
 * Copyright (c) 2015 Andrew Lutomirski
 *
 * Based on asm and ptrace code by many authors.  The code here originated
 * in ptrace.c and signal.c.
 */

#include <linux/kernel/kernel.h>
#include <linux/kernel/sched.h>
#include <linux/sched/task_stack.h>
#include <linux/smp/smp.h>
#include <uapi/linux/errno.h>
#include <linux/kernel/ptrace.h>
#include <linux/kernel/export.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/uaccess.h>

#include <asm/traps.h>
#include <asm/syscall.h>


__visible noinstr void do_syscall_64(pt_regs_s *regs, int nr)
{
	// add_random_kstack_offset();
	// nr = syscall_enter_from_user_mode(regs, nr);

	// instrumentation_begin();

	// if (!do_syscall_x64(regs, nr) && !do_syscall_x32(regs, nr) && nr != -1) {
	// 	/* Invalid system call, but still a system call. */
	// 	regs->ax = __x64_sys_ni_syscall(regs);
	// }

	// static __always_inline bool do_syscall_x64(pt_regs_s *regs, int nr)
	// {
	// 	/*
	// 	* Convert negative numbers to very high and thus out of range
	// 	* numbers for comparisons.
	// 	*/
		unsigned int unr = nr;
		if (unr < __NR_syscalls) {
			// unr = array_index_nospec(unr, NR_syscalls);
			regs->ax = sys_call_table[unr](regs);
			// return true;
		}
	// 	return false;
	// }

	// instrumentation_end();
	// syscall_exit_to_user_mode(regs);
}