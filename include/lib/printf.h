/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __PRINTF_H__

#define __PRINTF_H__

#include <stdarg.h>

	#define ZEROPAD		1		/* pad with zero */
	#define SIGN		2		/* unsigned/signed long */
	#define PLUS		4		/* show plus */
	#define SPACE		8		/* space if plus */
	#define LEFT		16		/* left justified */
	#define SPECIAL		32		/* 0x */
	#define SMALL		64		/* use 'abcdef' instead of 'ABCDEF' */

	#define is_digit(c)	((c) >= '0' && (c) <= '9')

	int skip_atoi(const char **s);
	int do_div(long *num,int base);

#endif

