/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
// Linux file name :
//./arch/x86/include/generated/asm/syscalls_64.h

SYSCALL_COMMON(__NR_open,sys_open)
SYSCALL_COMMON(__NR_close,sys_close)
SYSCALL_COMMON(__NR_read,sys_read)
SYSCALL_COMMON(__NR_write,sys_write)

// SYSCALL_COMMON(__NR_newfstat,sys_newfstat)

SYSCALL_COMMON(__NR_lseek,sys_lseek)

SYSCALL_COMMON(__NR_sbrk,sys_sbrk)

SYSCALL_COMMON(__NR_getpid,sys_getpid)

SYSCALL_COMMON(__NR_fork,sys_fork)
SYSCALL_COMMON(__NR_vfork,sys_vfork)
SYSCALL_COMMON(__NR_execve,sys_execve)
SYSCALL_COMMON(__NR_exit,sys_exit)
SYSCALL_COMMON(__NR_wait4,sys_wait4)

// SYSCALL_COMMON(__NR_getdents,sys_getdents)
SYSCALL_COMMON(__NR_getcwd,sys_getcwd)
SYSCALL_COMMON(__NR_chdir,sys_chdir)

SYSCALL_COMMON(__NR_mkdir,sys_mkdir)
SYSCALL_COMMON(__NR_rmdir,sys_rmdir)
SYSCALL_COMMON(__NR_creat,sys_creat)
// SYSCALL_COMMON(__NR_link,sys_link)
SYSCALL_COMMON(__NR_unlink,sys_unlink)

SYSCALL_COMMON(__NR_getppid,sys_getppid)

SYSCALL_COMMON(__NR_reboot,sys_reboot)

// SYSCALL_COMMON(__NR_init_module,sys_init_module)
// SYSCALL_COMMON(__NR_delete_module,sys_delete_module)

SYSCALL_COMMON(__NR_getdents64,sys_getdents64)

SYSCALL_COMMON(__NR_putstring,sys_putstring)