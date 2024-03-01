/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROCESSOR_TYPES_H_
#define _ASM_X86_PROCESSOR_TYPES_H_

	#include <linux/kernel/bits.h>

	#define X86_VENDOR_INTEL		0
	#define X86_VENDOR_CYRIX		1
	#define X86_VENDOR_AMD			2
	#define X86_VENDOR_UMC			3
	#define X86_VENDOR_CENTAUR		5
	#define X86_VENDOR_TRANSMETA	7
	#define X86_VENDOR_NSC			8
	#define X86_VENDOR_HYGON		9
	#define X86_VENDOR_ZHAOXIN		10
	#define X86_VENDOR_VORTEX		11
	#define X86_VENDOR_NUM			12

	#define X86_VENDOR_UNKNOWN		0xff

	/*
	 * IO-bitmap sizes:
	 */
	#define IO_BITMAP_BITS 65536
	#define IO_BITMAP_BYTES (IO_BITMAP_BITS / BITS_PER_BYTE)
	#define IO_BITMAP_LONGS (IO_BITMAP_BYTES / sizeof(long))

	/*
	 * sizeof(unsigned long) coming from an extra "long" at the end of the
	 * iobitmap. The limit is inclusive, i.e. the last valid byte.
	 */
	#define __KERNEL_TSS_LIMIT							\
				(offsetof(struct tss_struct, x86_tss) +	\
					sizeof(x86_hw_tss_s) - 1)

	/* Base offset outside of TSS_LIMIT so unpriviledged IO causes #GP */
	#define IO_BITMAP_OFFSET_INVALID (__KERNEL_TSS_LIMIT + 1)
    
#endif /* _ASM_X86_PROCESSOR_TYPES_H_ */