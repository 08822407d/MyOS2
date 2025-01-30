#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>

#include "lock_ipc_const.h"
#include "lock_ipc_types.h"
#include "lock_ipc_api.h"

// #include <asm/unistd_64.h>
// #include <asm/syscalls_64.h>



// #define __NR_rt_sigaction	13
/**
 *  sys_rt_sigaction - alter an action taken by a process
 *  @sig: signal to be sent
 *  @act: new sigaction
 *  @oact: used to save the previous sigaction
 *  @sigsetsize: size of sigset_t type
 */
MYOS_SYSCALL_DEFINE4(rt_sigaction,
		int, sig, const sigaction_s __user *, act,
		sigaction_s __user *, oact, size_t, sigsetsize)
{
	k_sigaction_s new_sa, old_sa;
	int ret;

	/* XXX: Don't preclude handling different sized sigset_t's.  */
	if (sigsetsize != sizeof(sigset_t))
		return -EINVAL;

	if (act && copy_from_user(&new_sa.sa, act, sizeof(new_sa.sa)))
		return -EFAULT;

	ret = do_sigaction(sig, act ? &new_sa : NULL, oact ? &old_sa : NULL);
	if (ret)
		return ret;

	if (oact && copy_to_user(oact, &old_sa.sa, sizeof(old_sa.sa)))
		return -EFAULT;

	return 0;
}


// #define __NR_rt_sigprocmask	14
/**
 *  sys_rt_sigprocmask - change the list of currently blocked signals
 *  @how: whether to add, remove, or set signals
 *  @nset: stores pending signals
 *  @oset: previous value of signal mask if non-null
 *  @sigsetsize: size of sigset_t type
 */
MYOS_SYSCALL_DEFINE4(rt_sigprocmask, int, how, sigset_t __user *, nset, sigset_t __user *, oset, size_t, sigsetsize)
{
	sigset_t old_set, new_set;
	int error;

	/* XXX: Don't preclude handling different sized sigset_t's.  */
	if (sigsetsize != sizeof(sigset_t))
		return -EINVAL;

	old_set = current->blocked;

	if (nset) {
		if (copy_from_user(&new_set, nset, sizeof(sigset_t)))
			return -EFAULT;
		sigdelsetmask(&new_set, sigmask(SIGKILL)|sigmask(SIGSTOP));

		error = sigprocmask(how, &new_set, NULL);
		if (error)
			return error;
	}

	if (oset) {
		if (copy_to_user(oset, &old_set, sizeof(sigset_t)))
			return -EFAULT;
	}

	return 0;
}


// #define __NR_kill			62
/**
 *  sys_kill - send a signal to a process
 *  @pid: the PID of the process
 *  @sig: signal to be sent
 */
MYOS_SYSCALL_DEFINE2(kill, pid_t, pid, int, sig)
{
	ALERT_DUMMY_SYSCALL(kill, IF_ALERT_DUMMY_SYSCALL);

	kernel_siginfo_t info;

	prepare_kill_siginfo(sig, &info, PIDTYPE_TGID);

	return kill_something_info(sig, &info, pid);
}


// #define __NR_futex				202
MYOS_SYSCALL_DEFINE6(futex, u32 __user *, uaddr, int, op,
		u32, val, const __kernel_timespec_s __user *, utime,
		u32 __user *, uaddr2, u32, val3)
{
	ALERT_DUMMY_SYSCALL(futex, IF_ALERT_DUMMY_SYSCALL);
	while (1);
	

	int ret, cmd = op & FUTEX_CMD_MASK;
	ktime_t t, *tp = NULL;
	timespec64_s ts;

	if (utime && futex_cmd_has_timeout(cmd)) {
		pr_err("Unsupported feature: Futex Timeout\n");
		while (1);
		
		// if (unlikely(should_fail_futex(!(op & FUTEX_PRIVATE_FLAG))))
		// 	return -EFAULT;
		if (get_timespec64(&ts, utime))
			return -EFAULT;
		ret = futex_init_timeout(cmd, op, &ts, &t);
		if (ret)
			return ret;
		tp = &t;
	}

	return do_futex(uaddr, op, val, tp, uaddr2, (ulong)utime, val3);
}