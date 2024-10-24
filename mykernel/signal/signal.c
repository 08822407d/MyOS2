#include <linux/kernel/sched.h>

#include "signal_const.h"
#include "signal_types.h"
#include "signal_api.h"



// static int
// kill_pid_info_type(int sig, kernel_siginfo_t *info,
// 		pid_s *pid, enum pid_type type) {

// 	int error = -ESRCH;
// 	task_s *p;

// 	for (;;) {
// 		// rcu_read_lock();
// 		p = pid_task(pid, PIDTYPE_PID);
// 		if (p)
// 			error = group_send_sig_info(sig, info, p, type);
// 		// rcu_read_unlock();
// 		if (likely(!p || error != -ESRCH))
// 			return error;
// 		/*
// 		 * The task was unhashed in between, try again.  If it
// 		 * is dead, pid_task() will return NULL, if we race with
// 		 * de_thread() it will find the new leader.
// 		 */
// 	}
// }

static int
kill_proc_info(int sig, kernel_siginfo_t *info, pid_t pid) {
	int error;
	// rcu_read_lock();
	// error = kill_pid_info(sig, info, find_vpid(pid));
	// error = kill_pid_info_type(sig, info, find_vpid(pid), PIDTYPE_TGID);
	// rcu_read_unlock();
	return error;
}


/*
 * kill_something_info() interprets pid in interesting ways just like kill(2).
 *
 * POSIX specifies that kill(-1,sig) is unspecified, but what we have
 * is probably wrong.  Should make it like BSD or SYSV.
 */
int kill_something_info(int sig, kernel_siginfo_t *info, pid_t pid)
{
	int ret;

	if (pid > 0)
		return kill_proc_info(sig, info, pid);

	/* -INT_MIN is undefined.  Exclude this case to avoid a UBSAN warning */
	if (pid == INT_MIN)
		return -ESRCH;

	// read_lock(&tasklist_lock);
	// if (pid != -1) {
	// 	ret = __kill_pgrp_info(sig, info,
	// 			pid ? find_vpid(-pid) : task_pgrp(current));
	// } else {
	// 	int retval = 0, count = 0;
	// 	struct task_struct * p;

	// 	for_each_process(p) {
	// 		if (task_pid_vnr(p) > 1 &&
	// 				!same_thread_group(p, current)) {
	// 			int err = group_send_sig_info(sig, info, p,
	// 						      PIDTYPE_MAX);
	// 			++count;
	// 			if (err != -EPERM)
	// 				retval = err;
	// 		}
	// 	}
	// 	ret = count ? retval : -ESRCH;
	// }
	// read_unlock(&tasklist_lock);

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
	// info->si_pid = task_tgid_vnr(current);
	// info->si_uid = from_kuid_munged(current_user_ns(), current_uid());
}