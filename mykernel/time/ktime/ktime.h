/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KTIME_H_
#define _LINUX_KTIME_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern ktime_t
		ktime_set(const s64 secs, const ulong nsecs);

		extern ktime_t
		timespec64_to_ktime(timespec64_s ts);

		extern s64
		ktime_to_ns(const ktime_t kt);

		extern int
		ktime_compare(const ktime_t cmp1, const ktime_t cmp2);

		extern bool
		ktime_after(const ktime_t cmp1, const ktime_t cmp2);

		extern bool
		ktime_before(const ktime_t cmp1, const ktime_t cmp2);

		extern s64
		ktime_divns(const ktime_t kt, s64 div);

		extern s64
		ktime_divns(const ktime_t kt, s64 div);

	#endif

	#include "ktime_macro.h"
	
	#if defined(KTIME_DEFINATION) || !(DEBUG)

		/**
		 * ktime_set - Set a ktime_t variable from a seconds/nanoseconds value
		 * @secs:	seconds to set
		 * @nsecs:	nanoseconds to set
		 *
		 * Return: The ktime_t representation of the value.
		 */
		PREFIX_STATIC_INLINE
		ktime_t
		ktime_set(const s64 secs, const ulong nsecs) {
			if (unlikely(secs >= KTIME_SEC_MAX))
				return KTIME_MAX;

			return secs * NSEC_PER_SEC + (s64)nsecs;
		}

		/* convert a timespec64 to ktime_t format: */
		PREFIX_STATIC_INLINE
		ktime_t
		timespec64_to_ktime(timespec64_s ts) {
			return ktime_set(ts.tv_sec, ts.tv_nsec);
		}

		/* Convert ktime_t to nanoseconds */
		PREFIX_STATIC_INLINE
		s64
		ktime_to_ns(const ktime_t kt) {
			return kt;
		}

		/**
		 * ktime_compare - Compares two ktime_t variables for less, greater or equal
		 * @cmp1:	comparable1
		 * @cmp2:	comparable2
		 *
		 * Return: ...
		 *   cmp1  < cmp2: return <0
		 *   cmp1 == cmp2: return 0
		 *   cmp1  > cmp2: return >0
		 */
		PREFIX_STATIC_INLINE
		int
		ktime_compare(const ktime_t cmp1, const ktime_t cmp2) {
			if (cmp1 < cmp2)
				return -1;
			if (cmp1 > cmp2)
				return 1;
			return 0;
		}

		/**
		 * ktime_after - Compare if a ktime_t value is bigger than another one.
		 * @cmp1:	comparable1
		 * @cmp2:	comparable2
		 *
		 * Return: true if cmp1 happened after cmp2.
		 */
		PREFIX_STATIC_INLINE
		bool
		ktime_after(const ktime_t cmp1, const ktime_t cmp2) {
			return ktime_compare(cmp1, cmp2) > 0;
		}

		/**
		 * ktime_before - Compare if a ktime_t value is smaller than another one.
		 * @cmp1:	comparable1
		 * @cmp2:	comparable2
		 *
		 * Return: true if cmp1 happened before cmp2.
		 */
		PREFIX_STATIC_INLINE
		bool
		ktime_before(const ktime_t cmp1, const ktime_t cmp2) {
			return ktime_compare(cmp1, cmp2) < 0;
		}

	#if BITS_PER_LONG != 64
	#  error "\'BITS_PER_LONG\' Not Equal to 64"
	#endif
		PREFIX_STATIC_INLINE
		s64
		ktime_divns(const ktime_t kt, s64 div) {
			/*
			 * 32-bit implementation cannot handle negative divisors,
			 * so catch them on 64bit as well.
			 */
			WARN_ON(div < 0);
			return kt / div;
		}

	#endif

#endif /* _LINUX_KTIME_H_ */