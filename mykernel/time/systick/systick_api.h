/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SYSTICK_API_H_
#define _LINUX_SYSTICK_API_H_

	#include "systick.h"



	#ifndef __jiffy_arch_data
	#  define __jiffy_arch_data
	#endif

	/*
	 * The 64-bit value is not atomic on 32-bit systems - you MUST NOT read it
	 * without sampling the sequence number in jiffies_lock.
	 * get_jiffies_64() will do this for you as appropriate.
	 *
	 * jiffies and jiffies_64 are at the same address for little-endian systems
	 * and for 64-bit big-endian systems.
	 * On 32-bit big-endian systems, jiffies is the lower 32 bits of jiffies_64
	 * (i.e., at address @jiffies_64 + 4).
	 * See arch/ARCH/kernel/vmlinux.lds.S
	 */
	extern u64 __cacheline_aligned_in_smp jiffies_64;
	extern ulong volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;

	extern int register_refined_jiffies(long clock_tick_rate);

#endif /* _LINUX_SYSTICK_API_H_ */