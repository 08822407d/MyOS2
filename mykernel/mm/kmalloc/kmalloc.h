#ifndef _LINUX_KMALLOC_H_
#define _LINUX_KMALLOC_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern uint
		arch_slab_minalign(void);

		extern void
		*kmem_cache_zalloc(kmem_cache_s *k, gfp_t flags);

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
		 * Arches can define this function if they want to decide the minimum slab
		 * alignment at runtime. The value returned by the function must be a power
		 * of two and >= ARCH_SLAB_MINALIGN.
		 */
		PREFIX_STATIC_INLINE
		uint
		arch_slab_minalign(void) {
			return ARCH_SLAB_MINALIGN;
		}

		PREFIX_STATIC_INLINE
		void
		*kmem_cache_zalloc(kmem_cache_s *k, gfp_t flags) {
			return kmem_cache_alloc(k, flags | __GFP_ZERO);
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

		/**
		 * kmalloc_array - allocate memory for an array.
		 * @n: number of elements.
		 * @size: element size.
		 * @flags: the type of memory to allocate (see kmalloc).
		 */
		PREFIX_STATIC_INLINE
		__alloc_size(1, 2) void
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
		__alloc_size(1, 2) void
		*kcalloc(size_t n, size_t size, gfp_t flags) {
			return kmalloc_array(n, size, flags | __GFP_ZERO);
		}

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
			folio_s *folio;
			page_s *slab;
			kmem_cache_s *s;

			if (unlikely(object == NULL))
				return;

			folio = virt_to_folio(object);
			if (unlikely(!folio_test_slab(folio)))
				free_large_kmalloc(folio, (void *)object);
			else
				kmem_cache_free(((slab_s *)folio)->slab_cache, (void *)object);
		}

	#endif

#endif /* _LINUX_KMALLOC_H_ */