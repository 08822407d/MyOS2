#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

#include <linux/kernel/sched.h>

	typedef struct 
	{
		__volatile__ long value;
	} atomic_T;

	typedef struct
	{
		atomic_T	lock;
	} spinlock_T;

	typedef struct
	{
		spinlock_T	selflock;
		task_s *	owner;
		__volatile__	unsigned	counter;
	} recurs_lock_T;

	// typedef struct
	// {
	// 	atomic_T	counter;
	// 	task_list_s	waiting_tasks;
	// } semaphore_T;

	// m_define_list_header(recurs_wait)
	// typedef struct recurs_wait
	// {
	// 	recurs_wait_s *		prev;
	// 	recurs_wait_s *		next;

	// 	task_s *	owner;
	// 	__volatile__	unsigned	counter;
	// } recurs_wait_s;
	
	// typedef struct
	// {
	// 	spinlock_T		selflock;
	// 	atomic_T		counter;
	// 	task_list_s		waiting_tasks;
	// 	recurs_wait_list_s	owner_list_head;
	// } recurs_semaphore_T;
	
	void init_spin_lock(spinlock_T * lock);
	void lock_spin_lock(spinlock_T * lock);
	void unlock_spin_lock(spinlock_T * lock);

	void init_recurs_lock(recurs_lock_T * lock);
	void lock_recurs_lock(recurs_lock_T * lock);
	void unlock_recurs_lock(recurs_lock_T * lock);

	// void init_semaphore(semaphore_T * semaphore, long count);
	// void up_semaphore(semaphore_T * semaphore);
	// void down_semaphore(semaphore_T * semaphore);

	// void init_recurs_semaphore(recurs_semaphore_T * semaphore, long max_nr);
	// void up_recurs_semaphore(recurs_semaphore_T * semaphore);
	// void down_recurs_semaphore(recurs_semaphore_T * semaphore);

	#define atomic_read(atomic)	((atomic)->value)
	#define atomic_set(atomic,val)	(((atomic)->value) = (val))

	void atomic_inc(atomic_T * atomic);
	void atomic_dec(atomic_T * atomic);
	void atomic_add(atomic_T * atomic, long value);
	void atomic_sub(atomic_T * atomic, long value);
	void atomic_set_mask(atomic_T * atomic, long mask);
	void atomic_clear_mask(atomic_T * atomic, long mask);

#endif /* _POSIX_THREAD_H_ */