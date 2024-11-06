/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HASH_LIST_TYPES_H_
#define _LINUX_HASH_LIST_TYPES_H_

	#include "../lib_type_declaration.h"


	struct hlist_head {
		HList_s		*first;
	};

	struct hlist_node {
		HList_s		*next;
		HList_s		**pprev;
	};

#endif /* _LINUX_HASH_LIST_TYPES_H_ */