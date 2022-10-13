// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */
#define CONFIG_ARCH_HAS_SYSCALL_WRAPPER

#include <linux/kernel/linkage.h>
// #include <linux/sys.h>
// #include <linux/cache.h>
#include <linux/kernel/syscalls.h>
#include <asm/syscall.h>

// #define __SYSCALL(nr, sym) extern long __x64_##sym(const struct pt_regs *);
#define __SYSCALL(nr, sym)	extern long sym(void);
	__SYSCALL(0, myos_no_system_call)
#	include <asm/syscalls_64.h>
#undef __SYSCALL

// #define __SYSCALL(nr, sym)	__x64_##sym,
#define __SYSCALL(nr, sym)	[nr] = sym,

const sys_call_ptr_t sys_call_table[] = {
	[0 ... __NR_syscalls - 1] = myos_no_system_call,
#include <asm/syscalls_64.h>
};

#undef CONFIG_ARCH_HAS_SYSCALL_WRAPPER