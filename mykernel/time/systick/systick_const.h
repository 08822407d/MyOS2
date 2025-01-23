/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SYSTICK_CONST_H_
#define _LINUX_SYSTICK_CONST_H_


	/* TICK_NSEC is the time between ticks in nsec assuming SHIFTED_HZ */
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)

	/*
	 * The following defines establish the engineering parameters of the PLL
	 * model. The HZ variable establishes the timer interrupt frequency, 100 Hz
	 * for the SunOS kernel, 256 Hz for the Ultrix kernel and 1024 Hz for the
	 * OSF/1 kernel. The SHIFT_HZ define expresses the same value as the
	 * nearest power of two in order to avoid hardware multiply operations.
	 */
	#if (HZ >= 12 && HZ < 24)
	#  define SHIFT_HZ	    4
	#elif (HZ >= 24 && HZ < 48)
	#  define SHIFT_HZ	    5
	#elif (HZ >= 48 && HZ < 96)
	#  define SHIFT_HZ	    6
	#elif (HZ >= 96 && HZ < 192)
	#  define SHIFT_HZ	    7
	#elif (HZ >= 192 && HZ < 384)
	#  define SHIFT_HZ	    8
	#elif (HZ >= 384 && HZ < 768)
	#  define SHIFT_HZ	    9
	#elif (HZ >= 768 && HZ < 1536)
	#  define SHIFT_HZ	    10
	#elif (HZ >= 1536 && HZ < 3072)
	#  define SHIFT_HZ	    11
	#elif (HZ >= 3072 && HZ < 6144)
	#  define SHIFT_HZ	    12
	#elif (HZ >= 6144 && HZ < 12288)
	#  define SHIFT_HZ	    13
	#else
	#  error Invalid value of HZ.
	#endif

	/* Suppose we want to divide two numbers NOM and DEN: NOM/DEN, then we can
	 * improve accuracy by shifting LSH bits, hence calculating:
	 *     (NOM << LSH) / DEN
	 * This however means trouble for large NOM, because (NOM << LSH) may no
	 * longer fit in 32 bits. The following way of calculating this gives us
	 * some slack, under the following conditions:
	 *   - (NOM / DEN) fits in (32 - LSH) bits.
	 *   - (NOM % DEN) fits in (32 - LSH) bits.
	 */
	#define SH_DIV(NOM,DEN,LSH) (				\
				(((NOM) / (DEN)) << (LSH)) +	\
				((((NOM) % (DEN)) << (LSH)) + 	\
					(DEN) / 2) /				\
				(DEN)							\
			)

	/* LATCH is used in the interval timer and ftape setup. */
	#define LATCH			((CLOCK_TICK_RATE + HZ/2) / HZ)	/* For divider */

	/* TICK_USEC is the time between ticks in usec assuming SHIFTED_HZ */
	#define TICK_USEC		((USEC_PER_SEC + HZ/2) / HZ)

	/* USER_TICK_USEC is the time between ticks in usec assuming fake USER_HZ */
	#define USER_TICK_USEC	((1000000UL + USER_HZ/2) / USER_HZ)

	/*
	 * Have the 32 bit jiffies value wrap 5 minutes after boot
	 * so jiffies wrap bugs show up earlier.
	 */
	#define INITIAL_JIFFIES ((ulong)(uint) (-300*HZ))

	/*
	 * Change timeval to jiffies, trying to avoid the
	 * most obvious overflows..
	 *
	 * And some not so obvious.
	 *
	 * Note that we don't want to return LONG_MAX, because
	 * for various timeout reasons we often end up having
	 * to wait "jiffies+1" in order to guarantee that we wait
	 * at _least_ "jiffies" - so "jiffies+1" had better still
	 * be positive.
	 */
	#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)

#endif /* _LINUX_SYSTICK_CONST_H_ */