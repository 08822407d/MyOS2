// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */

#include <linux/kernel/linkage.h>
// #include <linux/sys.h>
#include <linux/kernel/cache.h>
#include <linux/kernel/syscalls.h>
#include <asm/syscall.h>

#define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
#	include <asm/syscalls_64.h>
#undef __SYSCALL

/*
 * The sys_call_table[] is no longer used for system calls, but
 * kernel/trace/trace_syscalls.c still wants to know the system
 * call address.
 */
#define __SYSCALL(nr, sym)	[nr] = __x64_##sym,
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
#undef __SYSCALL


#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
	}
};