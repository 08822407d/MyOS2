#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

#include "../../../include/task.h"

	typedef struct 
	{
		__volatile__ long value;
	} atomic_T;

	typedef struct
	{
		__volatile__ unsigned long lock;
	} spinlock_T;

	typedef struct
	{
		task_s *	owner;
		spinlock_T	selflock;
		atomic_T	counter;
	} recursive_lock_T;

	typedef struct
	{
		atomic_T	counter;
		task_list_s	waiting_tasks;
	} semaphore_T;
	
	typedef struct
	{
		task_s *	owner;
		atomic_T	counter;
		task_list_s	waiting_tasks;
	} recursive_semaphore_T;
	

	#define atomic_read(atomic)	((atomic)->value)
	#define atomic_set(atomic,val)	(((atomic)->value) = (val))

	void init_spinlock(spinlock_T * lock);
	void lock_spinlock(spinlock_T * lock);
	void unlock_spinlock(spinlock_T * lock);

	void init_recursivelock(recursive_lock_T * lock);
	void lock_recursivelock(recursive_lock_T * lock);
	void unlock_recursivelock(recursive_lock_T * lock);

	void atomic_add(atomic_T * atomic, long value);
	void atomic_sub(atomic_T * atomic, long value);
	void atomic_inc(atomic_T * atomic);
	void atomic_dec(atomic_T * atomic);
	void atomic_set_mask(atomic_T * atomic, long mask);
	void atomic_clear_mask(atomic_T * atomic, long mask);

#endif /* _POSIX_THREAD_H_ */