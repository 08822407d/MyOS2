/*	$NetBSD: types.h,v 1.95 2015/08/27 12:30:51 pooka Exp $	*/

/*-
 * Copyright (c) 1982, 1986, 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)types.h	8.4 (Berkeley) 1/21/94
 */

#ifndef _SYS_TYPES_H_
#define	_SYS_TYPES_H_

// #ifdef __GNUC_

	#ifndef	int8_t
	typedef	__INT8_TYPE__	int8_t;
	#define	int8_t			__int8_t
	#endif

	#ifndef	uint8_t
	typedef	__UINT8_TYPE__	uint8_t;
	#define	uint8_t			__uint8_t
	#endif

	#ifndef	int16_t
	typedef	__INT16_TYPE__	int16_t;
	#define	int16_t			__int16_t
	#endif

	#ifndef	uint16_t
	typedef	__UINT16_TYPE__	uint16_t;
	#define	uint16_t		__uint16_t
	#endif

	#ifndef	int32_t
	typedef	__INT32_TYPE__	int32_t;
	#define	int32_t			__int32_t
	#endif

	#ifndef	uint32_t
	typedef	__UINT32_TYPE__	uint32_t;
	#define	uint32_t		__uint32_t
	#endif

	#ifndef	int64_t
	typedef	__INT64_TYPE__	int64_t;
	#define	int64_t			__int64_t
	#endif

	#ifndef	uint64_t
	typedef	__UINT64_TYPE__	uint64_t;
	#define	uint64_t		__uint64_t
	#endif

	typedef __UINT64_TYPE__	size_t;

// #endif

#endif /* !_SYS_TYPES_H_ */