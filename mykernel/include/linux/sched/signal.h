/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_SIGNAL_H
#define _LINUX_SCHED_SIGNAL_H

	#include <linux/kernel/sched.h>
	// #include <linux/kernel/mm.h>
	// #include <asm/ptrace.h>

	// /*
	// * Types defining task->signal and task->sighand and APIs using them:
	// */

	// struct sighand_struct {
	// 	spinlock_t		siglock;
	// 	refcount_t		count;
	// 	wait_queue_head_t	signalfd_wqh;
	// 	struct k_sigaction	action[_NSIG];
	// };

	// /*
	// * Per-process accounting stats:
	// */
	// struct pacct_struct {
	// 	int			ac_flag;
	// 	long			ac_exitcode;
	// 	unsigned long		ac_mem;
	// 	u64			ac_utime, ac_stime;
	// 	unsigned long		ac_minflt, ac_majflt;
	// };

	// struct cpu_itimer {
	// 	u64 expires;
	// 	u64 incr;
	// };

	// /*
	// * This is the atomic variant of task_cputime, which can be used for
	// * storing and updating task_cputime statistics without locking.
	// */
	// struct task_cputime_atomic {
	// 	atomic64_t utime;
	// 	atomic64_t stime;
	// 	atomic64_t sum_exec_runtime;
	// };

	// #define INIT_CPUTIME_ATOMIC \
	// 	(struct task_cputime_atomic) {				\
	// 		.utime = ATOMIC64_INIT(0),			\
	// 		.stime = ATOMIC64_INIT(0),			\
	// 		.sum_exec_runtime = ATOMIC64_INIT(0),		\
	// 	}
	// /**
	//  * struct thread_group_cputimer - thread group interval timer counts
	//  * @cputime_atomic:	atomic thread group interval timers.
	//  *
	//  * This structure contains the version of task_cputime, above, that is
	//  * used for thread group CPU timer calculations.
	//  */
	// struct thread_group_cputimer {
	// 	struct task_cputime_atomic cputime_atomic;
	// };

	// struct multiprocess_signals {
	// 	sigset_t signal;
	// 	struct hlist_node node;
	// };

	// struct core_thread {
	// 	task_s *task;
	// 	struct core_thread *next;
	// };

	// struct core_state {
	// 	atomic_t nr_threads;
	// 	struct core_thread dumper;
	// 	struct completion startup;
	// };

	// /*
	// * Bits in flags field of signal_struct.
	// */
	// #define SIGNAL_STOP_STOPPED	0x00000001 /* job control stop in effect */
	// #define SIGNAL_STOP_CONTINUED	0x00000002 /* SIGCONT since WCONTINUED reap */
	// #define SIGNAL_GROUP_EXIT	0x00000004 /* group exit in progress */
	// /*
	// * Pending notifications to parent.
	// */
	// #define SIGNAL_CLD_STOPPED	0x00000010
	// #define SIGNAL_CLD_CONTINUED	0x00000020
	// #define SIGNAL_CLD_MASK		(SIGNAL_CLD_STOPPED|SIGNAL_CLD_CONTINUED)

	// #define SIGNAL_UNKILLABLE	0x00000040 /* for init: ignore fatal signals */

	// #define SIGNAL_STOP_MASK (SIGNAL_CLD_MASK | SIGNAL_STOP_STOPPED | \
	// 			SIGNAL_STOP_CONTINUED)

	// static inline void signal_set_stop_flags(struct signal_struct *sig,
	// 					unsigned int flags)
	// {
	// 	WARN_ON(sig->flags & SIGNAL_GROUP_EXIT);
	// 	sig->flags = (sig->flags & ~SIGNAL_STOP_MASK) | flags;
	// }

	// extern void flush_signals(task_s *);
	// extern void ignore_signals(task_s *);
	// extern void flush_signal_handlers(task_s *, int force_default);
	// extern int dequeue_signal(task_s *task, sigset_t *mask,
	// 			kernel_siginfo_t *info, enum pid_type *type);

	// static inline int kernel_dequeue_signal(void)
	// {
	// 	task_s *task = current;
	// 	kernel_siginfo_t __info;
	// 	enum pid_type __type;
	// 	int ret;

	// 	spin_lock_irq(&task->sighand->siglock);
	// 	ret = dequeue_signal(task, &task->blocked, &__info, &__type);
	// 	spin_unlock_irq(&task->sighand->siglock);

	// 	return ret;
	// }

	// static inline void kernel_signal_stop(void)
	// {
	// 	spin_lock_irq(&current->sighand->siglock);
	// 	if (current->jobctl & JOBCTL_STOP_DEQUEUED)
	// 		set_special_state(TASK_STOPPED);
	// 	spin_unlock_irq(&current->sighand->siglock);

	// 	schedule();
	// }
	// #ifdef __ia64__
	// # define ___ARCH_SI_IA64(_a1, _a2, _a3) , _a1, _a2, _a3
	// #else
	// # define ___ARCH_SI_IA64(_a1, _a2, _a3)
	// #endif

	// int force_sig_fault_to_task(int sig, int code, void __user *addr
	// 	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr)
	// 	, task_s *t);
	// int force_sig_fault(int sig, int code, void __user *addr
	// 	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr));
	// int send_sig_fault(int sig, int code, void __user *addr
	// 	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr)
	// 	, task_s *t);

	// int force_sig_mceerr(int code, void __user *, short);
	// int send_sig_mceerr(int code, void __user *, short, task_s *);

	// int force_sig_bnderr(void __user *addr, void __user *lower, void __user *upper);
	// int force_sig_pkuerr(void __user *addr, u32 pkey);
	// int force_sig_perf(void __user *addr, u32 type, u64 sig_data);

	// int force_sig_ptrace_errno_trap(int errno, void __user *addr);
	// int force_sig_fault_trapno(int sig, int code, void __user *addr, int trapno);
	// int send_sig_fault_trapno(int sig, int code, void __user *addr, int trapno,
	// 			task_s *t);
	// int force_sig_seccomp(int syscall, int reason, bool force_coredump);

	// extern int send_sig_info(int, struct kernel_siginfo *, task_s *);
	// extern void force_sigsegv(int sig);
	// extern int force_sig_info(struct kernel_siginfo *);
	// extern int __kill_pgrp_info(int sig, struct kernel_siginfo *info, pid_s *pgrp);
	// extern int kill_pid_info(int sig, struct kernel_siginfo *info, pid_s *pid);
	// extern int kill_pid_usb_asyncio(int sig, int errno, sigval_t addr, pid_s *,
	// 				const struct cred *);
	// extern int kill_pgrp(pid_s *pid, int sig, int priv);
	// extern int kill_pid(pid_s *pid, int sig, int priv);
	// extern __must_check bool do_notify_parent(task_s *, int);
	// extern void __wake_up_parent(task_s *p, task_s *parent);
	// extern void force_sig(int);
	// extern void force_fatal_sig(int);
	// extern void force_exit_sig(int);
	// extern int send_sig(int, task_s *, int);
	// extern int zap_other_threads(task_s *p);
	// extern struct sigqueue *sigqueue_alloc(void);
	// extern void sigqueue_free(struct sigqueue *);
	// extern int send_sigqueue(struct sigqueue *, pid_s *, enum pid_type);
	// extern int do_sigaction(int, struct k_sigaction *, struct k_sigaction *);

	// static inline int restart_syscall(void)
	// {
	// 	set_tsk_thread_flag(current, TIF_SIGPENDING);
	// 	return -ERESTARTNOINTR;
	// }

	// static inline int task_sigpending(task_s *p)
	// {
	// 	return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
	// }

	// static inline int signal_pending(task_s *p)
	// {
	// 	/*
	// 	* TIF_NOTIFY_SIGNAL isn't really a signal, but it requires the same
	// 	* behavior in terms of ensuring that we break out of wait loops
	// 	* so that notify signal callbacks can be processed.
	// 	*/
	// 	if (unlikely(test_tsk_thread_flag(p, TIF_NOTIFY_SIGNAL)))
	// 		return 1;
	// 	return task_sigpending(p);
	// }

	// static inline int __fatal_signal_pending(task_s *p)
	// {
	// 	return unlikely(sigismember(&p->pending.signal, SIGKILL));
	// }

	// static inline int fatal_signal_pending(task_s *p)
	// {
	// 	return task_sigpending(p) && __fatal_signal_pending(p);
	// }

	// static inline int signal_pending_state(unsigned int state, task_s *p)
	// {
	// 	if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
	// 		return 0;
	// 	if (!signal_pending(p))
	// 		return 0;

	// 	return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
	// }

	// /*
	// * This should only be used in fault handlers to decide whether we
	// * should stop the current fault routine to handle the signals
	// * instead, especially with the case where we've got interrupted with
	// * a VM_FAULT_RETRY.
	// */
	// static inline bool fault_signal_pending(vm_fault_t fault_flags,
	// 					pt_regs_s *regs)
	// {
	// 	return unlikely((fault_flags & VM_FAULT_RETRY) &&
	// 			(fatal_signal_pending(current) ||
	// 			(user_mode(regs) && signal_pending(current))));
	// }

	// /*
	// * Reevaluate whether the task has signals pending delivery.
	// * Wake the task if so.
	// * This is required every time the blocked sigset_t changes.
	// * callers must hold sighand->siglock.
	// */
	// extern void recalc_sigpending_and_wake(task_s *t);
	// extern void recalc_sigpending(void);
	// extern void calculate_sigpending(void);

	// extern void signal_wake_up_state(task_s *t, unsigned int state);

	// static inline void signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0);
	// }
	// static inline void ptrace_signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? __TASK_TRACED : 0);
	// }

	// void task_join_group_stop(task_s *task);

	// #ifdef TIF_RESTORE_SIGMASK
	// /*
	// * Legacy restore_sigmask accessors.  These are inefficient on
	// * SMP architectures because they require atomic operations.
	// */

	// /**
	//  * set_restore_sigmask() - make sure saved_sigmask processing gets done
	//  *
	//  * This sets TIF_RESTORE_SIGMASK and ensures that the arch signal code
	//  * will run before returning to user mode, to process the flag.  For
	//  * all callers, TIF_SIGPENDING is already set or it's no harm to set
	//  * it.  TIF_RESTORE_SIGMASK need not be in the set of bits that the
	//  * arch code will notice on return to user mode, in case those bits
	//  * are scarce.  We set TIF_SIGPENDING here to ensure that the arch
	//  * signal code always gets run when TIF_RESTORE_SIGMASK is set.
	//  */
	// static inline void set_restore_sigmask(void)
	// {
	// 	set_thread_flag(TIF_RESTORE_SIGMASK);
	// }

	// static inline void clear_tsk_restore_sigmask(task_s *task)
	// {
	// 	clear_tsk_thread_flag(task, TIF_RESTORE_SIGMASK);
	// }

	// static inline void clear_restore_sigmask(void)
	// {
	// 	clear_thread_flag(TIF_RESTORE_SIGMASK);
	// }
	// static inline bool test_tsk_restore_sigmask(task_s *task)
	// {
	// 	return test_tsk_thread_flag(task, TIF_RESTORE_SIGMASK);
	// }
	// static inline bool test_restore_sigmask(void)
	// {
	// 	return test_thread_flag(TIF_RESTORE_SIGMASK);
	// }
	// static inline bool test_and_clear_restore_sigmask(void)
	// {
	// 	return test_and_clear_thread_flag(TIF_RESTORE_SIGMASK);
	// }

	// #else	/* TIF_RESTORE_SIGMASK */

	// /* Higher-quality implementation, used if TIF_RESTORE_SIGMASK doesn't exist. */
	// static inline void set_restore_sigmask(void)
	// {
	// 	current->restore_sigmask = true;
	// }
	// static inline void clear_tsk_restore_sigmask(task_s *task)
	// {
	// 	task->restore_sigmask = false;
	// }
	// static inline void clear_restore_sigmask(void)
	// {
	// 	current->restore_sigmask = false;
	// }
	// static inline bool test_restore_sigmask(void)
	// {
	// 	return current->restore_sigmask;
	// }
	// static inline bool test_tsk_restore_sigmask(task_s *task)
	// {
	// 	return task->restore_sigmask;
	// }
	// static inline bool test_and_clear_restore_sigmask(void)
	// {
	// 	if (!current->restore_sigmask)
	// 		return false;
	// 	current->restore_sigmask = false;
	// 	return true;
	// }
	// #endif

	// static inline void restore_saved_sigmask(void)
	// {
	// 	if (test_and_clear_restore_sigmask())
	// 		__set_current_blocked(&current->saved_sigmask);
	// }

	// extern int set_user_sigmask(const sigset_t __user *umask, size_t sigsetsize);

	// static inline void restore_saved_sigmask_unless(bool interrupted)
	// {
	// 	if (interrupted)
	// 		WARN_ON(!signal_pending(current));
	// 	else
	// 		restore_saved_sigmask();
	// }

	// static inline sigset_t *sigmask_to_save(void)
	// {
	// 	sigset_t *res = &current->blocked;
	// 	if (unlikely(test_restore_sigmask()))
	// 		res = &current->saved_sigmask;
	// 	return res;
	// }

	// static inline int kill_cad_pid(int sig, int priv)
	// {
	// 	return kill_pid(cad_pid, sig, priv);
	// }

	// /* These can be the second arg to send_sig_info/send_group_sig_info.  */
	// #define SEND_SIG_NOINFO ((struct kernel_siginfo *) 0)
	// #define SEND_SIG_PRIV	((struct kernel_siginfo *) 1)

	// static inline int __on_sig_stack(unsigned long sp)
	// {
	// 	return sp > current->sas_ss_sp &&
	// 		sp - current->sas_ss_sp <= current->sas_ss_size;
	// }

	// /*
	// * True if we are on the alternate signal stack.
	// */
	// static inline int on_sig_stack(unsigned long sp)
	// {
	// 	/*
	// 	* If the signal stack is SS_AUTODISARM then, by construction, we
	// 	* can't be on the signal stack unless user code deliberately set
	// 	* SS_AUTODISARM when we were already on it.
	// 	*
	// 	* This improves reliability: if user state gets corrupted such that
	// 	* the stack pointer points very close to the end of the signal stack,
	// 	* then this check will enable the signal to be handled anyway.
	// 	*/
	// 	if (current->sas_ss_flags & SS_AUTODISARM)
	// 		return 0;

	// 	return __on_sig_stack(sp);
	// }

	// static inline int sas_ss_flags(unsigned long sp)
	// {
	// 	if (!current->sas_ss_size)
	// 		return SS_DISABLE;

	// 	return on_sig_stack(sp) ? SS_ONSTACK : 0;
	// }

	// static inline void sas_ss_reset(task_s *p)
	// {
	// 	p->sas_ss_sp = 0;
	// 	p->sas_ss_size = 0;
	// 	p->sas_ss_flags = SS_DISABLE;
	// }

	// static inline unsigned long sigsp(unsigned long sp, struct ksignal *ksig)
	// {
	// 	if (unlikely((ksig->ka.sa.sa_flags & SA_ONSTACK)) && ! sas_ss_flags(sp))
	// 		return current->sas_ss_sp + current->sas_ss_size;
	// 	return sp;
	// }

	// extern void __cleanup_sighand(struct sighand_struct *);
	// extern void flush_itimer_signals(void);

	// #define tasklist_empty() \
	// 	list_is_empty_entry(&init_task.tasks)

	// #define next_task(p) \
	// 	list_entry_rcu((p)->tasks.next, task_s, tasks)

	// #define for_each_process(p) \
	// 	for (p = &init_task ; (p = next_task(p)) != &init_task ; )

	// extern bool current_is_single_threaded(void);

	// /*
	// * Careful: do_each_thread/while_each_thread is a double loop so
	// *          'break' will not work as expected - use goto instead.
	// */
	// #define do_each_thread(g, t) \
	// 	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do

	// #define while_each_thread(g, t) \
	// 	while ((t = next_thread(t)) != g)

	// #define __for_each_thread(signal, t)	\
	// 	list_for_each_entry_rcu(t, &(signal)->thread_head, thread_node)

	// #define for_each_thread(p, t)		\
	// 	__for_each_thread((p)->signal, t)

	// /* Careful: this is a double loop, 'break' won't work as expected. */
	// #define for_each_process_thread(p, t)	\
	// 	for_each_process(p) for_each_thread(p, t)

	// typedef int (*proc_visitor)(task_s *p, void *data);
	// void walk_process_tree(task_s *top, proc_visitor, void *);

	// static inline
	// pid_s *task_pid_type(task_s *task, enum pid_type type)
	// {
	// 	pid_s *pid;
	// 	if (type == PIDTYPE_PID)
	// 		pid = task_pid(task);
	// 	else
	// 		pid = task->signal->pids[type];
	// 	return pid;
	// }

	// static inline pid_s *task_tgid(task_s *task)
	// {
	// 	return task->signal->pids[PIDTYPE_TGID];
	// }

	// /*
	// * Without tasklist or RCU lock it is not safe to dereference
	// * the result of task_pgrp/task_session even if task == current,
	// * we can race with another thread doing sys_setsid/sys_setpgid.
	// */
	// static inline pid_s *task_pgrp(task_s *task)
	// {
	// 	return task->signal->pids[PIDTYPE_PGID];
	// }

	// static inline pid_s *task_session(task_s *task)
	// {
	// 	return task->signal->pids[PIDTYPE_SID];
	// }

	// static inline int get_nr_threads(task_s *task)
	// {
	// 	return task->signal->nr_threads;
	// }

	static inline bool thread_group_leader(task_s *p) {
		return p->exit_signal >= 0;
	}

	// static inline
	// bool same_thread_group(task_s *p1, task_s *p2)
	// {
	// 	return p1->signal == p2->signal;
	// }

	// static inline task_s *next_thread(const task_s *p)
	// {
	// 	return list_entry_rcu(p->thread_group.next,
	// 				task_s, thread_group);
	// }

	// static inline int thread_group_empty(task_s *p)
	// {
	// 	return list_is_empty_entry(&p->thread_group);
	// }

	// #define delay_group_leader(p) \
	// 		(thread_group_leader(p) && !thread_group_empty(p))

	// extern bool thread_group_exited(pid_s *pid);

	// extern struct sighand_struct *__lock_task_sighand(task_s *task,
	// 							unsigned long *flags);

	// static inline struct sighand_struct *lock_task_sighand(task_s *task,
	// 							unsigned long *flags)
	// {
	// 	struct sighand_struct *ret;

	// 	ret = __lock_task_sighand(task, flags);
	// 	(void)__cond_lock(&task->sighand->siglock, ret);
	// 	return ret;
	// }

	// static inline void unlock_task_sighand(task_s *task,
	// 						unsigned long *flags)
	// {
	// 	spin_unlock_irqrestore(&task->sighand->siglock, *flags);
	// }

	// #ifdef CONFIG_LOCKDEP
	// extern void lockdep_assert_task_sighand_held(task_s *task);
	// #else
	// static inline void lockdep_assert_task_sighand_held(task_s *task) { }
	// #endif

	static inline unsigned long rlimit(unsigned int limit) {
		return READ_ONCE(current->signal->rlim[limit].rlim_cur);
	}

	static inline unsigned long rlimit_max(unsigned int limit) {
		return READ_ONCE(current->signal->rlim[limit].rlim_max);
	}

#endif /* _LINUX_SCHED_SIGNAL_H */
