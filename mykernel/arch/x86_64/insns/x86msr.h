// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MSR_H
#define _ASM_X86_MSR_H

	#include <linux/compiler/myos_debug_option.h>
	#include <linux/kernel/types.h>

	#include "x86msr_const.h"


	#ifdef DEBUG
	
		extern unsigned long long
		rdtsc(void);

		extern unsigned long long
		__rdmsr(unsigned int msr);

		extern void
		__wrmsr(unsigned int msr, u32 low, u32 high);

		extern void
		rdmsr(unsigned int msr, u32 *low, u32 *high);

		extern void
		wrmsr(unsigned int msr, u32 low, u32 high);

		extern void
		rdmsrl(unsigned int msr, u64 *val);

		extern void
		wrmsrl(unsigned int msr, u64 val);

	#endif

	#if defined(ARCH_INSTRUCTION_DEFINATION) || !(DEBUG)

		/*
		 * both i386 and x86_64 returns 64-bit value in edx:eax, but gcc's "A"
		 * constraint has different meanings. For i386, "A" means exactly
		 * edx:eax, while for x86_64 it doesn't mean rdx:rax or edx:eax. Instead,
		 * it means rax *or* rdx.
		 */
		/* Using 64-bit values saves one instruction clearing the high half of low */
		#define DECLARE_ARGS(val, low, high)	unsigned long low, high
		#define EAX_EDX_VAL(val, low, high)		((low) | (high) << 32)
		#define EAX_EDX_RET(val, low, high)		"=a" (low), "=d" (high)

		/**
		 * rdtsc() - returns the current TSC without ordering constraints
		 *
		 * rdtsc() returns the result of RDTSC as a 64-bit integer.  The
		 * only ordering constraint it supplies is the ordering implied by
		 * "asm volatile": it will put the RDTSC in the place you expect.  The
		 * CPU can and will speculatively execute that RDTSC, though, so the
		 * results can be non-monotonic if compared on different CPUs.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long long
		rdtsc(void) {
			DECLARE_ARGS(val, low, high);

			asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

			return EAX_EDX_VAL(val, low, high);
		}


		/*
		 * __rdmsr() and __wrmsr() are the two primitives which are the bare minimum MSR
		 * accessors and should not have any tracing or other functionality piggybacking
		 * on them - those are *purely* for accessing MSRs and nothing more. So don't even
		 * think of extending them - you will be slapped with a stinking trout or a frozen
		 * shark will reach you, wherever you are! You've been warned.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long long
		__rdmsr(unsigned int msr) {
			DECLARE_ARGS(val, low, high);

			asm volatile(	"1: rdmsr			\n"
							"2:					\n"
							// _ASM_EXTABLE_TYPE(1b, 2b, EX_TYPE_RDMSR)
						:	EAX_EDX_RET(val, low, high)
						:	"c" (msr)
						:
						);

			return EAX_EDX_VAL(val, low, high);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		__wrmsr(unsigned int msr, u32 low, u32 high) {
			asm volatile(	"1: wrmsr			\n"
							"2:					\n"
							// _ASM_EXTABLE_TYPE(1b, 2b, EX_TYPE_WRMSR)
						:
						:	"c" (msr),
							"a" (low),
							"d" (high)
						:	"memory"
						);
		}

		/*
		 * Access to machine-specific registers (available on 586 and better only)
		 * Note: the rd* operations modify the parameters directly (without using
		 * pointer indirection), this allows gcc to optimize better
		 */
		PREFIX_STATIC_INLINE
		void
		rdmsr(unsigned int msr, u32 *low, u32 *high) {
			u64 __val = __rdmsr(msr);
			*low = (u32)__val;
			*high = (u32)(__val >> 32);
		}

		PREFIX_STATIC_INLINE
		void
		wrmsr(unsigned int msr, u32 low, u32 high) {
			__wrmsr(msr, low, high);
		}

		// #define rdmsrl(msr, val)	\
		// 		((val) = __rdmsr((msr)))
		PREFIX_STATIC_INLINE
		void
		rdmsrl(unsigned int msr, u64 *val) {
			*val = __rdmsr(msr);
		}

		PREFIX_STATIC_INLINE
		void
		wrmsrl(unsigned int msr, u64 val) {
			__wrmsr(msr, (u32)(val & 0xffffffffULL),
					(u32)(val >> 32));
		}

	#endif

	#ifndef __ASSEMBLY__

	#	include <asm/asm.h>
	// #	include <asm/errno.h>
	// #	include <asm/cpumask.h>
	// #	include <uapi/asm/msr.h>
	// #	include <asm/shared/msr.h>

		// struct msr_info {
		// 	u32 msr_no;
		// 	struct msr reg;
		// 	struct msr *msrs;
		// 	int err;
		// };

		// struct msr_regs_info {
		// 	u32 *regs;
		// 	int err;
		// };

		// struct saved_msr {
		// 	bool valid;
		// 	struct msr_info info;
		// };

		// struct saved_msrs {
		// 	unsigned int num;
		// 	struct saved_msr *array;
		// };


	/*
	 * Be very careful with includes. This header is prone to include loops.
	 */
	#	include <asm/lock_ipc.h>
	// #include <linux/tracepoint-defs.h>

	// #	ifdef CONFIG_TRACEPOINTS
	// 		DECLARE_TRACEPOINT(read_msr);
	// 		DECLARE_TRACEPOINT(write_msr);
	// 		DECLARE_TRACEPOINT(rdpmc);
	// 		extern void do_trace_write_msr(unsigned int msr, u64 val, int failed);
	// 		extern void do_trace_read_msr(unsigned int msr, u64 val, int failed);
	// 		extern void do_trace_rdpmc(unsigned int msr, u64 val, int failed);
	// #	else
	// 		static inline void do_trace_write_msr(unsigned int msr, u64 val, int failed) {}
	// 		static inline void do_trace_read_msr(unsigned int msr, u64 val, int failed) {}
	// 		static inline void do_trace_rdpmc(unsigned int msr, u64 val, int failed) {}
	// #	endif



		// #define native_rdmsr(msr, val1, val2)			\
		// do {							\
		// 	u64 __val = __rdmsr((msr));			\
		// 	(void)((val1) = (u32)__val);			\
		// 	(void)((val2) = (u32)(__val >> 32));		\
		// } while (0)

		// #define native_wrmsr(msr, low, high)			\
		// 	__wrmsr(msr, low, high)

		// #define native_wrmsrl(msr, val)				\
		// 	__wrmsr((msr), (u32)((u64)(val)),		\
		// 			(u32)((u64)(val) >> 32))

		// static inline unsigned long long native_read_msr(unsigned int msr)
		// {
		// 	unsigned long long val;

		// 	val = __rdmsr(msr);

		// 	if (tracepoint_enabled(read_msr))
		// 		do_trace_read_msr(msr, val, 0);

		// 	return val;
		// }

		// static inline unsigned long long native_read_msr_safe(unsigned int msr,
		// 							int *err)
		// {
		// 	DECLARE_ARGS(val, low, high);

		// 	asm volatile("1: rdmsr ; xor %[err],%[err]\n"
		// 			"2:\n\t"
		// 			_ASM_EXTABLE_TYPE_REG(1b, 2b, EX_TYPE_RDMSR_SAFE, %[err])
		// 			: [err] "=r" (*err), EAX_EDX_RET(val, low, high)
		// 			: "c" (msr));
		// 	if (tracepoint_enabled(read_msr))
		// 		do_trace_read_msr(msr, EAX_EDX_VAL(val, low, high), *err);
		// 	return EAX_EDX_VAL(val, low, high);
		// }

		// /* Can be uninlined because referenced by paravirt */
		// static inline void notrace
		// native_write_msr(unsigned int msr, u32 low, u32 high)
		// {
		// 	__wrmsr(msr, low, high);

		// 	if (tracepoint_enabled(write_msr))
		// 		do_trace_write_msr(msr, ((u64)high << 32 | low), 0);
		// }

		// /* Can be uninlined because referenced by paravirt */
		// static inline int notrace
		// native_write_msr_safe(unsigned int msr, u32 low, u32 high)
		// {
		// 	int err;

		// 	asm volatile("1: wrmsr ; xor %[err],%[err]\n"
		// 			"2:\n\t"
		// 			_ASM_EXTABLE_TYPE_REG(1b, 2b, EX_TYPE_WRMSR_SAFE, %[err])
		// 			: [err] "=a" (err)
		// 			: "c" (msr), "0" (low), "d" (high)
		// 			: "memory");
		// 	if (tracepoint_enabled(write_msr))
		// 		do_trace_write_msr(msr, ((u64)high << 32 | low), err);
		// 	return err;
		// }

		// extern int rdmsr_safe_regs(u32 regs[8]);
		// extern int wrmsr_safe_regs(u32 regs[8]);

		// /**
		// * rdtsc_ordered() - read the current TSC in program order
		// *
		// * rdtsc_ordered() returns the result of RDTSC as a 64-bit integer.
		// * It is ordered like a load to a global in-memory counter.  It should
		// * be impossible to observe non-monotonic rdtsc_unordered() behavior
		// * across multiple CPUs as long as the TSC is synced.
		// */
		// static __always_inline unsigned long long rdtsc_ordered(void)
		// {
		// 	DECLARE_ARGS(val, low, high);

		// 	/*
		// 	* The RDTSC instruction is not ordered relative to memory
		// 	* access.  The Intel SDM and the AMD APM are both vague on this
		// 	* point, but empirically an RDTSC instruction can be
		// 	* speculatively executed before prior loads.  An RDTSC
		// 	* immediately after an appropriate barrier appears to be
		// 	* ordered as a normal load, that is, it provides the same
		// 	* ordering guarantees as reading from a global memory location
		// 	* that some other imaginary CPU is updating continuously with a
		// 	* time stamp.
		// 	*
		// 	* Thus, use the preferred barrier on the respective CPU, aiming for
		// 	* RDTSCP as the default.
		// 	*/
		// 	asm volatile(ALTERNATIVE_2("rdtsc",
		// 				"lfence; rdtsc", X86_FEATURE_LFENCE_RDTSC,
		// 				"rdtscp", X86_FEATURE_RDTSCP)
		// 			: EAX_EDX_RET(val, low, high)
		// 			/* RDTSCP clobbers ECX with MSR_TSC_AUX. */
		// 			:: "ecx");

		// 	return EAX_EDX_VAL(val, low, high);
		// }

		// static inline unsigned long long native_read_pmc(int counter)
		// {
		// 	DECLARE_ARGS(val, low, high);

		// 	asm volatile("rdpmc" : EAX_EDX_RET(val, low, high) : "c" (counter));
		// 	if (tracepoint_enabled(rdpmc))
		// 		do_trace_rdpmc(counter, EAX_EDX_VAL(val, low, high), 0);
		// 	return EAX_EDX_VAL(val, low, high);
		// }

	#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_MSR_H */
