/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PERCPU_H
#define __LINUX_PERCPU_H

	#include <linux/smp/smp.h>
	#include <linux/kernel/cpumask.h>
	#include <linux/init/init.h>

	#include <asm/percpu.h>

	#include "percpu_chunk.h"


	extern size_t chunk_size;

	extern void *pcpu_base_addr;
	extern const unsigned long *pcpu_unit_offsets;

	void __percpu *simple_pcpu_alloc_noprof(size_t size,
		size_t align, bool reserved, gfp_t gfp);

	extern void __init setup_per_cpu_areas(void);

	extern void __init simple_pcpu_setup_first_chunk(void);


	#define pcpu_alloc_noprof simple_pcpu_alloc_noprof
	#define __alloc_percpu_gfp(_size, _align, _gfp)	\
				pcpu_alloc_noprof(_size,			\
					_align, false, _gfp)
	#define __alloc_percpu(_size, _align)			\
				pcpu_alloc_noprof(_size, _align,	\
					false, GFP_KERNEL)
	#define __alloc_reserved_percpu(_size, _align)	\
				pcpu_alloc_noprof(_size, _align,	\
					true, GFP_KERNEL)

	#define alloc_percpu_gfp(type, gfp)						\
			(typeof(type) __percpu *)__alloc_percpu_gfp(	\
					sizeof(type), __alignof__(type), gfp	\
			)
	#define alloc_percpu(type)								\
			(typeof(type) __percpu *)__alloc_percpu(		\
				sizeof(type), __alignof__(type)				\
			)
	#define alloc_percpu_noprof(type)						\
			(typeof(type) __percpu *)pcpu_alloc_noprof(		\
				sizeof(type), __alignof__(type),			\
					false, GFP_KERNEL						\
			)

#endif /* __LINUX_PERCPU_H */
