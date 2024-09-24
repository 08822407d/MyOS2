/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FORK_EXEC_API_H_
#define _LINUX_FORK_EXEC_API_H_

	#include "forkexec.h"


	__printf(4, 5)
	task_s *kthread_create_on_node(int (*threadfn)(void *data),
			void *data, int node, const char namefmt[], ...);

	// task_s *kthread_create_on_cpu(int (*threadfn)(void *data),
	// 					void *data,
	// 					unsigned int cpu,
	// 					const char *namefmt);

	bool set_kthread_struct(task_s *p);

	/**
	 * kthread_run_on_cpu - create and wake a cpu bound thread.
	 * @threadfn: the function to run until signal_pending(current).
	 * @data: data ptr for @threadfn.
	 * @cpu: The cpu on which the thread should be bound,
	 * @namefmt: printf-style name for the thread. Format is restricted
	 *	     to "name.*%u". Code fills in cpu number.
	 *
	 * Description: Convenient wrapper for kthread_create_on_cpu()
	 * followed by myos_wake_up_new_task().  Returns the kthread or
	 * ERR_PTR(-ENOMEM).
	 */
	// static inline task_s *
	// kthread_run_on_cpu(int (*threadfn)(void *data), void *data,
	// 			unsigned int cpu, const char *namefmt)
	// {
	// 	task_s *p;

	// 	p = kthread_create_on_cpu(threadfn, data, cpu, namefmt);
	// 	if (!IS_ERR(p))
	// 		myos_wake_up_new_task(p);

	// 	return p;
	// }

	void kthread_exit(long result) __noreturn;
	#include <linux/kernel/completion.h>
	void kthread_complete_and_exit(completion_s *, long) __noreturn;
	int kthreadd(void *unused);
	extern task_s *kthreadd_task;

	extern void mm_cache_init(void);
	extern void proc_caches_init(void);

	extern void exit_mm_release(task_s *tsk, mm_s *mm);
	extern void exec_mm_release(task_s *tsk, mm_s *mm);

#endif /* _LINUX_FORK_EXEC_API_H_ */