__SYSCALL(__NR_syscalls - 1, sys_no_syscall)


__SYSCALL(__NR_read, sys_read)
__SYSCALL(__NR_write, sys_write)
__SYSCALL(__NR_open, sys_open)
__SYSCALL(__NR_close, sys_close)

// __SYSCALL(__NR_newfstat, sys_newfstat)

__SYSCALL(__NR_lseek, sys_lseek)

__SYSCALL(__NR_brk, sys_brk)

__SYSCALL(__NR_getpid, sys_getpid)

__SYSCALL(__NR_fork, sys_fork)
// __SYSCALL(__NR_vfork, sys_vfork)
__SYSCALL(__NR_execve, sys_execve)
__SYSCALL(__NR_exit, sys_exit)
__SYSCALL(__NR_wait4, sys_wait4)

// __SYSCALL(__NR_getdents, sys_getdents)
__SYSCALL(__NR_getcwd, sys_getcwd)
__SYSCALL(__NR_chdir, sys_chdir)

__SYSCALL(__NR_mkdir, sys_mkdir)
__SYSCALL(__NR_rmdir, sys_rmdir)
__SYSCALL(__NR_creat, sys_creat)
// __SYSCALL(__NR_link, sys_link)
__SYSCALL(__NR_unlink, sys_unlink)

__SYSCALL(__NR_getppid, sys_getppid)

__SYSCALL(__NR_reboot, sys_reboot)

// __SYSCALL(__NR_init_module, sys_init_module)
// __SYSCALL(__NR_delete_module, sys_delete_module)

__SYSCALL(__NR_getdents64, sys_getdents64)

__SYSCALL(__NR_putstring, sys_myos_putstring)