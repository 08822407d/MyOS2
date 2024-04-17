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

#endif /* _LINUX_TASK_API_H_ */