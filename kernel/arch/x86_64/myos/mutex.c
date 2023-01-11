#include <linux/kernel/slab.h>
#include <linux/kernel/stddef.h>
#include <linux/lib/string.h>
#include <linux/sched/myos_percpu.h>

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
// 		m_enqueue_list(current, &semaphore->waiting_tasks);
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
// 	spin_lock(&semaphore->lock);
// 	m_init_list_header(&semaphore->owner_list_head);
// 	m_init_list_header(&semaphore->waiting_tasks);
// }

// void down_recurs_semaphore(recurs_semaphore_T * semaphore)
// {	
// 	task_s * curr = current;
// 	cpudata_s * cpudata_p = curr_cpu;
// 	lock_spin_lock(&semaphore->selflock);
// 	recurs_wait_s * user = find_recurs_waiting_task(curr, &semaphore->owner_list_head);
// 	if (user != NULL)
// 	{
// 		user->counter++;
		// spin_unlock_no_resched(&lock->selflock);
// 	}
// 	else if (semaphore->counter.value > 0)
// 	{
// 		recurs_wait_s * new_user = kmalloc(sizeof(recurs_wait_s));
// 		memset(new_user, 0, sizeof(recurs_wait_s));
// 		new_user->owner = curr;
// 		new_user->counter = 1;
// 		m_enqueue_list(new_user, &semaphore->owner_list_head);
		// spin_unlock_no_resched(&lock->selflock);
// 	}
// 	else
// 	{
// 		m_enqueue_list(curr, &semaphore->waiting_tasks);
// 		cpudata_p->curr_task = NULL;
		// spin_unlock_no_resched(&lock->selflock);
// 		schedule();
// 	}
// }

// void up_recurs_semaphore(recurs_semaphore_T * semaphore)
// {
// 	task_s * curr = current;
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
		// spin_unlock_no_resched(&lock->selflock);
// 	}
// 	else if (semaphore->waiting_tasks.count == 0)
// 	{
// 		m_list_delete(user);
// 		semaphore->owner_list_head.count--;
// 		semaphore->counter.value++;
// 		kfree(user);
		// spin_unlock_no_resched(&lock->selflock);
// 	}
// 	else
// 	{
// 		m_dequeue_list(user->owner, &semaphore->waiting_tasks);
// 		user->counter = 1;
		// spin_unlock_no_resched(&lock->selflock);
// 		myos_wake_up_new_task(user->owner);
// 	}
// }

