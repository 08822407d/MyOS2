// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CACHE_H
#define _ASM_X86_CACHE_H

	#include <linux/kernel/linkage.h>


	#include <generated/bounds.h>

	/* L1 cache line size */
	#define L1_CACHE_SHIFT	(CONFIG_X86_L1_CACHE_SHIFT)
	#define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)

	#define __read_mostly	__section(".data..read_mostly")

#endif /* _ASM_X86_CACHE_H */
