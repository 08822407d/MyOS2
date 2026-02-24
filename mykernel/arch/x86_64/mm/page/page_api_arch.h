/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PAGE_API_H_
#define _ASM_X86_PAGE_API_H_

	#include "page_arch.h"


	/* duplicated to the one in bootmem.h */
	extern ulong max_pfn;
	extern ulong phys_base;


	extern struct pglist_data	*node_data[];
	#define NODE_DATA(nid)		(node_data[nid])

#endif /* _ASM_X86_PAGE_API_H_ */