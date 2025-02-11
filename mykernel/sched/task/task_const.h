/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_CONST_H_
#define _LINUX_TASK_CONST_H_


	/*
	 * Define the task command name length as enum, then it can be visible to
	 * BPF programs.
	 */
	enum {
		TASK_COMM_LEN = 32,
	};

	/*
	 * Task state bitmask. NOTE! These bits are also
	 * encoded in fs/proc/array.c: get_task_state().
	 *
	 * We have two separate sets of flags: task->__state
	 * is about runnability, while task->exit_state are
	 * about the task exiting. Confusing, but this way
	 * modifying one set can't modify the other one by
	 * mistake.
	 */

	/* Used in tsk->__state: */
	#define TASK_RUNNING			0x00000000
	#define TASK_INTERRUPTIBLE		0x00000001
	#define TASK_UNINTERRUPTIBLE	0x00000002
	#define __TASK_STOPPED			0x00000004
	#define __TASK_TRACED			0x00000008
	/* Used in tsk->exit_state: */
	#define EXIT_DEAD				0x00000010
	#define EXIT_ZOMBIE				0x00000020
	#define EXIT_TRACE				(EXIT_ZOMBIE | EXIT_DEAD)
	/* Used in tsk->__state again: */
	#define TASK_PARKED				0x00000040
	#define TASK_DEAD				0x00000080
	#define TASK_WAKEKILL			0x00000100
	#define TASK_WAKING				0x00000200
	#define TASK_NOLOAD				0x00000400
	#define TASK_NEW				0x00000800
	#define TASK_RTLOCK_WAIT		0x00001000
	#define TASK_FREEZABLE			0x00002000
	#define __TASK_FREEZABLE_UNSAFE	(						\
				0x00004000 * IS_ENABLED(CONFIG_LOCKDEP)		\
			)
	#define TASK_FROZEN				0x00008000
	#define TASK_STATE_MAX			0x00010000

	#define TASK_ANY				(TASK_STATE_MAX-1)

	/*
	* DO NOT ADD ANY NEW USERS !
	*/
	#define TASK_FREEZABLE_UNSAFE	(						\
				TASK_FREEZABLE | __TASK_FREEZABLE_UNSAFE	\
			)

	/* Convenience macros for the sake of set_current_state: */
	#define TASK_KILLABLE			(						\
				TASK_WAKEKILL | TASK_UNINTERRUPTIBLE		\
			)
	#define TASK_STOPPED			(						\
				TASK_WAKEKILL | __TASK_STOPPED				\
			)
	#define TASK_TRACED				__TASK_TRACED

	#define TASK_IDLE				(						\
				TASK_UNINTERRUPTIBLE | TASK_NOLOAD			\
			)

	/* Convenience macros for the sake of wake_up(): */
	#define TASK_NORMAL				(						\
				TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE	\
			)

	/* get_task_state(): */
	#define TASK_REPORT				(						\
				TASK_RUNNING | TASK_INTERRUPTIBLE |			\
				TASK_UNINTERRUPTIBLE | __TASK_STOPPED |		\
				__TASK_TRACED | EXIT_DEAD | EXIT_ZOMBIE |	\
				TASK_PARKED									\
			)


	#define MAX_SCHEDULE_TIMEOUT	LONG_MAX

	/*
	 * Per process flags
	 */
	#define PF_VCPU					0x00000001	/* I'm a virtual CPU */
	#define PF_IDLE					0x00000002	/* I am an IDLE thread */
	#define PF_EXITING				0x00000004	/* Getting shut down */
	#define PF_POSTCOREDUMP			0x00000008	/* Coredumps should ignore this task */
	#define PF_IO_WORKER			0x00000010	/* Task is an IO worker */
	#define PF_WQ_WORKER			0x00000020	/* I'm a workqueue worker */
	#define PF_FORKNOEXEC			0x00000040	/* Forked but didn't exec */
	#define PF_MCE_PROCESS			0x00000080	/* Process policy on mce errors */
	#define PF_SUPERPRIV			0x00000100	/* Used super-user privileges */
	#define PF_DUMPCORE				0x00000200	/* Dumped core */
	#define PF_SIGNALED				0x00000400	/* Killed by a signal */
	#define PF_MEMALLOC				0x00000800	/* Allocating memory */
	#define PF_NPROC_EXCEEDED		0x00001000	/* set_user() noticed that RLIMIT_NPROC was exceeded */
	#define PF_USED_MATH			0x00002000	/* If unset the fpu must be initialized before use */
	#define PF_NOFREEZE				0x00008000	/* This thread should not be frozen */
	#define PF_FROZEN				0x00010000	/* Frozen for system suspend */
	#define PF_KSWAPD				0x00020000	/* I am kswapd */
	#define PF_MEMALLOC_NOFS		0x00040000	/* All allocation requests will inherit GFP_NOFS */
	#define PF_MEMALLOC_NOIO		0x00080000	/* All allocation requests will inherit GFP_NOIO */
	#define PF_LOCAL_THROTTLE		0x00100000	/* Throttle writes only against the bdi I write to,
												 * I am cleaning dirty pages from some other bdi. */
	#define PF_KTHREAD				0x00200000	/* I am a kernel thread */
	#define PF_RANDOMIZE			0x00400000	/* Randomize virtual address space */
	#define PF_SWAPWRITE			0x00800000	/* Allowed to write to swap */
	#define PF_NO_SETAFFINITY		0x04000000	/* Userland is not allowed to meddle with cpus_mask */
	#define PF_MCE_EARLY			0x08000000	/* Early kill for mce process policy */
	#define PF_MEMALLOC_PIN			0x10000000	/* Allocation context constrained to zones which
												 * allow long term pinning. */
	#define PF_FREEZER_SKIP			0x40000000	/* Freezer should not count it as freezable */
	#define PF_SUSPEND_TASK			0x80000000	/* This thread called freeze_processes() and should
												 * not be frozen */

#endif /* _LINUX_TASK_CONST_H_ */