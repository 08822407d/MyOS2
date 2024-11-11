#ifndef _LINUX_SIGNAL_H_
#define _LINUX_SIGNAL_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern void
		copy_siginfo(kernel_siginfo_t *to,
				const kernel_siginfo_t *from);

		extern void
		clear_siginfo(kernel_siginfo_t *info);

		extern pid_s
		*task_pid_type(task_s *task, enum pid_type type);

		extern pid_s
		*task_tgid(task_s *task);

		extern pid_s
		*task_pgrp(task_s *task);

		extern pid_s
		*task_session(task_s *task);

		// extern int
		// get_nr_threads(task_s *task);

		extern bool
		thread_group_leader(task_s *p);

		extern bool
		same_thread_group(task_s *p1, task_s *p2);


		extern void
		sigaddset(sigset_t *set, int _sig);

		extern void
		sigdelset(sigset_t *set, int _sig);

		extern int
		sigismember(sigset_t *set, int _sig);

		extern void
		sigemptyset(sigset_t *set);

		extern void
		sigfillset(sigset_t *set);

		extern void
		sigaddsetmask(sigset_t *set, ulong mask);

		extern void
		sigdelsetmask(sigset_t *set, ulong mask);

		extern int
		sigtestsetmask(sigset_t *set, ulong mask);

		extern void
		siginitset(sigset_t *set, ulong mask);

		extern void
		siginitsetinv(sigset_t *set, ulong mask);

		extern void
		init_sigpending(sigpending_s *sig);

	#endif

	#include "signal_macro.h"
	
	#if defined(SIGNAL_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		copy_siginfo(kernel_siginfo_t *to,
				const kernel_siginfo_t *from) {
			memcpy(to, from, sizeof(*to));
		}

		PREFIX_STATIC_INLINE
		void
		clear_siginfo(kernel_siginfo_t *info) {
			memset(info, 0, sizeof(*info));
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_pid_type(task_s *task, enum pid_type type) {
			pid_s *pid;
			if (type == PIDTYPE_PID)
				pid = task_pid(task);
			else
				pid = task->signal->pids[type];
			return pid;
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_tgid(task_s *task) {
			return task->signal->pids[PIDTYPE_TGID];
		}

		/*
		 * Without tasklist or RCU lock it is not safe to dereference
		 * the result of task_pgrp/task_session even if task == current,
		 * we can race with another thread doing sys_setsid/sys_setpgid.
		 */
		PREFIX_STATIC_INLINE
		pid_s
		*task_pgrp(task_s *task) {
			return task->signal->pids[PIDTYPE_PGID];
		}

		PREFIX_STATIC_INLINE
		pid_s
		*task_session(task_s *task) {
			return task->signal->pids[PIDTYPE_SID];
		}

		// PREFIX_STATIC_INLINE
		// int
		// get_nr_threads(task_s *task) {
		// 	return task->signal->nr_threads;
		// }

		PREFIX_STATIC_INLINE
		bool
		thread_group_leader(task_s *p) {
			return p->exit_signal >= 0;
		}

		PREFIX_STATIC_INLINE
		bool
		same_thread_group(task_s *p1, task_s *p2) {
			return p1->signal == p2->signal;
		}


		/* We don't use <linux/bitops.h> for these because there is no need to
		   be atomic.  */
		PREFIX_STATIC_INLINE
		void
		sigaddset(sigset_t *set, int _sig) {
			ulong sig = _sig - 1;
			if (_NSIG_WORDS == 1)
				set->sig[0] |= 1UL << sig;
			else
				set->sig[sig / _NSIG_BPW] |= 1UL << (sig % _NSIG_BPW);
		}

		PREFIX_STATIC_INLINE
		void
		sigdelset(sigset_t *set, int _sig) {
			ulong sig = _sig - 1;
			if (_NSIG_WORDS == 1)
				set->sig[0] &= ~(1UL << sig);
			else
				set->sig[sig / _NSIG_BPW] &= ~(1UL << (sig % _NSIG_BPW));
		}

		PREFIX_STATIC_INLINE
		int
		sigismember(sigset_t *set, int _sig) {
			ulong sig = _sig - 1;
			if (_NSIG_WORDS == 1)
				return 1 & (set->sig[0] >> sig);
			else
				return 1 & (set->sig[sig / _NSIG_BPW] >> (sig % _NSIG_BPW));
		}

		PREFIX_STATIC_INLINE
		void
		sigemptyset(sigset_t *set) {
			switch (_NSIG_WORDS) {
			default:
				memset(set, 0, sizeof(sigset_t));
				break;
			case 2: set->sig[1] = 0;
				fallthrough;
			case 1:	set->sig[0] = 0;
				break;
			}
		}

		PREFIX_STATIC_INLINE
		void
		sigfillset(sigset_t *set) {
			switch (_NSIG_WORDS) {
			default:
				memset(set, -1, sizeof(sigset_t));
				break;
			case 2: set->sig[1] = -1;
				fallthrough;
			case 1:	set->sig[0] = -1;
				break;
			}
		}

		/* Some extensions for manipulating the low 32 signals in particular.  */

		PREFIX_STATIC_INLINE
		void
		sigaddsetmask(sigset_t *set, ulong mask) {
			set->sig[0] |= mask;
		}

		PREFIX_STATIC_INLINE
		void
		sigdelsetmask(sigset_t *set, ulong mask) {
			set->sig[0] &= ~mask;
		}

		PREFIX_STATIC_INLINE
		int
		sigtestsetmask(sigset_t *set, ulong mask) {
			return (set->sig[0] & mask) != 0;
		}

		PREFIX_STATIC_INLINE
		void
		siginitset(sigset_t *set, ulong mask) {
			set->sig[0] = mask;
			switch (_NSIG_WORDS) {
			default:
				memset(&set->sig[1], 0, sizeof(long)*(_NSIG_WORDS-1));
				break;
			case 2: set->sig[1] = 0;
				break;
			case 1: ;
			}
		}

		PREFIX_STATIC_INLINE
		void
		siginitsetinv(sigset_t *set, ulong mask) {
			set->sig[0] = ~mask;
			switch (_NSIG_WORDS) {
			default:
				memset(&set->sig[1], -1, sizeof(long)*(_NSIG_WORDS-1));
				break;
			case 2: set->sig[1] = -1;
				break;
			case 1: ;
			}
		}

		PREFIX_STATIC_INLINE
		void
		init_sigpending(sigpending_s *sig) {
			// sigemptyset(&sig->signal);
			INIT_LIST_HEADER_S(&sig->list);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SIGNAL_H_ */