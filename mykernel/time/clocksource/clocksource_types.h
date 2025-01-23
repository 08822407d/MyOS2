/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_TYPES_H_
#define _LINUX_CLOCKSOURCE_TYPES_H_

	#include "../time_type_declaration.h"


	/**
	 * struct clocksource - hardware abstraction for a free running counter
	 *	Provides mostly state-free accessors to the underlying hardware.
	 *	This is the structure used for system time.
	 *
	 * @read:		Returns a cycle value, passes clocksource as argument
	 * @mask:		Bitmask for two's complement
	 *			subtraction of non 64 bit counters
	 * @mult:		Cycle to nanosecond multiplier
	 * @shift:		Cycle to nanosecond divisor (power of two)
	 * @max_idle_ns:	Maximum idle time permitted by the clocksource (nsecs)
	 * @maxadj:		Maximum adjustment value to mult (~11%)
	 * @uncertainty_margin:	Maximum uncertainty in nanoseconds per half second.
	 *			Zero says to use default WATCHDOG_THRESHOLD.
	 * @archdata:		Optional arch-specific data
	 * @max_cycles:		Maximum safe cycle value which won't overflow on
	 *			multiplication
	 * @name:		Pointer to clocksource name
	 * @list:		List head for registration (internal)
	 * @rating:		Rating value for selection (higher is better)
	 *			To avoid rating inflation the following
	 *			list should give you a guide as to how
	 *			to assign your clocksource a rating
	 *			1-99: Unfit for real use
	 *				Only available for bootup and testing purposes.
	 *			100-199: Base level usability.
	 *				Functional for real use, but not desired.
	 *			200-299: Good.
	 *				A correct and usable clocksource.
	 *			300-399: Desired.
	 *				A reasonably fast and accurate clocksource.
	 *			400-499: Perfect
	 *				The ideal clocksource. A must-use where
	 *				available.
	 * @id:			Defaults to CSID_GENERIC. The id value is captured
	 *			in certain snapshot functions to allow callers to
	 *			validate the clocksource from which the snapshot was
	 *			taken.
	 * @flags:		Flags describing special properties
	 * @enable:		Optional function to enable the clocksource
	 * @disable:		Optional function to disable the clocksource
	 * @suspend:		Optional suspend function for the clocksource
	 * @resume:		Optional resume function for the clocksource
	 * @mark_unstable:	Optional function to inform the clocksource driver that
	 *			the watchdog marked the clocksource unstable
	 * @tick_stable:        Optional function called periodically from the watchdog
	 *			code to provide stable synchronization points
	 * @wd_list:		List head to enqueue into the watchdog list (internal)
	 * @cs_last:		Last clocksource value for clocksource watchdog
	 * @wd_last:		Last watchdog value corresponding to @cs_last
	 * @owner:		Module reference, must be set by clocksource in modules
	 *
	 * Note: This struct is not used in hotpathes of the timekeeping code
	 * because the timekeeper caches the hot path fields in its own data
	 * structure, so no cache line alignment is required,
	 *
	 * The pointer to the clocksource itself is handed to the read
	 * callback. If you need extra information there you can wrap struct
	 * clocksource into your own struct. Depending on the amount of
	 * information you need you should consider to cache line align that
	 * structure.
	 */
	struct clocksource {
		u64				(*read)(clocksrc_s *cs);
		u64				mask;
		u32				mult;
		u32				shift;
		u64				max_idle_ns;
		u32				maxadj;
		u32				uncertainty_margin;
	// #ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
	// 	struct arch_clocksource_data archdata;
	// #endif
		u64				max_cycles;
		const char		*name;
		List_s			list;
		int				rating;
		// enum clocksource_ids	id;
		// enum vdso_clock_mode	vdso_clock_mode;
		ulong			flags;

		int				(*enable)(clocksrc_s *cs);
		void			(*disable)(clocksrc_s *cs);
		void			(*suspend)(clocksrc_s *cs);
		void			(*resume)(clocksrc_s *cs);
		void			(*mark_unstable)(clocksrc_s *cs);
		void			(*tick_stable)(clocksrc_s *cs);

		// /* private: */
	// #ifdef CONFIG_CLOCKSOURCE_WATCHDOG
		// /* Watchdog related data, used by the framework */
		// struct list_head	wd_list;
		// u64			cs_last;
		// u64			wd_last;
	// #endif
		// struct module		*owner;
	};

#endif /* _LINUX_CLOCKSOURCE_TYPES_H_ */