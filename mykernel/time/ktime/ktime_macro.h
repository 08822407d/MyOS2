/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KTIME_MACRO_H_
#define _LINUX_KTIME_MACRO_H_


	/* Subtract two ktime_t variables. rem = lhs -rhs: */
	#define ktime_sub(lhs, rhs)		((lhs) - (rhs))

	/* Add two ktime_t variables. res = lhs + rhs: */
	#define ktime_add(lhs, rhs)		((lhs) + (rhs))

	/*
	 * Same as ktime_add(), but avoids undefined behaviour on overflow; however,
	 * this means that you must check the result for overflow yourself.
	 */
	#define ktime_add_unsafe(lhs, rhs)	((u64) (lhs) + (rhs))

	/*
	 * Add a ktime_t variable and a scalar nanosecond value.
	 * res = kt + nsval:
	 */
	#define ktime_add_ns(kt, nsval)		((kt) + (nsval))

	/*
	 * Subtract a scalar nanosecod from a ktime_t variable
	 * res = kt - nsval:
	 */
	#define ktime_sub_ns(kt, nsval)		((kt) - (nsval))

	/* Map the ktime_t to timespec conversion to ns_to_timespec function */
	#define ktime_to_timespec64(kt)		ns_to_timespec64((kt))

#endif /* _LINUX_KTIME_MACRO_H_ */