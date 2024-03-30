/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FORK_EXEC_API_H_
#define _LINUX_FORK_EXEC_API_H_

	#include <linux/kernel/sched.h>


	__printf(4, 5)
	task_s *kthread_create_on_node(int (*threadfn)(void *data),
			void *data, int node, const char namefmt[], ...);

	/**
	 * kthread_create - create a kthread on the current node
	 * @threadfn: the function to run in the thread
	 * @data: data pointer for @threadfn()
	 * @namefmt: printf-style format string for the thread name
	 * @arg: arguments for @namefmt.
	 *
	 * This macro will create a kthread on the current node, leaving it in
	 * the stopped state.  This is just a helper for kthread_create_on_node();
	 * see the documentation there for more details.
	 */
	#define kthread_create(threadfn, data, namefmt, arg...)	\
				kthread_create_on_node(threadfn, data,		\
					NUMA_NO_NODE, namefmt, ##arg)

	// task_s *kthread_create_on_cpu(int (*threadfn)(void *data),
	// 					void *data,
	// 					unsigned int cpu,
	// 					const char *namefmt);

	bool set_kthread_struct(task_s *p);

	/**
	 * kthread_run - create and wake a thread.
	 * @threadfn: the function to run until signal_pending(current).
	 * @data: data ptr for @threadfn.
	 * @namefmt: printf-style name for the thread.
	 *
	 * Description: Convenient wrapper for kthread_create() followed by
	 * myos_wake_up_new_task().  Returns the kthread or ERR_PTR(-ENOMEM).
	 */
	#define kthread_run(threadfn, data, namefmt, ...)	({	\
				task_s *__k									\
					= kthread_create(threadfn, data,		\
						namefmt, ## __VA_ARGS__);			\
				if (!IS_ERR(__k)) wake_up_process(__k);		\
				__k;										\
			})

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
	void kthread_complete_and_exit(completion_s *, long) __noreturn;
	int kthreadd(void *unused);
	extern task_s *kthreadd_task;

	extern void proc_caches_init(void);

#endif /* _LINUX_FORK_EXEC_API_H_ */