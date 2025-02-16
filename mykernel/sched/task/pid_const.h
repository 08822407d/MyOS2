/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_CONST_H_
#define _LINUX_PID_CONST_H_


	enum pid_type {
		PIDTYPE_PID,
		PIDTYPE_TGID,
		PIDTYPE_PGID,
		PIDTYPE_SID,
		PIDTYPE_MAX,
	};


	#define RESERVED_PIDS		300
	#define PIDNS_ADDING		(1U << 31)

	/*
	 * This controls the default maximum pid allocated to a process
	 */
	#define PID_MAX_DEFAULT (CONFIG_BASE_SMALL ? 0x1000 : 0x8000)

	/*
	 * A maximum of 4 million PIDs should be enough for a while.
	 * [NOTE: PID/TIDs are limited to 2^30 ~= 1 billion, see FUTEX_TID_MASK.]
	 */
	#define PID_MAX_LIMIT (CONFIG_BASE_SMALL ? PAGE_SIZE * 8 : \
				(sizeof(long) > 4 ? 4 * 1024 * 1024 : PID_MAX_DEFAULT))

	/*
	 * Define a minimum number of pids per cpu.  Heuristically based
	 * on original pid max of 32k for 32 cpus.  Also, increase the
	 * minimum settable value for pid_max on the running system based
	 * on similar defaults.  See kernel/pid.c:pid_idr_init() for details.
	 */
	#define PIDS_PER_CPU_DEFAULT	1024
	#define PIDS_PER_CPU_MIN		8


	/*
	 * Bits in flags field of signal_struct.
	 */
	#define SIGNAL_STOP_STOPPED		0x00000001 /* job control stop in effect */
	#define SIGNAL_STOP_CONTINUED	0x00000002 /* SIGCONT since WCONTINUED reap */
	#define SIGNAL_GROUP_EXIT		0x00000004 /* group exit in progress */
	/*
	 * Pending notifications to parent.
	 */
	#define SIGNAL_CLD_STOPPED		0x00000010
	#define SIGNAL_CLD_CONTINUED	0x00000020
	#define SIGNAL_CLD_MASK			(SIGNAL_CLD_STOPPED|SIGNAL_CLD_CONTINUED)

	#define SIGNAL_UNKILLABLE		0x00000040 /* for init: ignore fatal signals */

	#define SIGNAL_STOP_MASK 		(SIGNAL_CLD_MASK | SIGNAL_STOP_STOPPED | \
										SIGNAL_STOP_CONTINUED)
#endif /* _LINUX_PID_CONST_H_ */