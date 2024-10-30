/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_THREADS_H
#define _LINUX_THREADS_H


	/*
	 * The default limit for the nr of threads is now in
	 * /proc/sys/kernel/threads-max.
	 */

	/*
	 * Maximum supported processors.  Setting this smaller saves quite a
	 * bit of memory.  Use nr_cpu_ids instead of this except for static bitmaps.
	 */
	#ifndef CONFIG_NR_CPUS
	/* FIXME: This should be fixed in the arch's Kconfig */
	#	define CONFIG_NR_CPUS	1
	#endif

	/* Places which use this should consider cpumask_var_t. */
	#define NR_CPUS				CONFIG_NR_CPUS

	// #define MIN_THREADS_LEFT_FOR_ROOT	4

#endif
