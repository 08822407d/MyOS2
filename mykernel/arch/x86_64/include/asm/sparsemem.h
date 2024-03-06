/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPARSEMEM_H
#define _ASM_X86_SPARSEMEM_H

	/*
	 * generic non-linear memory support:
	 *
	 * 1) we will not split memory into more chunks than will fit into the flags
	 *    field of the page_s
	 *
	 * SECTION_SIZE_BITS		2^n: size of each section
	 * MAX_PHYSMEM_BITS		2^n: max size of physical address space
	 *
	 */
	// #define MAX_PHYSMEM_BITS (pgtable_l5_enabled() ? 52 : 46)
	#define MAX_PHYSMEM_BITS 46

#endif /* _ASM_X86_SPARSEMEM_H */
