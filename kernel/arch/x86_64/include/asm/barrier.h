/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BARRIER_H
#define _ASM_X86_BARRIER_H

	// #include <asm/alternative.h>
	// #include <asm/nops.h>

	/*
	* Force strict CPU ordering.
	* And yes, this might be required on UP too when we're talking
	* to devices.
	*/

	#define __mb()		asm volatile("mfence":::"memory")
	#define __rmb()		asm volatile("lfence":::"memory")
	#define __wmb()		asm volatile("sfence" ::: "memory")

	// /**
	//  * array_index_mask_nospec() - generate a mask that is ~0UL when the
	//  * 	bounds check succeeds and 0 otherwise
	//  * @index: array element index
	//  * @size: number of elements in array
	//  *
	//  * Returns:
	//  *     0 - (index < size)
	//  */
	// static inline unsigned long
	// array_index_mask_nospec(unsigned long index, unsigned long size)
	// {
	// 	unsigned long mask;
	// 	asm volatile(	"cmp	%1,		%2	\n\t"
	// 					"sbb	%0,		%0	\n\t"
	// 				:	"=r"(mask)
	// 				:	"g"(size),
	// 					"r"(index)
	// 				:	"cc");
	// 	return mask;
	// }

	// /* Override the default implementation from linux/nospec.h. */
	// #define array_index_mask_nospec array_index_mask_nospec

	// /* Prevent speculative execution past this barrier. */
	// #define barrier_nospec() alternative("", "lfence", X86_FEATURE_LFENCE_RDTSC)

	// #define __dma_rmb()	barrier()
	// #define __dma_wmb()	barrier()

	// #define __smp_mb()	asm volatile("lock; addl $0,-4(%%" _ASM_SP ")" ::: "memory", "cc")

	// #define __smp_rmb()	dma_rmb()
	// #define __smp_wmb()	barrier()
	// #define __smp_store_mb(var, value) do { (void)xchg(&var, value); } while (0)

	// #define __smp_store_release(p, v)					\
	// do {									\
	// 	compiletime_assert_atomic_type(*p);				\
	// 	barrier();							\
	// 	WRITE_ONCE(*p, v);						\
	// } while (0)

	// #define __smp_load_acquire(p)						\
	// ({									\
	// 	typeof(*p) ___p1 = READ_ONCE(*p);				\
	// 	compiletime_assert_atomic_type(*p);				\
	// 	barrier();							\
	// 	___p1;								\
	// })

	// /* Atomic operations are already serializing on x86 */
	// #define __smp_mb__before_atomic()	do { } while (0)
	// #define __smp_mb__after_atomic()	do { } while (0)

	#include <linux/kernel/asm-generic/barrier.h>

	// /*
	// * Make previous memory operations globally visible before
	// * a WRMSR.
	// *
	// * MFENCE makes writes visible, but only affects load/store
	// * instructions.  WRMSR is unfortunately not a load/store
	// * instruction and is unaffected by MFENCE.  The LFENCE ensures
	// * that the WRMSR is not reordered.
	// *
	// * Most WRMSRs are full serializing instructions themselves and
	// * do not require this barrier.  This is only required for the
	// * IA32_TSC_DEADLINE and X2APIC MSRs.
	// */
	// static inline void weak_wrmsr_fence(void)
	// {
	// 	asm volatile("mfence; lfence" : : : "memory");
	// }

#endif /* _ASM_X86_BARRIER_H */
