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

		extern int
		get_nr_threads(task_s *task);
		extern bool
		thread_group_leader(task_s *p);
		extern bool
		same_thread_group(task_s *p1, task_s *p2);
		extern task_s
		*__next_thread(task_s *p);
		extern task_s
		*next_thread(task_s *p);
		extern int
		thread_group_empty(task_s *p);


		extern void
		sigaddset(sigset_t *set, int _sig);
		extern void
		sigdelset(sigset_t *set, int _sig);

		extern int
		sigismember(sigset_t *set, int _sig);
		extern int
		sigisemptyset(sigset_t *set);
		extern int
		sigequalsets(const sigset_t *set1, const sigset_t *set2);

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


		extern int
		restart_syscall(void);
		extern int
		task_sigpending(task_s *p);
		extern int
		signal_pending(task_s *p);
		extern int
		__fatal_signal_pending(task_s *p);
		extern int
		fatal_signal_pending(task_s *p);
		extern int
		signal_pending_state(uint state, task_s *p);

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

		PREFIX_STATIC_INLINE
		int
		get_nr_threads(task_s *task) {
			return task->signal->nr_threads;
		}

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

		/*
		 * returns NULL if p is the last thread in the thread group
		 */
		PREFIX_STATIC_INLINE
		task_s
		*__next_thread(task_s *p) {
			// return list_next_or_null_rcu(&p->signal->thread_head,
			// 				&p->thread_node,
			// 				struct task_struct,
			// 				thread_node);
		}

		PREFIX_STATIC_INLINE
		task_s
		*next_thread(task_s *p) {
			// return __next_thread(p) ?: p->group_leader;
			return NULL;
		}

		PREFIX_STATIC_INLINE
		int
		thread_group_empty(task_s *p) {
			// return thread_group_leader(p) &&
			// 	list_is_last(&p->thread_node, &p->signal->thread_head);
			return 0;
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
		int
		sigisemptyset(sigset_t *set) {
			switch (_NSIG_WORDS) {
				case 4:
					return (set->sig[3] | set->sig[2] |
							set->sig[1] | set->sig[0]) == 0;
				case 2:
					return (set->sig[1] | set->sig[0]) == 0;
				case 1:
					return set->sig[0] == 0;
				default:
					BUILD_BUG();
					return 0;
			}
		}

		PREFIX_STATIC_INLINE
		int
		sigequalsets(const sigset_t *set1, const sigset_t *set2) {
			switch (_NSIG_WORDS) {
				case 4:
					return	(set1->sig[3] == set2->sig[3]) &&
							(set1->sig[2] == set2->sig[2]) &&
							(set1->sig[1] == set2->sig[1]) &&
							(set1->sig[0] == set2->sig[0]);
				case 2:
					return	(set1->sig[1] == set2->sig[1]) &&
							(set1->sig[0] == set2->sig[0]);
				case 1:
					return	set1->sig[0] == set2->sig[0];
			}
			return 0;
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



		PREFIX_STATIC_INLINE
		int
		restart_syscall(void) {
			set_tsk_thread_flag(current, TIF_SIGPENDING);
			return -ERESTARTNOINTR;
		}

		PREFIX_STATIC_INLINE
		int
		task_sigpending(task_s *p) {
			return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
		}

		PREFIX_STATIC_INLINE
		int
		signal_pending(task_s *p) {
			/*
			* TIF_NOTIFY_SIGNAL isn't really a signal, but it requires the same
			* behavior in terms of ensuring that we break out of wait loops
			* so that notify signal callbacks can be processed.
			*/
			if (unlikely(test_tsk_thread_flag(p, TIF_NOTIFY_SIGNAL)))
				return 1;
			return task_sigpending(p);
		}

		PREFIX_STATIC_INLINE
		int
		__fatal_signal_pending(task_s *p) {
			return unlikely(sigismember(&p->pending.signal, SIGKILL));
		}

		PREFIX_STATIC_INLINE
		int
		fatal_signal_pending(task_s *p) {
			return task_sigpending(p) && __fatal_signal_pending(p);
		}

		PREFIX_STATIC_INLINE
		int
		signal_pending_state(uint state, task_s *p) {
			if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
				return 0;
			if (!signal_pending(p))
				return 0;

			return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
		}

	#endif /* !DEBUG */


	#define _sig_or(x,y)	((x) | (y))
			_SIG_SET_BINOP(sigorsets, _sig_or)

	#define _sig_and(x,y)	((x) & (y))
			_SIG_SET_BINOP(sigandsets, _sig_and)

	#define _sig_andn(x,y)	((x) & ~(y))
			_SIG_SET_BINOP(sigandnsets, _sig_andn)

	#undef _SIG_SET_BINOP
	#undef _sig_or
	#undef _sig_and
	#undef _sig_andn

	#define _sig_not(x)	(~(x))
			_SIG_SET_OP(signotset, _sig_not)

	#undef _SIG_SET_OP
	#undef _sig_not

#endif /* _LINUX_SIGNAL_H_ */