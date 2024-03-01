/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROCESSOR_H
#define _ASM_X86_PROCESSOR_H

	#include <asm/processor-flags.h>
	#include <asm/segment.h>
	#include <asm/current.h>
	#include <asm/cpufeatures.h>
	#include <asm/page.h>
	#include <asm/percpu.h>
	#include <asm/msr.h>
	#include <asm/desc_defs.h>
	#include <asm/insns.h>

	#include <linux/kernel/cache.h>
	#include <linux/kernel/math64.h>
	#include <linux/kernel/err.h>
	#include <linux/kernel/irqflags.h>

	#include <asm/ptrace.h>
	#include <asm/thread_info.h>
	#include <asm/mm.h>


	static inline void
	native_cpuid(unsigned int *eax, unsigned int *ebx,
			unsigned int *ecx, unsigned int *edx) {
		/* ecx is often an input as well as an output. */
		asm volatile(	"cpuid"
					:	"=a"(*eax),	"=b"(*ebx),
						"=c"(*ecx),	"=d"(*edx)
					:	"0"(*eax),	"2"(*ecx)
					:	"memory");
	}

	/* Save actual FS/GS selectors and bases to current->thread */
	void current_save_fsgs(void);

	// struct perf_event;


	// extern void fpu_thread_struct_whitelist(unsigned long *offset, unsigned long *size);

	// static inline void arch_thread_struct_whitelist(unsigned long *offset,
	// 												unsigned long *size)
	// {
	// 	fpu_thread_struct_whitelist(offset, size);
	// }

	// static __always_inline void native_swapgs(void)
	// {
	// 	asm volatile("swapgs" ::
	// 					: "memory");
	// }

	// static inline unsigned long current_top_of_stack(void)
	// {
	// 	/*
	// 	*  We can't read directly from tss.sp0: sp0 on x86_32 is special in
	// 	*  and around vm86 mode and sp0 on x86_64 is special because of the
	// 	*  entry trampoline.
	// 	*/
	// 	return this_cpu_read_stable(cpu_current_top_of_stack);
	// }

	// static inline bool on_thread_stack(void)
	// {
	// 	return (unsigned long)(current_top_of_stack() -
	// 						current_stack_pointer) < THREAD_SIZE;
	// }

	#define __cpuid	native_cpuid

	static inline void load_sp0(unsigned long sp0) {
		// native_load_sp0(unsigned long sp0) {
			this_cpu_ptr(&cpu_tss_rw)->x86_tss.sp0 = sp0;
		// }
	}

	/*
	 * Generic CPUID function
	 * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
	 * resulting in stale register contents being returned.
	 */
	static inline void cpuid(unsigned int op,
			unsigned int *eax, unsigned int *ebx,
			unsigned int *ecx, unsigned int *edx) {
		*eax = op;
		*ecx = 0;
		__cpuid(eax, ebx, ecx, edx);
	}

	/* Some CPUID calls want 'count' to be placed in ecx */
	static inline void cpuid_count(
			unsigned int op, int count,
			unsigned int *eax, unsigned int *ebx,
			unsigned int *ecx, unsigned int *edx) {
		*eax = op;
		*ecx = count;
		__cpuid(eax, ebx, ecx, edx);
	}

	/*
	 * CPUID functions returning a single datum
	 */
	static inline unsigned int cpuid_eax(unsigned int op) {
		unsigned int eax, ebx, ecx, edx;
		cpuid(op, &eax, &ebx, &ecx, &edx);
		return eax;
	}

	static inline unsigned int cpuid_ebx(unsigned int op) {
		unsigned int eax, ebx, ecx, edx;
		cpuid(op, &eax, &ebx, &ecx, &edx);
		return ebx;
	}

	static inline unsigned int cpuid_ecx(unsigned int op) {
		unsigned int eax, ebx, ecx, edx;
		cpuid(op, &eax, &ebx, &ecx, &edx);
		return ecx;
	}

	static inline unsigned int cpuid_edx(unsigned int op) {
		unsigned int eax, ebx, ecx, edx;
		cpuid(op, &eax, &ebx, &ecx, &edx);
		return edx;
	}



	// #define HAVE_ARCH_PICK_MMAP_LAYOUT 1
	// #define ARCH_HAS_PREFETCHW
	// #define ARCH_HAS_SPINLOCK_PREFETCH

	// #define BASE_PREFETCH "prefetcht0 %P1"

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

	#define TOP_OF_INIT_STACK (										\
				(unsigned long)&init_stack + sizeof(init_stack) -	\
					TOP_OF_KERNEL_STACK_PADDING						\
			)

	#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))

	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})

	extern void
	start_thread(pt_regs_s *regs, unsigned long new_ip, unsigned long new_sp);

#endif /* _ASM_X86_PROCESSOR_H */
