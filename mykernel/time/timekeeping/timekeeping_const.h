/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMEKEEPING_CONST_H_
#define _LINUX_TIMEKEEPING_CONST_H_


	#define TK_CLEAR_NTP		(1 << 0)
	#define TK_MIRROR			(1 << 1)
	#define TK_CLOCK_WAS_SET	(1 << 2)

	#ifdef CONFIG_NO_HZ_COMMON
	/*
	 * If multiple bases need to be locked, use the base ordering for lock
	 * nesting, i.e. lowest number first.
	 */
	#  define NR_BASES			3
	#  define BASE_LOCAL		0
	#  define BASE_GLOBAL		1
	#  define BASE_DEF			2
	#else
	#  define NR_BASES			1
	#  define BASE_LOCAL		0
	#  define BASE_GLOBAL		0
	#  define BASE_DEF			0
	#endif


#endif /* _LINUX_TIMEKEEPING_CONST_H_ */