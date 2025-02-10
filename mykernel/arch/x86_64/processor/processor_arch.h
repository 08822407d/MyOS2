/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROCESSOR_H_
#define _ASM_X86_PROCESSOR_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <asm/current.h>
	#include <asm/percpu.h>

	#include "processor_const_arch.h"
	#include "processor_types_arch.h"


	#ifdef DEBUG

		extern void
		native_cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx);
		#define __cpuid	native_cpuid

		extern ulong
		cpu_kernelmode_gs_base(int cpu);

		extern void
		native_swapgs(void);

		// extern unsigned long
		// current_top_of_stack(void);

		extern void
		load_sp0(unsigned long sp0);

		extern void
		cpuid(unsigned int op,
				unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx);

		extern void
		cpuid_count(unsigned int op, int count,
				unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx);

		extern unsigned int
		cpuid_eax(unsigned int op);
		extern unsigned int
		cpuid_ebx(unsigned int op);
		extern unsigned int
		cpuid_ecx(unsigned int op);
		extern unsigned int
		cpuid_edx(unsigned int op);

		extern void
		rep_nop(void);
		extern void
		cpu_relax(void);

	#endif

	#if defined(ARCH_PROCESSOR_DEFINATION) || !(DEBUG)
	
		// /*
		// * Prefetch instructions for Pentium III (+) and AMD Athlon (+)
		// *
		// * It's not worth to care about 3dnow prefetches for the K6
		// * because they are microcoded there and very slow.
		// */
		// static inline void prefetch(const void *x)
		// {
		// 	alternative_input(BASE_PREFETCH, "prefetchnta %P1",
		// 					X86_FEATURE_XMM,
		// 					"m"(*(const char *)x));
		// }

		// /*
		// * 3dnow prefetch to get an exclusive cache line.
		// * Useful for spinlocks to avoid one state transition in the
		// * cache coherency protocol:
		// */
		// static __always_inline void prefetchw(const void *x)
		// {
		// 	alternative_input(BASE_PREFETCH, "prefetchw %P1",
		// 					X86_FEATURE_3DNOWPREFETCH,
		// 					"m"(*(const char *)x));
		// }

		// static inline void spin_lock_prefetch(const void *x)
		// {
		// 	prefetchw(x);
		// }

		PREFIX_STATIC_INLINE
		void
		native_cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx) {
			/* ecx is often an input as well as an output. */
			asm volatile(	"cpuid"
						:	"=a" (*eax),	"=b" (*ebx),
							"=c" (*ecx),	"=d" (*edx)
						:	"0" (*eax),	"2" (*ecx)
						:	"memory"
						);
		}
		
		PREFIX_STATIC_INLINE
		ulong
		cpu_kernelmode_gs_base(int cpu) {
			// return (ulong)per_cpu(fixed_percpu_data.gs_base, cpu);
		}

		// static inline void arch_thread_struct_whitelist(unsigned long *offset,
		// 												unsigned long *size)
		// {
		// 	fpu_thread_struct_whitelist(offset, size);
		// }

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		native_swapgs(void) {
			asm volatile(	"swapgs"
						:
						:
						:	"memory"
						);
		}

		// PREFIX_STATIC_INLINE
		// unsigned long
		// current_top_of_stack(void) {
		// 	/*
		// 	 *  We can't read directly from tss.sp0: sp0 on x86_32 is special in
		// 	 *  and around vm86 mode and sp0 on x86_64 is special because of the
		// 	 *  entry trampoline.
		// 	 */
		// 	// return this_cpu_read_stable(cpu_current_top_of_stack);
		// 	return task_top_of_stack(current);
		// }

		// static inline bool on_thread_stack(void)
		// {
		// 	return (unsigned long)(current_top_of_stack() -
		// 						current_stack_pointer) < THREAD_SIZE;
		// }

		PREFIX_STATIC_INLINE
		void
		load_sp0(unsigned long sp0) {
			// native_load_sp0(unsigned long sp0) {
				this_cpu_ptr(&cpu_tss_rw)->x86_tss.sp0 = sp0;
			// }
		}

		/*
		 * Generic CPUID function
		 * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
		 * resulting in stale register contents being returned.
		 */
		PREFIX_STATIC_INLINE
		void
		cpuid(unsigned int op,
				unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx) {
			*eax = op;
			*ecx = 0;
			__cpuid(eax, ebx, ecx, edx);
		}

		/* Some CPUID calls want 'count' to be placed in ecx */
		PREFIX_STATIC_INLINE
		void
		cpuid_count(unsigned int op, int count,
				unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx) {
			*eax = op;
			*ecx = count;
			__cpuid(eax, ebx, ecx, edx);
		}

		/*
		 * CPUID functions returning a single datum
		 */
		PREFIX_STATIC_INLINE
		unsigned int
		cpuid_eax(unsigned int op) {
			unsigned int eax, ebx, ecx, edx;
			cpuid(op, &eax, &ebx, &ecx, &edx);
			return eax;
		}

		PREFIX_STATIC_INLINE
		unsigned int
		cpuid_ebx(unsigned int op) {
			unsigned int eax, ebx, ecx, edx;
			cpuid(op, &eax, &ebx, &ecx, &edx);
			return ebx;
		}

		PREFIX_STATIC_INLINE
		unsigned int
		cpuid_ecx(unsigned int op) {
			unsigned int eax, ebx, ecx, edx;
			cpuid(op, &eax, &ebx, &ecx, &edx);
			return ecx;
		}

		PREFIX_STATIC_INLINE
		unsigned int
		cpuid_edx(unsigned int op) {
			unsigned int eax, ebx, ecx, edx;
			cpuid(op, &eax, &ebx, &ecx, &edx);
			return edx;
		}


		/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		rep_nop(void) {
			asm volatile(	"rep; nop"
						:
						:
						:	"memory");
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		cpu_relax(void) {
			rep_nop();
		}

	#endif

#endif /* _ASM_X86_PROCESSOR_H */
