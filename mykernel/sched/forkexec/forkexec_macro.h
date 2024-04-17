/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FORK_EXEC_MACRO_H_
#define _LINUX_FORK_EXEC_MACRO_H_

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

#endif /* _LINUX_FORK_EXEC_MACRO_H_ */