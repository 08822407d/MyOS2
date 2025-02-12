#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <uapi/asm-generic/signal.h>

#include "sched_const.h"
#include "sched_types.h"
#include "sched_api.h"

// #include <asm/unistd_64.h>
// #include <asm/syscalls_64.h>


// #define __NR_sched_yield	24
/**
 * sys_sched_yield - yield the current processor to other threads.
 *
 * This function yields the current CPU to other tasks. If there are no
 * other threads running on this CPU then this function will return.
 *
 * Return: 0.
 */
MYOS_SYSCALL_DEFINE0(sched_yield)
{
	ALERT_DUMMY_SYSCALL(sched_yield, IF_ALERT_DUMMY_SYSCALL);

	// do_sched_yield();
	// {
		rq_flags_s rf;
		rq_s *rq;

		// rq = this_rq_lock_irq(&rf);

		// schedstat_inc(rq->yld_count);
		current->sched_class->yield_task(rq);

		preempt_disable();
		// rq_unlock_irq(rq, &rf);
		sched_preempt_enable_no_resched();

		schedule();
	// }
	return 0;
}


// #define __NR_getpid			39
MYOS_SYSCALL_DEFINE0(getpid)
{
	return task_tgid_vnr(current);
}

// #define __NR_fork			57
MYOS_SYSCALL_DEFINE0(fork)
{
	barrier();
	ulong counter = 0;
	for (long i = 0; i < 0x1000; i++)
		for (long j = 0; j < 0x1000; j++)
			counter++;
	barrier();

	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
}

// #define __NR_exit			60
MYOS_SYSCALL_DEFINE1(exit, int, error_code)
{
	ALERT_DUMMY_SYSCALL(exit, IF_ALERT_DUMMY_SYSCALL);

	do_exit(error_code);
}

// #define __NR_wait4			61
MYOS_SYSCALL_DEFINE4(wait4, pid_t, pid, int *, start_addr,
		int, options, void *, rusage)
{

}

// #define __NR_getuid			102
// MYOS_SYSCALL_DEFINE0(getuid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getuid ---\n");

// 	return 1000;
// }

// #define __NR_getgid			104
// MYOS_SYSCALL_DEFINE0(getgid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getgid ---\n");

// 	return 1000;
// }

// #define __NR_geteuid		107
// MYOS_SYSCALL_DEFINE0(geteuid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- geteuid ---\n");

// 	return 1000;
// }

// #define __NR_getegid		108
// MYOS_SYSCALL_DEFINE0(getegid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getegid ---\n");

// 	return 1000;
// }

// #define __NR_getppid		110
MYOS_SYSCALL_DEFINE0(getppid)
{
	int pid;

	// rcu_read_lock();
	pid = task_tgid_vnr(current->real_parent);
	// rcu_read_unlock();

	return pid;
}


// #define __NR_exit_group			231
/*
 * this kills every thread in the thread group. Note that any externally
 * wait4()-ing process will get the correct exit code - even if this
 * thread is not the thread group leader.
 */
MYOS_SYSCALL_DEFINE1(exit_group, int, error_code)
{
	ALERT_DUMMY_SYSCALL(exit_group, IF_ALERT_DUMMY_SYSCALL);

	do_group_exit((error_code & 0xff) << 8);
	/* NOTREACHED */
	return 0;
}