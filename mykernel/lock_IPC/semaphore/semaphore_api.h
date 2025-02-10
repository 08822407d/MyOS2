/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LINUX_SEMAPHORE_API_H_
#define _LINUX_SEMAPHORE_API_H_

	#include "semaphore.h"

	extern void down(sema_t *sem);
	extern int __must_check down_interruptible(sema_t *sem);
	extern int __must_check down_killable(sema_t *sem);
	extern int __must_check down_trylock(sema_t *sem);
	extern int __must_check down_timeout(sema_t *sem, long jiffies);
	extern void up(sema_t *sem);

#endif /* _LINUX_SEMAPHORE_API_H_ */
