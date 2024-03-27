/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DOUBLE_LIST_TYPES_H_
#define _LINUX_DOUBLE_LIST_TYPES_H_

	#include <linux/kernel/types.h>

	struct list_head;
	typedef struct list_head List_s;
	typedef struct list_head {
		List_s *	next;
		List_s *	prev;
	} List_s;

	typedef struct List_hdr {
		ulong		count;
		List_s		anchor;
	} List_hdr_s;

#endif /* _LINUX_DOUBLE_LIST_TYPES_H_ */