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

	extern void __init setup_per_cpu_areas(void);

	extern void __init simple_pcpu_setup_first_chunk(void);

#endif /* __LINUX_PERCPU_H */
