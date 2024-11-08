#ifndef _SIGNAL_TYPES_H_
#define _SIGNAL_TYPES_H_

	#include "../lock_ipc_type_declaration.h"

	#include <uapi/asm-generic/signal.h>
	#include "../atomic/refcount_types.h"


	/*
	 * The default "si_band" type is "long", as specified by POSIX.
	 * However, some architectures want to override this to "int"
	 * for historical compatibility reasons, so we allow that.
	 */
	#ifndef __ARCH_SI_BAND_T
	#  define __ARCH_SI_BAND_T		long
	#endif

	#ifndef __ARCH_SI_CLOCK_T
	#  define __ARCH_SI_CLOCK_T		__kernel_clock_t
	#endif

	#ifndef __ARCH_SI_ATTRIBUTES
	#  define __ARCH_SI_ATTRIBUTES
	#endif


	union sigval {
		int			sival_int;
		void __user	*sival_ptr;
	};

	/*
	 * Be careful when extending this union.  On 32bit siginfo_t is 32bit
	 * aligned.  Which means that a 64bit field or any other field that
	 * would increase the alignment of siginfo_t will break the ABI.
	 */
	union __sifields {
		/* kill() */
		struct {
			__kernel_pid_t		_pid;			/* sender's pid */
			__kernel_uid32_t	_uid;			/* sender's uid */
		} _kill;

		/* POSIX.1b timers */
		struct {
			__kernel_timer_t	_tid;			/* timer id */
			int					_overrun;		/* overrun count */
			sigval_t			_sigval;		/* same as below */
			int					_sys_private;	/* not to be passed to user */
		} _timer;

		/* POSIX.1b signals */
		struct {
			__kernel_pid_t		_pid;			/* sender's pid */
			__kernel_uid32_t	_uid;			/* sender's uid */
			sigval_t _sigval;
		} _rt;

		/* SIGCHLD */
		struct {
			__kernel_pid_t		_pid;			/* which child */
			__kernel_uid32_t	_uid;			/* sender's uid */
			int					_status;		/* exit code */
			__ARCH_SI_CLOCK_T	_utime;
			__ARCH_SI_CLOCK_T	_stime;
		} _sigchld;

		// /* SIGILL, SIGFPE, SIGSEGV, SIGBUS, SIGTRAP, SIGEMT */
		// struct {
		// 	void __user *_addr; /* faulting insn/memory ref. */

	// #define __ADDR_BND_PKEY_PAD	(						\
	// 			__alignof__(void *) < sizeof(short) ?	\
	// 				sizeof(short) : __alignof__(void *)	\
	// 		)
		// 	union {
		// 		/* used on alpha and sparc */
		// 		int _trapno;	/* TRAP # which caused the signal */
		// 		/*
		// 		 * used when si_code=BUS_MCEERR_AR or
		// 		 * used when si_code=BUS_MCEERR_AO
		// 		 */
		// 		short _addr_lsb; /* LSB of the reported address */
		// 		/* used when si_code=SEGV_BNDERR */
		// 		struct {
		// 			char _dummy_bnd[__ADDR_BND_PKEY_PAD];
		// 			void __user *_lower;
		// 			void __user *_upper;
		// 		} _addr_bnd;
		// 		/* used when si_code=SEGV_PKUERR */
		// 		struct {
		// 			char _dummy_pkey[__ADDR_BND_PKEY_PAD];
		// 			__u32 _pkey;
		// 		} _addr_pkey;
		// 		/* used when si_code=TRAP_PERF */
		// 		struct {
		// 			unsigned long _data;
		// 			__u32 _type;
		// 			__u32 _flags;
		// 		} _perf;
		// 	};
		// } _sigfault;

		/* SIGPOLL */
		struct {
			__ARCH_SI_BAND_T	_band;			/* POLL_IN, POLL_OUT, POLL_MSG */
			int _fd;
		} _sigpoll;

		/* SIGSYS */
		struct {
			void __user			*_call_addr;	/* calling user insn */
			int					_syscall;		/* triggering system call number */
			uint				_arch;			/* AUDIT_ARCH_* of syscall */
		} _sigsys;
	};

	struct kernel_siginfo {
		int				si_signo;
		int				si_errno;
		int				si_code;
		__sifields_u	_sifields;
	};

	/*
	 * Real Time signals may be queued.
	 */

	struct sigqueue {
		List_s				list;
		int					flags;
		kernel_siginfo_t	info;
		// struct ucounts		*ucounts;
	};

	struct sigpending {
		List_s		list;
		sigset_t	signal;
	};

	// struct sigaction {
	// #ifndef __ARCH_HAS_IRIX_SIGACTION
	// 	__sighandler_t	sa_handler;
	// 	unsigned long	sa_flags;
	// #else
	// 	unsigned int	sa_flags;
	// 	__sighandler_t	sa_handler;
	// #endif
	// #ifdef __ARCH_HAS_SA_RESTORER
	// 	__sigrestore_t sa_restorer;
	// #endif
	// 	sigset_t	sa_mask;	/* mask last for extensibility */
	// };

	// struct k_sigaction {
	// 	struct sigaction sa;
	// #ifdef __ARCH_HAS_KA_RESTORER
	// 	__sigrestore_t ka_restorer;
	// #endif
	// };

	struct ksignal {
		// struct k_sigaction ka;
		kernel_siginfo_t	info;
		int					sig;
	};

	/*
	 * NOTE! "signal_struct" does not have its own
	 * locking, because a shared signal_struct always
	 * implies a shared sighand_struct, so locking
	 * sighand_struct is always a proper superset of
	 * the locking of signal_struct.
	 */
	struct signal_struct {
		refcount_t		sigcnt;
		// atomic_t		live;
		int				nr_threads;
		// struct list_head	thread_head;

		// wait_queue_head_t	wait_chldexit;	/* for wait4() */

		/* current thread group signal load-balancing target: */
		// task_s	*curr_target;

		/* shared signal handling: */
		sigpending_s	shared_pending;

		/* For collecting multiprocess signals during fork */
		// HList_hdr_s	multiprocess;

		/* thread group exit support */
		// int			group_exit_code;
		/* notify group_exec_task when notify_count is less or equal to 0 */
		// int			notify_count;
		// task_s	*group_exec_task;

		/* thread group stop support, overloads group_exit_code too */
		// int			group_stop_count;
		uint			flags; /* see SIGNAL_* flags below */

		// struct core_state *core_state; /* coredumping support */

		/*
		 * PR_SET_CHILD_SUBREAPER marks a process, like a service
		 * manager, to re-parent orphan (double-forking) child processes
		 * to this process instead of 'init'. The service manager is
		 * able to receive SIGCHLD signals and is able to investigate
		 * the process until it calls wait(). All children of this
		 * process will inherit a flag if they should look for a
		 * child_subreaper process at exit.
		 */
		// unsigned int		is_child_subreaper:1;
		// unsigned int		has_child_subreaper:1;

	// #ifdef CONFIG_POSIX_TIMERS

		/* POSIX.1b Interval Timers */
		// int			posix_timer_id;
		// struct list_head	posix_timers;

		/* ITIMER_REAL timer for the process */
		// struct hrtimer real_timer;
		// ktime_t it_real_incr;

		/*
		 * ITIMER_PROF and ITIMER_VIRTUAL timers for the process, we use
		 * CPUCLOCK_PROF and CPUCLOCK_VIRT for indexing array as these
		 * values are defined to 0 and 1 respectively
		 */
		// struct cpu_itimer it[2];

		/*
		 * Thread group totals for process CPU timers.
		 * See thread_group_cputimer(), et al, for details.
		 */
		// struct thread_group_cputimer cputimer;

	// #endif
		/* Empty if CONFIG_POSIX_TIMERS=n */
		// struct posix_cputimers posix_cputimers;

		/* PID/PID hash table linkage. */
		pid_s			*pids[PIDTYPE_MAX];

	// #ifdef CONFIG_NO_HZ_FULL
		// atomic_t tick_dep_mask;
	// #endif

		// pid_s *tty_old_pgrp;

		/* boolean value for session group leader */
		// int leader;

		// struct tty_struct *tty; /* NULL if no tty */

	// #ifdef CONFIG_SCHED_AUTOGROUP
	// 	struct autogroup *autogroup;
	// #endif
		/*
		 * Cumulative resource counters for dead threads in the group,
		 * and for reaped dead child processes forked by this group.
		 * Live threads maintain their own counters and add to these
		 * in __exit_signal, except for the group leader.
		 */
		// seqlock_t stats_lock;
		// u64 utime, stime, cutime, cstime;
		// u64 gtime;
		// u64 cgtime;
		// struct prev_cputime prev_cputime;
		// unsigned long nvcsw, nivcsw, cnvcsw, cnivcsw;
		// unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
		// unsigned long inblock, oublock, cinblock, coublock;
		// unsigned long maxrss, cmaxrss;
		// struct task_io_accounting ioac;

		/*
		 * Cumulative ns of schedule CPU time fo dead threads in the
		 * group, not including a zombie group leader, (This only differs
		 * from jiffies_to_ns(utime + stime) if sched_clock uses something
		 * other than jiffies.)
		 */
		// unsigned long long sum_sched_runtime;

		/*
		 * We don't bother to synchronize most readers of this at all,
		 * because there is no reader checking a limit that actually needs
		 * to get both rlim_cur and rlim_max atomically, and either one
		 * alone is a single word that can safely be read normally.
		 * getrlimit/setrlimit use task_lock(current->group_leader) to
		 * protect this instead of the siglock, because they really
		 * have no need to disable irqs.
		 */
		rlimit_s		rlim[RLIM_NLIMITS];

	// #ifdef CONFIG_BSD_PROCESS_ACCT
		// struct pacct_struct pacct;	/* per-process accounting information */
	// #endif
	// #ifdef CONFIG_TASKSTATS
		// struct taskstats *stats;
	// #endif
	// #ifdef CONFIG_AUDIT
		// unsigned audit_tty;
		// struct tty_audit_buf *tty_audit_buf;
	// #endif

		// /*
		// * Thread is the potential origin of an oom condition; kill first on
		// * oom
		// */
		// bool oom_flag_origin;
		// short oom_score_adj;		/* OOM kill score adjustment */
		// short oom_score_adj_min;	/* OOM kill score adjustment min value.
		// 				* Only settable by CAP_SYS_RESOURCE. */
		// mm_s *oom_mm;	/* recorded mm when the thread group got
		// 				* killed by the oom killer */

		// struct mutex cred_guard_mutex;	/* guard against foreign influences on
		// 				* credential calculations
		// 				* (notably. ptrace)
		// 				* Deprecated do not use in new code.
		// 				* Use exec_update_lock instead.
		// 				*/
		// rwsem_t exec_update_lock;	/* Held while task_struct is
		// 					* being updated during exec,
		// 					* and may have inconsistent
		// 					* permissions.
		// 					*/
	};

#endif /* _SIGNAL_TYPES_H_ */