/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LINUX_READWRITE_SEMAPHORE_API_H_
#define _LINUX_READWRITE_SEMAPHORE_API_H_

	#include "rwsem.h"


	void __init_rwsem(rwsem_t *sem, const char *name);

	/*
	 * lock for reading
	 */
	extern void down_read(rwsem_t *sem);
	extern int __must_check down_read_interruptible(rwsem_t *sem);
	extern int __must_check down_read_killable(rwsem_t *sem);

	/*
	 * trylock for reading -- returns 1 if successful, 0 if contention
	 */
	extern int down_read_trylock(rwsem_t *sem);

	/*
	 * lock for writing
	 */
	extern void down_write(rwsem_t *sem);
	extern int __must_check down_write_killable(rwsem_t *sem);

	/*
	 * trylock for writing -- returns 1 if successful, 0 if contention
	 */
	extern int down_write_trylock(rwsem_t *sem);

	/*
	 * release a read lock
	 */
	extern void up_read(rwsem_t *sem);

	/*
	 * release a write lock
	 */
	extern void up_write(rwsem_t *sem);

	// DEFINE_GUARD(rwsem_read, rwsem_t *, down_read(_T), up_read(_T))
	// DEFINE_GUARD_COND(rwsem_read, _try, down_read_trylock(_T))
	// DEFINE_GUARD_COND(rwsem_read, _intr, down_read_interruptible(_T) == 0)

	// DEFINE_GUARD(rwsem_write, rwsem_t *, down_write(_T), up_write(_T))
	// DEFINE_GUARD_COND(rwsem_write, _try, down_write_trylock(_T))

	/*
	 * downgrade write lock to read lock
	 */
	extern void downgrade_write(rwsem_t *sem);

#endif /* _LINUX_READWRITE_SEMAPHORE_API_H_ */
