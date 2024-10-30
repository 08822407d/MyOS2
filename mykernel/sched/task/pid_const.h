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

#endif /* _LINUX_PID_CONST_H_ */