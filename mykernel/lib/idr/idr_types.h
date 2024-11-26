/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_TYPES_H_
#define _LINUX_IDR_TYPES_H_

	#include "../lib_type_declaration.h"


	struct idr {
		// struct radix_tree_root	idr_rt;
		void		**idr_rt;
		uint		idr_base;
		uint		idr_next;

		bool		initiated;
	};

	struct ida {
		ulong		*bitmap;

		bool		initiated;
	};

#endif /* _LINUX_IDR_TYPES_H_ */