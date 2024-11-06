// SPDX-License-Identifier: GPL-2.0
#ifndef _LINUX_REFCOUNT_MACRO_H_
#define _LINUX_REFCOUNT_MACRO_H_


	#define REFCOUNT_INIT(n)	{ .refs = ATOMIC_INIT(n), }
	#define REFCOUNT_MAX		INT_MAX
	#define REFCOUNT_SATURATED	(INT_MIN / 2)

#endif /* _LINUX_REFCOUNT_MACRO_H_ */