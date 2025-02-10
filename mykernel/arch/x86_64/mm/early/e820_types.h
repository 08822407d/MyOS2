// source: linux-6.4.9

// arch/x86/include/asm/e820/types.h

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_E820_TYPES_H_
#define _ASM_E820_TYPES_H_

	#include "../mm_type_declaration_arch.h"

	#include "../mm_const_arch.h"


	/*
	 * A single E820 map entry, describing a memory range of [addr...addr+size-1],
	 * of 'type' memory type:
	 *
	 * (We pack it because there can be thousands of them on large systems.)
	 */
	struct e820_entry {
		u64			addr;
		u64			size;
		enum e820_type	type;
	};

	/*
	 * The whole array of E820 entries:
	 */
	struct e820_table {
		__u32			nr_entries;
		e820_entry_s	entries[E820_MAX_ENTRIES];
	};

#endif /* _ASM_E820_TYPES_H */
