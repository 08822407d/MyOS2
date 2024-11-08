#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>

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