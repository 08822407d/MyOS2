// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_BITOPS_CONST_H_
#define _ASM_X86_BITOPS_CONST_H_

	#include <linux/kernel/asm-generic/bitsperlong.h>

	#if BITS_PER_LONG == 64
	#  define _BITOPS_LONG_SHIFT	6
	#else
	#  error "Unexpected BITS_PER_LONG"
	#endif

	#define BIT_64(n)	(U64_C(1) << (n))

	/*
	 * These have to be done with inline assembly: that way the bit-setting
	 * is guaranteed to be atomic. All bit operations return 0 if the bit
	 * was cleared before the operation and != 0 if it was not.
	 *
	 * bit 0 is the LSB of addr; bit 32 is the LSB of (addr+1).
	 */

	#define RLONG_ADDR(x)	"m"(*(volatile long *)(x))
	#define WBYTE_ADDR(x)	"+m"(*(volatile char *)(x))

	#define ADDR RLONG_ADDR(addr)

	/*
	 * We do the locked ops that don't return the old value as
	 * a mask operation on a byte.
	 */
	#define CONST_MASK_ADDR(nr, addr)	WBYTE_ADDR((void *)(addr) + ((nr) >> 3))
	#define CONST_MASK(nr)				(1 << ((nr)&7))

#endif /* _ASM_X86_BITOPS_CONST_H_ */