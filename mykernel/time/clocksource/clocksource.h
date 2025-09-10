/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_H_
#define _LINUX_CLOCKSOURCE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../time_const.h"
	#include "../time_types.h"
	#include "../time_api.h"


	#ifdef DEBUG

		extern u32
		clocksource_freq2mult(u32 freq, u32 shift_constant, u64 from);

		extern u32
		clocksource_khz2mult(u32 khz, u32 shift_constant);

		extern u32
		clocksource_hz2mult(u32 hz, u32 shift_constant);

		extern s64
		clocksource_cyc2ns(u64 cycles, u32 mult, u32 shift);

	#endif

	#include "clocksource_macro.h"
	
	#if defined(CLOCKSOURCE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		u32
		clocksource_freq2mult(u32 freq, u32 shift_constant, u64 from) {
			/*  freq = cyc/from
			 *  mult/2^shift  = ns/cyc
			 *  mult = ns/cyc * 2^shift
			 *  mult = from/freq * 2^shift
			 *  mult = from * 2^shift / freq
			 *  mult = (from<<shift) / freq
			 */
			u64 tmp = ((u64)from) << shift_constant;

			tmp += freq/2; /* round for do_div */
			do_div(tmp, freq);

			return (u32)tmp;
		}

		/**
		 * clocksource_khz2mult - calculates mult from khz and shift
		 * @khz:		Clocksource frequency in KHz
		 * @shift_constant:	Clocksource shift factor
		 *
		 * Helper functions that converts a khz counter frequency to a timsource
		 * multiplier, given the clocksource shift value
		 */
		PREFIX_STATIC_INLINE
		u32
		clocksource_khz2mult(u32 khz, u32 shift_constant) {
			return clocksource_freq2mult(khz, shift_constant, NSEC_PER_MSEC);
		}

		/**
		 * clocksource_hz2mult - calculates mult from hz and shift
		 * @hz:			Clocksource frequency in Hz
		 * @shift_constant:	Clocksource shift factor
		 *
		 * Helper functions that converts a hz counter
		 * frequency to a timsource multiplier, given the
		 * clocksource shift value
		 */
		PREFIX_STATIC_INLINE
		u32
		clocksource_hz2mult(u32 hz, u32 shift_constant) {
			return clocksource_freq2mult(hz, shift_constant, NSEC_PER_SEC);
		}

		/**
		 * clocksource_cyc2ns - converts clocksource cycles to nanoseconds
		 * @cycles:	cycles
		 * @mult:	cycle to nanosecond multiplier
		 * @shift:	cycle to nanosecond divisor (power of two)
		 *
		 * Converts clocksource cycles to nanoseconds, using the given @mult and @shift.
		 * The code is optimized for performance and is not intended to work
		 * with absolute clocksource cycles (as those will easily overflow),
		 * but is only intended to be used with relative (delta) clocksource cycles.
		 *
		 * XXX - This could use some mult_lxl_ll() asm optimization
		 */
		PREFIX_STATIC_INLINE
		s64
		clocksource_cyc2ns(u64 cycles, u32 mult, u32 shift) {
			return ((u64) cycles * mult) >> shift;
		}

	#endif

#endif /* _LINUX_CLOCKSOURCE_H_ */