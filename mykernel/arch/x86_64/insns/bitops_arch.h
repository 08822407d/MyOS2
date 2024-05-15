/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BITOPS_H_
#define _ASM_X86_BITOPS_H_

	#include "bitops_const_arch.h"


	/*
	 * Copyright 1992, Linus Torvalds.
	 *
	 * Note: inlines with more than a single statement should be marked
	 * __always_inline to avoid problems with older gcc's inlining heuristics.
	 */
	#include <linux/compiler/myos_debug_option.h>
	#include <linux/compiler/compiler.h>
	#include <asm/alternative.h>
	#include <asm/rmwcc.h>

	#include <linux/kernel/types.h>


	#define test_bit(nr, addr) (						\
				__builtin_constant_p((nr))				\
					? constant_test_bit((nr), (addr))	\
					: variable_test_bit((nr), (addr))	\
			)

	#define arch_test_bit(nr, addr) (					\
				__builtin_constant_p((nr))				\
					? constant_test_bit((nr), (addr))	\
					: variable_test_bit((nr), (addr))	\
			)


	#ifdef DEBUG

		extern unsigned long
		__ffs(unsigned long word);
		extern unsigned long
		ffz(unsigned long word);
		extern unsigned long
		__fls(unsigned long word);
		extern int
		ffs(int x);
		extern int
		fls(unsigned int x);
		extern int
		fls64(__u64 x);


		extern void
		arch_set_bit(long nr, volatile unsigned long *addr);
		#define set_bit arch_set_bit
		extern void
		arch___set_bit(long nr, volatile unsigned long *addr);
		#define __set_bit arch___set_bit
		extern void
		arch_clear_bit(long nr, volatile unsigned long *addr);
		#define clear_bit arch_clear_bit
		extern void
		arch_clear_bit_unlock(long nr, volatile unsigned long *addr);
		#define clear_bit_unlock arch_clear_bit_unlock
		extern void
		arch___clear_bit(long nr, volatile unsigned long *addr);
		#define __clear_bit arch___clear_bit

		extern void
		arch___clear_bit_unlock(long nr, volatile unsigned long *addr);
		#define __clear_bit_unlock arch___clear_bit_unlock
		extern void
		arch___change_bit(long nr, volatile unsigned long *addr);
		#define __change_bit arch___change_bit
		extern void
		arch_change_bit(long nr, volatile unsigned long *addr);
		#define change_bit arch_change_bit


		extern bool
		arch_test_and_set_bit(long nr, volatile unsigned long *addr);
		#define test_and_set_bit arch_test_and_set_bit
		extern bool
		arch_test_and_set_bit_lock(long nr, volatile unsigned long *addr);
		#define test_and_set_bit_lock arch_test_and_set_bit_lock
		extern bool
		arch___test_and_set_bit(long nr, volatile unsigned long *addr);
		#define __test_and_set_bit arch___test_and_set_bit

		extern bool
		arch_test_and_clear_bit(long nr, volatile unsigned long *addr);
		#define test_and_clear_bit arch_test_and_clear_bit
		extern bool
		arch___test_and_clear_bit(long nr, volatile unsigned long *addr);
		#define __test_and_clear_bit arch___test_and_clear_bit

		extern bool
		arch___test_and_change_bit(long nr, volatile unsigned long *addr);
		#define __test_and_change_bit arch___test_and_change_bit
		extern bool
		arch_test_and_change_bit(long nr, volatile unsigned long *addr);
		#define test_and_change_bit arch_test_and_change_bit


		extern bool
		constant_test_bit(long nr, const volatile unsigned long *addr);
		extern bool
		variable_test_bit(long nr, volatile const unsigned long *addr);

	#endif
	
	#if defined(ARCH_X86_BITOPS_DEFINATION) || !(DEBUG)

		/**
		 * __ffs - find first set bit in word
		 * @word: The word to search
		 *
		 * Undefined if no bit exists, so code should check against 0 first.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long
		__ffs(unsigned long word) {
			asm (	"rep					\t\n"
					"bsf	%1,		%0		\t\n"
				:	"=r" (word)
				:	"rm" (word)
				);
			return word;
		}
		/**
		 * ffz - find first zero bit in word
		 * @word: The word to search
		 *
		 * Undefined if no zero exists, so code should check against ~0UL first.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long
		ffz(unsigned long word) {
			asm (	"rep					\t\n"
					"bsf	%1,		%0		\t\n"
				:	"=r" (word)
				:	"r" (~word)
				);
			return word;
		}
		/*
		 * __fls: find last set bit in word
		 * @word: The word to search
		 *
		 * Undefined if no set bit exists, so code should check against 0 first.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned long
		__fls(unsigned long word) {
			asm (	"bsr	%1,		%0		\t\n"
				:	"=r" (word)
				:	"rm" (word)
				);
			return word;
		}
		/**
		 * ffs - find first set bit in word
		 * @x: the word to search
		 *
		 * This is defined the same way as the libc and compiler builtin ffs
		 * routines, therefore differs in spirit from the other bitops.
		 *
		 * ffs(value) returns 0 if value is 0 or the position of the first
		 * set bit if value is nonzero. The first (least significant) bit
		 * is at position 1.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		ffs(int x) {
			int r;

			/*
			 * AMD64 says BSFL won't clobber the dest reg if x==0; Intel64 says the
			 * dest reg is undefined if x==0, but their CPU architect says its
			 * value is written to set it to the same as before, except that the
			 * top 32 bits will be cleared.
			 *
			 * We cannot do this on 32 bits because at the very least some
			 * 486 CPUs did not behave this way.
			 */
			asm (	"bsfl	%1,		%0		\t\n"
				:	"=r" (r)
				:	"rm" (x),
					"0" (-1)
				);

			return r + 1;
		}
		/**
		 * fls - find last set bit in word
		 * @x: the word to search
		 *
		 * This is defined in a similar way as the libc and compiler builtin
		 * ffs, but returns the position of the most significant set bit.
		 *
		 * fls(value) returns 0 if value is 0 or the position of the last
		 * set bit if value is nonzero. The last (most significant) bit is
		 * at position 32.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		fls(unsigned int x) {
			int r;

			/*
			 * AMD64 says BSRL won't clobber the dest reg if x==0; Intel64 says the
			 * dest reg is undefined if x==0, but their CPU architect says its
			 * value is written to set it to the same as before, except that the
			 * top 32 bits will be cleared.
			 *
			 * We cannot do this on 32 bits because at the very least some
			 * 486 CPUs did not behave this way.
			 */
			asm (	"bsrl	%1,		%0		\n\t"
				:	"=r" (r)
				:	"rm" (x),
					"0" (-1)
				);

			return r + 1;
		}
		/**
		 * fls64 - find last set bit in a 64-bit word
		 * @x: the word to search
		 *
		 * This is defined in a similar way as the libc and compiler builtin
		 * ffsll, but returns the position of the most significant set bit.
		 *
		 * fls64(value) returns 0 if value is 0 or the position of the last
		 * set bit if value is nonzero. The last (most significant) bit is
		 * at position 64.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		int
		fls64(__u64 x) {
			int bitpos = -1;
			/*
			 * AMD64 says BSRQ won't clobber the dest reg if x==0; Intel64 says the
			 * dest reg is undefined if x==0, but their CPU architect says its
			 * value is written to set it to the same as before.
			 */
			asm (	"bsrq	%1,		%q0		\n\t"
				:	"+r" (bitpos)
				:	"rm" (x)
				);
			return bitpos + 1;
		}





		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_set_bit(long nr, volatile unsigned long *addr) {
			if (__builtin_constant_p(nr)) {
				asm volatile(LOCK_PREFIX
								"orb	%b1,	%0		\t\n"
							:	CONST_MASK_ADDR (nr, addr)
							:	"iq" (CONST_MASK(nr))
							:	"memory"
							);
			}
			else {
				asm volatile(LOCK_PREFIX
								"btsq	%1,		%0		\t\n"
							:
							:	RLONG_ADDR (addr),
								"Ir" (nr)
							:	"memory"
							);
			}
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch___set_bit(long nr, volatile unsigned long *addr) {
			asm volatile(	"btsq	%1,		%0		\n\t"
						:
						:	ADDR,
							"Ir" (nr)
						:	"memory"
						);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_clear_bit(long nr, volatile unsigned long *addr) {
			if (__builtin_constant_p(nr)) {
				asm volatile(LOCK_PREFIX
								"andb	%b1,	%0		\n\t"
							:	CONST_MASK_ADDR (nr, addr)
							:	"iq" (~CONST_MASK(nr))
							);
			} else {
				asm volatile(LOCK_PREFIX
								"btrq	%1,		%0		\n\t"
							:
							:	RLONG_ADDR (addr),
								"Ir" (nr)
							:	"memory"
							);
			}
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_clear_bit_unlock(long nr, volatile unsigned long *addr) {
			barrier();
			arch_clear_bit(nr, addr);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch___clear_bit(long nr, volatile unsigned long *addr) {
			asm volatile(	"btrq	%1,		%0		\n\t"
						:
						:	ADDR,
							"Ir" (nr)
						:	"memory"
						);
		}
		// static __always_inline bool
		// arch_clear_bit_unlock_is_negative_byte(long nr, volatile unsigned long *addr)
		// {
		// 	bool negative;
		// 	asm volatile(LOCK_PREFIX "andb %2,%1" CC_SET(s)
		// 				: CC_OUT(s)(negative), WBYTE_ADDR(addr)
		// 				: "ir"((char)~(1 << nr))
		// 				: "memory");
		// 	return negative;
		// }
		// #define arch_clear_bit_unlock_is_negative_byte \
		// 	arch_clear_bit_unlock_is_negative_byte
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch___clear_bit_unlock(long nr, volatile unsigned long *addr) {
			arch___clear_bit(nr, addr);
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch___change_bit(long nr, volatile unsigned long *addr) {
			asm volatile(	"btcq	%1,		%0		\n\t"
						:
						:	ADDR,
							"Ir"(nr)
						:	"memory"
						);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		arch_change_bit(long nr, volatile unsigned long *addr) {
			if (__builtin_constant_p(nr)) {
				asm volatile(LOCK_PREFIX
								"xorb	%b1,	%0		\n\t"
							:	CONST_MASK_ADDR (nr, addr)
							:	"iq" (CONST_MASK(nr))
							);
			}
			else {
				asm volatile(LOCK_PREFIX
								"btcq	%1,		%0		\n\t"
							:
							:	RLONG_ADDR (addr),
								"Ir" (nr)
							:	"memory"
							);
			}
		}


		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_test_and_set_bit(long nr, volatile unsigned long *addr) {
			// return GEN_BINARY_RMWcc(LOCK_PREFIX "btsq", *addr, c, "Ir", nr);
			bool c;
			asm volatile(LOCK_PREFIX
							"btsq	%2,		%0		\n\t"
						:	"+m" (*addr),
							"=@cc" "c" (c)
						:	"Ir" (nr)
						:	"memory"
						);
			return c;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_test_and_set_bit_lock(long nr, volatile unsigned long *addr) {
			return arch_test_and_set_bit(nr, addr);
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch___test_and_set_bit(long nr, volatile unsigned long *addr) {
			bool oldbit;
			asm (	"btsq	%2,		%1		\n\t"
				:	"=@cc" "c" (oldbit)
				:	ADDR,
					"Ir" (nr)
				:	"memory"
				);
			return oldbit;
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_test_and_clear_bit(long nr, volatile unsigned long *addr) {
			// return GEN_BINARY_RMWcc(LOCK_PREFIX "btrq", *addr, c, "Ir", nr);
			bool c;
			asm volatile(LOCK_PREFIX
							"btrq	%2,		%0		\n\t"
						:	"+m" (*addr),
							"=@cc" "c" (c)
						:	"Ir" (nr)
						:	"memory"
						);
			return c;
		}
		/*
		 * Note: the operation is performed atomically with respect to
		 * the local CPU, but not other CPUs. Portable code should not
		 * rely on this behaviour.
		 * KVM relies on this behaviour on x86 for modifying memory that is also
		 * accessed from a hypervisor on the same CPU if running in a VM: don't change
		 * this without also updating arch/x86/kernel/kvm.c
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch___test_and_clear_bit(long nr, volatile unsigned long *addr) {
			bool oldbit;
			asm (	"btrq	%2,		%1		\n\t"
				:	"=@cc" "c" (oldbit)
				:	ADDR,
					"Ir" (nr)
				:	"memory"
				);
			return oldbit;
		}

		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch___test_and_change_bit(long nr, volatile unsigned long *addr) {
			bool oldbit;
			asm volatile(	"btcq	%2,		%1		\n\t"
						:	"=@cc" "c" (oldbit)
						:	ADDR,
							"Ir" (nr)
						:	"memory"
						);
			return oldbit;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		arch_test_and_change_bit(long nr, volatile unsigned long *addr) {
			// return GEN_BINARY_RMWcc(LOCK_PREFIX "btcq", *addr, c, "Ir", nr);
			bool c;
			asm volatile(LOCK_PREFIX
							"btcq	%2,		%0		\n\t"
						:	"+m" (*addr),
							"=@cc" "c" (c)
						:	"Ir" (nr)
						:	"memory"
						);
			return c;
		}


		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		constant_test_bit(long nr, const volatile unsigned long *addr) {
			return ((1UL << (nr & (BITS_PER_LONG - 1))) &
					(addr[nr >> _BITOPS_LONG_SHIFT])) != 0;
		}
		PREFIX_STATIC_AWLWAYS_INLINE
		bool
		variable_test_bit(long nr, volatile const unsigned long *addr) {
			bool oldbit;

			asm volatile(	"btq	%2,		%1		\n\t"
							"setc	%0				\n\t"
						:	"=qm" (oldbit)
						:	"m" (*(unsigned long *)addr),
							"Ir" (nr)
						:	"memory"
						);

			return oldbit;
		}

	#endif

#endif /* _ASM_X86_BITOPS_H */
