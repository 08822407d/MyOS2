// SPDX-License-Identifier: GPL-2.0
#ifndef _LINUX_ATOMIC_LONG_MACRO_H_
#define _LINUX_ATOMIC_LONG_MACRO_H_

	#define ATOMIC_LONG_INIT(i)		ATOMIC64_INIT(i)


	#define atomic64_read arch_atomic64_read

	// static __always_inline s64
	// atomic64_read_acquire(const atomic64_t *v)
	// {
	// 	instrument_atomic_read(v, sizeof(*v));
	// 	return arch_atomic64_read_acquire(v);
	// }

	#define atomic64_set arch_atomic64_set

	// static __always_inline void
	// atomic64_set_release(atomic64_t *v, s64 i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_write(v, sizeof(*v));
	// 	arch_atomic64_set_release(v, i);
	// }

	#define atomic64_add arch_atomic64_add

	#define atomic64_add_return arch_atomic64_add_return

	// static __always_inline s64
	// atomic64_add_return_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_add_return_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_add_return_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_add_return_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_add_return_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_add_return_relaxed(i, v);
	// }

	#define atomic64_fetch_add arch_atomic64_fetch_add

	// static __always_inline s64
	// atomic64_fetch_add_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_add_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_add_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_add_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_add_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_add_relaxed(i, v);
	// }

	#define atomic64_sub arch_atomic64_sub

	#define atomic64_sub_return arch_atomic64_sub_return

	// static __always_inline s64
	// atomic64_sub_return_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_sub_return_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_sub_return_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_sub_return_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_sub_return_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_sub_return_relaxed(i, v);
	// }

	#define atomic64_fetch_sub arch_atomic64_fetch_sub

	// static __always_inline s64
	// atomic64_fetch_sub_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_sub_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_sub_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_sub_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_sub_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_sub_relaxed(i, v);
	// }

	#define atomic64_inc arch_atomic64_inc

	// static __always_inline s64
	// atomic64_inc_return(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_return(v);
	// }

	// static __always_inline s64
	// atomic64_inc_return_acquire(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_return_acquire(v);
	// }

	// static __always_inline s64
	// atomic64_inc_return_release(atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_return_release(v);
	// }

	// static __always_inline s64
	// atomic64_inc_return_relaxed(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_return_relaxed(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_inc(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_inc(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_inc_acquire(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_inc_acquire(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_inc_release(atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_inc_release(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_inc_relaxed(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_inc_relaxed(v);
	// }

	#define atomic64_dec arch_atomic64_dec

	// static __always_inline s64
	// atomic64_dec_return(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_return(v);
	// }

	// static __always_inline s64
	// atomic64_dec_return_acquire(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_return_acquire(v);
	// }

	// static __always_inline s64
	// atomic64_dec_return_release(atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_return_release(v);
	// }

	// static __always_inline s64
	// atomic64_dec_return_relaxed(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_return_relaxed(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_dec(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_dec(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_dec_acquire(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_dec_acquire(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_dec_release(atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_dec_release(v);
	// }

	// static __always_inline s64
	// atomic64_fetch_dec_relaxed(atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_dec_relaxed(v);
	// }

	#define atomic64_and arch_atomic64_and

	// static __always_inline s64
	// atomic64_fetch_and(s64 i, atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_and(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_and_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_and_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_and_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_and_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_and_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_and_relaxed(i, v);
	// }

	// static __always_inline void
	// atomic64_andnot(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic64_andnot(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_andnot(s64 i, atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_andnot(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_andnot_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_andnot_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_andnot_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_andnot_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_andnot_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_andnot_relaxed(i, v);
	// }

	#define atomic64_or arch_atomic64_or

	// static __always_inline s64
	// atomic64_fetch_or(s64 i, atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_or(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_or_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_or_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_or_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_or_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_or_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_or_relaxed(i, v);
	// }

	#define atomic64_xor arch_atomic64_xor

	// static __always_inline s64
	// atomic64_fetch_xor(s64 i, atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_xor(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_xor_acquire(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_xor_acquire(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_xor_release(s64 i, atomic64_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_xor_release(i, v);
	// }

	// static __always_inline s64
	// atomic64_fetch_xor_relaxed(s64 i, atomic64_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_xor_relaxed(i, v);
	// }

	#define atomic64_xchg arch_atomic64_xchg

	// static __always_inline s64
	// atomic64_xchg_acquire(atomic64_t *v, s64 i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_xchg_acquire(v, i);
	// }

	// static __always_inline s64
	// atomic64_xchg_release(atomic64_t *v, s64 i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_xchg_release(v, i);
	// }

	// static __always_inline s64
	// atomic64_xchg_relaxed(atomic64_t *v, s64 i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_xchg_relaxed(v, i);
	// }

	#define atomic64_cmpxchg arch_atomic64_cmpxchg

	// static __always_inline s64
	// atomic64_cmpxchg_acquire(atomic64_t *v, s64 old, s64 new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline s64
	// atomic64_cmpxchg_release(atomic64_t *v, s64 old, s64 new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_cmpxchg_release(v, old, new);
	// }

	// static __always_inline s64
	// atomic64_cmpxchg_relaxed(atomic64_t *v, s64 old, s64 new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_cmpxchg_relaxed(v, old, new);
	// }

	#define atomic64_try_cmpxchg arch_atomic64_try_cmpxchg

	// static __always_inline bool
	// atomic64_try_cmpxchg_acquire(atomic64_t *v, s64 *old, s64 new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic64_try_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline bool
	// atomic64_try_cmpxchg_release(atomic64_t *v, s64 *old, s64 new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic64_try_cmpxchg_release(v, old, new);
	// }

	// static __always_inline bool
	// atomic64_try_cmpxchg_relaxed(atomic64_t *v, s64 *old, s64 new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic64_try_cmpxchg_relaxed(v, old, new);
	// }

	// static __always_inline bool
	// atomic64_sub_and_test(s64 i, atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_sub_and_test(i, v);
	// }

	#define atomic64_dec_and_test arch_atomic64_dec_and_test

	#define atomic64_inc_and_test arch_atomic64_inc_and_test

	#define atomic64_add_negative arch_atomic64_add_negative

	// static __always_inline s64
	// atomic64_fetch_add_unless(atomic64_t *v, s64 a, s64 u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_fetch_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic64_add_unless(atomic64_t *v, s64 a, s64 u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic64_inc_not_zero(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_not_zero(v);
	// }

	// static __always_inline bool
	// atomic64_inc_unless_negative(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_inc_unless_negative(v);
	// }

	// static __always_inline bool
	// atomic64_dec_unless_positive(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_unless_positive(v);
	// }

	// static __always_inline s64
	// atomic64_dec_if_positive(atomic64_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic64_dec_if_positive(v);
	// }

	#define atomic_long_read arch_atomic_long_read

	// static __always_inline long
	// atomic_long_read_acquire(const atomic_long_t *v)
	// {
	// 	instrument_atomic_read(v, sizeof(*v));
	// 	return arch_atomic_long_read_acquire(v);
	// }

	#define atomic_long_set arch_atomic_long_set

	// static __always_inline void
	// atomic_long_set_release(atomic_long_t *v, long i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_write(v, sizeof(*v));
	// 	arch_atomic_long_set_release(v, i);
	// }

	#define atomic_long_add arch_atomic_long_add

	// static __always_inline long
	// atomic_long_add_return(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_return(i, v);
	// }

	// static __always_inline long
	// atomic_long_add_return_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_return_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_add_return_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_return_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_add_return_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_return_relaxed(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_add(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_add(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_add_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_add_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_add_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_add_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_add_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_add_relaxed(i, v);
	// }

	#define atomic_long_sub arch_atomic_long_sub

	// static __always_inline long
	// atomic_long_sub_return(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_sub_return(i, v);
	// }

	// static __always_inline long
	// atomic_long_sub_return_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_sub_return_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_sub_return_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_sub_return_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_sub_return_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_sub_return_relaxed(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_sub(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_sub(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_sub_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_sub_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_sub_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_sub_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_sub_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_sub_relaxed(i, v);
	// }

	#define atomic_long_inc arch_atomic_long_inc

	// static __always_inline long
	// atomic_long_inc_return(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_return(v);
	// }

	// static __always_inline long
	// atomic_long_inc_return_acquire(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_return_acquire(v);
	// }

	// static __always_inline long
	// atomic_long_inc_return_release(atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_return_release(v);
	// }

	// static __always_inline long
	// atomic_long_inc_return_relaxed(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_return_relaxed(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_inc(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_inc(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_inc_acquire(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_inc_acquire(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_inc_release(atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_inc_release(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_inc_relaxed(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_inc_relaxed(v);
	// }

	#define atomic_long_dec arch_atomic_long_dec

	// static __always_inline long
	// atomic_long_dec_return(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_return(v);
	// }

	// static __always_inline long
	// atomic_long_dec_return_acquire(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_return_acquire(v);
	// }

	// static __always_inline long
	// atomic_long_dec_return_release(atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_return_release(v);
	// }

	// static __always_inline long
	// atomic_long_dec_return_relaxed(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_return_relaxed(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_dec(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_dec(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_dec_acquire(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_dec_acquire(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_dec_release(atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_dec_release(v);
	// }

	// static __always_inline long
	// atomic_long_fetch_dec_relaxed(atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_dec_relaxed(v);
	// }

	// static __always_inline void
	// atomic_long_and(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic_long_and(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_and(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_and(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_and_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_and_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_and_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_and_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_and_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_and_relaxed(i, v);
	// }

	// static __always_inline void
	// atomic_long_andnot(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic_long_andnot(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_andnot(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_andnot(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_andnot_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_andnot_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_andnot_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_andnot_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_andnot_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_andnot_relaxed(i, v);
	// }

	// static __always_inline void
	// atomic_long_or(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic_long_or(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_or(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_or(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_or_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_or_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_or_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_or_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_or_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_or_relaxed(i, v);
	// }

	// static __always_inline void
	// atomic_long_xor(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic_long_xor(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_xor(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_xor(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_xor_acquire(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_xor_acquire(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_xor_release(long i, atomic_long_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_xor_release(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_xor_relaxed(long i, atomic_long_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_xor_relaxed(i, v);
	// }

	// static __always_inline long
	// atomic_long_xchg(atomic_long_t *v, long i)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_xchg(v, i);
	// }

	// static __always_inline long
	// atomic_long_xchg_acquire(atomic_long_t *v, long i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_xchg_acquire(v, i);
	// }

	// static __always_inline long
	// atomic_long_xchg_release(atomic_long_t *v, long i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_xchg_release(v, i);
	// }

	// static __always_inline long
	// atomic_long_xchg_relaxed(atomic_long_t *v, long i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_xchg_relaxed(v, i);
	// }

	// static __always_inline long
	// atomic_long_cmpxchg(atomic_long_t *v, long old, long new)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_cmpxchg(v, old, new);
	// }

	// static __always_inline long
	// atomic_long_cmpxchg_acquire(atomic_long_t *v, long old, long new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline long
	// atomic_long_cmpxchg_release(atomic_long_t *v, long old, long new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_cmpxchg_release(v, old, new);
	// }

	// static __always_inline long
	// atomic_long_cmpxchg_relaxed(atomic_long_t *v, long old, long new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_cmpxchg_relaxed(v, old, new);
	// }

	// static __always_inline bool
	// atomic_long_try_cmpxchg(atomic_long_t *v, long *old, long new)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_long_try_cmpxchg(v, old, new);
	// }

	// static __always_inline bool
	// atomic_long_try_cmpxchg_acquire(atomic_long_t *v, long *old, long new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_long_try_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline bool
	// atomic_long_try_cmpxchg_release(atomic_long_t *v, long *old, long new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_long_try_cmpxchg_release(v, old, new);
	// }

	// static __always_inline bool
	// atomic_long_try_cmpxchg_relaxed(atomic_long_t *v, long *old, long new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_long_try_cmpxchg_relaxed(v, old, new);
	// }

	#define atomic_long_sub_and_test arch_atomic_long_sub_and_test

	#define atomic_long_dec_and_test arch_atomic_long_dec_and_test

	// static __always_inline bool
	// atomic_long_inc_and_test(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_and_test(v);
	// }

	// static __always_inline bool
	// atomic_long_add_negative(long i, atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_negative(i, v);
	// }

	// static __always_inline long
	// atomic_long_fetch_add_unless(atomic_long_t *v, long a, long u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_fetch_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic_long_add_unless(atomic_long_t *v, long a, long u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic_long_inc_not_zero(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_not_zero(v);
	// }

	// static __always_inline bool
	// atomic_long_inc_unless_negative(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_inc_unless_negative(v);
	// }

	// static __always_inline bool
	// atomic_long_dec_unless_positive(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_unless_positive(v);
	// }

	// static __always_inline long
	// atomic_long_dec_if_positive(atomic_long_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_long_dec_if_positive(v);
	// }

#endif /* _LINUX_ATOMIC_LONG_MACRO_H_ */