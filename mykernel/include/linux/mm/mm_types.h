/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_TYPES_H
#define _LINUX_MM_TYPES_H

	#include <linux/lib/list.h>
	#include <linux/kernel/lock_ipc.h>
	#include <linux/kernel/completion.h>
	#include <linux/kernel/cpumask.h>
	#include <linux/kernel/workqueue.h>


	#include <mm/mm_types.h>

	/*
	 * page_private can be used on tail pages.  However, PagePrivate is only
	 * checked by the VM on the head page.  So page_private on the tail pages
	 * should be used for data that's ancillary to the head page (eg attaching
	 * buffer heads to tail pages after attaching buffer heads to the head page)
	 */
	#define page_private(page)	((page)->private)

#endif /* _LINUX_MM_TYPES_H */
