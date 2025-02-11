/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SYSTICK_H_
#define _LINUX_SYSTICK_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern u64
		jiffies_to_nsecs(const ulong j);

		extern ulong
		_msecs_to_jiffies(const uint m);

		extern ulong
		msecs_to_jiffies(const uint m);

		extern ulong
		_usecs_to_jiffies(const uint u);

		extern ulong
		usecs_to_jiffies(const uint u);

	#endif

	#include "systick_macro.h"
	
	#if defined(SYSTICK_DEFINATION) || !(DEBUG)

		/**
		 * jiffies_to_nsecs - Convert jiffies to nanoseconds
		 * @j: jiffies value
		 *
		 * Return: nanoseconds value
		 */
		PREFIX_STATIC_INLINE
		u64
		jiffies_to_nsecs(const ulong j) {
			return (u64)jiffies_to_usecs(j) * NSEC_PER_USEC;
		}

		#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
			/*
			 * HZ is equal to or smaller than 1000, and 1000 is a nice round
			 * multiple of HZ, divide with the factor between them, but round
			 * upwards:
			 */
			PREFIX_STATIC_INLINE
			ulong
			_msecs_to_jiffies(const uint m) {
				return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
			}
		#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
			/*
			 * HZ is larger than 1000, and HZ is a nice round multiple of 1000 -
			 * simply multiply with the factor between them.
			 *
			 * But first make sure the multiplication result cannot overflow:
			 */
			PREFIX_STATIC_INLINE
			ulong
			_msecs_to_jiffies(const uint m) {
				if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
					return MAX_JIFFY_OFFSET;
				return m * (HZ / MSEC_PER_SEC);
			}
		#else
			/*
			 * Generic case - multiply, round and divide. But first check that if
			 * we are doing a net multiplication, that we wouldn't overflow:
			 */
			PREFIX_STATIC_INLINE
			ulong
			_msecs_to_jiffies(const uint m) {
				if (HZ > MSEC_PER_SEC && m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
					return MAX_JIFFY_OFFSET;

				return (MSEC_TO_HZ_MUL32 * m + MSEC_TO_HZ_ADJ32) >> MSEC_TO_HZ_SHR32;
			}
		#endif
		/**
		 * msecs_to_jiffies: - convert milliseconds to jiffies
		 * @m:	time in milliseconds
		 *
		 * conversion is done as follows:
		 *
		 * - negative values mean 'infinite timeout' (MAX_JIFFY_OFFSET)
		 *
		 * - 'too large' values [that would result in larger than
		 *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
		 *
		 * - all other values are converted to jiffies by either multiplying
		 *   the input value by a factor or dividing it with a factor and
		 *   handling any 32-bit overflows.
		 *   for the details see __msecs_to_jiffies()
		 *
		 * msecs_to_jiffies() checks for the passed in value being a constant
		 * via __builtin_constant_p() allowing gcc to eliminate most of the
		 * code. __msecs_to_jiffies() is called if the value passed does not
		 * allow constant folding and the actual conversion must be done at
		 * runtime.
		 * The HZ range specific helpers _msecs_to_jiffies() are called both
		 * directly here and from __msecs_to_jiffies() in the case where
		 * constant folding is not possible.
		 *
		 * Return: jiffies value
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		ulong
		msecs_to_jiffies(const uint m) {
			if (__builtin_constant_p(m)) {
				if ((int)m < 0)
					return MAX_JIFFY_OFFSET;
				return _msecs_to_jiffies(m);
			} else {
				return __msecs_to_jiffies(m);
			}
		}

		#if !(USEC_PER_SEC % HZ)
			PREFIX_STATIC_INLINE
			ulong
			_usecs_to_jiffies(const uint u) {
				return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
			}
		#else
			PREFIX_STATIC_INLINE
			ulong
			_usecs_to_jiffies(const uint u) {
				return (USEC_TO_HZ_MUL32 * u + USEC_TO_HZ_ADJ32)
					>> USEC_TO_HZ_SHR32;
			}
		#endif

		/**
		 * usecs_to_jiffies: - convert microseconds to jiffies
		 * @u:	time in microseconds
		 *
		 * conversion is done as follows:
		 *
		 * - 'too large' values [that would result in larger than
		 *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
		 *
		 * - all other values are converted to jiffies by either multiplying
		 *   the input value by a factor or dividing it with a factor and
		 *   handling any 32-bit overflows as for msecs_to_jiffies.
		 *
		 * usecs_to_jiffies() checks for the passed in value being a constant
		 * via __builtin_constant_p() allowing gcc to eliminate most of the
		 * code. __usecs_to_jiffies() is called if the value passed does not
		 * allow constant folding and the actual conversion must be done at
		 * runtime.
		 * The HZ range specific helpers _usecs_to_jiffies() are called both
		 * directly here and from __msecs_to_jiffies() in the case where
		 * constant folding is not possible.
		 *
		 * Return: jiffies value
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		ulong
		usecs_to_jiffies(const uint u) {
			if (__builtin_constant_p(u)) {
				if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
					return MAX_JIFFY_OFFSET;
				return _usecs_to_jiffies(u);
			} else {
				return __usecs_to_jiffies(u);
			}
		}

	#endif

#endif /* _LINUX_ADJTIME_H_ */