/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BITOPS_H
#define _LINUX_BITOPS_H

	// #include <asm/types.h>
	#include <linux/kernel/bits.h>
	// #include <linux/typecheck.h>

	#include <uapi/linux/kernel.h>

	// /* Set bits in the first 'n' bytes when loaded from memory */
	// #ifdef __LITTLE_ENDIAN
	// #  define aligned_byte_mask(n) ((1UL << 8*(n))-1)
	// #else
	// #  define aligned_byte_mask(n) (~0xffUL << (BITS_PER_LONG - 8 - 8*(n)))
	// #endif

	// #define BITS_PER_TYPE(type)	(sizeof(type) * BITS_PER_BYTE)
	// #define BITS_TO_LONGS(nr)	__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(long))
	// #define BITS_TO_U64(nr)		__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(u64))
	// #define BITS_TO_U32(nr)		__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(u32))
	// #define BITS_TO_BYTES(nr)	__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(char))

	// extern unsigned int __sw_hweight8(unsigned int w);
	// extern unsigned int __sw_hweight16(unsigned int w);
	// extern unsigned int __sw_hweight32(unsigned int w);
	// extern unsigned long __sw_hweight64(__u64 w);

	/*
	* Include this here because some architectures need generic_ffs/fls in
	* scope
	*/
	#include <asm/bitops.h>

	// static inline int get_bitmask_order(unsigned int count)
	// {
	// 	int order;

	// 	order = fls(count);
	// 	return order;	/* We could be slightly more clever with -1 here... */
	// }

	// static __always_inline unsigned long hweight_long(unsigned long w)
	// {
	// 	return sizeof(w) == 4 ? hweight32(w) : hweight64((__u64)w);
	// }

	// /**
	//  * rol64 - rotate a 64-bit value left
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u64 rol64(__u64 word, unsigned int shift)
	// {
	// 	return (word << (shift & 63)) | (word >> ((-shift) & 63));
	// }

	// /**
	//  * ror64 - rotate a 64-bit value right
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u64 ror64(__u64 word, unsigned int shift)
	// {
	// 	return (word >> (shift & 63)) | (word << ((-shift) & 63));
	// }

	// /**
	//  * rol32 - rotate a 32-bit value left
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u32 rol32(__u32 word, unsigned int shift)
	// {
	// 	return (word << (shift & 31)) | (word >> ((-shift) & 31));
	// }

	// /**
	//  * ror32 - rotate a 32-bit value right
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u32 ror32(__u32 word, unsigned int shift)
	// {
	// 	return (word >> (shift & 31)) | (word << ((-shift) & 31));
	// }

	// /**
	//  * rol16 - rotate a 16-bit value left
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u16 rol16(__u16 word, unsigned int shift)
	// {
	// 	return (word << (shift & 15)) | (word >> ((-shift) & 15));
	// }

	// /**
	//  * ror16 - rotate a 16-bit value right
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u16 ror16(__u16 word, unsigned int shift)
	// {
	// 	return (word >> (shift & 15)) | (word << ((-shift) & 15));
	// }

	// /**
	//  * rol8 - rotate an 8-bit value left
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u8 rol8(__u8 word, unsigned int shift)
	// {
	// 	return (word << (shift & 7)) | (word >> ((-shift) & 7));
	// }

	// /**
	//  * ror8 - rotate an 8-bit value right
	//  * @word: value to rotate
	//  * @shift: bits to roll
	//  */
	// static inline __u8 ror8(__u8 word, unsigned int shift)
	// {
	// 	return (word >> (shift & 7)) | (word << ((-shift) & 7));
	// }

	// /**
	//  * sign_extend32 - sign extend a 32-bit value using specified bit as sign-bit
	//  * @value: value to sign extend
	//  * @index: 0 based bit index (0<=index<32) to sign bit
	//  *
	//  * This is safe to use for 16- and 8-bit types as well.
	//  */
	// static __always_inline __s32 sign_extend32(__u32 value, int index)
	// {
	// 	__u8 shift = 31 - index;
	// 	return (__s32)(value << shift) >> shift;
	// }

	// /**
	//  * sign_extend64 - sign extend a 64-bit value using specified bit as sign-bit
	//  * @value: value to sign extend
	//  * @index: 0 based bit index (0<=index<64) to sign bit
	//  */
	// static __always_inline __s64 sign_extend64(__u64 value, int index)
	// {
	// 	__u8 shift = 63 - index;
	// 	return (__s64)(value << shift) >> shift;
	// }

	// static inline unsigned fls_long(unsigned long l)
	// {
	// 	if (sizeof(l) == 4)
	// 		return fls(l);
	// 	return fls64(l);
	// }

	// static inline int get_count_order(unsigned int count)
	// {
	// 	if (count == 0)
	// 		return -1;

	// 	return fls(--count);
	// }

	// /**
	//  * get_count_order_long - get order after rounding @l up to power of 2
	//  * @l: parameter
	//  *
	//  * it is same as get_count_order() but with long type parameter
	//  */
	// static inline int get_count_order_long(unsigned long l)
	// {
	// 	if (l == 0UL)
	// 		return -1;
	// 	return (int)fls_long(--l);
	// }

	// /**
	//  * __ffs64 - find first set bit in a 64 bit word
	//  * @word: The 64 bit word
	//  *
	//  * On 64 bit arches this is a synonym for __ffs
	//  * The result is not defined if no bits are set, so check that @word
	//  * is non-zero before calling this.
	//  */
	// static inline unsigned long __ffs64(u64 word)
	// {
	// 	return __ffs((unsigned long)word);
	// }

	// /**
	//  * assign_bit - Assign value to a bit in memory
	//  * @nr: the bit to set
	//  * @addr: the address to start counting from
	//  * @value: the value to assign
	//  */
	// static __always_inline void assign_bit(long nr, volatile unsigned long *addr,
	// 					bool value)
	// {
	// 	if (value)
	// 		set_bit(nr, addr);
	// 	else
	// 		clear_bit(nr, addr);
	// }

	// static __always_inline void __assign_bit(long nr, volatile unsigned long *addr,
	// 					bool value)
	// {
	// 	if (value)
	// 		__set_bit(nr, addr);
	// 	else
	// 		__clear_bit(nr, addr);
	// }

	// /**
	//  * __ptr_set_bit - Set bit in a pointer's value
	//  * @nr: the bit to set
	//  * @addr: the address of the pointer variable
	//  *
	//  * Example:
	//  *	void *p = foo();
	// *	__ptr_set_bit(bit, &p);
	// */
	// #define __ptr_set_bit(nr, addr)                         \
	// 	({                                              \
	// 		typecheck_pointer(*(addr));             \
	// 		__set_bit(nr, (unsigned long *)(addr)); \
	// 	})

	// /**
	//  * __ptr_clear_bit - Clear bit in a pointer's value
	//  * @nr: the bit to clear
	//  * @addr: the address of the pointer variable
	//  *
	//  * Example:
	//  *	void *p = foo();
	// *	__ptr_clear_bit(bit, &p);
	// */
	// #define __ptr_clear_bit(nr, addr)                         \
	// 	({                                                \
	// 		typecheck_pointer(*(addr));               \
	// 		__clear_bit(nr, (unsigned long *)(addr)); \
	// 	})

	// /**
	//  * __ptr_test_bit - Test bit in a pointer's value
	//  * @nr: the bit to test
	//  * @addr: the address of the pointer variable
	//  *
	//  * Example:
	//  *	void *p = foo();
	// *	if (__ptr_test_bit(bit, &p)) {
	// *	        ...
	// *	} else {
	// *		...
	// *	}
	// */
	// #define __ptr_test_bit(nr, addr)                       \
	// 	({                                             \
	// 		typecheck_pointer(*(addr));            \
	// 		test_bit(nr, (unsigned long *)(addr)); \
	// 	})

	// #ifdef __KERNEL__

	// 	#ifndef set_mask_bits
	// 	#define set_mask_bits(ptr, mask, bits)	\
	// 	({								\
	// 		const typeof(*(ptr)) mask__ = (mask), bits__ = (bits);	\
	// 		typeof(*(ptr)) old__, new__;				\
	// 									\
	// 		do {							\
	// 			old__ = READ_ONCE(*(ptr));			\
	// 			new__ = (old__ & ~mask__) | bits__;		\
	// 		} while (cmpxchg(ptr, old__, new__) != old__);		\
	// 									\
	// 		old__;							\
	// 	})
	// 	#endif

	// 	#ifndef bit_clear_unless
	// 	#define bit_clear_unless(ptr, clear, test)	\
	// 	({								\
	// 		const typeof(*(ptr)) clear__ = (clear), test__ = (test);\
	// 		typeof(*(ptr)) old__, new__;				\
	// 									\
	// 		do {							\
	// 			old__ = READ_ONCE(*(ptr));			\
	// 			new__ = old__ & ~clear__;			\
	// 		} while (!(old__ & test__) &&				\
	// 			cmpxchg(ptr, old__, new__) != old__);		\
	// 									\
	// 		!(old__ & test__);					\
	// 	})
	// 	#endif

	// #endif /* __KERNEL__ */

#endif
