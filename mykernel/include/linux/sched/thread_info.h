/* SPDX-License-Identifier: GPL-2.0 */
/* thread_info.h: common low-level thread information accessors
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds
 */

#ifndef _LINUX_THREAD_INFO_H
#define _LINUX_THREAD_INFO_H

	#include <linux/kernel/types.h>
	#include <linux/kernel/limits.h>
	#include <linux/debug/bug.h>
	// #include <linux/restart_block.h>
	// #include <linux/errno.h>

	// /*
	//  * For CONFIG_THREAD_INFO_IN_TASK kernels we need <asm/current.h> for the
	//  * definition of current, but for !CONFIG_THREAD_INFO_IN_TASK kernels,
	//  * including <asm/current.h> can cause a circular dependency on some platforms.
	//  */
	// #include <asm/current.h>
	#include <asm/thread_info.h>
	// #define current_thread_info() ((struct thread_info *)current)
	#define current_thread_info() (&(current->thread_info))

#endif /* _LINUX_THREAD_INFO_H */
