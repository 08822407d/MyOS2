#ifndef _SIGNAL_MACRO_H_
#define _SIGNAL_MACRO_H_

	// #define tasklist_empty() \
	// 	list_empty(&init_task.tasks)

	// #define next_task(p) \
	// 	list_entry_rcu((p)->tasks.next, struct task_struct, tasks)

	// #define for_each_process(p) \
	// 	for (p = &init_task ; (p = next_task(p)) != &init_task ; )

	/* These can be the second arg to send_sig_info/send_group_sig_info.  */
	#define SEND_SIG_NOINFO		((kernel_siginfo_t *) 0)
	#define SEND_SIG_PRIV		((kernel_siginfo_t *) 1)

	#define SI_EXPANSION_SIZE	(sizeof(siginfo_t) - sizeof(kernel_siginfo_t))

	#define sigmask(sig)		(1UL << ((sig) - 1))

	/*
	 * In POSIX a signal is sent either to a specific thread (Linux task)
	 * or to the process as a whole (Linux thread group).  How the signal
	 * is sent determines whether it's to one thread or the whole group,
	 * which determines which signal mask(s) are involved in blocking it
	 * from being delivered until later.  When the signal is delivered,
	 * either it's caught or ignored by a user handler or it has a default
	 * effect that applies to the whole thread group (POSIX process).
	 *
	 * The possible effects an unblocked signal set to SIG_DFL can have are:
	 *   ignore	- Nothing Happens
	 *   terminate	- kill the process, i.e. all threads in the group,
	 * 		  similar to exit_group.  The group leader (only) reports
	 *		  WIFSIGNALED status to its parent.
	 *   coredump	- write a core dump file describing all threads using
	 *		  the same mm and then kill all those threads
	 *   stop 	- stop all the threads in the group, i.e. TASK_STOPPED state
	 *
	 * SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
	 * Other signals when not blocked and set to SIG_DFL behaves as follows.
	 * The job control signals also have other special effects.
	 *
	 *	+--------------------+------------------+
	 *	|  POSIX signal      |  default action  |
	 *	+--------------------+------------------+
	 *	|  SIGHUP            |  terminate	|
	 *	|  SIGINT            |	terminate	|
	 *	|  SIGQUIT           |	coredump 	|
	 *	|  SIGILL            |	coredump 	|
	 *	|  SIGTRAP           |	coredump 	|
	 *	|  SIGABRT/SIGIOT    |	coredump 	|
	 *	|  SIGBUS            |	coredump 	|
	 *	|  SIGFPE            |	coredump 	|
	 *	|  SIGKILL           |	terminate(+)	|
	 *	|  SIGUSR1           |	terminate	|
	 *	|  SIGSEGV           |	coredump 	|
	 *	|  SIGUSR2           |	terminate	|
	 *	|  SIGPIPE           |	terminate	|
	 *	|  SIGALRM           |	terminate	|
	 *	|  SIGTERM           |	terminate	|
	 *	|  SIGCHLD           |	ignore   	|
	 *	|  SIGCONT           |	ignore(*)	|
	 *	|  SIGSTOP           |	stop(*)(+)  	|
	 *	|  SIGTSTP           |	stop(*)  	|
	 *	|  SIGTTIN           |	stop(*)  	|
	 *	|  SIGTTOU           |	stop(*)  	|
	 *	|  SIGURG            |	ignore   	|
	 *	|  SIGXCPU           |	coredump 	|
	 *	|  SIGXFSZ           |	coredump 	|
	 *	|  SIGVTALRM         |	terminate	|
	 *	|  SIGPROF           |	terminate	|
	 *	|  SIGPOLL/SIGIO     |	terminate	|
	 *	|  SIGSYS/SIGUNUSED  |	coredump 	|
	 *	|  SIGSTKFLT         |	terminate	|
	 *	|  SIGWINCH          |	ignore   	|
	 *	|  SIGPWR            |	terminate	|
	 *	|  SIGRTMIN-SIGRTMAX |	terminate       |
	 *	+--------------------+------------------+
	 *	|  non-POSIX signal  |  default action  |
	 *	+--------------------+------------------+
	 *	|  SIGEMT            |  coredump	|
	 *	+--------------------+------------------+
	 *
	 * (+) For SIGKILL and SIGSTOP the action is "always", not just "default".
	 * (*) Special job control effects:
	 * When SIGCONT is sent, it resumes the process (all threads in the group)
	 * from TASK_STOPPED state and also clears any pending/queued stop signals
	 * (any of those marked with "stop(*)").  This happens regardless of blocking,
	 * catching, or ignoring SIGCONT.  When any stop signal is sent, it clears
	 * any pending/queued SIGCONT signals; this happens regardless of blocking,
	 * catching, or ignored the stop signal, though (except for SIGSTOP) the
	 * default action of stopping the process may happen later or never.
	 */

	#ifdef SIGEMT
	#  define SIGEMT_MASK		rt_sigmask(SIGEMT)
	#else
	#  define SIGEMT_MASK		0
	#endif

	#if SIGRTMIN > BITS_PER_LONG
	#  define rt_sigmask(sig)	(1ULL << ((sig)-1))
	#else
	#  define rt_sigmask(sig)	sigmask(sig)
	#endif

	#define siginmask(sig, mask) \
			((sig) > 0 && (sig) < SIGRTMIN && (rt_sigmask(sig) & (mask)))

	#define SIG_KERNEL_ONLY_MASK (\
			rt_sigmask(SIGKILL)   |  rt_sigmask(SIGSTOP))

	#define SIG_KERNEL_STOP_MASK (\
			rt_sigmask(SIGSTOP)   |  rt_sigmask(SIGTSTP)   | \
			rt_sigmask(SIGTTIN)   |  rt_sigmask(SIGTTOU)   )

	#define SIG_KERNEL_COREDUMP_MASK (\
	        rt_sigmask(SIGQUIT)   |  rt_sigmask(SIGILL)    | \
			rt_sigmask(SIGTRAP)   |  rt_sigmask(SIGABRT)   | \
	        rt_sigmask(SIGFPE)    |  rt_sigmask(SIGSEGV)   | \
			rt_sigmask(SIGBUS)    |  rt_sigmask(SIGSYS)    | \
	        rt_sigmask(SIGXCPU)   |  rt_sigmask(SIGXFSZ)   | \
			SIGEMT_MASK				       )

	#define SIG_KERNEL_IGNORE_MASK (\
	        rt_sigmask(SIGCONT)   |  rt_sigmask(SIGCHLD)   | \
			rt_sigmask(SIGWINCH)  |  rt_sigmask(SIGURG)    )

	#define SIG_SPECIFIC_SICODES_MASK (\
			rt_sigmask(SIGILL)    |  rt_sigmask(SIGFPE)    | \
			rt_sigmask(SIGSEGV)   |  rt_sigmask(SIGBUS)    | \
			rt_sigmask(SIGTRAP)   |  rt_sigmask(SIGCHLD)   | \
			rt_sigmask(SIGPOLL)   |  rt_sigmask(SIGSYS)    | \
			SIGEMT_MASK                                    )

	#define sig_kernel_only(sig)		siginmask(sig, SIG_KERNEL_ONLY_MASK)
	#define sig_kernel_coredump(sig)	siginmask(sig, SIG_KERNEL_COREDUMP_MASK)
	#define sig_kernel_ignore(sig)		siginmask(sig, SIG_KERNEL_IGNORE_MASK)
	#define sig_kernel_stop(sig)		siginmask(sig, SIG_KERNEL_STOP_MASK)
	#define sig_specific_sicodes(sig)	siginmask(sig, SIG_SPECIFIC_SICODES_MASK)

	#define sig_fatal(t, signr) \
			(!siginmask(signr, SIG_KERNEL_IGNORE_MASK|SIG_KERNEL_STOP_MASK) && \
				(t)->sighand->action[(signr)-1].sa.sa_handler == SIG_DFL)


	#define _SIG_SET_BINOP(name, op)						\
			static inline void								\
			name(sigset_t *r, const sigset_t *a,			\
					const sigset_t *b) {					\
															\
				ulong a0, a1, a2, a3, b0, b1, b2, b3;		\
				switch (_NSIG_WORDS) {						\
					case 4:									\
						a3 = a->sig[3]; a2 = a->sig[2];		\
						b3 = b->sig[3]; b2 = b->sig[2];		\
						r->sig[3] = op(a3, b3);				\
						r->sig[2] = op(a2, b2);				\
						fallthrough;						\
					case 2:									\
						a1 = a->sig[1]; b1 = b->sig[1];		\
						r->sig[1] = op(a1, b1);				\
						fallthrough;						\
					case 1:									\
						a0 = a->sig[0]; b0 = b->sig[0];		\
						r->sig[0] = op(a0, b0);				\
						break;								\
					default:								\
						BUILD_BUG();						\
				}											\
			}

	#define _SIG_SET_OP(name, op)							\
			static inline void								\
			name(sigset_t *set) {							\
				switch (_NSIG_WORDS) {						\
					case 4:									\
						set->sig[3] = op(set->sig[3]);		\
						set->sig[2] = op(set->sig[2]);		\
						fallthrough;						\
					case 2:									\
						set->sig[1] = op(set->sig[1]);		\
						fallthrough;						\
					case 1:									\
						set->sig[0] = op(set->sig[0]);		\
						break;								\
					default:								\
						BUILD_BUG();						\
				}											\
			}


	// static inline void signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0);
	// }
	#define signal_wake_up(t, resume)	\
			signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0)
	// static inline void ptrace_signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? __TASK_TRACED : 0);
	// }
	#define ptrace_signal_wake_up(t, resume)	\
			signal_wake_up_state(t, resume ? __TASK_TRACED : 0)


#endif /* _SIGNAL_MACRO_H_ */