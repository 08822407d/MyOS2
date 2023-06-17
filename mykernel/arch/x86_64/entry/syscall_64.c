// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */

#include <linux/kernel/linkage.h>
// #include <linux/sys.h>
#include <linux/kernel/cache.h>
#include <linux/kernel/syscalls.h>
#include <asm/syscall.h>

// #define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
#define __SYSCALL(nr, sym) extern long sym(const pt_regs_s *);
#	include <asm/syscalls_64.h>
#undef __SYSCALL

// #define __SYSCALL(nr, sym)	__x64_##sym,
#define __SYSCALL(nr, sym)	[nr] = sym,

const sys_call_ptr_t sys_call_table[] = {
	[0 ... __NR_syscalls - 1] = sys_no_syscall,
#include <asm/syscalls_64.h>
};