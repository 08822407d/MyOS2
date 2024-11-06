/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_TYPES_H_
#define _LINUX_IDR_TYPES_H_

	#include "../lib_type_declaration.h"


	struct idr {
		// struct radix_tree_root	idr_rt;
		void		*idr_rt[MYOS_IDR_BUF_SIZE];
		uint		idr_base;
		uint		idr_next;
	};

#endif /* _LINUX_IDR_TYPES_H_ */