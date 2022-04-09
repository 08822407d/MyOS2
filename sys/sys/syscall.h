/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__


	#define	__NR_read			0
	#define	__NR_write			1
	#define	__NR_open			2
	#define	__NR_close			3

	#define	__NR_newfstat		5

	#define	__NR_lseek			8

	#define	__NR_brk			12

	#define __NR_getpid			19

	#define	__NR_fork			57
	#define	__NR_vfork			58
	#define	__NR_execve			59
	#define	__NR_exit			60
	#define	__NR_wait4			61

	#define	__NR_getdents		78
	#define	__NR_chdir			80

	#define __NR_getppid		110

	#define	__NR_reboot			169

	#define	__NR_init_module	175
	#define	__NR_delete_module	176

	#define	__NR_putstring		511

#endif /* __SYSCALL_H__ */
