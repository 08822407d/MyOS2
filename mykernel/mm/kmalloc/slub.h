#ifndef _LINUX_SLUB_H_
#define _LINUX_SLUB_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern slab_s
		*virt_to_slab(const void *addr);
		extern int
		slab_order(const slab_s *slab);
		extern size_t
		slab_size(const slab_s *slab);


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

	#include "slub_macro.h"
	
	#if defined(SLUB_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		slab_s
		*virt_to_slab(const void *addr) {
			folio_s *folio = virt_to_folio(addr);
			if (!folio_test_slab(folio))
				return NULL;
			return folio_slab(folio);
		}
		PREFIX_STATIC_INLINE
		int
		slab_order(const slab_s *slab) {
			return folio_order((folio_s *)slab_folio(slab));
		}
		PREFIX_STATIC_INLINE
		size_t
		slab_size(const slab_s *slab) {
			return PAGE_SIZE << slab_order(slab);
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
				kmem_cache_free(folio_slab(folio)->slab_cache, (void *)object);
		}


		static inline void
		slab_post_alloc_hook(kmem_cache_s *s, gfp_t flags, void **obj_p) {
			unsigned int zero_size = s->object_size;
			// bool kasan_init = init;
			// size_t i;

			// flags &= gfp_allowed_mask;

			// /*
			// * For kmalloc object, the allocated memory size(object_size) is likely
			// * larger than the requested size(orig_size). If redzone check is
			// * enabled for the extra space, don't zero it, as it will be redzoned
			// * soon. The redzone operation for this extra space could be seen as a
			// * replacement of current poisoning under certain debug option, and
			// * won't break other sanity checks.
			// */
			// if (kmem_cache_debug_flags(s, SLAB_STORE_USER | SLAB_RED_ZONE) &&
			// 	(s->flags & SLAB_KMALLOC))
			// 	zero_size = orig_size;

			// /*
			// * When slub_debug is enabled, avoid memory initialization integrated
			// * into KASAN and instead zero out the memory via the memset below with
			// * the proper size. Otherwise, KASAN might overwrite SLUB redzones and
			// * cause false-positive reports. This does not lead to a performance
			// * penalty on production builds, as slub_debug is not intended to be
			// * enabled there.
			// */
			// if (__slub_debug_enabled())
			// 	kasan_init = false;

			// /*
			// * As memory initialization might be integrated into KASAN,
			// * kasan_slab_alloc and initialization memset must be
			// * kept together to avoid discrepancies in behavior.
			// *
			// * As p[i] might get tagged, memset and kmemleak hook come after KASAN.
			// */
			// for (i = 0; i < size; i++) {
			// 	p[i] = kasan_slab_alloc(s, p[i], flags, kasan_init);
			// 	if (p[i] && init && (!kasan_init || !kasan_has_integrated_init()))
			// 		memset(p[i], 0, zero_size);
			// 	kmemleak_alloc_recursive(p[i], s->object_size, 1,
			// 				s->flags, flags);
			// 	kmsan_slab_alloc(s, p[i], flags);
			// }
			if (flags & __GFP_ZERO)
				memset(*obj_p, 0, zero_size);

			// memcg_slab_post_alloc_hook(s, objcg, flags, size, p);
		}

	#endif

#endif /* _LINUX_SLUB_H_ */