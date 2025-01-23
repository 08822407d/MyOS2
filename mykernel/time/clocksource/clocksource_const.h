/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_CONST_H_
#define _LINUX_CLOCKSOURCE_CONST_H_


	/*
	 * Clock source flags bits::
	 */
	#define CLOCK_SOURCE_IS_CONTINUOUS		0x01
	#define CLOCK_SOURCE_MUST_VERIFY		0x02

	#define CLOCK_SOURCE_WATCHDOG			0x10
	#define CLOCK_SOURCE_VALID_FOR_HRES		0x20
	#define CLOCK_SOURCE_UNSTABLE			0x40
	#define CLOCK_SOURCE_SUSPEND_NONSTOP	0x80
	#define CLOCK_SOURCE_RESELECT			0x100
	#define CLOCK_SOURCE_VERIFY_PERCPU		0x200
	/* simplify initialization of mask field */
	#define CLOCKSOURCE_MASK(bits)			GENMASK_ULL((bits) - 1, 0)

#endif /* _LINUX_CLOCKSOURCE_CONST_H_ */