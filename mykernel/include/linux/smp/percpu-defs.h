/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * linux/percpu-defs.h - basic definitions for percpu areas
 *
 * DO NOT INCLUDE DIRECTLY OUTSIDE PERCPU IMPLEMENTATION PROPER.
 *
 * This file is separate from linux/percpu.h to avoid cyclic inclusion
 * dependency from arch header files.  Only to be included from
 * asm/percpu.h.
 *
 * This file includes macros necessary to declare percpu sections and
 * variables, and definitions of percpu accessors and operations.  It
 * should provide enough percpu features to arch header files even when
 * they can only include asm/percpu.h to avoid cyclic inclusion dependency.
 */

#ifndef _LINUX_PERCPU_DEFS_H
#define _LINUX_PERCPU_DEFS_H

	// #ifdef MODULE
	// #define PER_CPU_SHARED_ALIGNED_SECTION ""
	// #define PER_CPU_ALIGNED_SECTION ""
	// #else
	// #define PER_CPU_SHARED_ALIGNED_SECTION "..shared_aligned"
	// #define PER_CPU_ALIGNED_SECTION "..shared_aligned"
	// #endif
	// #define PER_CPU_FIRST_SECTION "..first"

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
	#define __PCPU_ATTRS(sec)												\
				__percpu __attribute__((section(PER_CPU_BASE_SECTION sec)))

	// #define __PCPU_DUMMY_ATTRS						\
	// 	__section(".discard") __attribute__((unused))

	/*
	 * Normal declaration and definition macros.
	 */
	// #define DECLARE_PER_CPU_SECTION(type, name, sec)			\
	// 			extern __PCPU_ATTRS(sec) __typeof__(type) name

	#define DEFINE_PER_CPU_SECTION(type, name, sec)				\
				__PCPU_ATTRS(sec) __typeof__(type) name

	// /*
	//  * Variant on the per-CPU variable declaration/definition theme used for
	//  * ordinary per-CPU variables.
	//  */
	// #define DECLARE_PER_CPU(type, name)					\
	// 	DECLARE_PER_CPU_SECTION(type, name, "")

	// #define DEFINE_PER_CPU(type, name)					\
	// 	DEFINE_PER_CPU_SECTION(type, name, "")

	// /*
	//  * Declaration/definition used for per-CPU variables that must come first in
	//  * the set of variables.
	//  */
	// #define DECLARE_PER_CPU_FIRST(type, name)				\
	// 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)

	// #define DEFINE_PER_CPU_FIRST(type, name)				\
	// 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)

	// /*
	//  * Declaration/definition used for per-CPU variables that must be cacheline
	//  * aligned under SMP conditions so that, whilst a particular instance of the
	//  * data corresponds to a particular CPU, inefficiencies due to direct access by
	//  * other CPUs are reduced by preventing the data from unnecessarily spanning
	//  * cachelines.
	//  *
	//  * An example of this would be statistical data, where each CPU's set of data
	//  * is updated by that CPU alone, but the data from across all CPUs is collated
	//  * by a CPU processing a read from a proc file.
	//  */
	// #define DECLARE_PER_CPU_SHARED_ALIGNED(type, name)			\
	// 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
	// 	____cacheline_aligned_in_smp

	// #define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)			\
	// 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
	// 	____cacheline_aligned_in_smp

	// #define DECLARE_PER_CPU_ALIGNED(type, name)				\
	// 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
	// 	____cacheline_aligned

	// #define DEFINE_PER_CPU_ALIGNED(type, name)				\
	// 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
	// 	____cacheline_aligned

	// /*
	//  * Declaration/definition used for per-CPU variables that must be page aligned.
	//  */
	// #define DECLARE_PER_CPU_PAGE_ALIGNED(type, name)			\
	// 	DECLARE_PER_CPU_SECTION(type, name, "..page_aligned")		\
	// 	__aligned(PAGE_SIZE)

	// #define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)						\
	// 			DEFINE_PER_CPU_SECTION(type, name, "..page_aligned")	\
	// 			__aligned(PAGE_SIZE)

	// /*
	//  * Declaration/definition used for per-CPU variables that must be read mostly.
	//  */
	// #define DECLARE_PER_CPU_READ_MOSTLY(type, name)			\
	// 	DECLARE_PER_CPU_SECTION(type, name, "..read_mostly")

	// #define DEFINE_PER_CPU_READ_MOSTLY(type, name)				\
	// 			DEFINE_PER_CPU_SECTION(type, name, "..read_mostly")


	/*
	 * Accessors and operations.
	 */
	#ifndef __ASSEMBLY__

	/*
	 * Add an offset to a pointer but keep the pointer as-is.  Use RELOC_HIDE()
	 * to prevent the compiler from making incorrect assumptions about the
	 * pointer value.  The weird cast keeps both GCC and sparse happy.
	 */
	#	define SHIFT_PERCPU_PTR(__p, __offset)	RELOC_HIDE(					\
					(typeof(*(__p)) __kernel __force *)(__p), (__offset)	\
				)

	#	define per_cpu_ptr(ptr, cpu)	({		\
					SHIFT_PERCPU_PTR((ptr),		\
						per_cpu_offset((cpu)));	\
				})


	#	define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)

	#	define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))



	#define DEFINE_PER_CPU(type, name)						\
				DEFINE_PER_CPU_SECTION(type, name, "")
	#define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)						\
				DEFINE_PER_CPU_SECTION(type, name, "..page_aligned")	\
				__aligned(PAGE_SIZE)

	#endif /* __ASSEMBLY__ */

#endif /* _LINUX_PERCPU_DEFS_H */
