/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PERCPU_H
#define __LINUX_PERCPU_H

	// #include <linux/mmdebug.h>
	#include <linux/kernel/preempt.h>
	#include <linux/smp/smp.h>
	#include <linux/kernel/cpumask.h>
	#include <linux/mm/pfn.h>
	#include <linux/init/init.h>

	#include <asm/percpu.h>

	extern void *pcpu_base_addr;

	extern void __init setup_per_cpu_areas(void);

#endif /* __LINUX_PERCPU_H */
