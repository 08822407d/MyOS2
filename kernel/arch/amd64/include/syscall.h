/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __SYS_H__
#define __SYS_H__

	#define MAX_SYSTEM_CALL_NR 512
	typedef unsigned long (* system_call_t)(void);
	extern	system_call_t syscall_table[MAX_SYSTEM_CALL_NR ];

#endif /* __SYS_H__ */