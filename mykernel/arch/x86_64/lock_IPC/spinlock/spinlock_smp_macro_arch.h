/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_SMP_MACRO_H_
#define _ASM_X86_SPINLOCK_SMP_MACRO_H_

	/*
	 * Initializier
	 */
	#define	__ARCH_SPIN_LOCK_UNLOCKED	{	\
				.head = 0U,					\
				.tail = 0U,					\
			}

#endif /* _ASM_X86_SPINLOCK_SMP_MACRO_H_ */
