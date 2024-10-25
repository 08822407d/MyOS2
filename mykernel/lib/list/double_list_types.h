/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DOUBLE_LIST_TYPES_H_
#define _LINUX_DOUBLE_LIST_TYPES_H_

	#include "../lib_type_declaration.h"


	struct list_head {
		List_s *	next;
		List_s *	prev;
	};

	struct list_hdr {
		List_s		anchor;
		ulong		count;
	};

#endif /* _LINUX_DOUBLE_LIST_TYPES_H_ */