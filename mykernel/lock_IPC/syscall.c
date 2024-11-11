#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>

#include "lock_ipc_const.h"
#include "lock_ipc_types.h"
#include "lock_ipc_api.h"

// #include <asm/unistd_64.h>
// #include <asm/syscalls_64.h>


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

    return 0;
}


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