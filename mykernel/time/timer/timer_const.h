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

	/*
	 * @TIMER_DEFERRABLE: A deferrable timer will work normally when the
	 * system is busy, but will not cause a CPU to come out of idle just
	 * to service it; instead, the timer will be serviced when the CPU
	 * eventually wakes up with a subsequent non-deferrable timer.
	 *
	 * @TIMER_IRQSAFE: An irqsafe timer is executed with IRQ disabled and
	 * it's safe to wait for the completion of the running instance from
	 * IRQ handlers, for example, by calling del_timer_sync().
	 *
	 * Note: The irq disabled callback execution is a special case for
	 * workqueue locking issues. It's not meant for executing random crap
	 * with interrupts disabled. Abuse is monitored!
	 *
	 * @TIMER_PINNED: A pinned timer will always expire on the CPU on which the
	 * timer was enqueued. When a particular CPU is required, add_timer_on()
	 * has to be used. Enqueue via mod_timer() and add_timer() is always done
	 * on the local CPU.
	 */
	#define TIMER_CPUMASK			0x0003FFFF
	#define TIMER_MIGRATING			0x00040000
	#define TIMER_BASEMASK			(				\
				TIMER_CPUMASK | TIMER_MIGRATING		\
			)
	#define TIMER_DEFERRABLE		0x00080000
	#define TIMER_PINNED			0x00100000
	#define TIMER_IRQSAFE			0x00200000
	#define TIMER_INIT_FLAGS		(				\
				TIMER_DEFERRABLE | TIMER_PINNED |	\
				TIMER_IRQSAFE						\
			)
	#define TIMER_ARRAYSHIFT		22
	#define TIMER_ARRAYMASK			0xFFC00000

	#define TIMER_TRACE_FLAGMASK	(				\
				TIMER_MIGRATING | TIMER_DEFERRABLE |\
				TIMER_PINNED | TIMER_IRQSAFE		\
			)

#endif /* _LINUX_TIMER_CONST_H_ */