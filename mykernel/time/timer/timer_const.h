/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_CONST_H_
#define _LINUX_TIMER_CONST_H_

	/*
	 * The timer wheel has LVL_DEPTH array levels. Each level provides an array of
	 * LVL_SIZE buckets. Each level is driven by its own clock and therefore each
	 * level has a different granularity.
	 *
	 * The level granularity is:		LVL_CLK_DIV ^ level
	 * The level clock frequency is:	HZ / (LVL_CLK_DIV ^ level)
	 *
	 * The array level of a newly armed timer depends on the relative expiry
	 * time. The farther the expiry time is away the higher the array level and
	 * therefore the granularity becomes.
	 *
	 * Contrary to the original timer wheel implementation, which aims for 'exact'
	 * expiry of the timers, this implementation removes the need for recascading
	 * the timers into the lower array levels. The previous 'classic' timer wheel
	 * implementation of the kernel already violated the 'exact' expiry by adding
	 * slack to the expiry time to provide batched expiration. The granularity
	 * levels provide implicit batching.
	 *
	 * This is an optimization of the original timer wheel implementation for the
	 * majority of the timer wheel use cases: timeouts. The vast majority of
	 * timeout timers (networking, disk I/O ...) are canceled before expiry. If
	 * the timeout expires it indicates that normal operation is disturbed, so it
	 * does not matter much whether the timeout comes with a slight delay.
	 *
	 * The only exception to this are networking timers with a small expiry
	 * time. They rely on the granularity. Those fit into the first wheel level,
	 * which has HZ granularity.
	 *
	 * We don't have cascading anymore. timers with a expiry time above the
	 * capacity of the last wheel level are force expired at the maximum timeout
	 * value of the last wheel level. From data sampling we know that the maximum
	 * value observed is 5 days (network connection tracking), so this should not
	 * be an issue.
	 *
	 * The currently chosen array constants values are a good compromise between
	 * array size and granularity.
	 *
	 * This results in the following granularity and range levels:
	 *
	 * HZ 1000 steps
	 * Level Offset  Granularity            Range
	 *  0      0         1 ms                0 ms -         63 ms
	 *  1     64         8 ms               64 ms -        511 ms
	 *  2    128        64 ms              512 ms -       4095 ms (512ms - ~4s)
	 *  3    192       512 ms             4096 ms -      32767 ms (~4s - ~32s)
	 *  4    256      4096 ms (~4s)      32768 ms -     262143 ms (~32s - ~4m)
	 *  5    320     32768 ms (~32s)    262144 ms -    2097151 ms (~4m - ~34m)
	 *  6    384    262144 ms (~4m)    2097152 ms -   16777215 ms (~34m - ~4h)
	 *  7    448   2097152 ms (~34m)  16777216 ms -  134217727 ms (~4h - ~1d)
	 *  8    512  16777216 ms (~4h)  134217728 ms - 1073741822 ms (~1d - ~12d)
	 *
	 * HZ  300
	 * Level Offset  Granularity            Range
	 *  0	   0         3 ms                0 ms -        210 ms
	 *  1	  64        26 ms              213 ms -       1703 ms (213ms - ~1s)
	 *  2	 128       213 ms             1706 ms -      13650 ms (~1s - ~13s)
	 *  3	 192      1706 ms (~1s)      13653 ms -     109223 ms (~13s - ~1m)
	 *  4	 256     13653 ms (~13s)    109226 ms -     873810 ms (~1m - ~14m)
	 *  5	 320    109226 ms (~1m)     873813 ms -    6990503 ms (~14m - ~1h)
	 *  6	 384    873813 ms (~14m)   6990506 ms -   55924050 ms (~1h - ~15h)
	 *  7	 448   6990506 ms (~1h)   55924053 ms -  447392423 ms (~15h - ~5d)
	 *  8    512  55924053 ms (~15h) 447392426 ms - 3579139406 ms (~5d - ~41d)
	 *
	 * HZ  250
	 * Level Offset  Granularity            Range
	 *  0	   0         4 ms                0 ms -        255 ms
	 *  1	  64        32 ms              256 ms -       2047 ms (256ms - ~2s)
	 *  2	 128       256 ms             2048 ms -      16383 ms (~2s - ~16s)
	 *  3	 192      2048 ms (~2s)      16384 ms -     131071 ms (~16s - ~2m)
	 *  4	 256     16384 ms (~16s)    131072 ms -    1048575 ms (~2m - ~17m)
	 *  5	 320    131072 ms (~2m)    1048576 ms -    8388607 ms (~17m - ~2h)
	 *  6	 384   1048576 ms (~17m)   8388608 ms -   67108863 ms (~2h - ~18h)
	 *  7	 448   8388608 ms (~2h)   67108864 ms -  536870911 ms (~18h - ~6d)
	 *  8    512  67108864 ms (~18h) 536870912 ms - 4294967288 ms (~6d - ~49d)
	 *
	 * HZ  100
	 * Level Offset  Granularity            Range
	 *  0	   0         10 ms               0 ms -        630 ms
	 *  1	  64         80 ms             640 ms -       5110 ms (640ms - ~5s)
	 *  2	 128        640 ms            5120 ms -      40950 ms (~5s - ~40s)
	 *  3	 192       5120 ms (~5s)     40960 ms -     327670 ms (~40s - ~5m)
	 *  4	 256      40960 ms (~40s)   327680 ms -    2621430 ms (~5m - ~43m)
	 *  5	 320     327680 ms (~5m)   2621440 ms -   20971510 ms (~43m - ~5h)
	 *  6	 384    2621440 ms (~43m) 20971520 ms -  167772150 ms (~5h - ~1d)
	 *  7	 448   20971520 ms (~5h) 167772160 ms - 1342177270 ms (~1d - ~15d)
	 */

	/* Clock divisor for the next level */
	#define LVL_CLK_SHIFT	3
	#define LVL_CLK_DIV		(1UL << LVL_CLK_SHIFT)
	#define LVL_CLK_MASK	(LVL_CLK_DIV - 1)
	#define LVL_SHIFT(n)	((n) * LVL_CLK_SHIFT)
	#define LVL_GRAN(n)		(1UL << LVL_SHIFT(n))

	/*
	 * The time start value for each level to select the bucket at enqueue
	 * time. We start from the last possible delta of the previous level
	 * so that we can later add an extra LVL_GRAN(n) to n (see calc_index()).
	 */
	#define LVL_START(n)	((LVL_SIZE - 1) << (((n) - 1) * LVL_CLK_SHIFT))

	/* Size of each clock level */
	#define LVL_BITS		6
	#define LVL_SIZE		(1UL << LVL_BITS)
	#define LVL_MASK		(LVL_SIZE - 1)
	#define LVL_OFFS(n)		((n) * LVL_SIZE)

	/* Level depth */
	#if HZ > 100
	#  define LVL_DEPTH		9
	#else
	#  define LVL_DEPTH		8
	#endif

	/* The cutoff (max. capacity of the wheel) */
	#define WHEEL_TIMEOUT_CUTOFF	(LVL_START(LVL_DEPTH))
	#define WHEEL_TIMEOUT_MAX		(WHEEL_TIMEOUT_CUTOFF - LVL_GRAN(LVL_DEPTH - 1))

	/*
	 * The resulting wheel size. If NOHZ is configured we allocate two
	 * wheels so we have a separate storage for the deferrable timers.
	 */
	#define WHEEL_SIZE		(LVL_SIZE * LVL_DEPTH)


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


	#define MOD_TIMER_PENDING_ONLY	0x01
	#define MOD_TIMER_REDUCE		0x02
	#define MOD_TIMER_NOTPENDING	0x04

#endif /* _LINUX_TIMER_CONST_H_ */