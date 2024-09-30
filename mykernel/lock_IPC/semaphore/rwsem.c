#define READWRITE_SEMAPHORE_DEFINATION
#include "rwsem.h"


#include <linux/sched/debug.h>

/*
 * Guide to the rw_semaphore's count field.
 *
 * When the RWSEM_WRITER_LOCKED bit in count is set, the lock is owned
 * by a writer.
 *
 * The lock is owned by readers when
 * (1) the RWSEM_WRITER_LOCKED isn't set in count,
 * (2) some of the reader bits are set in count, and
 * (3) the owner field has RWSEM_READ_OWNED bit set.
 *
 * Having some reader bits set is not enough to guarantee a readers owned
 * lock as the readers may be in the process of backing out from the count
 * and a writer has just released the lock. So another writer may steal
 * the lock immediately after that.
 */

/*
 * Initialize an rwsem:
 */
// void __init_rwsem(rwsem_t *sem,
// 		const char *name, struct lock_class_key *key)
void __init_rwsem(rwsem_t *sem, const char *name)
{
	atomic_long_set(&sem->count, RWSEM_UNLOCKED_VALUE);
	raw_spin_lock_init(&sem->wait_lock);
	INIT_LIST_HEAD(&sem->wait_list);
	atomic_long_set(&sem->owner, 0L);
}
EXPORT_SYMBOL(__init_rwsem);



/*
 * lock for reading
 */
static __always_inline int
__down_read_common(struct rw_semaphore *sem, int state) {
// 	int ret = 0;
// 	long count;

// 	preempt_disable();
// 	if (!rwsem_read_trylock(sem, &count)) {
// 		if (IS_ERR(rwsem_down_read_slowpath(sem, count, state))) {
// 			ret = -EINTR;
// 			goto out;
// 		}
// 		DEBUG_RWSEMS_WARN_ON(!is_rwsem_reader_owned(sem), sem);
// 	}
// out:
// 	preempt_enable();
// 	return ret;
}

static __always_inline void
__down_read(struct rw_semaphore *sem) {
	__down_read_common(sem, TASK_UNINTERRUPTIBLE);
}

static __always_inline int
__down_read_interruptible(struct rw_semaphore *sem) {
	return __down_read_common(sem, TASK_INTERRUPTIBLE);
}

static __always_inline int
__down_read_killable(struct rw_semaphore *sem) {
	return __down_read_common(sem, TASK_KILLABLE);
}

static inline int
__down_read_trylock(struct rw_semaphore *sem) {
	// int ret = 0;
	// long tmp;

	// DEBUG_RWSEMS_WARN_ON(sem->magic != sem, sem);

	// preempt_disable();
	// tmp = atomic_long_read(&sem->count);
	// while (!(tmp & RWSEM_READ_FAILED_MASK)) {
	// 	if (atomic_long_try_cmpxchg_acquire(&sem->count, &tmp,
	// 					    tmp + RWSEM_READER_BIAS)) {
	// 		rwsem_set_reader_owned(sem);
	// 		ret = 1;
	// 		break;
	// 	}
	// }
	// preempt_enable();

	// return ret;
}

/*
 * lock for writing
 */
static __always_inline int
__down_write_common(struct rw_semaphore *sem, int state) {
	// int ret = 0;

	// preempt_disable();
	// if (unlikely(!rwsem_write_trylock(sem))) {
	// 	if (IS_ERR(rwsem_down_write_slowpath(sem, state)))
	// 		ret = -EINTR;
	// }
	// preempt_enable();

	// return ret;
}

static __always_inline void
__down_write(struct rw_semaphore *sem) {
	__down_write_common(sem, TASK_UNINTERRUPTIBLE);
}

static __always_inline int
__down_write_killable(struct rw_semaphore *sem) {
	return __down_write_common(sem, TASK_KILLABLE);
}

static inline int
__down_write_trylock(struct rw_semaphore *sem) {
	// int ret;

	// preempt_disable();
	// DEBUG_RWSEMS_WARN_ON(sem->magic != sem, sem);
	// ret = rwsem_write_trylock(sem);
	// preempt_enable();

	// return ret;
}

/*
 * unlock after reading
 */
static inline void
__up_read(struct rw_semaphore *sem) {
	// long tmp;

	// DEBUG_RWSEMS_WARN_ON(sem->magic != sem, sem);
	// DEBUG_RWSEMS_WARN_ON(!is_rwsem_reader_owned(sem), sem);

	// preempt_disable();
	// rwsem_clear_reader_owned(sem);
	// tmp = atomic_long_add_return_release(-RWSEM_READER_BIAS, &sem->count);
	// DEBUG_RWSEMS_WARN_ON(tmp < 0, sem);
	// if (unlikely((tmp & (RWSEM_LOCK_MASK|RWSEM_FLAG_WAITERS)) ==
	// 	      RWSEM_FLAG_WAITERS)) {
	// 	clear_nonspinnable(sem);
	// 	rwsem_wake(sem);
	// }
	// preempt_enable();
}

/*
 * unlock after writing
 */
static inline void
__up_write(struct rw_semaphore *sem) {
	// long tmp;

	// DEBUG_RWSEMS_WARN_ON(sem->magic != sem, sem);
	// /*
	//  * sem->owner may differ from current if the ownership is transferred
	//  * to an anonymous writer by setting the RWSEM_NONSPINNABLE bits.
	//  */
	// DEBUG_RWSEMS_WARN_ON((rwsem_owner(sem) != current) &&
	// 		    !rwsem_test_oflags(sem, RWSEM_NONSPINNABLE), sem);

	// preempt_disable();
	// rwsem_clear_owner(sem);
	// tmp = atomic_long_fetch_add_release(-RWSEM_WRITER_LOCKED, &sem->count);
	// if (unlikely(tmp & RWSEM_FLAG_WAITERS))
	// 	rwsem_wake(sem);
	// preempt_enable();
}

/*
 * downgrade write lock to read lock
 */
static inline void
__downgrade_write(struct rw_semaphore *sem) {
	// long tmp;

	// /*
	//  * When downgrading from exclusive to shared ownership,
	//  * anything inside the write-locked region cannot leak
	//  * into the read side. In contrast, anything in the
	//  * read-locked region is ok to be re-ordered into the
	//  * write side. As such, rely on RELEASE semantics.
	//  */
	// DEBUG_RWSEMS_WARN_ON(rwsem_owner(sem) != current, sem);

	// preempt_disable();
	// tmp = atomic_long_fetch_add_release(
	// 	-RWSEM_WRITER_LOCKED+RWSEM_READER_BIAS, &sem->count);
	// rwsem_set_reader_owned(sem);
	// if (tmp & RWSEM_FLAG_WAITERS)
	// 	rwsem_downgrade_wake(sem);
	// preempt_enable();
}

/*
 * lock for reading
 */
void __sched down_read(rwsem_t *sem)
{
	// might_sleep();
	// rwsem_acquire_read(&sem->dep_map, 0, 0, _RET_IP_);
	LOCK_CONTENDED(sem, __down_read_trylock, __down_read);
}
EXPORT_SYMBOL(down_read);

int __sched down_read_interruptible(rwsem_t *sem)
{
	// might_sleep();
	// rwsem_acquire_read(&sem->dep_map, 0, 0, _RET_IP_);
	if (LOCK_CONTENDED_RETURN(sem, __down_read_trylock,
			__down_read_interruptible)) {
		// rwsem_release(&sem->dep_map, _RET_IP_);
		return -EINTR;
	}
	return 0;
}
EXPORT_SYMBOL(down_read_interruptible);

int __sched down_read_killable(rwsem_t *sem)
{
	// might_sleep();
	// rwsem_acquire_read(&sem->dep_map, 0, 0, _RET_IP_);
	if (LOCK_CONTENDED_RETURN(sem, __down_read_trylock,
			__down_read_killable)) {
		// rwsem_release(&sem->dep_map, _RET_IP_);
		return -EINTR;
	}
	return 0;
}
EXPORT_SYMBOL(down_read_killable);

/*
 * trylock for reading -- returns 1 if successful, 0 if contention
 */
int down_read_trylock(rwsem_t *sem)
{
	int ret = __down_read_trylock(sem);
	// if (ret == 1)
	// 	rwsem_acquire_read(&sem->dep_map, 0, 1, _RET_IP_);
	return ret;
}
EXPORT_SYMBOL(down_read_trylock);

/*
 * lock for writing
 */
void __sched down_write(rwsem_t *sem)
{
	// might_sleep();
	// rwsem_acquire(&sem->dep_map, 0, 0, _RET_IP_);
	LOCK_CONTENDED(sem, __down_write_trylock, __down_write);
}
EXPORT_SYMBOL(down_write);

/*
 * lock for writing
 */
int __sched down_write_killable(rwsem_t *sem)
{
	// might_sleep();
	// rwsem_acquire(&sem->dep_map, 0, 0, _RET_IP_);
	if (LOCK_CONTENDED_RETURN(sem, __down_write_trylock,
				__down_write_killable)) {
		// rwsem_release(&sem->dep_map, _RET_IP_);
		return -EINTR;
	}
	return 0;
}
EXPORT_SYMBOL(down_write_killable);

/*
 * trylock for writing -- returns 1 if successful, 0 if contention
 */
int down_write_trylock(rwsem_t *sem)
{
	int ret = __down_write_trylock(sem);
	// if (ret == 1)
	// 	rwsem_acquire(&sem->dep_map, 0, 1, _RET_IP_);
	return ret;
}
EXPORT_SYMBOL(down_write_trylock);

/*
 * release a read lock
 */
void up_read(rwsem_t *sem)
{
	// rwsem_release(&sem->dep_map, _RET_IP_);
	__up_read(sem);
}
EXPORT_SYMBOL(up_read);

/*
 * release a write lock
 */
void up_write(rwsem_t *sem)
{
	// rwsem_release(&sem->dep_map, _RET_IP_);
	__up_write(sem);
}
EXPORT_SYMBOL(up_write);

/*
 * downgrade write lock to read lock
 */
void downgrade_write(rwsem_t *sem)
{
	// lock_downgrade(&sem->dep_map, _RET_IP_);
	__downgrade_write(sem);
}
EXPORT_SYMBOL(downgrade_write);