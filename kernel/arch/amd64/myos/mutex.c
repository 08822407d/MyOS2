#include <linux/kernel/slab.h>
#include <linux/kernel/stddef.h>
#include <linux/lib/string.h>

#include <obsolete/mutex.h>

/*==============================================================================================*
 *											spin lock											*
 *==============================================================================================*/
inline __always_inline void init_spin_lock(myos_spinlock_T * lock)
{
	lock->lock.value = 1;
}

inline __always_inline void lock_spin_lock(myos_spinlock_T * lock)
{
	asm volatile(	"1:						\n\t"
					"lock	decq	%0		\n\t"
					"jns	3f				\n\t"
					"2:						\n\t"
					"pause					\n\t"
					"cmpq	$0,		%0		\n\t"
					"jle	2b				\n\t"
					"jmp	1b				\n\t"
					"3:						\n\t"
					// "cli					\n\t"
				:	"=m"(lock->lock.value)
				:
				:	"memory"
				);
	curr_tsk->spin_count++;
}

inline __always_inline void unlock_spin_lock(myos_spinlock_T * lock)
{
	asm volatile(	"movq	$1,		%0		\n\t"
					// "sti					\n\t"
				:	"=m"(lock->lock.value)
				:
				:	"memory"
				);
	curr_tsk->spin_count--;
}

/*==============================================================================================*
 *										recursive spin lock										*
 *==============================================================================================*/
inline __always_inline void init_recurs_lock(recurs_lock_T * lock)
{
	lock->counter = 0;
	lock->owner = NULL;
	init_spin_lock(&lock->selflock);
}

void lock_recurs_lock(recurs_lock_T * lock)
{
	task_s * owner = NULL;
	do
	{
		lock_spin_lock(&lock->selflock);
		owner = lock->owner;
		if (lock->owner == NULL)
			lock->owner = curr_tsk;
		unlock_spin_lock(&lock->selflock);
	} while(owner != curr_tsk);
	lock->counter++;
}

void unlock_recurs_lock(recurs_lock_T * lock)
{
	while (lock->owner != curr_tsk);
	lock->counter--;
	if (lock->counter == 0)
		lock->owner = NULL;
}

/*==============================================================================================*
 *											semaphore											*
 *==============================================================================================*/
// inline __always_inline void init_semaphore(semaphore_T * semaphore, long count)
// {
// 	semaphore->counter.value = count;
// 	m_init_list_header(&semaphore->waiting_tasks);
// }

// void down_semaphore(semaphore_T * semaphore)
// {
// 	per_cpudata_s * cpudata_p = curr_cpu;
// 	if (atomic_read(&semaphore->counter) > 0)
// 	{
// 		atomic_dec(&semaphore->counter);
// 		return;
// 	}
// 	else
// 	{
// 		curr_cpu->curr_task == NULL;
// 		m_enqueue_list(curr_tsk, &semaphore->waiting_tasks);
// 		schedule();
// 	}
// }

// void up_semaphore(semaphore_T * semaphore)
// {
// 	task_s * next = NULL;
// 	per_cpudata_s * cpudata_p = curr_cpu;
// 	if (semaphore->waiting_tasks.count == 0)
// 	{
// 		atomic_inc(&semaphore->counter);
// 		return;
// 	}
// 	else
// 	{
// 		m_dequeue_list(next, &semaphore->waiting_tasks);
// 		m_push_list(next, &cpudata_p->ready_tasks);
// 		schedule();
// 	}
// }
/*==============================================================================================*
 *										recursive semaphore										*
 *==============================================================================================*/
// recurs_wait_s * find_recurs_waiting_task(task_s * tsk, recurs_wait_list_s * list)
// {
// 	recurs_wait_s * ret_val = NULL;
// 	recurs_wait_s * tmp = list->head_p;
// 	unsigned count = list->count;
// 	while (count)
// 	{
// 		if (tmp->owner = tsk)
// 		{
// 			ret_val = tmp;
// 			break;
// 		}
// 		tmp = tmp->next;
// 		count--;
// 	}
// 	return ret_val;
// }

// inline __always_inline void init_recurs_semaphore(recurs_semaphore_T * semaphore, long max_nr)
// {
// 	semaphore->counter.value = max_nr;
// 	init_spin_lock(&semaphore->selflock);
// 	m_init_list_header(&semaphore->owner_list_head);
// 	m_init_list_header(&semaphore->waiting_tasks);
// }

// void down_recurs_semaphore(recurs_semaphore_T * semaphore)
// {	
// 	task_s * curr = curr_tsk;
// 	cpudata_s * cpudata_p = curr_cpu;
// 	lock_spin_lock(&semaphore->selflock);
// 	recurs_wait_s * user = find_recurs_waiting_task(curr, &semaphore->owner_list_head);
// 	if (user != NULL)
// 	{
// 		user->counter++;
// 		unlock_spin_lock(&semaphore->selflock);
// 	}
// 	else if (semaphore->counter.value > 0)
// 	{
// 		recurs_wait_s * new_user = kmalloc(sizeof(recurs_wait_s));
// 		memset(new_user, 0, sizeof(recurs_wait_s));
// 		new_user->owner = curr;
// 		new_user->counter = 1;
// 		m_enqueue_list(new_user, &semaphore->owner_list_head);
// 		unlock_spin_lock(&semaphore->selflock);
// 	}
// 	else
// 	{
// 		m_enqueue_list(curr, &semaphore->waiting_tasks);
// 		cpudata_p->curr_task = NULL;
// 		unlock_spin_lock(&semaphore->selflock);
// 		schedule();
// 	}
// }

// void up_recurs_semaphore(recurs_semaphore_T * semaphore)
// {
// 	task_s * curr = curr_tsk;
// 	cpudata_s * cpudata_p = curr_cpu;
// 	lock_spin_lock(&semaphore->selflock);
// 	recurs_wait_s * user = find_recurs_waiting_task(curr, &semaphore->owner_list_head);
// 	if (user == NULL)
// 	{
// 		// there must be an error
// 		while (1);
// 	}
// 	if (user->counter != 0)
// 	{
// 		user->counter--;
// 		unlock_spin_lock(&semaphore->selflock);
// 	}
// 	else if (semaphore->waiting_tasks.count == 0)
// 	{
// 		m_list_delete(user);
// 		semaphore->owner_list_head.count--;
// 		semaphore->counter.value++;
// 		kfree(user);
// 		unlock_spin_lock(&semaphore->selflock);
// 	}
// 	else
// 	{
// 		m_dequeue_list(user->owner, &semaphore->waiting_tasks);
// 		user->counter = 1;
// 		unlock_spin_lock(&semaphore->selflock);
// 		wake_up_process(user->owner);
// 	}
// }

/*==============================================================================================*
 *										atomic operations										*
 *==============================================================================================*/
inline __always_inline void myos_atomic_add(myos_atomic_T * atomic, long value)
{
	asm volatile(	"lock	addq	%1,	%0	\n\t"
						:	"=m"(atomic->value)
						:	"r"(value)
						:	"memory"
						);
}

inline __always_inline void myos_atomic_sub(myos_atomic_T * atomic, long value)
{
	asm volatile(	"lock	subq	%1,	%0	\n\t"
				:	"=m"(atomic->value)
				:	"r"(value)
				:	"memory"
				);
}

inline __always_inline void myos_atomic_inc(myos_atomic_T * atomic)
{
	asm volatile(	"lock	incq	%0		\n\t"
				:	"=m"(atomic->value)
				:	"m"(atomic->value)
				:	"memory"
				);
}

inline __always_inline void myos_atomic_dec(myos_atomic_T * atomic)
{
	asm volatile(	"lock	decq	%0		\n\t"
				:	"=m"(atomic->value)
				:	"m"(atomic->value)
				:	"memory"
				);
}

inline __always_inline void myos_atomic_set_mask(myos_atomic_T * atomic, long mask)
{
	asm volatile(	"lock	orq		%1,	%0	\n\t"
				:	"=m"(atomic->value)
				:	"r"(mask)
				:	"memory"
				);
}

inline __always_inline void myos_atomic_clear_mask(myos_atomic_T * atomic, long mask)
{
	asm volatile(	"lock	andq	%1,	%0	\n\t"
				:	"=m"(atomic->value)
				:	"r"(~(mask))
				:	"memory"
				);
}