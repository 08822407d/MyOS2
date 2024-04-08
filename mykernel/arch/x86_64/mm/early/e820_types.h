// source: linux-6.4.9

// arch/x86/include/asm/e820/types.h

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_E820_TYPES_H_
#define _ASM_E820_TYPES_H_

	#include "../mm_type_declaration_arch.h"

	#include "../mm_const_arch.h"

	
	/*
	 * These are the E820 types known to the kernel:
	 */
	enum e820_type {
		E820_TYPE_RAM		= 1,
		E820_TYPE_RESERVED	= 2,
		E820_TYPE_ACPI		= 3,
		E820_TYPE_NVS		= 4,
		E820_TYPE_UNUSABLE	= 5,
		E820_TYPE_PMEM		= 7,

		/*
		 * This is a non-standardized way to represent ADR or
		 * NVDIMM regions that persist over a reboot.
		 *
		 * The kernel will ignore their special capabilities
		 * unless the CONFIG_X86_PMEM_LEGACY=y option is set.
		 *
		 * ( Note that older platforms also used 6 for the same
		 *   type of memory, but newer versions switched to 12 as
		 *   6 was assigned differently. Some time they will learn... )
		 */
		E820_TYPE_PRAM		= 12,

		/*
		 * Special-purpose memory is indicated to the system via the
		 * EFI_MEMORY_SP attribute. Define an e820 translation of this
		 * memory type for the purpose of reserving this range and
		 * marking it with the IORES_DESC_SOFT_RESERVED designation.
		 */
		E820_TYPE_SOFT_RESERVED	= 0xefffffff,

		/*
		 * Reserved RAM used by the kernel itself if
		 * CONFIG_INTEL_TXT=y is enabled, memory of this type
		 * will be included in the S3 integrity calculation
		 * and so should not include any memory that the BIOS
		 * might alter over the S3 transition:
		 */
		E820_TYPE_RESERVED_KERN	= 128,
	};

	/*
	 * A single E820 map entry, describing a memory range of [addr...addr+size-1],
	 * of 'type' memory type:
	 *
	 * (We pack it because there can be thousands of them on large systems.)
	 */
	typedef struct e820_entry {
		u64			addr;
		u64			size;
		enum e820_type	type;
	} e820_entry_s;

	/*
	 * The whole array of E820 entries:
	 */
	typedef struct e820_table {
		__u32			nr_entries;
		e820_entry_s	entries[E820_MAX_ENTRIES];
	} e820_table_s;

#endif /* _ASM_E820_TYPES_H */
