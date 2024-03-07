// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BARRIER_H
#define _ASM_X86_BARRIER_H

	#include <asm/alternative.h>

	/*
	 * Force strict CPU ordering.
	 * And yes, this might be required on UP too when we're talking
	 * to devices.
	 */

	#define __mb()		asm volatile("mfence":::"memory")
	#define __rmb()		asm volatile("lfence":::"memory")
	#define __wmb()		asm volatile("sfence" ::: "memory")

	#define __dma_rmb()	barrier()
	#define __dma_wmb()	barrier()

	#define __smp_rmb()	dma_rmb()
	#define __smp_wmb()	barrier()

	#include <asm-generic/barrier.h>

#endif /* _ASM_X86_BARRIER_H */
