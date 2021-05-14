#ifndef _POSIX_THREAD_H_
#define _POSIX_THREAD_H_

	typedef struct
	{
		__volatile__ unsigned long lock;		//1:unlock,0:lock
	} spinlock_T;

	void spin_init(spinlock_T * lock);
	void spin_lock(spinlock_T * lock);
	void spin_unlock(spinlock_T * lock);

#endif /* _POSIX_THREAD_H_ */