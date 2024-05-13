/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DOUBLE_LIST_TYPES_H_
#define _LINUX_DOUBLE_LIST_TYPES_H_

	#include "../lib_type_declaration.h"


	typedef struct list_head {
		List_s *	next;
		List_s *	prev;
	} List_s;

	typedef struct List_hdr {
		List_s		anchor;
		ulong		count;
	} List_hdr_s;

#endif /* _LINUX_DOUBLE_LIST_TYPES_H_ */