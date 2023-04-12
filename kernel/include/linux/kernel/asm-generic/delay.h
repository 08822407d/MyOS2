/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_DELAY_H
#define __ASM_GENERIC_DELAY_H

	/* Undefined functions to get compile-time errors */
	extern void __bad_sdelay(void);
	extern void __bad_mdelay(void);
	extern void __bad_udelay(void);
	extern void __bad_ndelay(void);

	extern void __sdelay(unsigned long secs);
	extern void __mdelay(unsigned long msecs);
	extern void __udelay(unsigned long usecs);
	extern void __ndelay(unsigned long nsecs);
	extern void __const_udelay(unsigned long xloops);
	extern void __delay(unsigned long loops);

	/*
	 * The weird n/20000 thing suppresses a "comparison is always false due to
	 * limited range of data type" warning with non-const 8-bit arguments.
	 */

	/* 0x100000000 is 2**32 (rounded up) */
	#define sdelay(n) ({										\
				if (__builtin_constant_p(n)) {					\
					if ((n) / 20000 >= 1)						\
						__bad_sdelay();							\
					else										\
						__const_udelay((n) * 0x100000000ul);	\
				} else {										\
					__sdelay(n);								\
				}												\
			})

	/* 0x418937 is 2**32 / 1000 (rounded up) */
	#define mdelay(n) ({									\
				if (__builtin_constant_p(n)) {				\
					if ((n) / 20000 >= 1)					\
						__bad_mdelay();						\
					else									\
						__const_udelay((n) * 0x418938ul);	\
				} else {									\
					__mdelay(n);							\
				}											\
			})

	/* 0x10c7 is 2**32 / 1000000 (rounded up) */
	#define udelay(n) ({								\
				if (__builtin_constant_p(n)) {			\
					if ((n) / 20000 >= 1)				\
						__bad_udelay();					\
					else								\
						__const_udelay((n) * 0x10c7ul);	\
				} else {								\
					__udelay(n);						\
				}										\
			})

	/* 0x5 is 2**32 / 1000000000 (rounded up) */
	#define ndelay(n) ({							\
				if (__builtin_constant_p(n)) {		\
					if ((n) / 20000 >= 1)			\
						__bad_ndelay();				\
					else							\
						__const_udelay((n) * 5ul);	\
				} else {							\
					__ndelay(n);					\
				}									\
			})

#endif /* __ASM_GENERIC_DELAY_H */
