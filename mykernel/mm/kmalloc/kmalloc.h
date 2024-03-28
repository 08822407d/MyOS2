#ifndef _LINUX_KMALLOC_H_
#define _LINUX_KMALLOC_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/kernel/overflow.h>
	#include <linux/kernel/mm.h>

	#include "kmalloc_const.h"
	#include "kmalloc_types.h"
	#include "myos_slab_types.h"

	#include "slub_types.h"

	#define kmalloc_index(s) __kmalloc_index(s, true)

	#ifdef DEBUG

		extern unsigned int
		__kmalloc_index(size_t size, bool size_is_constant);

		extern __alloc_size(1) void
		*kmalloc(size_t size, gfp_t flags);

		extern __alloc_size(1, 2) void
		*kmalloc_array(size_t n, size_t size, gfp_t flags);

		extern __alloc_size(1, 2) void
		*kcalloc(size_t n, size_t size, gfp_t flags);

		extern __alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags);

		extern void
		kfree(const void *object);

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
		 * kmalloc - allocate kernel memory
		 * @size: how many bytes of memory are required.
		 * @flags: describe the allocation context
		 *
		 * kmalloc is the normal method of allocating memory
		 * for objects smaller than page size in the kernel.
		 *
		 * The allocated object address is aligned to at least ARCH_KMALLOC_MINALIGN
		 * bytes. For @size of power of two bytes, the alignment is also guaranteed
		 * to be at least to the size.
		 *
		 * The @flags argument may be one of the GFP flags defined at
		 * include/linux/gfp_types.h and described at
		 * :ref:`Documentation/core-api/mm-api.rst <mm-api-gfp-flags>`
		 *
		 * The recommended usage of the @flags is described at
		 * :ref:`Documentation/core-api/memory-allocation.rst <memory_allocation>`
		 *
		 * Below is a brief outline of the most useful GFP flags
		 *
		 * %GFP_KERNEL
		 *	Allocate normal kernel ram. May sleep.
		 *
		 * %GFP_NOWAIT
		 *	Allocation will not sleep.
		 *
		 * %GFP_ATOMIC
		 *	Allocation will not sleep.  May use emergency pools.
		 *
		 * Also it is possible to set different flags by OR'ing
		 * in one or more of the following additional @flags:
		 *
		 * %__GFP_ZERO
		 *	Zero the allocated memory before returning. Also see kzalloc().
		 *
		 * %__GFP_HIGH
		 *	This allocation has high priority and may use emergency pools.
		 *
		 * %__GFP_NOFAIL
		 *	Indicate that this allocation is in no way allowed to fail
		 *	(think twice before using).
		 *
		 * %__GFP_NORETRY
		 *	If memory is not immediately available,
		 *	then give up at once.
		 *
		 * %__GFP_NOWARN
		 *	If allocation fails, don't issue any warnings.
		 *
		 * %__GFP_RETRY_MAYFAIL
		 *	Try really hard to succeed the allocation but fail
		 *	eventually.
		 */
		PREFIX_STATIC_AWLWAYS_INLINE
		__alloc_size(1) void
		*kmalloc(size_t size, gfp_t flags) {
			void *ret_val = NULL;
			if (size > KMALLOC_MAX_CACHE_SIZE)
				ret_val = kmalloc_large(size, flags);
			else
				ret_val = __kmalloc(size, flags);

			return ret_val;
		}

		// /**
		//  * kmalloc_array - allocate memory for an array.
		//  * @n: number of elements.
		//  * @size: element size.
		//  * @flags: the type of memory to allocate (see kmalloc).
		//  */
		// PREFIX_STATIC_INLINE
		// __alloc_size(1, 2) void
		// *kmalloc_array(size_t n, size_t size, gfp_t flags) {
		// 	size_t bytes;

		// 	if (unlikely(check_mul_overflow(n, size, &bytes)))
		// 		return NULL;
		// 	if (__builtin_constant_p(n) && __builtin_constant_p(size))
		// 		return kmalloc(bytes, flags);
		// 	return __kmalloc(bytes, flags);
		// }

		// /**
		//  * kcalloc - allocate memory for an array. The memory is set to zero.
		//  * @n: number of elements.
		//  * @size: element size.
		//  * @flags: the type of memory to allocate (see kmalloc).
		//  */
		// PREFIX_STATIC_INLINE
		// __alloc_size(1, 2) void
		// *kcalloc(size_t n, size_t size, gfp_t flags) {
		// 	return kmalloc_array(n, size, flags | __GFP_ZERO);
		// }


		/**
		 * kzalloc - allocate memory. The memory is set to zero.
		 * @size: how many bytes of memory are required.
		 * @flags: the type of memory to allocate (see kmalloc).
		 */
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}


		/**
		 * kfree - free previously allocated memory
		 * @object: pointer returned by kmalloc() or kmem_cache_alloc()
		 *
		 * If @object is NULL, no operation is performed.
		 */
		PREFIX_STATIC_INLINE
		void
		kfree(const void *object) {
			page_s *slab;
			kmem_cache_s *s;

			if (unlikely(object == NULL))
				return;

			slab = virt_to_page(object);
			// if (unlikely(!folio_test_slab(folio)))
			if (PageHead(slab))
				free_large_kmalloc((folio_s *)slab, (void *)object);
			else
				// myos_kfree(object);
				kmem_cache_free(slab->slab_cache, (void *)object);
		}

	#endif

#endif /* _LINUX_KMALLOC_H_ */