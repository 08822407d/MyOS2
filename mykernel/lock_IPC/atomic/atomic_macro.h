// SPDX-License-Identifier: GPL-2.0
#ifndef _LINUX_ATOMIC_MACRO_H_
#define _LINUX_ATOMIC_MACRO_H_

	#define ATOMIC_INIT(i) { (i) }


	#define atomic_read arch_atomic_read

	// static __always_inline int
	// atomic_read_acquire(const atomic_t *v)
	// {
	// 	instrument_atomic_read(v, sizeof(*v));
	// 	return arch_atomic_read_acquire(v);
	// }

	#define atomic_set arch_atomic_set

	// static __always_inline void
	// atomic_set_release(atomic_t *v, int i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_write(v, sizeof(*v));
	// 	arch_atomic_set_release(v, i);
	// }

	#define atomic_add arch_atomic_add

	#define atomic_add_return arch_atomic_add_return

	// static __always_inline int
	// atomic_add_return_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_add_return_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_add_return_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_add_return_release(i, v);
	// }

	// static __always_inline int
	// atomic_add_return_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_add_return_relaxed(i, v);
	// }

	#define atomic_fetch_add arch_atomic_fetch_add

	// static __always_inline int
	// atomic_fetch_add_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_add_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_add_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_add_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_add_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_add_relaxed(i, v);
	// }

	#define atomic_sub arch_atomic_sub

	#define atomic_sub_return arch_atomic_sub_return

	// static __always_inline int
	// atomic_sub_return_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_sub_return_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_sub_return_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_sub_return_release(i, v);
	// }

	// static __always_inline int
	// atomic_sub_return_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_sub_return_relaxed(i, v);
	// }

	#define atomic_fetch_sub arch_atomic_fetch_sub

	// static __always_inline int
	// atomic_fetch_sub_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_sub_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_sub_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_sub_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_sub_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_sub_relaxed(i, v);
	// }

	#define atomic_inc arch_atomic_inc

	// static __always_inline int
	// atomic_inc_return(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_return(v);
	// }

	// static __always_inline int
	// atomic_inc_return_acquire(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_return_acquire(v);
	// }

	// static __always_inline int
	// atomic_inc_return_release(atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_return_release(v);
	// }

	// static __always_inline int
	// atomic_inc_return_relaxed(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_return_relaxed(v);
	// }

	// static __always_inline int
	// atomic_fetch_inc(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_inc(v);
	// }

	// static __always_inline int
	// atomic_fetch_inc_acquire(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_inc_acquire(v);
	// }

	// static __always_inline int
	// atomic_fetch_inc_release(atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_inc_release(v);
	// }

	// static __always_inline int
	// atomic_fetch_inc_relaxed(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_inc_relaxed(v);
	// }

	#define atomic_dec arch_atomic_dec

	// static __always_inline int
	// atomic_dec_return(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_return(v);
	// }

	// static __always_inline int
	// atomic_dec_return_acquire(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_return_acquire(v);
	// }

	// static __always_inline int
	// atomic_dec_return_release(atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_return_release(v);
	// }

	// static __always_inline int
	// atomic_dec_return_relaxed(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_return_relaxed(v);
	// }

	// static __always_inline int
	// atomic_fetch_dec(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_dec(v);
	// }

	// static __always_inline int
	// atomic_fetch_dec_acquire(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_dec_acquire(v);
	// }

	// static __always_inline int
	// atomic_fetch_dec_release(atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_dec_release(v);
	// }

	// static __always_inline int
	// atomic_fetch_dec_relaxed(atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_dec_relaxed(v);
	// }

	#define atomic_and arch_atomic_and
	#define atomic_fetch_and arch_atomic_fetch_and

	// static __always_inline int
	// atomic_fetch_and(int i, atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_and(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_and_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_and_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_and_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_and_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_and_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_and_relaxed(i, v);
	// }

	// static __always_inline void
	// atomic_andnot(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	arch_atomic_andnot(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_andnot(int i, atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_andnot(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_andnot_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_andnot_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_andnot_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_andnot_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_andnot_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_andnot_relaxed(i, v);
	// }

	#define atomic_or arch_atomic_or
	#define atomic_fetch_or arch_atomic_fetch_or

	// static __always_inline int
	// atomic_fetch_or(int i, atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_or(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_or_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_or_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_or_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_or_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_or_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_or_relaxed(i, v);
	// }

	#define atomic_xor arch_atomic_xor
	#define atomic_fetch_xor arch_atomic_fetch_xor

	// static __always_inline int
	// atomic_fetch_xor(int i, atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_xor(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_xor_acquire(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_xor_acquire(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_xor_release(int i, atomic_t *v)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_xor_release(i, v);
	// }

	// static __always_inline int
	// atomic_fetch_xor_relaxed(int i, atomic_t *v)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_xor_relaxed(i, v);
	// }

	#define atomic_xchg arch_atomic_xchg

	// static __always_inline int
	// atomic_xchg_acquire(atomic_t *v, int i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_xchg_acquire(v, i);
	// }

	// static __always_inline int
	// atomic_xchg_release(atomic_t *v, int i)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_xchg_release(v, i);
	// }

	// static __always_inline int
	// atomic_xchg_relaxed(atomic_t *v, int i)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_xchg_relaxed(v, i);
	// }

	#define atomic_cmpxchg arch_atomic_cmpxchg

	// static __always_inline int
	// atomic_cmpxchg_acquire(atomic_t *v, int old, int new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline int
	// atomic_cmpxchg_release(atomic_t *v, int old, int new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_cmpxchg_release(v, old, new);
	// }

	// static __always_inline int
	// atomic_cmpxchg_relaxed(atomic_t *v, int old, int new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_cmpxchg_relaxed(v, old, new);
	// }

	#define atomic_try_cmpxchg arch_atomic_try_cmpxchg

	// static __always_inline bool
	// atomic_try_cmpxchg_acquire(atomic_t *v, int *old, int new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_try_cmpxchg_acquire(v, old, new);
	// }

	// static __always_inline bool
	// atomic_try_cmpxchg_release(atomic_t *v, int *old, int new)
	// {
	// 	kcsan_release();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_try_cmpxchg_release(v, old, new);
	// }

	// static __always_inline bool
	// atomic_try_cmpxchg_relaxed(atomic_t *v, int *old, int new)
	// {
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	instrument_atomic_read_write(old, sizeof(*old));
	// 	return arch_atomic_try_cmpxchg_relaxed(v, old, new);
	// }

	#define atomic_sub_and_test arch_atomic_sub_and_test

	#define atomic_dec_and_test arch_atomic_dec_and_test

	#define atomic_inc_and_test arch_atomic_inc_and_test

	#define atomic_add_negative arch_atomic_add_negative

	// static __always_inline int
	// atomic_fetch_add_unless(atomic_t *v, int a, int u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_fetch_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic_add_unless(atomic_t *v, int a, int u)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_add_unless(v, a, u);
	// }

	// static __always_inline bool
	// atomic_inc_not_zero(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_not_zero(v);
	// }

	// static __always_inline bool
	// atomic_inc_unless_negative(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_inc_unless_negative(v);
	// }

	// static __always_inline bool
	// atomic_dec_unless_positive(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_unless_positive(v);
	// }

	// static __always_inline int
	// atomic_dec_if_positive(atomic_t *v)
	// {
	// 	kcsan_mb();
	// 	instrument_atomic_read_write(v, sizeof(*v));
	// 	return arch_atomic_dec_if_positive(v);
	// }


	#define xchg(ptr, ...) ({						\
				typeof(ptr) __ai_ptr = (ptr);		\
				arch_xchg(__ai_ptr, __VA_ARGS__);	\
			})

	// #define xchg_acquire(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_xchg_acquire(__ai_ptr, __VA_ARGS__); \
	// })

	// #define xchg_release(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_release(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_xchg_release(__ai_ptr, __VA_ARGS__); \
	// })

	// #define xchg_relaxed(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_xchg_relaxed(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_mb(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg_acquire(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_acquire(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg_release(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_release(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_release(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg_relaxed(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_relaxed(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg64(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_mb(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg64(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg64_acquire(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg64_acquire(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg64_release(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_release(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg64_release(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg64_relaxed(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg64_relaxed(__ai_ptr, __VA_ARGS__); \
	// })

	// #define try_cmpxchg(ptr, oldp, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	typeof(oldp) __ai_oldp = (oldp); \
	// 	kcsan_mb(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	instrument_atomic_write(__ai_oldp, sizeof(*__ai_oldp)); \
	// 	arch_try_cmpxchg(__ai_ptr, __ai_oldp, __VA_ARGS__); \
	// })

	// #define try_cmpxchg_acquire(ptr, oldp, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	typeof(oldp) __ai_oldp = (oldp); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	instrument_atomic_write(__ai_oldp, sizeof(*__ai_oldp)); \
	// 	arch_try_cmpxchg_acquire(__ai_ptr, __ai_oldp, __VA_ARGS__); \
	// })

	// #define try_cmpxchg_release(ptr, oldp, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	typeof(oldp) __ai_oldp = (oldp); \
	// 	kcsan_release(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	instrument_atomic_write(__ai_oldp, sizeof(*__ai_oldp)); \
	// 	arch_try_cmpxchg_release(__ai_ptr, __ai_oldp, __VA_ARGS__); \
	// })

	// #define try_cmpxchg_relaxed(ptr, oldp, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	typeof(oldp) __ai_oldp = (oldp); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	instrument_atomic_write(__ai_oldp, sizeof(*__ai_oldp)); \
	// 	arch_try_cmpxchg_relaxed(__ai_ptr, __ai_oldp, __VA_ARGS__); \
	// })

	// #define cmpxchg_local(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_local(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg64_local(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg64_local(__ai_ptr, __VA_ARGS__); \
	// })

	// #define sync_cmpxchg(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_mb(); \
	// 	instrument_atomic_write(__ai_ptr, sizeof(*__ai_ptr)); \
	// 	arch_sync_cmpxchg(__ai_ptr, __VA_ARGS__); \
	// })

	// #define cmpxchg_double(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	kcsan_mb(); \
	// 	instrument_atomic_write(__ai_ptr, 2 * sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_double(__ai_ptr, __VA_ARGS__); \
	// })


	// #define cmpxchg_double_local(ptr, ...) \
	// ({ \
	// 	typeof(ptr) __ai_ptr = (ptr); \
	// 	instrument_atomic_write(__ai_ptr, 2 * sizeof(*__ai_ptr)); \
	// 	arch_cmpxchg_double_local(__ai_ptr, __VA_ARGS__); \
	// })

#endif /* _LINUX_ATOMIC_MACRO_H_ */