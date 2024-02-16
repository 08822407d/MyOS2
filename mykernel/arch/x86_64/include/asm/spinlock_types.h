/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_TYPES_H
#define _ASM_X86_SPINLOCK_TYPES_H

	#include <linux/kernel/types.h>

	// #include <asm-generic/tspinlock_types.h>
	typedef struct tspinlock {
		u32		head;
		u32		tail;
	} arch_spinlock_t;

	/*
	 * Initializier
	 */
	#define	__ARCH_SPIN_LOCK_UNLOCKED	{	\
				.head = 0U,					\
				.tail = 0U,					\
			}

	// #include <asm-generic/qrwlock_types.h>

#endif /* _ASM_X86_SPINLOCK_TYPES_H */
