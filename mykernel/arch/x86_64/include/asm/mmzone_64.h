// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/* K8 NUMA support */
/* Copyright 2002,2003 by Andi Kleen, SuSE Labs */
/* 2.5 Version loosely based on the NUMAQ Code by Pat Gaughen. */
#ifndef _ASM_X86_MMZONE_64_H
#define _ASM_X86_MMZONE_64_H

	#include <asm/smp.h>

	extern struct pglist_data	*node_data[];
	#define NODE_DATA(nid)		(node_data[nid])

#endif /* _ASM_X86_MMZONE_64_H */
