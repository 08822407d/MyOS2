/*
 * Copyright (C) 2018-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ASM_GENERIC_TSPINLOCK_TYPES_H
#define __ASM_GENERIC_TSPINLOCK_TYPES_H

	#include <linux/kernel/types.h>

	typedef struct tspinlock {
		u32		head;
		u32		tail;
	} arch_spinlock_t;

#endif /* __ASM_GENERIC_TSPINLOCK_TYPES_H */