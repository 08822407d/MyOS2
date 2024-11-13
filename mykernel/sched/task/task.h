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

		extern ulong
		task_rlimit(const task_s *task, uint limit);
		extern ulong
		task_rlimit_max(const task_s *task, uint limit);
		extern ulong
		rlimit(uint limit);
		extern ulong
		rlimit_max(uint limit);


		extern void
		set_ti_thread_flag(thread_info_s *ti, int flag);
		extern void
		clear_ti_thread_flag(thread_info_s *ti, int flag);
		extern void
		update_ti_thread_flag(thread_info_s *ti, int flag, bool value);
		extern int
		test_and_set_ti_thread_flag(thread_info_s *ti, int flag);
		extern int
		test_and_clear_ti_thread_flag(thread_info_s *ti, int flag);
		extern int
		test_ti_thread_flag(thread_info_s *ti, int flag);
		extern ulong
		read_ti_thread_flags(thread_info_s *ti);

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


		PREFIX_STATIC_INLINE
		ulong
		task_rlimit(const task_s *task, uint limit) {
			return READ_ONCE(task->signal->rlim[limit].rlim_cur);
		}

		PREFIX_STATIC_INLINE
		ulong
		task_rlimit_max(const task_s *task, uint limit) {
			return READ_ONCE(task->signal->rlim[limit].rlim_max);
		}

		PREFIX_STATIC_INLINE
		ulong
		rlimit(uint limit) {
			return task_rlimit(current, limit);
		}

		PREFIX_STATIC_INLINE
		ulong
		rlimit_max(uint limit) {
			return task_rlimit_max(current, limit);
		}



		/*
		 * flag set/clear/test wrappers
		 * - pass TIF_xxxx constants to these functions
		 */
		PREFIX_STATIC_INLINE
		void
		set_ti_thread_flag(thread_info_s *ti, int flag) {
			set_bit(flag, (ulong *)&ti->flags);
		}

		PREFIX_STATIC_INLINE
		void
		clear_ti_thread_flag(thread_info_s *ti, int flag) {
			clear_bit(flag, (ulong *)&ti->flags);
		}

		PREFIX_STATIC_INLINE
		void
		update_ti_thread_flag(thread_info_s *ti, int flag, bool value) {
			if (value)
				set_ti_thread_flag(ti, flag);
			else
				clear_ti_thread_flag(ti, flag);
		}

		PREFIX_STATIC_INLINE
		int
		test_and_set_ti_thread_flag(thread_info_s *ti, int flag) {
			return test_and_set_bit(flag, (ulong *)&ti->flags);
		}

		PREFIX_STATIC_INLINE
		int
		test_and_clear_ti_thread_flag(thread_info_s *ti, int flag) {
			return test_and_clear_bit(flag, (ulong *)&ti->flags);
		}

		PREFIX_STATIC_INLINE
		int
		test_ti_thread_flag(thread_info_s *ti, int flag) {
			return test_bit(flag, (ulong *)&ti->flags);
		}

		/*
		 * This may be used in noinstr code, and needs to be __always_inline to prevent
		 * inadvertent instrumentation.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		ulong
		read_ti_thread_flags(thread_info_s *ti) {
			return READ_ONCE(ti->flags);
		}

	#endif

#endif /* _LINUX_TASK_H_ */