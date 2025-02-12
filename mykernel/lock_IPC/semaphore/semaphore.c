#define SEMAPHORE_DEFINATION
#include "semaphore.h"


static noinline void __down(sema_t *sem);
static noinline int __down_interruptible(sema_t *sem);
static noinline int __down_killable(sema_t *sem);
static noinline int __down_timeout(sema_t *sem, long timeout);
static noinline void __up(sema_t *sem);

/**
 * down - acquire the semaphore
 * @sem: the semaphore to be acquired
 *
 * Acquires the semaphore.  If no more tasks are allowed to acquire the
 * semaphore, calling this function will put the task to sleep until the
 * semaphore is released.
 *
 * Use of this function is deprecated, please use down_interruptible() or
 * down_killable() instead.
 */
void __sched down(sema_t *sem)
{
	ulong flags;

	// might_sleep();
	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		__down(sem);
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);
}
EXPORT_SYMBOL(down);

/**
 * down_interruptible - acquire the semaphore unless interrupted
 * @sem: the semaphore to be acquired
 *
 * Attempts to acquire the semaphore.  If no more tasks are allowed to
 * acquire the semaphore, calling this function will put the task to sleep.
 * If the sleep is interrupted by a signal, this function will return -EINTR.
 * If the semaphore is successfully acquired, this function returns 0.
 */
int __sched down_interruptible(sema_t *sem)
{
	ulong flags;
	int result = 0;

	// might_sleep();
	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_interruptible(sem);
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);

	return result;
}
EXPORT_SYMBOL(down_interruptible);

/**
 * down_killable - acquire the semaphore unless killed
 * @sem: the semaphore to be acquired
 *
 * Attempts to acquire the semaphore.  If no more tasks are allowed to
 * acquire the semaphore, calling this function will put the task to sleep.
 * If the sleep is interrupted by a fatal signal, this function will return
 * -EINTR.  If the semaphore is successfully acquired, this function returns
 * 0.
 */
int __sched down_killable(sema_t *sem)
{
	ulong flags;
	int result = 0;

	// might_sleep();
	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_killable(sem);
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);

	return result;
}
EXPORT_SYMBOL(down_killable);

/**
 * down_trylock - try to acquire the semaphore, without waiting
 * @sem: the semaphore to be acquired
 *
 * Try to acquire the semaphore atomically.  Returns 0 if the semaphore has
 * been acquired successfully or 1 if it cannot be acquired.
 *
 * NOTE: This return value is inverted from both spin_trylock and
 * mutex_trylock!  Be careful about this when converting code.
 *
 * Unlike mutex_trylock, this function can be used from interrupt context,
 * and the semaphore can be released by any task or interrupt.
 */
int __sched down_trylock(sema_t *sem)
{
	ulong flags;
	int count;

	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	count = sem->count - 1;
	if (likely(count >= 0))
		sem->count = count;
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);

	return (count < 0);
}
EXPORT_SYMBOL(down_trylock);

/**
 * down_timeout - acquire the semaphore within a specified time
 * @sem: the semaphore to be acquired
 * @timeout: how long to wait before failing
 *
 * Attempts to acquire the semaphore.  If no more tasks are allowed to
 * acquire the semaphore, calling this function will put the task to sleep.
 * If the semaphore is not released within the specified number of jiffies,
 * this function returns -ETIME.  It returns 0 if the semaphore was acquired.
 */
int __sched down_timeout(sema_t *sem, long timeout)
{
	ulong flags;
	int result = 0;

	// might_sleep();
	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_timeout(sem, timeout);
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);

	return result;
}
EXPORT_SYMBOL(down_timeout);

/**
 * up - release the semaphore
 * @sem: the semaphore to release
 *
 * Release the semaphore.  Unlike mutexes, up() may be called from any
 * context and even by tasks which have never called down().
 */
void __sched up(sema_t *sem)
{
	ulong flags;

	// raw_spin_lock_irqsave(&sem->lock, flags);
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(list_empty(&sem->wait_list)))
		sem->count++;
	else
		__up(sem);
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);
}
EXPORT_SYMBOL(up);

/* Functions for the contended case */


static noinline void __sched
__down(sema_t *sem) {
	__down_common(sem, TASK_UNINTERRUPTIBLE,
		MAX_SCHEDULE_TIMEOUT);
}

static noinline int __sched
__down_interruptible(sema_t *sem) {
	return __down_common(sem, TASK_INTERRUPTIBLE,
			MAX_SCHEDULE_TIMEOUT);
}

static noinline int __sched
__down_killable(sema_t *sem) {
	return __down_common(sem, TASK_KILLABLE,
			MAX_SCHEDULE_TIMEOUT);
}

static noinline int __sched
__down_timeout(sema_t *sem, long timeout) {
	return __down_common(sem,
			TASK_UNINTERRUPTIBLE, timeout);
}

static noinline void __sched
__up(sema_t *sem) {
	struct semaphore_waiter *waiter =
			list_first_entry(&sem->wait_list,
				struct semaphore_waiter, list);
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
}
