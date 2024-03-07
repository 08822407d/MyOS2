// source: linux-6.4.9

// arch/x86/include/asm/e820/types.h

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_E820_CONST_H_
#define _ASM_E820_CONST_H_

	/*
	 * Various well-known legacy memory ranges in physical memory:
	 */
	#define ISA_START_ADDRESS	0x000a0000
	#define ISA_END_ADDRESS		0x00100000

	#define BIOS_BEGIN			0x000a0000
	#define BIOS_END			0x00100000

	#define HIGH_MEMORY			0x00100000

	#define BIOS_ROM_BASE		0xffe00000
	#define BIOS_ROM_END		0xffffffff

#endif /* _ASM_E820_CONST_H_ */
