/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_CONST_H_
#define _LINUX_TIMER_CONST_H_

	/* POSIX timers */
	/*
	 * Bit fields within a clockid:
	 *
	 * The most significant 29 bits hold either a pid or a file descriptor.
	 *
	 * Bit 2 indicates whether a cpu clock refers to a thread or a process.
	 *
	 * Bits 1 and 0 give the type: PROF=0, VIRT=1, SCHED=2, or FD=3.
	 *
	 * A clockid is invalid if bits 2, 1, and 0 are all set.
	 */
	#define CPUCLOCK_PID(clock)	(		\
				(pid_t) ~((clock) >> 3)	\
			)
	#define CPUCLOCK_PERTHREAD(clock) (									\
				((clock) & (clockid_t) CPUCLOCK_PERTHREAD_MASK) != 0	\
			)

	#define CPUCLOCK_PERTHREAD_MASK	4
	#define CPUCLOCK_WHICH(clock)	(						\
				(clock) & (clockid_t) CPUCLOCK_CLOCK_MASK	\
			)
	#define CPUCLOCK_CLOCK_MASK		3
	#define CPUCLOCK_PROF			0
	#define CPUCLOCK_VIRT			1
	#define CPUCLOCK_SCHED			2
	#define CPUCLOCK_MAX			3
	#define CLOCKFD					CPUCLOCK_MAX
	#define CLOCKFD_MASK	(								\
				CPUCLOCK_PERTHREAD_MASK|CPUCLOCK_CLOCK_MASK	\
			)

#endif /* _LINUX_TIMER_CONST_H_ */