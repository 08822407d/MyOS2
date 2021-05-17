#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

#include "../../../include/task.h"

	typedef struct
	{
		__volatile__ unsigned long lock;		//1:unlock,0:lock
	} spinlock_T;

	typedef struct 
	{
		__volatile__ long value;
	} atomic_T;

	typedef struct
	{
		atomic_T	counter;
		task_list_s	waiting_tasks;
	} semaphore_T;
	

	#define atomic_read(atomic)	((atomic)->value)
	#define atomic_set(atomic,val)	(((atomic)->value) = (val))

	void spin_init(spinlock_T * lock);
	void spin_lock(spinlock_T * lock);
	void spin_unlock(spinlock_T * lock);

	void atomic_add(atomic_T * atomic, long value);
	void atomic_sub(atomic_T * atomic, long value);
	void atomic_inc(atomic_T * atomic);
	void atomic_dec(atomic_T * atomic);
	void atomic_set_mask(atomic_T * atomic, long mask);
	void atomic_clear_mask(atomic_T * atomic, long mask);

#endif /* _POSIX_THREAD_H_ */