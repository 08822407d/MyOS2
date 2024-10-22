/* SPDX-License-Identifier: GPL-2.0 */
/*
 * include/linux/cpu.h - generic cpu definition
 *
 * This is mainly for topological representation. We define the 
 * basic 'struct cpu' here, which can be embedded in per-arch 
 * definitions of processors.
 *
 * Basic handling of the devices is done in drivers/base/cpu.c
 *
 * CPUs are exported via sysfs in the devices/system/cpu
 * directory. 
 */
#ifndef _LINUX_CPU_H_
#define _LINUX_CPU_H_

	#include <linux/compiler/compiler.h>
	#include <linux/kernel/cpumask.h>

	extern void cpu_init(void);
	extern void trap_init(void);
	void cpu_startup_entry(void);
	void arch_cpu_finalize_init(void);

#endif /* _LINUX_CPU_H_ */
