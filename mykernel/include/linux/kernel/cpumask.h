/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_CPUMASK_H
#define __LINUX_CPUMASK_H

	// /*
	//  * Cpumasks provide a bitmap suitable for representing the
	//  * set of CPU's in a system, one bit position per CPU number.  In general,
	//  * only nr_cpu_ids (<= NR_CPUS) bits are valid.
	//  */
	// #include <linux/kernel.h>
	// #include <linux/threads.h>
	#include <linux/lib/bitmap.h>
	// #include <linux/atomic.h>
	// #include <linux/bug.h>
	// #include <linux/gfp_types.h>
	#include <linux/kernel/numa.h>

	extern uint nr_cpu_ids;
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)

	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
	#define for_each_online_cpu(cpu)	for_each_cpu((cpu))
	#define for_each_enabled_cpu(cpu)	for_each_cpu((cpu))
	#define for_each_present_cpu(cpu)	for_each_cpu((cpu))

	#define num_possible_cpus()		nr_cpu_ids
	#define nr_cpumask_bits			nr_cpu_ids

#endif /* __LINUX_CPUMASK_H */
