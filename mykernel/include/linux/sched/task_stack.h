/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_TASK_STACK_H
#define _LINUX_SCHED_TASK_STACK_H

	/*
	 * task->stack (kernel stack) handling interfaces:
	 */

	#include <linux/kernel/sched.h>
	#include <uapi/linux/magic.h>

	// #ifdef CONFIG_THREAD_INFO_IN_TASK

	/*
	 * When accessing the stack of a non-current task that might exit, use
	 * try_get_task_stack() instead.  task_stack_page will return a pointer
	 * that could get freed out from under you.
	 */
	static inline void
	*task_stack_page(const task_s *task) {
		return task->stack;
	}

	// #define setup_thread_stack(new,old)	do { } while(0)

	// #elif !defined(__HAVE_THREAD_FUNCTIONS)

	// #define task_stack_page(task)	((void *)(task)->stack)

	// static inline void setup_thread_stack(task_s *p, task_s *org)
	// {
	// 	*task_thread_info(p) = *task_thread_info(org);
	// 	task_thread_info(p)->task = p;
	// }

	// /*
	// * Return the address of the last usable long on the stack.
	// *
	// * When the stack grows down, this is just above the thread
	// * info struct. Going any lower will corrupt the threadinfo.
	// *
	// * When the stack grows up, this is the highest address.
	// * Beyond that position, we corrupt data on the next page.
	// */
	// static inline unsigned long *end_of_stack(task_s *p)
	// {
	// 	return (unsigned long *)(task_thread_info(p) + 1);
	// }

	// #endif

	// #ifdef CONFIG_THREAD_INFO_IN_TASK
	// static inline void *try_get_task_stack(task_s *tsk)
	// {
	// 	return refcount_inc_not_zero(&tsk->stack_refcount) ?
	// 		task_stack_page(tsk) : NULL;
	// }

	// extern void put_task_stack(task_s *tsk);
	// #else
	// static inline void *try_get_task_stack(task_s *tsk)
	// {
	// 	return task_stack_page(tsk);
	// }

	// static inline void put_task_stack(task_s *tsk) {}
	// #endif

	// #define task_stack_end_corrupted(task) \
	// 		(*(end_of_stack(task)) != STACK_END_MAGIC)

	// static inline int object_is_on_stack(const void *obj)
	// {
	// 	void *stack = task_stack_page(current);

	// 	return (obj >= stack) && (obj < (stack + THREAD_SIZE));
	// }

	// extern void thread_stack_cache_init(void);

	// #ifdef CONFIG_DEBUG_STACK_USAGE
	// static inline unsigned long stack_not_used(task_s *p)
	// {
	// 	unsigned long *n = end_of_stack(p);

	// 	do { 	/* Skip over canary */
	// 		n++;
	// 	} while (!*n);

	// 	return (unsigned long)n - (unsigned long)end_of_stack(p);
	// }
	// #endif
	// extern void set_task_stack_end_magic(task_s *tsk);

	// #ifndef __HAVE_ARCH_KSTACK_END
	// static inline int kstack_end(void *addr)
	// {
	// 	/* Reliable end of stack detection:
	// 	* Some APM bios versions misalign the stack
	// 	*/
	// 	return !(((unsigned long)addr+sizeof(void*)-1) & (THREAD_SIZE-sizeof(void*)));
	// }
	// #endif

#endif /* _LINUX_SCHED_TASK_STACK_H */
