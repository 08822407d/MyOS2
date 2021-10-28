/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)stddef.h	8.1 (Berkeley) 6/2/93
 *
 * $FreeBSD$
 */

#ifndef _STDDEF_H_
#define _STDDEF_H_

#include <sys/cdefs.h>
#include <sys/_null.h>
#include <sys/_types.h>

	#ifndef _PTRDIFF_T_DECLARED
	typedef	__ptrdiff_t	ptrdiff_t;
	#define	_PTRDIFF_T_DECLARED
	#endif

	#if __BSD_VISIBLE
	#ifndef _RUNE_T_DECLARED
	typedef	__rune_t	rune_t;
	#define	_RUNE_T_DECLARED
	#endif
	#endif

	#ifndef _SIZE_T_DECLARED
	typedef	__size_t	size_t;
	#define	_SIZE_T_DECLARED
	#endif

	#ifndef	__cplusplus
	#ifndef _WCHAR_T_DECLARED
	typedef	___wchar_t	wchar_t;
	#define	_WCHAR_T_DECLARED
	#endif
	#endif

	#if __ISO_C_VISIBLE >= 2011 || __cplusplus >= 201103L
	#ifndef __CLANG_MAX_ALIGN_T_DEFINED
	typedef	__max_align_t	max_align_t;
	#define __CLANG_MAX_ALIGN_T_DEFINED
	#define _GCC_MAX_ALIGN_T
	#endif
	#endif

	#define	offsetof(type, field)	__offsetof(type, field)

	#if __EXT1_VISIBLE
	/* ISO/IEC 9899:2011 K.3.3.2 */
	#ifndef _RSIZE_T_DEFINED
	#define _RSIZE_T_DEFINED
	typedef size_t rsize_t;
	#endif
	#endif /* __EXT1_VISIBLE */


	/*
	*	definations from xboot's <stddef.h>
	*/
	#ifdef __cplusplus
	extern "C" {
	#endif

		#if (defined(__GNUC__) && (__GNUC__ >= 3))
		#define likely(expr)	(__builtin_expect(!!(expr), 1))
		#define unlikely(expr)	(__builtin_expect(!!(expr), 0))
		#else
		#define likely(expr)	(!!(expr))
		#define unlikely(expr)	(!!(expr))
		#endif

		#define min(a, b)		({typeof(a) _amin = (a); typeof(b) _bmin = (b); (void)(&_amin == &_bmin); _amin < _bmin ? _amin : _bmin;})
		#define max(a, b)		({typeof(a) _amax = (a); typeof(b) _bmax = (b); (void)(&_amax == &_bmax); _amax > _bmax ? _amax : _bmax;})
		#define clamp(v, a, b)	min(max(a, v), b)

		#define ifloor(x)		((x) > 0 ? (int)(x) : (int)((x) - 0.9999999999))
		#define iround(x)		((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
		#define iceil(x)		((x) > 0 ? (int)((x) + 0.9999999999) : (int)(x))
		#define idiv255(x)		((((int)(x) + 1) * 257) >> 16)

		#define X(...)			("" #__VA_ARGS__ "")

		enum {
			FALSE				= 0,
			TRUE				= 1,
		};

		// #define container_of(ptr, type, member)		({const typeof(((type *)0)->member) *__mptr = (ptr); (type *)((char *)__mptr - offsetof(type,member));})
		#define container_of(member_ptr, container_type, member_name)										\
		({																									\
			typeof(((container_type *)0)->member_name) * p = (member_ptr);									\
			(container_type *)((unsigned long)p - (unsigned long)&(((container_type *)0)->member_name));	\
		})

	#ifdef __cplusplus
	}
	#endif

#endif /* _STDDEF_H_ */
