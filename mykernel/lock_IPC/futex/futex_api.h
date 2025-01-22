/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LINUX_FUTEX_API_H_
#define _LINUX_FUTEX_API_H_

	#include "futex.h"


	extern long do_futex(u32 __user *uaddr, int op, u32 val,
			ktime_t *timeout, u32 __user *uaddr2, u32 val2, u32 val3);

#endif /* _LINUX_FUTEX_API_H_ */
