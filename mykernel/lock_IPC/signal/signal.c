#define SIGNAL_DEFINATION
#include "signal.h"

#include <linux/kernel/sched_api.h>


static int
__send_signal_locked(int sig, kernel_siginfo_t *info,
		task_s *t, enum pid_type type, bool force) {
	// struct sigpending *pending;
	// struct sigqueue *q;
	int override_rlimit;
	int ret = 0, result;

	// lockdep_assert_held(&t->sighand->siglock);

	// result = TRACE_SIGNAL_IGNORED;
	// if (!prepare_signal(sig, t, force))
	// 	goto ret;

	// pending = (type != PIDTYPE_PID) ? &t->signal->shared_pending : &t->pending;
	// /*
	//  * Short-circuit ignored signals and support queuing
	//  * exactly one non-rt signal, so that we can get more
	//  * detailed information about the cause of the signal.
	//  */
	// result = TRACE_SIGNAL_ALREADY_PENDING;
	// if (legacy_queue(pending, sig))
	// 	goto ret;

	// result = TRACE_SIGNAL_DELIVERED;
	// /*
	//  * Skip useless siginfo allocation for SIGKILL and kernel threads.
	//  */
	// if ((sig == SIGKILL) || (t->flags & PF_KTHREAD))
	// 	goto out_set;

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

	// q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);

	// if (q) {
	// 	list_add_tail(&q->list, &pending->list);
	// 	switch ((unsigned long) info) {
	// 	case (unsigned long) SEND_SIG_NOINFO:
	// 		clear_siginfo(&q->info);
	// 		q->info.si_signo = sig;
	// 		q->info.si_errno = 0;
	// 		q->info.si_code = SI_USER;
	// 		q->info.si_pid = task_tgid_nr_ns(current,
	// 						task_active_pid_ns(t));
	// 		rcu_read_lock();
	// 		q->info.si_uid =
	// 			from_kuid_munged(task_cred_xxx(t, user_ns),
	// 					 current_uid());
	// 		rcu_read_unlock();
	// 		break;
	// 	case (unsigned long) SEND_SIG_PRIV:
	// 		clear_siginfo(&q->info);
	// 		q->info.si_signo = sig;
	// 		q->info.si_errno = 0;
	// 		q->info.si_code = SI_KERNEL;
	// 		q->info.si_pid = 0;
	// 		q->info.si_uid = 0;
	// 		break;
	// 	default:
	// 		copy_siginfo(&q->info, info);
	// 		break;
	// 	}
	// } else if (!is_si_special(info) &&
	// 	   sig >= SIGRTMIN && info->si_code != SI_USER) {
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
	// }

// out_set:
	// signalfd_notify(t, sig);
	// sigaddset(&pending->signal, sig);

	// /* Let multiprocess signals appear after on-going forks */
	// if (type > PIDTYPE_TGID) {
	// 	struct multiprocess_signals *delayed;
	// 	hlist_for_each_entry(delayed, &t->signal->multiprocess, node) {
	// 		sigset_t *signal = &delayed->signal;
	// 		/* Can't queue both a stop and a continue signal */
	// 		if (sig == SIGCONT)
	// 			sigdelsetmask(signal, SIG_KERNEL_STOP_MASK);
	// 		else if (sig_kernel_stop(sig))
	// 			sigdelset(signal, SIGCONT);
	// 		sigaddset(signal, sig);
	// 	}
	// }

	// complete_signal(sig, t, type);
// ret:
	// trace_signal_generate(sig, info, t, type != PIDTYPE_PID, result);
	return ret;
}

// static inline bool has_si_pid_and_uid(struct kernel_siginfo *info)
// {
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

int send_signal_locked(int sig, struct kernel_siginfo *info,
		       struct task_struct *t, enum pid_type type)
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
	// 	struct user_namespace *t_user_ns;

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

int do_send_sig_info(int sig, kernel_siginfo_t *info,
		task_s *p, enum pid_type type)
{
	ulong flags;
	int ret = -ESRCH;

	// if (lock_task_sighand(p, &flags)) {
		ret = send_signal_locked(sig, info, p, type);
	// 	unlock_task_sighand(p, &flags);
	// }

	return ret;
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

	// if (!ret && sig)
		ret = do_send_sig_info(sig, info, p, type);

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
kill_pid_info_type(int sig, kernel_siginfo_t *info,
		pid_s *pid, enum pid_type type) {

	int error = -ESRCH;
	task_s *p;

	for (;;) {
		// rcu_read_lock();
		p = pid_task(pid, PIDTYPE_PID);
		if (p)
			error = group_send_sig_info(sig, info, p, type);
		// rcu_read_unlock();
		if (likely(!p || error != -ESRCH))
			return error;
		/*
		 * The task was unhashed in between, try again.  If it
		 * is dead, pid_task() will return NULL, if we race with
		 * de_thread() it will find the new leader.
		 */
	}
}

static int
kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid) {
	int error;
	// rcu_read_lock();
	error = kill_pid_info_type(sig, info, find_vpid(pid), PIDTYPE_TGID);
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
	// clear_siginfo(info);
    memset(info, 0, sizeof(*info));
	info->si_signo = sig;
	info->si_errno = 0;
	info->si_code = (type == PIDTYPE_PID) ? SI_TKILL : SI_USER;
	info->si_pid = task_tgid_vnr(current);
	// info->si_uid = from_kuid_munged(current_user_ns(), current_uid());
}