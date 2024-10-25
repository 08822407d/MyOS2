/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_TYPES_H_
#define _ASM_X86_SPINLOCK_TYPES_H_

	#include "../lock_ipc_type_declaration_arch.h"


	struct tspinlock {
		union {
			atomic64_t	val;

			struct {
				u32		head;
				u32		tail;
			};
		};
	};

	/*
	 * Initializier
	 */
	#define	__ARCH_SPIN_LOCK_UNLOCKED	{	\
				.head = 0U,					\
				.tail = 0U,					\
			}

#endif /* _ASM_X86_SPINLOCK_TYPES_H_ */
