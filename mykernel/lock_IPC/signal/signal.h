#ifndef _LINUX_SIGNAL_H_
#define _LINUX_SIGNAL_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"

	#include <uapi/linux/signal.h>


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

		extern int
		valid_signal(ulong sig);

		extern int
		__on_sig_stack(ulong sp);

		extern int
		on_sig_stack(ulong sp);

		extern int
		sas_ss_flags(ulong sp);

		extern void
		sas_ss_reset(task_s *p);

		extern ulong
		sigsp(ulong sp, ksignal_s *ksig);

	#endif

	#include "signal_macro.h"
	
	#if defined(SIGNAL_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void __user
		*sig_handler(task_s *t, int sig) {
			return t->sighand->action[sig - 1].sa.sa_handler;
		}

		PREFIX_STATIC_INLINE
		bool
		sig_handler_ignored(void __user *handler, int sig) {
			/* Is it explicitly or implicitly ignored? */
			return handler == SIG_IGN ||
				(handler == SIG_DFL && sig_kernel_ignore(sig));
		}


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


		/* Test if 'sig' is valid signal. Use this instead of testing _NSIG directly */
		PREFIX_STATIC_INLINE
		int
		valid_signal(ulong sig) {
			return sig <= _NSIG ? 1 : 0;
		}




		PREFIX_STATIC_INLINE
		int
		__on_sig_stack(ulong sp) {
			return (sp > current->sas_ss_sp) &&
					(sp - current->sas_ss_sp <=
						current->sas_ss_size);
		}

		/*
		 * True if we are on the alternate signal stack.
		 */
		PREFIX_STATIC_INLINE
		int
		on_sig_stack(ulong sp) {
			/*
			 * If the signal stack is SS_AUTODISARM then, by construction, we
			 * can't be on the signal stack unless user code deliberately set
			 * SS_AUTODISARM when we were already on it.
			 *
			 * This improves reliability: if user state gets corrupted such that
			 * the stack pointer points very close to the end of the signal stack,
			 * then this check will enable the signal to be handled anyway.
			 */
			if (current->sas_ss_flags & SS_AUTODISARM)
				return 0;

			return __on_sig_stack(sp);
		}

		PREFIX_STATIC_INLINE
		int
		sas_ss_flags(ulong sp) {
			if (!current->sas_ss_size)
				return SS_DISABLE;

			return on_sig_stack(sp) ? SS_ONSTACK : 0;
		}

		PREFIX_STATIC_INLINE
		void
		sas_ss_reset(task_s *p) {
			p->sas_ss_sp = 0;
			p->sas_ss_size = 0;
			p->sas_ss_flags = SS_DISABLE;
		}

		PREFIX_STATIC_INLINE
		ulong
		sigsp(ulong sp, ksignal_s *ksig) {
			if (unlikely((ksig->ka.sa.sa_flags & SA_ONSTACK)) && ! sas_ss_flags(sp))
				return current->sas_ss_sp + current->sas_ss_size;
			return sp;
		}

	#endif /* !DEBUG */

#endif /* _LINUX_SIGNAL_H_ */