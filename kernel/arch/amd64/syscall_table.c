/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#include <uapi/syscall.h>

#include "include/syscall.h"

#define SYSCALL_COMMON(nr,sym)	extern unsigned long sym(void);
SYSCALL_COMMON(0,no_system_call)
#include "syscall_table.h"
#undef	SYSCALL_COMMON

#define SYSCALL_COMMON(nr,sym)	[nr] = sym,

system_call_t syscall_table[MAX_SYSTEM_CALL_NR] = 
{
	[0 ... MAX_SYSTEM_CALL_NR-1] = no_system_call,
#include "syscall_table.h"
};

