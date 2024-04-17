/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_H_
#define _LINUX_TASK_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG

		extern void
		set_tsk_thread_flag(task_s *tsk, int flag);
		extern void
		clear_tsk_thread_flag(task_s *tsk, int flag);
		extern int
		test_tsk_thread_flag(task_s *tsk, int flag);

		extern void
		task_lock(task_s *p);
		extern void
		task_unlock(task_s *p);

	#endif

	#include "task_macro.h"
	
	#if defined(TASK_DEFINATION) || !(DEBUG)

		/*
		 * Set thread flags in other task's structures.
		 * See asm/thread_info.h for TIF_xxxx flags available:
		 */
		PREFIX_STATIC_INLINE
		void
		set_tsk_thread_flag(task_s *tsk, int flag) {
			// while (IS_INVAL_PTR(tsk));
			set_bit(flag, &(tsk->thread_info.flags));
		}
		PREFIX_STATIC_INLINE
		void
		clear_tsk_thread_flag(task_s *tsk, int flag) {
			// while (IS_INVAL_PTR(tsk));
			clear_bit(flag, &(tsk->thread_info.flags));
		}
		PREFIX_STATIC_INLINE
		int
		test_tsk_thread_flag(task_s *tsk, int flag) {
			// while (IS_INVAL_PTR(tsk));
			return arch_test_bit(flag, &(tsk->thread_info.flags));
		}


		/*
		 * Protects ->fs, ->files, ->mm, ->group_info, ->comm, keyring
		 * subscriptions and synchronises with wait4().  Also used in procfs.  Also
		 * pins the final release of task.io_context.  Also protects ->cpuset and
		 * ->cgroup.subsys[]. And ->vfork_done. And ->sysvshm.shm_clist.
		 *
		 * Nests both inside and outside of read_lock(&tasklist_lock).
		 * It must not be nested with write_lock_irq(&tasklist_lock),
		 * neither inside nor outside.
		 */
		PREFIX_STATIC_INLINE
		void
		task_lock(task_s *p) {
			spin_lock(&p->alloc_lock);
		}
		PREFIX_STATIC_INLINE
		void
		task_unlock(task_s *p) {
			spin_unlock(&p->alloc_lock);
		}

	#endif

#endif /* _LINUX_TASK_H_ */