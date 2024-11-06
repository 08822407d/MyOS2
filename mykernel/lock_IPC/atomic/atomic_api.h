/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LINUX_ATOMIC_API_H_
#define _LINUX_ATOMIC_API_H_

	#include "atomic.h"
	#include "atomic-long.h"
	#include "refcount.h"

	// /*
	//  * Relaxed variants of xchg, cmpxchg and some atomic operations.
	//  *
	//  * We support four variants:
	//  *
	//  * - Fully ordered: The default implementation, no suffix required.
	//  * - Acquire: Provides ACQUIRE semantics, _acquire suffix.
	//  * - Release: Provides RELEASE semantics, _release suffix.
	//  * - Relaxed: No ordering guarantees, _relaxed suffix.
	//  *
	//  * For compound atomics performing both a load and a store, ACQUIRE
	//  * semantics apply only to the load and RELEASE semantics only to the
	//  * store portion of the operation. Note that a failed cmpxchg_acquire
	//  * does -not- imply any memory ordering constraints.
	//  *
	//  * See Documentation/memory-barriers.txt for ACQUIRE/RELEASE definitions.
	//  */

	// #define atomic_cond_read_acquire(v, c) smp_cond_load_acquire(&(v)->counter, (c))
	// #define atomic_cond_read_relaxed(v, c) smp_cond_load_relaxed(&(v)->counter, (c))

	// #define atomic64_cond_read_acquire(v, c) smp_cond_load_acquire(&(v)->counter, (c))
	// #define atomic64_cond_read_relaxed(v, c) smp_cond_load_relaxed(&(v)->counter, (c))

	// /*
	//  * The idea here is to build acquire/release variants by adding explicit
	//  * barriers on top of the relaxed variant. In the case where the relaxed
	//  * variant is already fully ordered, no additional barriers are needed.
	//  *
	//  * If an architecture overrides __atomic_acquire_fence() it will probably
	//  * want to define smp_mb__after_spinlock().
	//  */
	// #ifndef __atomic_acquire_fence
	// #	define __atomic_acquire_fence		smp_mb__after_atomic
	// #endif

	// #ifndef __atomic_release_fence
	// #	define __atomic_release_fence		smp_mb__before_atomic
	// #endif

	// #ifndef __atomic_pre_full_fence
	// #	define __atomic_pre_full_fence		smp_mb__before_atomic
	// #endif

	// #ifndef __atomic_post_full_fence
	// #	define __atomic_post_full_fence		smp_mb__after_atomic
	// #endif

	// #define __atomic_op_acquire(op, args...) ({		\
	// 			typeof(op##_relaxed(args)) __ret =	\
	// 				op##_relaxed(args);				\
	// 			__atomic_acquire_fence();			\
	// 			__ret;								\
	// 		})

	// #define __atomic_op_release(op, args...) ({	\
	// 			__atomic_release_fence();		\
	// 			op##_relaxed(args);				\
	// 		})

	// #define __atomic_op_fence(op, args...) ({		\
	// 			typeof(op##_relaxed(args)) __ret;	\
	// 			__atomic_pre_full_fence();			\
	// 			__ret = op##_relaxed(args);			\
	// 			__atomic_post_full_fence();			\
	// 			__ret;								\
	// 		})

	#define arch_atomic_long_read(v)	\
				arch_atomic64_read(v)

	#define arch_atomic_long_set(v ,i)	\
				arch_atomic64_set(v, i)

	#define arch_atomic_long_add(i, v)	\
				arch_atomic64_add(i, v)

	#define arch_atomic_long_add_return(i, v)	\
				arch_atomic64_add_return(i, v)

	#define arch_atomic_long_fetch_add(i, v)	\
				arch_atomic64_fetch_add(i, v)

	#define arch_atomic_long_sub(i, v)	\
				arch_atomic64_sub(i, v)

	#define arch_atomic_long_sub_return(i, v)	\
				arch_atomic64_sub_return(i, v)

	#define arch_atomic_long_fetch_sub(i, v)	\
				arch_atomic64_fetch_sub(i, v)

	#define arch_atomic_long_inc(v)	\
				arch_atomic64_inc(v)

	#define arch_atomic_long_dec(v)	\
				arch_atomic64_dec(v)

	#define arch_atomic_long_and(i, v)	\
				arch_atomic64_and(i, v)

	#define arch_atomic_long_or(i, v)	\
				arch_atomic64_or(i, v)

	#define arch_atomic_long_xor(i, v)	\
				arch_atomic64_xor(i, v)

	#define arch_atomic_long_xchg(v, new)	\
				arch_atomic64_xchg(v, new)

	#define arch_atomic_long_cmpxchg(v, old, new)	\
				arch_atomic64_cmpxchg(v, old, new)

	#define arch_atomic_long_try_cmpxchg(v, old, new)	\
				arch_atomic64_try_cmpxchg(v, old, new)

	#define arch_atomic_long_sub_and_test(i, v)	\
				arch_atomic64_sub_and_test(i, v)

	#define arch_atomic_long_dec_and_test(v)	\
				arch_atomic64_dec_and_test(v)

	#define arch_atomic_long_inc_and_test(v)	\
				arch_atomic64_inc_and_test(v)

	#define arch_atomic_long_add_negative(i, v)	\
				arch_atomic64_add_negative(i, v)

#endif /* _LINUX_ATOMIC_API_H_ */
