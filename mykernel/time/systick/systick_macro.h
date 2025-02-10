/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SYSTICK_MACRO_H_
#define _LINUX_SYSTICK_MACRO_H_

	/**
	 * DOC: General information about time_* inlines
	 *
	 * These inlines deal with timer wrapping correctly. You are strongly encouraged
	 * to use them:
	 *
	 * #. Because people otherwise forget
	 * #. Because if the timer wrap changes in future you won't have to alter your
	 *    driver code.
	 */

	/**
	 * time_after - returns true if the time a is after time b.
	 * @a: first comparable as unsigned long
	 * @b: second comparable as unsigned long
	 *
	 * Do this with "<0" and ">=0" to only test the sign of the result. A
	 * good compiler would generate better code (and a really good compiler
	 * wouldn't care). Gcc is currently neither.
	 *
	 * Return: %true is time a is after time b, otherwise %false.
	 */
	#define time_after(a,b)	(				\
				typecheck(ulong, a) &&		\
				typecheck(ulong, b) &&		\
				((long)((b) - (a)) < 0)		\
			)
	/**
	 * time_before - returns true if the time a is before time b.
	 * @a: first comparable as unsigned long
	 * @b: second comparable as unsigned long
	 *
	 * Return: %true is time a is before time b, otherwise %false.
	 */
	#define time_before(a,b)	time_after(b,a)

	/**
	 * time_after_eq - returns true if the time a is after or the same as time b.
	 * @a: first comparable as unsigned long
	 * @b: second comparable as unsigned long
	 *
	 * Return: %true is time a is after or the same as time b, otherwise %false.
	 */
	#define time_after_eq(a,b)	(			\
				typecheck(ulong, a) &&		\
				typecheck(ulong, b) &&		\
				((long)((a) - (b)) >= 0)	\
			)
	/**
	 * time_before_eq - returns true if the time a is before or the same as time b.
	 * @a: first comparable as unsigned long
	 * @b: second comparable as unsigned long
	 *
	 * Return: %true is time a is before or the same as time b, otherwise %false.
	 */
	#define time_before_eq(a,b)	time_after_eq(b,a)

	/**
	 * time_in_range - Calculate whether a is in the range of [b, c].
	 * @a: time to test
	 * @b: beginning of the range
	 * @c: end of the range
	 *
	 * Return: %true is time a is in the range [b, c], otherwise %false.
	 */
	#define time_in_range(a,b,c)	(	\
				time_after_eq(a,b) &&	\
				time_before_eq(a,c)		\
			)

	/**
	 * time_in_range_open - Calculate whether a is in the range of [b, c).
	 * @a: time to test
	 * @b: beginning of the range
	 * @c: end of the range
	 *
	 * Return: %true is time a is in the range [b, c), otherwise %false.
	 */
	#define time_in_range_open(a,b,c)	(	\
				time_after_eq(a,b) && 		\
				time_before(a,c)			\
			)

	/* Same as above, but does so with platform independent 64bit types.
	* These must be used when utilizing jiffies_64 (i.e. return value of
	* get_jiffies_64()). */

	/**
	 * time_after64 - returns true if the time a is after time b.
	 * @a: first comparable as __u64
	 * @b: second comparable as __u64
	 *
	 * This must be used when utilizing jiffies_64 (i.e. return value of
	 * get_jiffies_64()).
	 *
	 * Return: %true is time a is after time b, otherwise %false.
	 */
	#define time_after64(a,b)	(			\
				typecheck(__u64, a) &&		\
				typecheck(__u64, b) &&		\
				((__s64)((b) - (a)) < 0)	\
			)
	/**
	 * time_before64 - returns true if the time a is before time b.
	 * @a: first comparable as __u64
	 * @b: second comparable as __u64
	 *
	 * This must be used when utilizing jiffies_64 (i.e. return value of
	 * get_jiffies_64()).
	 *
	 * Return: %true is time a is before time b, otherwise %false.
	 */
	#define time_before64(a,b)	time_after64(b,a)

	/**
	 * time_after_eq64 - returns true if the time a is after or the same as time b.
	 * @a: first comparable as __u64
	 * @b: second comparable as __u64
	 *
	 * This must be used when utilizing jiffies_64 (i.e. return value of
	 * get_jiffies_64()).
	 *
	 * Return: %true is time a is after or the same as time b, otherwise %false.
	 */
	#define time_after_eq64(a,b)	(		\
				typecheck(__u64, a) &&		\
				typecheck(__u64, b) &&		\
				((__s64)((a) - (b)) >= 0)	\
			)
	/**
	 * time_before_eq64 - returns true if the time a is before or the same as time b.
	 * @a: first comparable as __u64
	 * @b: second comparable as __u64
	 *
	 * This must be used when utilizing jiffies_64 (i.e. return value of
	 * get_jiffies_64()).
	 *
	 * Return: %true is time a is before or the same as time b, otherwise %false.
	 */
	#define time_before_eq64(a,b)	time_after_eq64(b,a)

	/**
	 * time_in_range64 - Calculate whether a is in the range of [b, c].
	 * @a: time to test
	 * @b: beginning of the range
	 * @c: end of the range
	 *
	 * Return: %true is time a is in the range [b, c], otherwise %false.
	 */
	#define time_in_range64(a, b, c)	(	\
				time_after_eq64(a, b) &&	\
				time_before_eq64(a, c)		\
			)

	/*
	* These eight macros compare jiffies[_64] and 'a' for convenience.
	*/

	/**
	 * time_is_before_jiffies - return true if a is before jiffies
	 * @a: time (unsigned long) to compare to jiffies
	 *
	 * Return: %true is time a is before jiffies, otherwise %false.
	 */
	#define time_is_before_jiffies(a)	time_after(jiffies, a)
	/**
	 * time_is_before_jiffies64 - return true if a is before jiffies_64
	 * @a: time (__u64) to compare to jiffies_64
	 *
	 * Return: %true is time a is before jiffies_64, otherwise %false.
	 */
	#define time_is_before_jiffies64(a)	time_after64(get_jiffies_64(), a)

	/**
	 * time_is_after_jiffies - return true if a is after jiffies
	 * @a: time (unsigned long) to compare to jiffies
	 *
	 * Return: %true is time a is after jiffies, otherwise %false.
	 */
	#define time_is_after_jiffies(a)	time_before(jiffies, a)
	/**
	 * time_is_after_jiffies64 - return true if a is after jiffies_64
	 * @a: time (__u64) to compare to jiffies_64
	 *
	 * Return: %true is time a is after jiffies_64, otherwise %false.
	 */
	#define time_is_after_jiffies64(a)	time_before64(get_jiffies_64(), a)

	/**
	 * time_is_before_eq_jiffies - return true if a is before or equal to jiffies
	 * @a: time (unsigned long) to compare to jiffies
	 *
	 * Return: %true is time a is before or the same as jiffies, otherwise %false.
	 */
	#define time_is_before_eq_jiffies(a)	time_after_eq(jiffies, a)
	/**
	 * time_is_before_eq_jiffies64 - return true if a is before or equal to jiffies_64
	 * @a: time (__u64) to compare to jiffies_64
	 *
	 * Return: %true is time a is before or the same jiffies_64, otherwise %false.
	 */
	#define time_is_before_eq_jiffies64(a)	time_after_eq64(get_jiffies_64(), a)

	/**
	 * time_is_after_eq_jiffies - return true if a is after or equal to jiffies
	 * @a: time (unsigned long) to compare to jiffies
	 *
	 * Return: %true is time a is after or the same as jiffies, otherwise %false.
	 */
	#define time_is_after_eq_jiffies(a)		time_before_eq(jiffies, a)
	/**
	 * time_is_after_eq_jiffies64 - return true if a is after or equal to jiffies_64
	 * @a: time (__u64) to compare to jiffies_64
	 *
	 * Return: %true is time a is after or the same as jiffies_64, otherwise %false.
	 */
	#define time_is_after_eq_jiffies64(a)	time_before_eq64(get_jiffies_64(), a)

	/*
	* Have the 32-bit jiffies value wrap 5 minutes after boot
	* so jiffies wrap bugs show up earlier.
	*/
	#define INITIAL_JIFFIES		((ulong)(uint) (-300*HZ))

#endif /* _LINUX_SYSTICK_MACRO_H_ */