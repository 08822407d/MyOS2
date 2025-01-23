/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ADJTIME_CONST_H_
#define _LINUX_ADJTIME_CONST_H_


	#define ADJ_ADJTIME				0x8000	/* switch between adjtime/adjtimex modes */
	#define ADJ_OFFSET_SINGLESHOT	0x0001	/* old-fashioned adjtime */
	#define ADJ_OFFSET_READONLY		0x2000	/* read-only adjtime */

	/*
	 * SHIFT_PLL is used as a dampening factor to define how much we
	 * adjust the frequency correction for a given offset in PLL mode.
	 * It also used in dampening the offset correction, to define how
	 * much of the current value in time_offset we correct for each
	 * second. Changing this value changes the stiffness of the ntp
	 * adjustment code. A lower value makes it more flexible, reducing
	 * NTP convergence time. A higher value makes it stiffer, increasing
	 * convergence time, but making the clock more stable.
	 *
	 * In David Mills' nanokernel reference implementation SHIFT_PLL is 4.
	 * However this seems to increase convergence time much too long.
	 *
	 * https://lists.ntp.org/pipermail/hackers/2008-January/003487.html
	 *
	 * In the above mailing list discussion, it seems the value of 4
	 * was appropriate for other Unix systems with HZ=100, and that
	 * SHIFT_PLL should be decreased as HZ increases. However, Linux's
	 * clock steering implementation is HZ independent.
	 *
	 * Through experimentation, a SHIFT_PLL value of 2 was found to allow
	 * for fast convergence (very similar to the NTPv3 code used prior to
	 * v2.6.19), with good clock stability.
	 *
	 *
	 * SHIFT_FLL is used as a dampening factor to define how much we
	 * adjust the frequency correction for a given offset in FLL mode.
	 * In David Mills' nanokernel reference implementation SHIFT_FLL is 2.
	 *
	 * MAXTC establishes the maximum time constant of the PLL.
	 */
	#define SHIFT_PLL			2		/* PLL frequency factor (shift) */
	#define SHIFT_FLL			2		/* FLL frequency factor (shift) */
	#define MAXTC				10		/* maximum time constant (shift) */

	/*
	 * SHIFT_USEC defines the scaling (shift) of the time_freq and
	 * time_tolerance variables, which represent the current frequency
	 * offset and maximum frequency tolerance.
	 */
	#define SHIFT_USEC			16		/* frequency offset scale (shift) */
	#define PPM_SCALE			((s64)NSEC_PER_USEC << (NTP_SCALE_SHIFT - SHIFT_USEC))
	#define PPM_SCALE_INV_SHIFT	19
	#define PPM_SCALE_INV		((1LL << (PPM_SCALE_INV_SHIFT + NTP_SCALE_SHIFT)) / \
									PPM_SCALE + 1)

	#define MAXPHASE			500000000L	/* max phase error (ns) */
	#define MAXFREQ				500000		/* max frequency error (ns/s) */
	#define MAXFREQ_SCALED		((s64)MAXFREQ << NTP_SCALE_SHIFT)
	#define MINSEC				256			/* min interval between updates (s) */
	#define MAXSEC				2048		/* max interval between updates (s) */
	#define NTP_PHASE_LIMIT		((MAXPHASE / NSEC_PER_USEC) << 5) /* beyond max. dispersion */

	#define NTP_SCALE_SHIFT		32

	#define NTP_INTERVAL_FREQ	(HZ)
	#define NTP_INTERVAL_LENGTH	(NSEC_PER_SEC/NTP_INTERVAL_FREQ)

	/* The clock frequency of the i8253/i8254 PIT */
	#define PIT_TICK_RATE		1193182ul

#endif /* _LINUX_ADJTIME_CONST_H_ */