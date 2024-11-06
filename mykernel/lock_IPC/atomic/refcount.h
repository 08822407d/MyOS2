// SPDX-License-Identifier: GPL-2.0

// Generated by scripts/atomic/gen-atomic-instrumented.sh
// DO NOT MODIFY THIS FILE DIRECTLY

/*
 * This file provides wrappers with KASAN instrumentation for atomic operations.
 * To use this functionality an arch's atomic.h file needs to define all
 * atomic operations with arch_ prefix (e.g. arch_atomic_read()) and include
 * this file at the end. This file provides atomic_read() that forwards to
 * arch_atomic_read() for actual atomic operation.
 * Note: if an arch atomic operation is implemented by means of other atomic
 * operations (e.g. atomic_read()/atomic_cmpxchg() loop), then it needs to use
 * arch_ variants (i.e. arch_atomic_read()/arch_atomic_cmpxchg()) to avoid
 * double instrumentation.
 */
#ifndef _LINUX_REFCOUNT_H_
#define _LINUX_REFCOUNT_H_
 
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"


	#ifdef DEBUG

		extern void
		refcount_set(refcount_t *r, int n);

		extern uint
		refcount_read(const refcount_t *r);

	#endif

	#include "refcount_macro.h"

	#if defined(REFCOUNT_DEFINATION) || !(DEBUG)

		/**
		 * refcount_set - set a refcount's value
		 * @r: the refcount
		 * @n: value to which the refcount will be set
		 */
		PREFIX_STATIC_INLINE
		void
		refcount_set(refcount_t *r, int n) {
			atomic_set(&r->refs, n);
		}


		/**
		 * refcount_read - get a refcount's value
		 * @r: the refcount
		 *
		 * Return: the refcount's value
		 */
		PREFIX_STATIC_INLINE
		uint
		refcount_read(const refcount_t *r) {
			return atomic_read(&r->refs);
		}

	#endif /* !DEBUG */

#endif /* _LINUX_REFCOUNT_H_ */