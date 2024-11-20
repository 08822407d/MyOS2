// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PERCPU_H
#define _ASM_X86_PERCPU_H


	#ifdef __ASSEMBLY__

	#	define __percpu_seg		gs
	#	define PER_CPU_VAR(var)			%__percpu_seg:var
	#	define INIT_PER_CPU_VAR(var)	init_per_cpu__##var

	#else /* ...!ASSEMBLY */

	#	include <linux/kernel/kernel.h>

	// #include <asm-generic/percpu.h>
	//<asm-generic/percpu.h>
	// {
		#include <linux/compiler/compiler.h>
		#include <linux/kernel/threads.h>
		#include <asm/mm_const.h>

		/*
		 * per_cpu_offset() is the offset that has to be added to a
		 * percpu variable to get to the instance for a certain processor.
		 *
		 * Most arches use the __per_cpu_offset array for those offsets but
		 * some arches have their own ways of determining the offset (x86_64, s390).
		 */
		#ifndef __per_cpu_offset
			extern unsigned long __per_cpu_offset[NR_CPUS];
		#	define per_cpu_offset(x) (__per_cpu_offset[x])
		#endif

		#ifndef PER_CPU_BASE_SECTION
		#	define PER_CPU_BASE_SECTION ".data..percpu"
		#endif
	// }

	#define PER_CPU_SHARED_ALIGNED_SECTION	""

	//<linux/smp/percpu-defs.h>
	/*
	 * Base implementations of per-CPU variable declarations and definitions, where
	 * the section in which the variable is to be placed is provided by the
	 * 'sec' argument.  This may be used to affect the parameters governing the
	 * variable's storage.
	 *
	 * NOTE!  The sections for the DECLARE and for the DEFINE must match, lest
	 * linkage errors occur due the compiler generating the wrong code to access
	 * that section.
	 */
	#define __PCPU_ATTRS(sec)	__percpu __attribute__(	\
				(section(PER_CPU_BASE_SECTION sec))		\
			)

	/*
	 * Normal declaration and definition macros.
	 */
	#define DECLARE_PER_CPU_SECTION(type, name, sec)	\
				extern __PCPU_ATTRS(sec) __typeof__(type) name

	#define DEFINE_PER_CPU_SECTION(type, name, sec)		\
				__PCPU_ATTRS(sec) __typeof__(type) name


	/*
	 * Variant on the per-CPU variable declaration/definition theme used for
	 * ordinary per-CPU variables.
	 */
	#define DECLARE_PER_CPU(type, name)					\
				DECLARE_PER_CPU_SECTION(type, name, "")

	#define DEFINE_PER_CPU(type, name)					\
				DEFINE_PER_CPU_SECTION(type, name, "")

	/*
	 * Declaration/definition used for per-CPU variables that must come first in
	 * the set of variables.
	 */
	#define DECLARE_PER_CPU_FIRST(type, name)	\
				DECLARE_PER_CPU_SECTION(type,	\
					name, PER_CPU_FIRST_SECTION)

	#define DEFINE_PER_CPU_FIRST(type, name)	\
				DEFINE_PER_CPU_SECTION(type,	\
					name, PER_CPU_FIRST_SECTION)

	/*
	 * Declaration/definition used for per-CPU variables that must be cacheline
	 * aligned under SMP conditions so that, whilst a particular instance of the
	 * data corresponds to a particular CPU, inefficiencies due to direct access by
	 * other CPUs are reduced by preventing the data from unnecessarily spanning
	 * cachelines.
	 *
	 * An example of this would be statistical data, where each CPU's set of data
	 * is updated by that CPU alone, but the data from across all CPUs is collated
	 * by a CPU processing a read from a proc file.
	 */
	#define DECLARE_PER_CPU_SHARED_ALIGNED(type, name)	\
				DECLARE_PER_CPU_SECTION(type, name,		\
					PER_CPU_SHARED_ALIGNED_SECTION)		\
				____cacheline_aligned_in_smp

	#define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)	\
				DEFINE_PER_CPU_SECTION(type, name,		\
					PER_CPU_SHARED_ALIGNED_SECTION)		\
				____cacheline_aligned_in_smp

	#define DECLARE_PER_CPU_ALIGNED(type, name)			\
				DECLARE_PER_CPU_SECTION(type, name,		\
					PER_CPU_ALIGNED_SECTION)			\
				____cacheline_aligned

	#define DEFINE_PER_CPU_ALIGNED(type, name)			\
				DEFINE_PER_CPU_SECTION(type, name,		\
					PER_CPU_ALIGNED_SECTION)			\
				____cacheline_aligned

	/*
	 * Declaration/definition used for per-CPU variables that must be page aligned.
	 */
	#define DECLARE_PER_CPU_PAGE_ALIGNED(type, name)	\
				DECLARE_PER_CPU_SECTION(type, name,		\
					"..page_aligned")					\
				__aligned(PAGE_SIZE)

	#define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)		\
				DEFINE_PER_CPU_SECTION(type, name,		\
					"..page_aligned")					\
				__aligned(PAGE_SIZE)

	/*
	 * Declaration/definition used for per-CPU variables that must be read mostly.
	 */
	#define DECLARE_PER_CPU_READ_MOSTLY(type, name)		\
				DECLARE_PER_CPU_SECTION(type, name,		\
					"..read_mostly")

	#define DEFINE_PER_CPU_READ_MOSTLY(type, name)		\
				DEFINE_PER_CPU_SECTION(type, name,		\
					"..read_mostly")

	/*
	 * Variant on the per-CPU variable declaration/definition theme used for
	 * ordinary per-CPU variables.
	 */
	#define DECLARE_PER_CPU(type, name)	\
				DECLARE_PER_CPU_SECTION(type, name, "")

	#define DEFINE_PER_CPU(type, name)	\
				DEFINE_PER_CPU_SECTION(type, name, "")

	#define DECLARE_PER_CPU_CACHE_ALIGNED(type, name)	\
				DECLARE_PER_CPU_SECTION(type, name, "")	\
				____cacheline_aligned

	#define DEFINE_PER_CPU_CACHE_ALIGNED(type, name)	\
				DEFINE_PER_CPU_SECTION(type, name, "")	\
				____cacheline_aligned

	/*
	 * Declaration/definition used for per-CPU variables that must be page aligned.
	 */
	#define DECLARE_PER_CPU_PAGE_ALIGNED(type, name)	\
				DECLARE_PER_CPU_SECTION(type, name, 	\
					"..page_aligned")					\
				__aligned(PAGE_SIZE)

	#define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)		\
				DEFINE_PER_CPU_SECTION(type, name,		\
					"..page_aligned")					\
				__aligned(PAGE_SIZE)




	extern unsigned long rdgsbase(void);
	#define raw_cpu_read_long(pcp)	((ulong)rdgsbase())

	#define arch_raw_cpu_ptr(_ptr)	({							\
				ulong tcp_ptr__ =								\
						raw_cpu_read_long(this_cpu_off);		\
				tcp_ptr__ += (__force ulong)(_ptr);				\
				(typeof(*(_ptr)) __kernel __force *)tcp_ptr__;	\
			})

	/*
	 * Accessors and operations.
	 */

	/*
	 * __verify_pcpu_ptr() verifies @ptr is a percpu pointer without evaluating
	 * @ptr and is invoked once before a percpu area is accessed by all
	 * accessors and operations.  This is performed in the generic part of
	 * percpu and arch overrides don't need to worry about it; however, if an
	 * arch wants to implement an arch-specific percpu accessor or operation,
	 * it may use __verify_pcpu_ptr() to verify the parameters.
	 *
	 * + 0 is required in order to convert the pointer type from a
	 * potential array type to a pointer to a single item of the array.
	 */
	#define __verify_pcpu_ptr(ptr)

	/*
	 * Add an offset to a pointer but keep the pointer as-is.  Use RELOC_HIDE()
	 * to prevent the compiler from making incorrect assumptions about the
	 * pointer value.  The weird cast keeps both GCC and sparse happy.
	 */
	#define SHIFT_PERCPU_PTR(__p, __offset)								\
			RELOC_HIDE(													\
				(typeof(*(__p)) __kernel __force *)(__p), (__offset)	\
			)

	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})

	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
	#define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))


	#include <asm-generic/sections.h>
	#define BOOT_PERCPU_OFFSET ((unsigned long)__per_cpu_load)

	#endif /* !__ASSEMBLY__ */

#endif /* _ASM_X86_PERCPU_H */
