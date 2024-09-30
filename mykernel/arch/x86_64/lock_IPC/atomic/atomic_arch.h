/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_ATOMIC_H_
#define _ASM_X86_ATOMIC_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <asm/alternative.h>
	// #include <insns/cmpxchg_api.h>
	#include <linux/kernel/lock_ipc_types.h>

	#include "../lock_ipc_types_arch.h"
	#include "../lock_ipc_api_arch.h"


	#ifdef DEBUG

		extern int
		arch_atomic_read(const atomic_t *v);

		extern void
		arch_atomic_set(atomic_t *v, int i);

		extern void
		arch_atomic_add(int i, atomic_t *v);

		extern void
		arch_atomic_sub(int i, atomic_t *v);

		extern bool
		arch_atomic_sub_test_zero(int i, atomic_t *v);
		
		extern void
		arch_atomic_inc(atomic_t *v);

		extern void
		arch_atomic_inc(atomic_t *v);

		extern void
		arch_atomic_dec(atomic_t *v);

		extern bool
		arch_atomic_inc_test_zero(atomic_t *v);

		extern bool
		arch_atomic_dec_test_zero(atomic_t *v);

		extern bool
		arch_atomic_add_test_negative(int i, atomic_t *v);

		extern int
		arch_atomic_add_return(int i, atomic_t *v);

		extern int
		arch_atomic_sub_return(int i, atomic_t *v);

		extern int
		arch_atomic_fetch_add(int i, atomic_t *v);

		extern int
		arch_atomic_fetch_sub(int i, atomic_t *v);

		extern int
		arch_atomic_cmpxchg(atomic_t *v, int old, int new);

		extern bool
		arch_atomic_try_cmpxchg(atomic_t *v, int *old, int new);

		extern int
		arch_atomic_xchg(atomic_t *v, int new);

		extern void
		arch_atomic_and(int i, atomic_t *v);
		extern int
		arch_atomic_fetch_and(int i, atomic_t *v);

		extern void
		arch_atomic_or(int i, atomic_t *v);
		extern int
		arch_atomic_fetch_or(int i, atomic_t *v);

		extern void
		arch_atomic_xor(int i, atomic_t *v);
		extern int
		arch_atomic_fetch_xor(int i, atomic_t *v);

	#endif

	#if defined(ARCH_ATOMIC_DEFINATION) || !(DEBUG)

		/*
		 * Atomic operations that C can't guarantee us.  Useful for
		 * resource counting etc..
		 */

		/**
		 * arch_atomic_read - read atomic variable
		 * @v: pointer of type atomic_t
		 *
		 * Atomically reads the value of @v.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_read(const atomic_t *v) {
			/*
			 * Note for KASAN: we deliberately don't use READ_ONCE_NOCHECK() here,
			 * it's non-inlined function that increases binary size and stack usage.
			 */
			return __READ_ONCE((v)->counter);
		}

		/**
		 * arch_atomic_set - set atomic variable
		 * @v: pointer of type atomic_t
		 * @i: required value
		 *
		 * Atomically sets the value of @v to @i.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_set(atomic_t *v, int i) {
			__WRITE_ONCE(v->counter, i);
		}

		/**
		 * arch_atomic_add - add integer to atomic variable
		 * @i: integer value to add
		 * @v: pointer of type atomic_t
		 *
		 * Atomically adds @i to @v.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_add(int i, atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"addl	%1,		%0		\n\t"
						:	"+m" (v->counter)
						:	"ir" (i)
						:	"memory"
						);
		}

		/**
		 * arch_atomic_sub - subtract integer from atomic variable
		 * @i: integer value to subtract
		 * @v: pointer of type atomic_t
		 *
		 * Atomically subtracts @i from @v.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_sub(int i, atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"subl	%1,		%0		\n\t"
						:	"+m" (v->counter)
						:	"ir" (i)
						:	"memory"
						);
		}

		/**
		 * arch_atomic_sub_test_zero - subtract value from variable and test result
		 * @i: integer value to subtract
		 * @v: pointer of type atomic_t
		 *
		 * Atomically subtracts @i from @v and returns
		 * true if the result is zero, or false for all
		 * other cases.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_atomic_sub_test_zero(int i, atomic_t *v) {
			// return GEN_BINARY_RMWcc(LOCK_PREFIX "subl", v->counter, e, "er", i);
			bool c;
			asm volatile(LOCK_PREFIX
							"subl	%2,		%0		\n\t"
						:	"+m" (v->counter) ,
							"=@cc" "e" (c)
						:	"er" (i)
						:	"memory"
						);
			return c;
		}

		/**
		 * arch_atomic_inc - increment atomic variable
		 * @v: pointer of type atomic_t
		 *
		 * Atomically increments @v by 1.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_inc(atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"incl	%0				\n\t"
						:	"+m"(v->counter)
						:
						:	"memory"
						);
		}

		/**
		 * arch_atomic_dec - decrement atomic variable
		 * @v: pointer of type atomic_t
		 *
		 * Atomically decrements @v by 1.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_dec(atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"decl	%0				\n\t"
						:	"+m"(v->counter)
						:
						:	"memory"
						);
		}

		/**
		 * arch_atomic_inc_test_zero - increment and test
		 * @v: pointer of type atomic_t
		 *
		 * Atomically increments @v by 1
		 * and returns true if the result is zero, or false for all
		 * other cases.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_atomic_inc_test_zero(atomic_t *v) {
			// return GEN_UNARY_RMWcc(LOCK_PREFIX "incl", v->counter, e);
			bool c;
			asm volatile(LOCK_PREFIX
							"incl	%0				\n\t"
						:	"+m"(v->counter) ,
							"=@cc" "e"(c)
						:
						:	"memory"
						);
			return c;
		}

		/**
		 * arch_atomic_dec_test_zero - decrement and test
		 * @v: pointer of type atomic_t
		 *
		 * Atomically decrements @v by 1 and
		 * returns true if the result is 0, or false for all other
		 * cases.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_atomic_dec_test_zero(atomic_t *v) {
			// return GEN_UNARY_RMWcc(LOCK_PREFIX "decl", v->counter, e);
			bool c;
			asm volatile(LOCK_PREFIX
							"decl	%0				\n\t"
						:	"+m"(v->counter) ,
							"=@cc" "e"(c)
						:
						:	"memory"
						);
			return c;
		}

		/**
		 * arch_atomic_add_test_negative - add and test if negative
		 * @i: integer value to add
		 * @v: pointer of type atomic_t
		 *
		 * Atomically adds @i to @v and returns true
		 * if the result is negative, or false when
		 * result is greater than or equal to zero.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_atomic_add_test_negative(int i, atomic_t *v) {
			// return GEN_BINARY_RMWcc(LOCK_PREFIX "addl", v->counter, s, "er", i);
			bool c;
			asm volatile(LOCK_PREFIX
							"subl	%2,		%0		\n\t"
						:	"+m" (v->counter) ,
							"=@cc" "s" (c)
						:	"er" (i)
						:	"memory"
						);
			return c;
		}

		/**
		 * arch_atomic_add_return - add integer and return
		 * @i: integer value to add
		 * @v: pointer of type atomic_t
		 *
		 * Atomically adds @i to @v and returns @i + @v
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_add_return(int i, atomic_t *v) {
			return i + xadd(&v->counter, i);
		}

		/**
		 * arch_atomic_sub_return - subtract integer and return
		 * @v: pointer of type atomic_t
		 * @i: integer value to subtract
		 *
		 * Atomically subtracts @i from @v and returns @v - @i
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_sub_return(int i, atomic_t *v) {
			return arch_atomic_add_return(-i, v);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_fetch_add(int i, atomic_t *v) {
			return xadd(&v->counter, i);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_fetch_sub(int i, atomic_t *v) {
			return xadd(&v->counter, -i);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_cmpxchg(atomic_t *v, int old, int new) {
			return arch_cmpxchg(&v->counter, old, new);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_atomic_try_cmpxchg(atomic_t *v, int *old, int new) {
			return arch_try_cmpxchg(&v->counter, old, new);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_xchg(atomic_t *v, int new) {
			return arch_xchg(&v->counter, new);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_and(int i, atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"andl	%1,		%0	\n\t"
						:	"+m"(v->counter)
						:	"ir"(i)
						:	"memory"
						);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_fetch_and(int i, atomic_t *v) {
			int val = arch_atomic_read(v);
			do { } while (!arch_atomic_try_cmpxchg(v, &val, val & i));
			return val;
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_or(int i, atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"orl	%1,		%0	\n\t"
						:	"+m"(v->counter)
						:	"ir"(i)
						:	"memory"
						);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_fetch_or(int i, atomic_t *v) {
			int val = arch_atomic_read(v);
			do { } while (!arch_atomic_try_cmpxchg(v, &val, val | i));
			return val;
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_atomic_xor(int i, atomic_t *v) {
			asm volatile(LOCK_PREFIX
							"xorl	%1,		%0	\n\t"
						:	"+m"(v->counter)
						:	"ir"(i)
						:	"memory"
						);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		int
		arch_atomic_fetch_xor(int i, atomic_t *v) {
			int val = arch_atomic_read(v);
			do { } while (!arch_atomic_try_cmpxchg(v, &val, val ^ i));
			return val;
		}

	#endif

	#define arch_atomic_sub_amd_test	arch_atomic_sub_test_zero
	#define arch_atomic_inc_and_test	arch_atomic_inc_test_zero
	#define arch_atomic_dec_and_test	arch_atomic_dec_test_zero
	#define arch_atomic_add_negative	arch_atomic_add_test_negative

	#define arch_atomic_fetch_and	arch_atomic_fetch_and
	#define arch_atomic_fetch_or	arch_atomic_fetch_or
	#define arch_atomic_fetch_xor	arch_atomic_fetch_xor

#endif /* _ASM_X86_ATOMIC_H_ */
