/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPARSEMEM_H
#define _ASM_X86_SPARSEMEM_H

	/*
	* generic non-linear memory support:
	*
	* 1) we will not split memory into more chunks than will fit into the flags
	*    field of the struct page
	*
	* SECTION_SIZE_BITS		2^n: size of each section
	* MAX_PHYSMEM_BITS		2^n: max size of physical address space
	*
	*/
	#define SECTION_SIZE_BITS 27 /* matt - 128 is convenient right now */
	// #define MAX_PHYSMEM_BITS (pgtable_l5_enabled() ? 52 : 46)
	#define MAX_PHYSMEM_BITS 46

	#ifndef __ASSEMBLY__
	#	ifdef CONFIG_NUMA_KEEP_MEMINFO
			extern int phys_to_target_node(phys_addr_t start);
	#		define phys_to_target_node phys_to_target_node
			extern int memory_add_physaddr_to_nid(u64 start);
	#		define memory_add_physaddr_to_nid memory_add_physaddr_to_nid
	#	endif
	#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_SPARSEMEM_H */
