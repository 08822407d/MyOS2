#ifndef _LINUX_KMALLOC_API_H_
#define _LINUX_KMALLOC_API_H_

	#include "kmalloc/kmalloc_const.h"
	#include "kmalloc/kmalloc_types.h"
	#include "kmalloc/kmalloc.h"

	#include <linux/kernel/cache.h>

	void __init kmem_cache_init(void);


	extern enum slab_state slab_state;

	// /* The slab cache mutex protects the management structures during changes */
	// extern struct mutex slab_mutex;

	/* The list of all slab caches on the system */
	extern List_hdr_s slab_caches;

	/* The slab cache that manages slab cache information */
	extern kmem_cache_s *kmem_cache;

	#define ARCH_KMALLOC_MINALIGN __alignof__(unsigned long long)

	/*
	 * Arches can define this function if they want to decide the minimum slab
	 * alignment at runtime. The value returned by the function must be a power
	 * of two and >= ARCH_SLAB_MINALIGN.
	 */
	#ifndef arch_slab_minalign
		static inline unsigned int
		arch_slab_minalign(void) {
			return ARCH_SLAB_MINALIGN;
		}
	#endif

	/* Kmalloc array related functions */
	void setup_kmalloc_cache_index_table(void);
	void create_kmalloc_caches(slab_flags_t);

	/* Find the kmalloc slab corresponding for a certain size */
	kmem_cache_s *kmalloc_slab(size_t, gfp_t);

	void *__kmalloc(size_t size, gfp_t flags) __alloc_size(1);

	kmem_cache_s *
    kmem_cache_create(const char *name, uint size,
    		uint align, slab_flags_t flags);
			
	/**
	 * kmem_cache_alloc - Allocate an object
	 * @cachep: The cache to allocate from.
	 * @flags: See kmalloc().
	 *
	 * Allocate an object from this cache.
	 * See kmem_cache_zalloc() for a shortcut of adding __GFP_ZERO to flags.
	 *
	 * Return: pointer to the new object or %NULL in case of error
	 */
	void *kmem_cache_alloc(kmem_cache_s *cachep, gfp_t flags) __malloc;
	// void *kmem_cache_alloc_lru(kmem_cache_s *s, struct list_lru *lru,
	// 			gfp_t gfpflags) __assume_slab_alignment __malloc;
	void kmem_cache_free(kmem_cache_s *s, void *objp);

	void __kmem_cache_free(kmem_cache_s *s, void *x, unsigned long caller);


	/* Functions provided by the slab allocators */
	int __kmem_cache_create(kmem_cache_s *, slab_flags_t flags);

	void *kmalloc_large(size_t size, gfp_t flags) __alloc_size(1);


	kmem_cache_s *create_kmalloc_cache(const char *name,
			unsigned int size, slab_flags_t flags);
	extern void create_boot_cache(kmem_cache_s *s, const char *name,
			unsigned int size, slab_flags_t flags);

	void free_large_kmalloc(folio_s *folio, void *object);

	void *__myos_kmalloc(size_t size, gfp_t flags);
	void myos_kfree(const void *objp);

	void myos_preinit_slab(void);
	void myos_init_slab(void);


#endif /* _LINUX_KMALLOC_API_H_ */