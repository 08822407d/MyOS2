// SPDX-License-Identifier: GPL-2.0-only
/*
 * common.c - C code for kernel entry and exit
 * Copyright (c) 2015 Andrew Lutomirski
 *
 * Based on asm and ptrace code by many authors.  The code here originated
 * in ptrace.c and signal.c.
 */

#include <linux/compiler/compiler.h>
#include <asm/syscall.h>
#include <asm/unistd.h>
#include <asm/processor.h>

#include <obsolete/printk.h>


long __x64_sys_ni_syscall(const pt_regs_s *regs) {
	color_printk(RED, BLACK, "Syscall not implemented: %d", regs->orig_ax);
	while (1);
}

__visible noinstr void do_syscall_64(pt_regs_s *regs, int nr)
{
	// color_printk(BLUE, BLACK, "Syscall: %d, %#018lx\n", nr, regs->ip);

	// add_random_kstack_offset();
	// nr = syscall_enter_from_user_mode(regs, nr);

	// instrumentation_begin();

	// if (!do_syscall_x64(regs, nr) && !do_syscall_x32(regs, nr) && nr != -1) {
	// 	/* Invalid system call, but still a system call. */
	// 	regs->ax = __x64_sys_ni_syscall(regs);
	// }

	unsigned int unr = nr;
	if (unr < __NR_syscalls) {
		// unr = array_index_nospec(unr, NR_syscalls);
		regs->ax = x64_sys_call(regs, unr);
		// return true;
	}

	// instrumentation_end();
	// syscall_exit_to_user_mode(regs);
}