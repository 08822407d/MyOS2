#define SIGNAL_DEFINATION
#include "signal.h"

#include <linux/kernel/sched_api.h>
#include <linux/kernel/mm_api.h>


/*
 * SLAB caches for signal bits.
 */
static kmem_cache_s *sigqueue_cachep;


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

	// if (override_rlimit || likely(sigpending <= task_rlimit(t, RLIMIT_SIGPENDING))) {
		q = kmem_cache_alloc(sigqueue_cachep, gfp_flags);
	// } else {
	// 	print_dropped_signal(sig);
	// }

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