#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

#include <linux/kernel/sched.h>
#include <linux/kernel/types.h>

	typedef struct
	{
		spinlock_t	selflock;
		task_s *	owner;
		__volatile__	unsigned	counter;
	} recurs_lock_T;

	void init_recurs_lock(recurs_lock_T * lock);
	void lock_recurs_lock(recurs_lock_T * lock);
	void unlock_recurs_lock(recurs_lock_T * lock);

#endif /* _POSIX_THREAD_H_ */