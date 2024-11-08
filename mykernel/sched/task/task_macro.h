/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_MACRO_H_
#define _LINUX_TASK_MACRO_H_

	#define set_tsk_need_resched(tsk)	\
				set_tsk_thread_flag(tsk, TIF_NEED_RESCHED)

	#define clear_tsk_need_resched(tsk)	\
				clear_tsk_thread_flag(tsk, TIF_NEED_RESCHED)

	#define test_tsk_need_resched(tsk)	\
				test_tsk_thread_flag(tsk, TIF_NEED_RESCHED)

	#define need_resched()	\
				test_tsk_need_resched(current)

	// #define tasklist_empty() \
	// 		list_empty(&init_task.tasks)

	// #define next_task(p) \
	// 		list_entry_rcu((p)->tasks.next, task_s, tasks)

	// #define for_each_process(p) \
	// 		for (p = (&init_task) ; p != &init_task ; p = next_task(p))

#endif /* _LINUX_TASK_API_H_ */