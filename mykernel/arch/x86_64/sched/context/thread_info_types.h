/* SPDX-License-Identifier: GPL-2.0 */
/* thread_info.h: low-level thread information
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds and Dave Miller
 */

#ifndef _ASM_X86_THREAD_INFO_TYPES_H_
#define _ASM_X86_THREAD_INFO_TYPES_H_

	#include "thread_info_const.h"

	/*
	 * low level task data that entry.S needs immediate access to
	 * - this struct should fit entirely inside of one cache line
	 * - this struct shares the supervisor stack pages
	 */
	struct task_struct;

	typedef struct thread_info {
		unsigned long	flags;		/* low level flags */
		unsigned long	syscall_work;	/* SYSCALL_WORK_ flags */
		u32				status;		/* thread synchronous flags */
		u32				cpu;		/* current CPU */
	} thread_info_s;

	#define INIT_THREAD_INFO(tsk)	{	\
				.flags		= 0,		\
			}

#endif /* _ASM_X86_THREAD_INFO_TYPES_H_ */
