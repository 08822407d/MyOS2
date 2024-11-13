#define SIGNAL_DEFINATION
#include "signal.h"

#include <linux/kernel/sched_api.h>
#include <linux/kernel/mm_api.h>


/*
 * SLAB caches for signal bits.
 */
static kmem_cache_s *sigqueue_cachep;



/*
 * allocate a new signal queue record
 * - this may be called without locks if and only if t == current, otherwise an
 *   appropriate lock must be held to stop the target task from exiting
 */
static sigqueue_s *
__sigqueue_alloc(int sig, task_s *t, gfp_t gfp_flags,
		int override_rlimit, const uint sigqueue_flags) {

	sigqueue_s *q = NULL;
	// struct ucounts *ucounts;
	long sigpending;

	// /*
	//  * Protect access to @t credentials. This can go away when all
	//  * callers hold rcu read lock.
	//  *
	//  * NOTE! A pending signal will hold on to the user refcount,
	//  * and we get/put the refcount only when the sigpending count
	//  * changes from/to zero.
	//  */
	// rcu_read_lock();
	// ucounts = task_ucounts(t);
	// sigpending = inc_rlimit_get_ucounts(ucounts, UCOUNT_RLIMIT_SIGPENDING);
	// rcu_read_unlock();
	// if (!sigpending)
	// 	return NULL;

	if (override_rlimit || likely(sigpending <= task_rlimit(t, RLIMIT_SIGPENDING))) {
		q = kmem_cache_alloc(sigqueue_cachep, gfp_flags);
	} else {
		// print_dropped_signal(sig);
	}

	if (unlikely(q == NULL)) {
		// dec_rlimit_put_ucounts(ucounts, UCOUNT_RLIMIT_SIGPENDING);
	} else {
		INIT_LIST_HEAD(&q->list);
		q->flags = sigqueue_flags;
		// q->ucounts = ucounts;
	}
	return q;
}

static void
__sigqueue_free(sigqueue_s *q) {
	if (q->flags & SIGQUEUE_PREALLOC)
		return;
	// if (q->ucounts) {
	// 	dec_rlimit_put_ucounts(q->ucounts, UCOUNT_RLIMIT_SIGPENDING);
	// 	q->ucounts = NULL;
	// }
	kmem_cache_free(sigqueue_cachep, q);
}

/*
 * Re-calculate pending state from the set of locally pending
 * signals, globally pending signals, and blocked signals.
 */
static inline bool has_pending_signals(sigset_t *signal, sigset_t *blocked)
{
	unsigned long ready;
	long i;

	switch (_NSIG_WORDS) {
	default:
		for (i = _NSIG_WORDS, ready = 0; --i >= 0 ;)
			ready |= signal->sig[i] &~ blocked->sig[i];
		break;

	case 4: ready  = signal->sig[3] &~ blocked->sig[3];
		ready |= signal->sig[2] &~ blocked->sig[2];
		ready |= signal->sig[1] &~ blocked->sig[1];
		ready |= signal->sig[0] &~ blocked->sig[0];
		break;

	case 2: ready  = signal->sig[1] &~ blocked->sig[1];
		ready |= signal->sig[0] &~ blocked->sig[0];
		break;

	case 1: ready  = signal->sig[0] &~ blocked->sig[0];
	}
	return ready !=	0;
}


void calculate_sigpending(void)
{
	/* Have any signals or users of TIF_SIGPENDING been delayed
	 * until after fork?
	 */
	spin_lock_irq(&current->sighand->siglock);
	set_tsk_thread_flag(current, TIF_SIGPENDING);
	// recalc_sigpending();
	spin_unlock_irq(&current->sighand->siglock);
}

/* Given the mask, find the first available signal that should be serviced. */
#define SYNCHRONOUS_MASK \
		(sigmask(SIGSEGV) | sigmask(SIGBUS) | sigmask(SIGILL) | \
			sigmask(SIGTRAP) | sigmask(SIGFPE) | sigmask(SIGSYS))

int next_signal(sigpending_s *pending, sigset_t *mask)
{
	ulong i, *s, *m, x;
	int sig = 0;

	s = pending->signal.sig;
	m = mask->sig;

	/*
	 * Handle the first word specially: it contains the
	 * synchronous signals that need to be dequeued first.
	 */
	x = *s &~ *m;
	if (x) {
		if (x & SYNCHRONOUS_MASK)
			x &= SYNCHRONOUS_MASK;
		sig = ffz(~x) + 1;
		return sig;
	}

	switch (_NSIG_WORDS) {
	default:
		for (i = 1; i < _NSIG_WORDS; ++i) {
			x = *++s &~ *++m;
			if (!x)
				continue;
			sig = ffz(~x) + i*_NSIG_BPW + 1;
			break;
		}
		break;

	case 2:
		x = s[1] &~ m[1];
		if (!x)
			break;
		sig = ffz(~x) + _NSIG_BPW + 1;
		break;

	case 1:
		/* Nothing to do */
		break;
	}

	return sig;
}

static void
collect_signal(int sig, sigpending_s *list,
		kernel_siginfo_t *info, bool *resched_timer) {

	sigqueue_s *q, *first = NULL;

	/*
	 * Collect the siginfo appropriate to this signal.  Check if
	 * there is another siginfo for the same signal.
	 */
	// list_for_each_entry(q, &list->list, list) {
	list_header_for_each_container(q, &list->list, list) {
		if (q->info.si_signo == sig) {
			if (first != NULL)
				goto still_pending;
			first = q;
		}
	}

	sigdelset(&list->signal, sig);

	if (first) {
still_pending:
		// list_del_init(&first->list);
		list_header_delete_node(&list->list, &first->list);
		copy_siginfo(info, &first->info);

		// *resched_timer =
		// 	(first->flags & SIGQUEUE_PREALLOC) &&
		// 	(info->si_code == SI_TIMER) &&
		// 	(info->si_sys_private);

		__sigqueue_free(first);
	} else {
		/*
		 * Ok, it wasn't in the queue.  This must be
		 * a fast-pathed signal or we must have been
		 * out of queue space.  So zero out the info.
		 */
		clear_siginfo(info);
		info->si_signo = sig;
		info->si_errno = 0;
		info->si_code = SI_USER;
		info->si_pid = 0;
		info->si_uid = 0;
	}
}

static int
__dequeue_signal(sigpending_s *pending, sigset_t *mask,
		kernel_siginfo_t *info, bool *resched_timer) {

	int sig = next_signal(pending, mask);
	if (sig)
		collect_signal(sig, pending, info, resched_timer);
	return sig;
}

/*
 * Dequeue a signal and return the element to the caller, which is
 * expected to free it.
 *
 * All callers have to hold the siglock.
 */
int dequeue_signal(task_s *tsk, sigset_t *mask,
		kernel_siginfo_t *info, enum pid_type *type)
{
	bool resched_timer = false;
	int signr;

	/* We only dequeue private signals from ourselves, we don't let
	 * signalfd steal them
	 */
	*type = PIDTYPE_PID;
	signr = __dequeue_signal(&tsk->pending, mask, info, &resched_timer);
	if (!signr) {
		*type = PIDTYPE_TGID;
		signr = __dequeue_signal(&tsk->signal->shared_pending,
					 mask, info, &resched_timer);
// #ifdef CONFIG_POSIX_TIMERS
// 		/*
// 		 * itimer signal ?
// 		 *
// 		 * itimers are process shared and we restart periodic
// 		 * itimers in the signal delivery path to prevent DoS
// 		 * attacks in the high resolution timer case. This is
// 		 * compliant with the old way of self-restarting
// 		 * itimers, as the SIGALRM is a legacy signal and only
// 		 * queued once. Changing the restart behaviour to
// 		 * restart the timer in the signal dequeue path is
// 		 * reducing the timer noise on heavy loaded !highres
// 		 * systems too.
// 		 */
// 		if (unlikely(signr == SIGALRM)) {
// 			struct hrtimer *tmr = &tsk->signal->real_timer;

// 			if (!hrtimer_is_queued(tmr) &&
// 			    tsk->signal->it_real_incr != 0) {
// 				hrtimer_forward(tmr, tmr->base->get_time(),
// 						tsk->signal->it_real_incr);
// 				hrtimer_restart(tmr);
// 			}
// 		}
// #endif
	}

// 	recalc_sigpending();
// 	if (!signr)
// 		return 0;

// 	if (unlikely(sig_kernel_stop(signr))) {
// 		/*
// 		 * Set a marker that we have dequeued a stop signal.  Our
// 		 * caller might release the siglock and then the pending
// 		 * stop signal it is about to process is no longer in the
// 		 * pending bitmasks, but must still be cleared by a SIGCONT
// 		 * (and overruled by a SIGKILL).  So those cases clear this
// 		 * shared flag after we've set it.  Note that this flag may
// 		 * remain set after the signal we return is ignored or
// 		 * handled.  That doesn't matter because its only purpose
// 		 * is to alert stop-signal processing code when another
// 		 * processor has come along and cleared the flag.
// 		 */
// 		current->jobctl |= JOBCTL_STOP_DEQUEUED;
// 	}
// #ifdef CONFIG_POSIX_TIMERS
// 	if (resched_timer) {
// 		/*
// 		 * Release the siglock to ensure proper locking order
// 		 * of timer locks outside of siglocks.  Note, we leave
// 		 * irqs disabled here, since the posix-timers code is
// 		 * about to disable them again anyway.
// 		 */
// 		spin_unlock(&tsk->sighand->siglock);
// 		posixtimer_rearm(info);
// 		spin_lock(&tsk->sighand->siglock);

// 		/* Don't expose the si_sys_private value to userspace */
// 		info->si_sys_private = 0;
// 	}
// #endif
	return signr;
}
EXPORT_SYMBOL_GPL(dequeue_signal);

static int
dequeue_synchronous_signal(kernel_siginfo_t *info) {
	task_s *tsk = current;
	sigpending_s *pending = &tsk->pending;
	sigqueue_s *q, *sync = NULL;

	/*
	 * Might a synchronous signal be in the queue?
	 */
	if (!((pending->signal.sig[0] & ~tsk->blocked.sig[0]) & SYNCHRONOUS_MASK))
		return 0;

	/*
	 * Return the first synchronous signal in the queue.
	 */
	// list_for_each_entry(q, &pending->list, list) {
	list_header_for_each_container(q, &pending->list, list) {
		/* Synchronous signals have a positive si_code */
		if ((q->info.si_code > SI_USER) &&
				(sigmask(q->info.si_signo) & SYNCHRONOUS_MASK)) {
			sync = q;
			goto next;
		}
	}
	return 0;
next:
	/*
	 * Check if there is another siginfo for the same signal.
	 */
	// list_for_each_entry_continue(q, &pending->list, list) {
	list_header_for_each_container(q, &pending->list, list) {
		if (q->info.si_signo == sync->info.si_signo)
			goto still_pending;
	}

	sigdelset(&pending->signal, sync->info.si_signo);
	// recalc_sigpending();
still_pending:
	// list_del_init(&sync->list);
	list_header_delete_node(&pending->list, &sync->list);
	copy_siginfo(info, &sync->info);
	__sigqueue_free(sync);
	return info->si_signo;
}

/*
 * Tell a process that it has a new active signal..
 *
 * NOTE! we rely on the previous spin_lock to
 * lock interrupts for us! We can only be called with
 * "siglock" held, and the local interrupt must
 * have been disabled when that got acquired!
 *
 * No need to set need_resched since signal event passing
 * goes through ->blocked
 */
void signal_wake_up_state(task_s *t, unsigned int state)
{
	// lockdep_assert_held(&t->sighand->siglock);

	set_tsk_thread_flag(t, TIF_SIGPENDING);

	/*
	 * TASK_WAKEKILL also means wake it up in the stopped/traced/killable
	 * case. We don't check t->state here because there is a race with it
	 * executing another processor and just now entering stopped state.
	 * By using wake_up_state, we ensure the process will wake up and
	 * handle its death signal.
	 */
	if (!wake_up_state(t, state | TASK_INTERRUPTIBLE))
		kick_process(t);
}


static void
complete_signal(int sig, task_s *p, enum pid_type type) {
	signal_s *signal = p->signal;
	task_s *t;

	// /*
	//  * Now find a thread we can wake up to take the signal off the queue.
	//  *
	//  * Try the suggested task first (may or may not be the main thread).
	//  */
	// if (wants_signal(sig, p))
	// 	t = p;
	// else if ((type == PIDTYPE_PID) || thread_group_empty(p))
	// 	/*
	// 	 * There is just one thread and it does not need to be woken.
	// 	 * It will dequeue unblocked signals before it runs again.
	// 	 */
	// 	return;
	// else {
	// 	/*
	// 	 * Otherwise try to find a suitable thread.
	// 	 */
	// 	t = signal->curr_target;
	// 	while (!wants_signal(sig, t)) {
	// 		t = next_thread(t);
	// 		if (t == signal->curr_target)
	// 			/*
	// 			 * No thread needs to be woken.
	// 			 * Any eligible threads will see
	// 			 * the signal in the queue soon.
	// 			 */
	// 			return;
	// 	}
	// 	signal->curr_target = t;
	// }

	// /*
	//  * Found a killable thread.  If the signal will be fatal,
	//  * then start taking the whole group down immediately.
	//  */
	// if (sig_fatal(p, sig) &&
	// 		(signal->core_state || !(signal->flags & SIGNAL_GROUP_EXIT)) &&
	// 		!sigismember(&t->real_blocked, sig) &&
	// 		(sig == SIGKILL || !p->ptrace)) {
	// 	/*
	// 	 * This signal will be fatal to the whole group.
	// 	 */
	// 	if (!sig_kernel_coredump(sig)) {
	// 		/*
	// 		 * Start a group exit and wake everybody up.
	// 		 * This way we don't have other threads
	// 		 * running and doing things after a slower
	// 		 * thread has the fatal signal pending.
	// 		 */
	// 		signal->flags = SIGNAL_GROUP_EXIT;
	// 		signal->group_exit_code = sig;
	// 		signal->group_stop_count = 0;
	// 		__for_each_thread(signal, t) {
	// 			task_clear_jobctl_pending(t, JOBCTL_PENDING_MASK);
	// 			sigaddset(&t->pending.signal, SIGKILL);
	// 			signal_wake_up(t, 1);
	// 		}
	// 		return;
	// 	}
	// }

	/*
	 * The signal is already in the shared-pending queue.
	 * Tell the chosen thread to wake up and dequeue it.
	 */
	signal_wake_up(t, sig == SIGKILL);
	return;
}


static int
__send_signal_locked(int sig, kernel_siginfo_t *info,
		task_s *t, enum pid_type type, bool force) {

	sigpending_s *pending;
	sigqueue_s *q;
	int override_rlimit;
	int ret = 0, result;

	// lockdep_assert_held(&t->sighand->siglock);

	// result = TRACE_SIGNAL_IGNORED;
	// if (!prepare_signal(sig, t, force))
	// 	goto ret;

	pending = (type != PIDTYPE_PID) ? &t->signal->shared_pending : &t->pending;
	/*
	 * Short-circuit ignored signals and support queuing
	 * exactly one non-rt signal, so that we can get more
	 * detailed information about the cause of the signal.
	 */
	// result = TRACE_SIGNAL_ALREADY_PENDING;
	// if (legacy_queue(pending, sig))
	// 	goto ret;

	// result = TRACE_SIGNAL_DELIVERED;
	/*
	 * Skip useless siginfo allocation for SIGKILL and kernel threads.
	 */
	if ((sig == SIGKILL) || (t->flags & PF_KTHREAD))
		goto out_set;

	// /*
	//  * Real-time signals must be queued if sent by sigqueue, or
	//  * some other real-time mechanism.  It is implementation
	//  * defined whether kill() does so.  We attempt to do so, on
	//  * the principle of least surprise, but since kill is not
	//  * allowed to fail with EAGAIN when low on memory we just
	//  * make sure at least one signal gets delivered and don't
	//  * pass on the info struct.
	//  */
	// if (sig < SIGRTMIN)
	// 	override_rlimit = (is_si_special(info) || info->si_code >= 0);
	// else
	// 	override_rlimit = 0;

	q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);

	if (q) {
		// list_add_tail(&q->list, &pending->list);
		list_header_add_to_tail(&pending->list, &q->list);
		switch ((ulong) info) {
		case (ulong) SEND_SIG_NOINFO:
			clear_siginfo(&q->info);
			q->info.si_signo	= sig;
			q->info.si_errno	= 0;
			q->info.si_code		= SI_USER;
			// q->info.si_pid		= task_tgid_nr_ns(current,
			// 						task_active_pid_ns(t));
			q->info.si_pid		= __task_pid_nr_ns(current,
					PIDTYPE_TGID, task_active_pid_ns(t));
			// rcu_read_lock();
			// q->info.si_uid		=
			// 		from_kuid_munged(task_cred_xxx(t, user_ns),
			// 			current_uid());
			// rcu_read_unlock();
			break;
		case (ulong) SEND_SIG_PRIV:
			clear_siginfo(&q->info);
			q->info.si_signo	= sig;
			q->info.si_errno	= 0;
			q->info.si_code		= SI_KERNEL;
			q->info.si_pid		= 0;
			q->info.si_uid		= 0;
			break;
		default:
			copy_siginfo(&q->info, info);
			break;
		}
	// } else if (!is_si_special(info) &&
	// 		sig >= SIGRTMIN && info->si_code != SI_USER) {
	// 	/*
	// 	 * Queue overflow, abort.  We may abort if the
	// 	 * signal was rt and sent by user using something
	// 	 * other than kill().
	// 	 */
	// 	result = TRACE_SIGNAL_OVERFLOW_FAIL;
	// 	ret = -EAGAIN;
	// 	goto ret;
	// } else {
	// 	/*
	// 	 * This is a silent loss of information.  We still
	// 	 * send the signal, but the *info bits are lost.
	// 	 */
	// 	result = TRACE_SIGNAL_LOSE_INFO;
	}

out_set:
	// signalfd_notify(t, sig);
	sigaddset(&pending->signal, sig);

	/* Let multiprocess signals appear after on-going forks */
	if (type > PIDTYPE_TGID) {
		// struct multiprocess_signals *delayed;
		// hlist_for_each_entry(delayed, &t->signal->multiprocess, node) {
		// 	sigset_t *signal = &delayed->signal;
		// 	/* Can't queue both a stop and a continue signal */
		// 	if (sig == SIGCONT)
		// 		sigdelsetmask(signal, SIG_KERNEL_STOP_MASK);
		// 	else if (sig_kernel_stop(sig))
		// 		sigdelset(signal, SIGCONT);
		// 	sigaddset(signal, sig);
		// }
	}

	// complete_signal(sig, t, type);
ret:
	// trace_signal_generate(sig, info, t, type != PIDTYPE_PID, result);
	return ret;
}

// static inline bool
// has_si_pid_and_uid(kernel_siginfo_t *info) {
// 	bool ret = false;
// 	switch (siginfo_layout(info->si_signo, info->si_code)) {
// 	case SIL_KILL:
// 	case SIL_CHLD:
// 	case SIL_RT:
// 		ret = true;
// 		break;
// 	case SIL_TIMER:
// 	case SIL_POLL:
// 	case SIL_FAULT:
// 	case SIL_FAULT_TRAPNO:
// 	case SIL_FAULT_MCEERR:
// 	case SIL_FAULT_BNDERR:
// 	case SIL_FAULT_PKUERR:
// 	case SIL_FAULT_PERF_EVENT:
// 	case SIL_SYS:
// 		ret = false;
// 		break;
// 	}
// 	return ret;
// }

int send_signal_locked(int sig, kernel_siginfo_t *info,
		task_s *t, enum pid_type type)
{
	/* Should SIGKILL or SIGSTOP be received by a pid namespace init? */
	bool force = false;

	// if (info == SEND_SIG_NOINFO) {
	// 	/* Force if sent from an ancestor pid namespace */
	// 	force = !task_pid_nr_ns(current, task_active_pid_ns(t));
	// } else if (info == SEND_SIG_PRIV) {
	// 	/* Don't ignore kernel generated signals */
	// 	force = true;
	// } else if (has_si_pid_and_uid(info)) {
	// 	/* SIGKILL and SIGSTOP is special or has ids */
	// 	user_ns_s *t_user_ns;

	// 	rcu_read_lock();
	// 	t_user_ns = task_cred_xxx(t, user_ns);
	// 	if (current_user_ns() != t_user_ns) {
	// 		kuid_t uid = make_kuid(current_user_ns(), info->si_uid);
	// 		info->si_uid = from_kuid_munged(t_user_ns, uid);
	// 	}
	// 	rcu_read_unlock();

	// 	/* A kernel generated signal? */
	// 	force = (info->si_code == SI_KERNEL);

	// 	/* From an ancestor pid namespace? */
	// 	if (!task_pid_nr_ns(current, task_active_pid_ns(t))) {
	// 		info->si_pid = 0;
	// 		force = true;
	// 	}
	// }
	return __send_signal_locked(sig, info, t, type, force);
}




/*
 * send signal info to all the members of a thread group or to the
 * individual thread if type == PIDTYPE_PID.
 */
int group_send_sig_info(int sig, kernel_siginfo_t *info,
		task_s *p, enum pid_type type)
{
	int ret;

	// rcu_read_lock();
	// ret = check_kill_permission(sig, info, p);
	// rcu_read_unlock();

	// if (!ret && sig) {
		// ret = do_send_sig_info(sig, info, p, type);
		// if (lock_task_sighand(p, &flags)) {
			ret = send_signal_locked(sig, info, p, type);
		// 	unlock_task_sighand(p, &flags);
		// }
	// }

	return ret;
}

// /*
//  * __kill_pgrp_info() sends a signal to a process group: this is what the tty
//  * control characters do (^C, ^Z etc)
//  * - the caller must hold at least a readlock on tasklist_lock
//  */
// int __kill_pgrp_info(int sig, struct kernel_siginfo *info, struct pid *pgrp)
// {
// 	struct task_struct *p = NULL;
// 	int ret = -ESRCH;

// 	do_each_pid_task(pgrp, PIDTYPE_PGID, p) {
// 		int err = group_send_sig_info(sig, info, p, PIDTYPE_PGID);
// 		/*
// 		 * If group_send_sig_info() succeeds at least once ret
// 		 * becomes 0 and after that the code below has no effect.
// 		 * Otherwise we return the last err or -ESRCH if this
// 		 * process group is empty.
// 		 */
// 		if (ret)
// 			ret = err;
// 	} while_each_pid_task(pgrp, PIDTYPE_PGID, p);

// 	return ret;
// }


static int
kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid) {
	// int error;
	// rcu_read_lock();
	// error = kill_pid_info_type(sig, info, find_vpid(pid), PIDTYPE_TGID);

// static int
// kill_pid_info_type(int sig, kernel_siginfo_t *info,
// 		pid_s *pid, enum pid_type type) {
	enum pid_type type = PIDTYPE_TGID;
	int error = -ESRCH;
	task_s *p;

	for (;;) {
		// rcu_read_lock();
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p)
			error = group_send_sig_info(sig, info, p, type);
		// rcu_read_unlock();
		if (likely(!p || error != -ESRCH))
			// return error;
			break;
		/*
		 * The task was unhashed in between, try again.  If it
		 * is dead, pid_task() will return NULL, if we race with
		 * de_thread() it will find the new leader.
		 */
	}
// }

	// rcu_read_unlock();
	return error;
}


/*
 * kill_something_info() interprets pid in interesting ways just like kill(2).
 *
 * POSIX specifies that kill(-1,sig) is unspecified, but what we have
 * is probably wrong.  Should make it like BSD or SYSV.
 */
int kill_something_info(int sig,
		kernel_siginfo_t *info, pid_t pid)
{
	int ret;

	if (pid > 0)
		return kill_proc_info(sig, info, pid);
	// 目前不支持pid <= 0的情形
	else
		while (1);
		

	// /* -INT_MIN is undefined.  Exclude this case to avoid a UBSAN warning */
	// if (pid == INT_MIN)
	// 	return -ESRCH;

	// // read_lock(&tasklist_lock);
	// if (pid != -1) {
	// 	// ret = __kill_pgrp_info(sig, info,
	// 	// 		pid ? find_vpid(-pid) : task_pgrp(current));
	// } else {
	// 	int retval = 0, count = 0;
	// 	task_s *p;
	// 	List_s *pos;

	// 	// for_each_process(p) {
	// 	list_header_foreach(pos, &all_task_lhdr) {
	// 		p = container_of(pos, task_s, tasks);
	// 		if (task_pid_vnr(p) > 1 &&
	// 				!same_thread_group(p, current)) {
	// 			// int err = group_send_sig_info(sig,
	// 			// 			info, p, PIDTYPE_MAX);
	// 			// ++count;
	// 			// if (err != -EPERM)
	// 			// 	retval = err;
	// 		}
	// 	}
	// 	ret = count ? retval : -ESRCH;
	// }
	// // read_unlock(&tasklist_lock);

	return ret;
}


bool get_signal(ksignal_s *ksig)
{
	task_s *curr = current;
	sighand_s *sighand = curr->sighand;
	signal_s *signal = curr->signal;
	int signr = 0;

	// clear_notify_signal();
	// if (unlikely(task_work_pending(current)))
	// 	task_work_run();

	// if (!task_sigpending(current))
	// 	return false;

	// if (unlikely(uprobe_deny_signal()))
	// 	return false;

	// /*
	//  * Do this once, we can't return to user-mode if freezing() == T.
	//  * do_signal_stop() and ptrace_stop() do freezable_schedule() and
	//  * thus do not need another check after return.
	//  */
	// try_to_freeze();

relock:
	spin_lock_irq(&sighand->siglock);

	// /*
	//  * Every stopped thread goes here after wakeup. Check to see if
	//  * we should notify the parent, prepare_signal(SIGCONT) encodes
	//  * the CLD_ si_code into SIGNAL_CLD_MASK bits.
	//  */
	// if (unlikely(signal->flags & SIGNAL_CLD_MASK)) {
	// 	int why;

	// 	if (signal->flags & SIGNAL_CLD_CONTINUED)
	// 		why = CLD_CONTINUED;
	// 	else
	// 		why = CLD_STOPPED;

	// 	signal->flags &= ~SIGNAL_CLD_MASK;

	// 	spin_unlock_irq(&sighand->siglock);

	// 	/*
	// 	 * Notify the parent that we're continuing.  This event is
	// 	 * always per-process and doesn't make whole lot of sense
	// 	 * for ptracers, who shouldn't consume the state via
	// 	 * wait(2) either, but, for backward compatibility, notify
	// 	 * the ptracer of the group leader too unless it's gonna be
	// 	 * a duplicate.
	// 	 */
	// 	read_lock(&tasklist_lock);
	// 	do_notify_parent_cldstop(current, false, why);

	// 	if (ptrace_reparented(current->group_leader))
	// 		do_notify_parent_cldstop(current->group_leader,
	// 					true, why);
	// 	read_unlock(&tasklist_lock);

	// 	goto relock;
	// }

	for (;;) {
		k_sigaction_s *ka;
		enum pid_type type;

		// /* Has this task already been marked for death? */
		// if ((signal->flags & SIGNAL_GROUP_EXIT) ||
		//      signal->group_exec_task) {
		// 	signr = SIGKILL;
		// 	sigdelset(&current->pending.signal, SIGKILL);
		// 	trace_signal_deliver(SIGKILL, SEND_SIG_NOINFO,
		// 			     &sighand->action[SIGKILL-1]);
		// 	recalc_sigpending();
		// 	/*
		// 	 * implies do_group_exit() or return to PF_USER_WORKER,
		// 	 * no need to initialize ksig->info/etc.
		// 	 */
		// 	goto fatal;
		// }

		// if (unlikely(current->jobctl & JOBCTL_STOP_PENDING) &&
		//     do_signal_stop(0))
		// 	goto relock;

		// if (unlikely(current->jobctl &
		// 	     (JOBCTL_TRAP_MASK | JOBCTL_TRAP_FREEZE))) {
		// 	if (current->jobctl & JOBCTL_TRAP_MASK) {
		// 		do_jobctl_trap();
		// 		spin_unlock_irq(&sighand->siglock);
		// 	} else if (current->jobctl & JOBCTL_TRAP_FREEZE)
		// 		do_freezer_trap();

		// 	goto relock;
		// }

		// /*
		//  * If the task is leaving the frozen state, let's update
		//  * cgroup counters and reset the frozen bit.
		//  */
		// if (unlikely(cgroup_task_frozen(current))) {
		// 	spin_unlock_irq(&sighand->siglock);
		// 	cgroup_leave_frozen(false);
		// 	goto relock;
		// }

		/*
		 * Signals generated by the execution of an instruction
		 * need to be delivered before any other pending signals
		 * so that the instruction pointer in the signal stack
		 * frame points to the faulting instruction.
		 */
		type = PIDTYPE_PID;
		signr = dequeue_synchronous_signal(&ksig->info);
		if (signr == 0)
			signr = dequeue_signal(curr, &curr->blocked,
							&ksig->info, &type);

		if (!signr)
			break; /* will return 0 */

		// if (unlikely(current->ptrace) && (signr != SIGKILL) &&
		//     !(sighand->action[signr -1].sa.sa_flags & SA_IMMUTABLE)) {
		// 	signr = ptrace_signal(signr, &ksig->info, type);
		// 	if (!signr)
		// 		continue;
		// }

		ka = &sighand->action[signr-1];

		// /* Trace actually delivered signals. */
		// trace_signal_deliver(signr, &ksig->info, ka);

		if (ka->sa.sa_handler == SIG_IGN) /* Do nothing.  */
			continue;
		if (ka->sa.sa_handler != SIG_DFL) {
			/* Run the handler.  */
			ksig->ka = *ka;

			if (ka->sa.sa_flags & SA_ONESHOT)
				ka->sa.sa_handler = SIG_DFL;

			break; /* will return non-zero "signr" value */
		}

		/*
		 * Now we are doing the default action for this signal.
		 */
		if (sig_kernel_ignore(signr)) /* Default is nothing. */
			continue;

		/*
		 * Global init gets no signals it doesn't want.
		 * Container-init gets no signals it doesn't want from same
		 * container.
		 *
		 * Note that if global/container-init sees a sig_kernel_only()
		 * signal here, the signal must have been generated internally
		 * or must have come from an ancestor namespace. In either
		 * case, the signal cannot be dropped.
		 */
		if (unlikely(signal->flags & SIGNAL_UNKILLABLE) &&
				!sig_kernel_only(signr))
			continue;

		// if (sig_kernel_stop(signr)) {
		// 	/*
		// 	 * The default action is to stop all threads in
		// 	 * the thread group.  The job control signals
		// 	 * do nothing in an orphaned pgrp, but SIGSTOP
		// 	 * always works.  Note that siglock needs to be
		// 	 * dropped during the call to is_orphaned_pgrp()
		// 	 * because of lock ordering with tasklist_lock.
		// 	 * This allows an intervening SIGCONT to be posted.
		// 	 * We need to check for that and bail out if necessary.
		// 	 */
		// 	if (signr != SIGSTOP) {
		// 		spin_unlock_irq(&sighand->siglock);

		// 		/* signals can be posted during this window */

		// 		if (is_current_pgrp_orphaned())
		// 			goto relock;

		// 		spin_lock_irq(&sighand->siglock);
		// 	}

		// 	if (likely(do_signal_stop(signr))) {
		// 		/* It released the siglock.  */
		// 		goto relock;
		// 	}

		// 	/*
		// 	 * We didn't actually stop, due to a race
		// 	 * with SIGCONT or something like that.
		// 	 */
		// 	continue;
		// }

	fatal:
		// spin_unlock_irq(&sighand->siglock);
		// if (unlikely(cgroup_task_frozen(current)))
		// 	cgroup_leave_frozen(true);

		/*
		 * Anything else is fatal, maybe with a core dump.
		 */
		current->flags |= PF_SIGNALED;

		// if (sig_kernel_coredump(signr)) {
		// 	if (print_fatal_signals)
		// 		print_fatal_signal(signr);
		// 	proc_coredump_connector(current);
		// 	/*
		// 	 * If it was able to dump core, this kills all
		// 	 * other threads in the group and synchronizes with
		// 	 * their demise.  If we lost the race with another
		// 	 * thread getting here, it set group_exit_code
		// 	 * first and our do_group_exit call below will use
		// 	 * that value and ignore the one we pass it.
		// 	 */
		// 	do_coredump(&ksig->info);
		// }

		// /*
		//  * PF_USER_WORKER threads will catch and exit on fatal signals
		//  * themselves. They have cleanup that must be performed, so we
		//  * cannot call do_exit() on their behalf. Note that ksig won't
		//  * be properly initialized, PF_USER_WORKER's shouldn't use it.
		//  */
		// if (current->flags & PF_USER_WORKER)
		// 	goto out;

		// /*
		//  * Death signals, no core dump.
		//  */
		// do_group_exit(signr);
		// /* NOTREACHED */
	}
	spin_unlock_irq(&sighand->siglock);

	ksig->sig = signr;

	// if (signr && !(ksig->ka.sa.sa_flags & SA_EXPOSE_TAGBITS))
	// 	hide_si_addr_tag_bits(ksig);
out:
	return signr > 0;
}




void prepare_kill_siginfo(int sig,
        kernel_siginfo_t *info, enum pid_type type)
{
	clear_siginfo(info);
	info->si_signo = sig;
	info->si_errno = 0;
	info->si_code = (type == PIDTYPE_PID) ? SI_TKILL : SI_USER;
	info->si_pid = task_tgid_vnr(current);
	// info->si_uid = from_kuid_munged(current_user_ns(), current_uid());
}


static void
__set_task_blocked(task_s *tsk, const sigset_t *newset) {
	if (task_sigpending(tsk) && !thread_group_empty(tsk)) {
		sigset_t newblocked;
		/* A set of now blocked but previously unblocked signals. */
		sigandnsets(&newblocked, newset, &current->blocked);
		// retarget_shared_pending(tsk, &newblocked);
	}
	tsk->blocked = *newset;
	// recalc_sigpending();
}

void __set_current_blocked(const sigset_t *newset)
{
	task_s *tsk = current;

	/*
	 * In case the signal mask hasn't changed, there is nothing we need
	 * to do. The current->blocked shouldn't be modified by other task.
	 */
	if (sigequalsets(&tsk->blocked, newset))
		return;

	spin_lock_irq(&tsk->sighand->siglock);
	__set_task_blocked(tsk, newset);
	spin_unlock_irq(&tsk->sighand->siglock);
}

/**
 * set_current_blocked - change current->blocked mask
 * @newset: new mask
 *
 * It is wrong to change ->blocked directly, this helper should be used
 * to ensure the process can't miss a shared signal we are going to block.
 */
void set_current_blocked(sigset_t *newset)
{
	sigdelsetmask(newset, sigmask(SIGKILL) | sigmask(SIGSTOP));
	__set_current_blocked(newset);
}

/*
 * This is also useful for kernel threads that want to temporarily
 * (or permanently) block certain signals.
 *
 * NOTE! Unlike the user-mode sys_sigprocmask(), the kernel
 * interface happily blocks "unblockable" signals like SIGKILL
 * and friends.
 */
int sigprocmask(int how, sigset_t *set, sigset_t *oldset)
{
	task_s *tsk = current;
	sigset_t newset;

	/* Lockless, only current can change ->blocked, never from irq */
	if (oldset)
		*oldset = tsk->blocked;

	switch (how) {
	case SIG_BLOCK:
		sigorsets(&newset, &tsk->blocked, set);
		break;
	case SIG_UNBLOCK:
		sigandnsets(&newset, &tsk->blocked, set);
		break;
	case SIG_SETMASK:
		newset = *set;
		break;
	default:
		return -EINVAL;
	}

	__set_current_blocked(&newset);
	return 0;
}
EXPORT_SYMBOL(sigprocmask);



int do_sigaction(int sig, k_sigaction_s *act, k_sigaction_s *oact)
{
	task_s *p = current, *t;
	k_sigaction_s *k;
	sigset_t mask;

	if (!valid_signal(sig) || sig < 1 || (act && sig_kernel_only(sig)))
		return -EINVAL;

	k = &p->sighand->action[sig-1];

	spin_lock_irq(&p->sighand->siglock);
	if (k->sa.sa_flags & SA_IMMUTABLE) {
		spin_unlock_irq(&p->sighand->siglock);
		return -EINVAL;
	}
	if (oact)
		*oact = *k;

	/*
	 * Make sure that we never accidentally claim to support SA_UNSUPPORTED,
	 * e.g. by having an architecture use the bit in their uapi.
	 */
	BUILD_BUG_ON(UAPI_SA_FLAGS & SA_UNSUPPORTED);

	/*
	 * Clear unknown flag bits in order to allow userspace to detect missing
	 * support for flag bits and to allow the kernel to use non-uapi bits
	 * internally.
	 */
	if (act)
		act->sa.sa_flags &= UAPI_SA_FLAGS;
	if (oact)
		oact->sa.sa_flags &= UAPI_SA_FLAGS;

	// sigaction_compat_abi(act, oact);

	if (act) {
		sigdelsetmask(&act->sa.sa_mask,
				sigmask(SIGKILL) | sigmask(SIGSTOP));
		*k = *act;
		/*
		 * POSIX 3.3.1.3:
		 *  "Setting a signal action to SIG_IGN for a signal that is
		 *   pending shall cause the pending signal to be discarded,
		 *   whether or not it is blocked."
		 *
		 *  "Setting a signal action to SIG_DFL for a signal that is
		 *   pending and whose default action is to ignore the signal
		 *   (for example, SIGCHLD), shall cause the pending signal to
		 *   be discarded, whether or not it is blocked"
		 */
		if (sig_handler_ignored(sig_handler(p, sig), sig)) {
			sigemptyset(&mask);
			sigaddset(&mask, sig);
			// flush_sigqueue_mask(&mask, &p->signal->shared_pending);
			// for_each_thread(p, t)
			// 	flush_sigqueue_mask(&mask, &t->pending);
		}
	}

	spin_unlock_irq(&p->sighand->siglock);
	return 0;
}




void __init signals_init(void)
{
	// siginfo_buildtime_checks();
	sigqueue_cachep = KMEM_CACHE(sigqueue, SLAB_PANIC | SLAB_ACCOUNT);
}