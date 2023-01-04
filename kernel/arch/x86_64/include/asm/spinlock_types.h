/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPINLOCK_TYPES_H
#define _ASM_X86_SPINLOCK_TYPES_H

	#include <linux/kernel/types.h>
	#ifdef USE_QSPINLOCK
	#	include <linux/kernel/asm-generic/qspinlock_types.h>
	#else
	#	include <linux/kernel/asm-generic/tspinlock_types.h>
	#endif
	// #include <asm-generic/qrwlock_types.h>

#endif /* _ASM_X86_SPINLOCK_TYPES_H */
