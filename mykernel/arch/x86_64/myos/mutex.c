#include <linux/kernel/slab.h>
#include <linux/kernel/stddef.h>
#include <linux/lib/string.h>

#include <obsolete/mutex.h>

/*==============================================================================================*
 *										recursive spin lock										*
 *==============================================================================================*/
inline __always_inline void init_recurs_lock(recurs_lock_T * lock)
{
	lock->counter = 0;
	lock->owner = NULL;
	spin_lock_init(&lock->selflock);
}

void lock_recurs_lock(recurs_lock_T * lock)
{
	task_s * owner = NULL;
	do
	{
		spin_lock(&lock->selflock);
		owner = lock->owner;
		if (lock->owner == NULL)
			lock->owner = current;
		spin_unlock_no_resched(&lock->selflock);
	} while(owner != current);
	lock->counter++;
}

void unlock_recurs_lock(recurs_lock_T * lock)
{
	while (lock->owner != current);
	lock->counter--;
	if (lock->counter == 0)
		lock->owner = NULL;
}