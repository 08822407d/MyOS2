/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
#include <errno.h>

#include <uapi/linux/unistd.h>

#define SYSFUNC_DEF(name)						\
			_SYSFUNC_DEF_(name, __NR_##name)

#define _SYSFUNC_DEF_(name, nr)					\
			__SYSFUNC_DEF__(name, nr)

#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
			);

SYSFUNC_DEF(open)
SYSFUNC_DEF(close)
SYSFUNC_DEF(read)
SYSFUNC_DEF(write)

SYSFUNC_DEF(fstat)

SYSFUNC_DEF(lseek)

SYSFUNC_DEF(brk)

SYSFUNC_DEF(getpid)

SYSFUNC_DEF(fork)
// SYSFUNC_DEF(vfork)
SYSFUNC_DEF(execve)
SYSFUNC_DEF(exit)
SYSFUNC_DEF(wait4)

SYSFUNC_DEF(getdents)
SYSFUNC_DEF(getcwd)
SYSFUNC_DEF(chdir)

SYSFUNC_DEF(mkdir)
SYSFUNC_DEF(rmdir)
SYSFUNC_DEF(creat)
SYSFUNC_DEF(link)
SYSFUNC_DEF(unlink)

SYSFUNC_DEF(getppid)

SYSFUNC_DEF(reboot)

// SYSFUNC_DEF(init_module)
// SYSFUNC_DEF(delete_module)

SYSFUNC_DEF(getdents64)

SYSFUNC_DEF(putstring)

__asm__	(
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
	"sysexit_ret_addr:						\n\t"
	"cmpq	$-0x1000,	%rax				\n\t"
	"jb	LABEL_SYSCALL_RET					\n\t"
	// "movq	%rax,	__errno(%rip)			\n\t"
	// "orq	$-1,	%rax					\n\t"
	"LABEL_SYSCALL_RET:						\n\t"
	"leaveq									\n\t"
	"retq									\n\t"
);







