#ifndef _LINUX_KMALLOC_API_H_
#define _LINUX_KMALLOC_API_H_

	#include "kmalloc/kmalloc_const.h"
	#include "kmalloc/kmalloc_types.h"
	#include "kmalloc/kmalloc.h"

	#include <linux/kernel/cache.h>


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

	void __init kmem_cache_init(void);
	/* Functions provided by the slab allocators */
	int __kmem_cache_create(kmem_cache_s *, slab_flags_t flags);

	kmem_cache_s *create_kmalloc_cache(const char *name, unsigned int size,
			slab_flags_t flags, unsigned int useroffset, unsigned int usersize);
	extern void create_boot_cache(kmem_cache_s *, const char *name,
			unsigned int size, slab_flags_t flags,
			unsigned int useroffset, unsigned int usersize);


	void *__kmalloc(size_t size, gfp_t flags);
	void *kmalloc(size_t size, gfp_t flags);
	void kfree(const void *objp);

	void myos_preinit_slab(void);
	void myos_init_slab(void);


	extern void *kmalloc_order(size_t size, gfp_t flags, unsigned int order);

#endif /* _LINUX_KMALLOC_API_H_ */