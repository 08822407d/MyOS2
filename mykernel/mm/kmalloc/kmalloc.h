#ifndef _LINUX_KMALLOC_H_
#define _LINUX_KMALLOC_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/kernel/overflow.h>
	#include <linux/mm/mm.h>

	#include "kmalloc_const.h"
	#include "kmalloc_types.h"
	#include "myos_slab_types.h"

	#define kmalloc_index(s) __kmalloc_index(s, true)

	#ifdef DEBUG

		extern unsigned int
		__kmalloc_index(size_t size, bool size_is_constant);

		extern void
		*kmalloc_array(size_t n, size_t size, gfp_t flags);

		extern void
		*kcalloc(size_t n, size_t size, gfp_t flags);

		extern void
		*kzalloc(size_t size, gfp_t flags);

	#endif
	
	#if defined(KMALLOC_DEFINATION) || !(DEBUG)

		/*
		 * Figure out which kmalloc slab an allocation of a certain size
		 * belongs to.
		 * 0 = zero alloc
		 * 1 =  65 .. 96 bytes
		 * 2 = 129 .. 192 bytes
		 * n = 2^(n-1)+1 .. 2^n
		 *
		 * Note: __kmalloc_index() is compile-time optimized, and not runtime optimized;
		 * typical usage is via kmalloc_index() and therefore evaluated at compile-time.
		 * Callers where !size_is_constant should only be test modules, where runtime
		 * overheads of __kmalloc_index() can be tolerated.  Also see kmalloc_slab().
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		unsigned int
		__kmalloc_index(size_t size, bool size_is_constant) {
			if (!size)
				return 0;

			if (size <= KMALLOC_MIN_SIZE)
				return KMALLOC_SHIFT_LOW;

			if (KMALLOC_MIN_SIZE <= 32 && size > 64 && size <= 96)
				return 1;
			if (KMALLOC_MIN_SIZE <= 64 && size > 128 && size <= 192)
				return 2;
			if (size <= 8)
				return 3;
			if (size <= 16)
				return 4;
			if (size <= 32)
				return 5;
			if (size <= 64)
				return 6;
			if (size <= 128)
				return 7;
			if (size <= 256)
				return 8;
			if (size <= 512)
				return 9;
			if (size <= 1024)
				return 10;
			if (size <= 2 * 1024)
				return 11;
			if (size <= 4 * 1024)
				return 12;
			if (size <= 8 * 1024)
				return 13;
			if (size <= 16 * 1024)
				return 14;
			if (size <= 32 * 1024)
				return 15;
			if (size <= 64 * 1024)
				return 16;
			if (size <= 128 * 1024)
				return 17;
			if (size <= 256 * 1024)
				return 18;
			if (size <= 512 * 1024)
				return 19;
			if (size <= 1024 * 1024)
				return 20;
			if (size <= 2 * 1024 * 1024)
				return 21;
			if (size <= 4 * 1024 * 1024)
				return 22;
			if (size <= 8 * 1024 * 1024)
				return 23;
			if (size <= 16 * 1024 * 1024)
				return 24;
			if (size <= 32 * 1024 * 1024)
				return 25;

			// if (!IS_ENABLED(CONFIG_PROFILE_ALL_BRANCHES) && size_is_constant)
			// 	BUILD_BUG_ON_MSG(1, "unexpected size in kmalloc_index()");
			// else
			// 	BUG();

			/* Will never be reached. Needed because the compiler may complain */
			return -1;
		}

		/**
		 * kmalloc_array - allocate memory for an array.
		 * @n: number of elements.
		 * @size: element size.
		 * @flags: the type of memory to allocate (see kmalloc).
		 */
		PREFIX_STATIC_INLINE
		void
		*kmalloc_array(size_t n, size_t size, gfp_t flags) {
			size_t bytes;

			if (unlikely(check_mul_overflow(n, size, &bytes)))
				return NULL;
			if (__builtin_constant_p(n) && __builtin_constant_p(size))
				return kmalloc(bytes, flags);
			return __kmalloc(bytes, flags);
		}

		/**
		 * kcalloc - allocate memory for an array. The memory is set to zero.
		 * @n: number of elements.
		 * @size: element size.
		 * @flags: the type of memory to allocate (see kmalloc).
		 */
		PREFIX_STATIC_INLINE
		void
		*kcalloc(size_t n, size_t size, gfp_t flags) {
			return kmalloc_array(n, size, flags | __GFP_ZERO);
		}


		/**
		 * kzalloc - allocate memory. The memory is set to zero.
		 * @size: how many bytes of memory are required.
		 * @flags: the type of memory to allocate (see kmalloc).
		 */
		PREFIX_STATIC_INLINE
		void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}

	#endif

#endif /* _LINUX_KMALLOC_H_ */