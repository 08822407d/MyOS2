#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

#include <linux/kernel/sched.h>

	typedef struct 
	{
		__volatile__ long value;
	} myos_atomic_T;

	typedef struct
	{
		myos_atomic_T	lock;
	} myos_spinlock_T;

	typedef struct
	{
		myos_spinlock_T	selflock;
		task_s *	owner;
		__volatile__	unsigned	counter;
	} recurs_lock_T;

	// typedef struct
	// {
	// 	myos_atomic_T	counter;
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
	// 	myos_spinlock_T		selflock;
	// 	myos_atomic_T		counter;
	// 	task_list_s		waiting_tasks;
	// 	recurs_wait_list_s	owner_list_head;
	// } recurs_semaphore_T;
	
	void init_spin_lock(myos_spinlock_T * lock);
	void lock_spin_lock(myos_spinlock_T * lock);
	void unlock_spin_lock(myos_spinlock_T * lock);

	void init_recurs_lock(recurs_lock_T * lock);
	void lock_recurs_lock(recurs_lock_T * lock);
	void unlock_recurs_lock(recurs_lock_T * lock);

	// void init_semaphore(semaphore_T * semaphore, long count);
	// void up_semaphore(semaphore_T * semaphore);
	// void down_semaphore(semaphore_T * semaphore);

	// void init_recurs_semaphore(recurs_semaphore_T * semaphore, long max_nr);
	// void up_recurs_semaphore(recurs_semaphore_T * semaphore);
	// void down_recurs_semaphore(recurs_semaphore_T * semaphore);

	#define myos_atomic_read(atomic)	((atomic)->value)

	void myos_atomic_inc(myos_atomic_T * atomic);
	void myos_atomic_dec(myos_atomic_T * atomic);
	void myos_atomic_add(myos_atomic_T * atomic, long value);
	void myos_atomic_sub(myos_atomic_T * atomic, long value);
	void myos_atomic_set_mask(myos_atomic_T * atomic, long mask);
	void myos_atomic_clear_mask(myos_atomic_T * atomic, long mask);

#endif /* _POSIX_THREAD_H_ */