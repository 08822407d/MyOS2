/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SLUB_CONST_H_
#define _SLUB_CONST_H_

	#include <linux/kernel/types.h>

	/*
	 * Flags to pass to kmem_cache_create().
	 * The ones marked DEBUG are only valid if CONFIG_DEBUG_SLAB is set.
	 */
	/* DEBUG: Perform (expensive) checks on alloc/free */
	#define SLAB_CONSISTENCY_CHECKS	((slab_flags_t __force)0x00000100U)
	/* DEBUG: Red zone objs in a cache */
	#define SLAB_RED_ZONE			((slab_flags_t __force)0x00000400U)
	/* DEBUG: Poison objects */
	#define SLAB_POISON				((slab_flags_t __force)0x00000800U)
	/* Indicate a kmalloc slab */
	#define SLAB_KMALLOC			((slab_flags_t __force)0x00001000U)
	/* Align objs on cache lines */
	#define SLAB_HWCACHE_ALIGN		((slab_flags_t __force)0x00002000U)
	/* Use GFP_DMA memory */
	#define SLAB_CACHE_DMA			((slab_flags_t __force)0x00004000U)
	/* Use GFP_DMA32 memory */
	#define SLAB_CACHE_DMA32		((slab_flags_t __force)0x00008000U)
	/* DEBUG: Store the last owner for bug hunting */
	#define SLAB_STORE_USER			((slab_flags_t __force)0x00010000U)
	/* Panic if kmem_cache_create() fails */
	#define SLAB_PANIC				((slab_flags_t __force)0x00040000U)
	/*
	 * SLAB_TYPESAFE_BY_RCU - **WARNING** READ THIS!
	 *
	 * This delays freeing the SLAB page by a grace period, it does _NOT_
	 * delay object freeing. This means that if you do kmem_cache_free()
	 * that memory location is free to be reused at any time. Thus it may
	 * be possible to see another object there in the same RCU grace period.
	 *
	 * This feature only ensures the memory location backing the object
	 * stays valid, the trick to using this is relying on an independent
	 * object validation pass. Something like:
	 *
	 *  rcu_read_lock()
	 * again:
	 *  obj = lockless_lookup(key);
	 *  if (obj) {
	 *    if (!try_get_ref(obj)) // might fail for free objects
	 *      goto again;
	 *
	 *    if (obj->key != key) { // not the object we expected
	 *      put_ref(obj);
	 *      goto again;
	 *    }
	 *  }
	 *  rcu_read_unlock();
	 *
	 * This is useful if we need to approach a kernel structure obliquely,
	 * from its address obtained without the usual locking. We can lock
	 * the structure to stabilize it and check it's still at the given address,
	 * only if we can be sure that the memory has not been meanwhile reused
	 * for some other kind of object (which our subsystem's lock might corrupt).
	 *
	 * rcu_read_lock before reading the address, then rcu_read_unlock after
	 * taking the spinlock within the structure expected at that address.
	 *
	 * Note that it is not possible to acquire a lock within a structure
	 * allocated with SLAB_TYPESAFE_BY_RCU without first acquiring a reference
	 * as described above.  The reason is that SLAB_TYPESAFE_BY_RCU pages
	 * are not zeroed before being given to the slab, which means that any
	 * locks must be initialized after each and every kmem_struct_alloc().
	 * Alternatively, make the ctor passed to kmem_cache_create() initialize
	 * the locks at page-allocation time, as is done in __i915_request_ctor(),
	 * sighand_ctor(), and anon_vma_ctor().  Such a ctor permits readers
	 * to safely acquire those ctor-initialized locks under rcu_read_lock()
	 * protection.
	 *
	 * Note that SLAB_TYPESAFE_BY_RCU was originally named SLAB_DESTROY_BY_RCU.
	 */
	/* Defer freeing slabs to RCU */
	#define SLAB_TYPESAFE_BY_RCU	((slab_flags_t __force)0x00080000U)
	/* Spread some memory over cpuset */
	#define SLAB_MEM_SPREAD			((slab_flags_t __force)0x00100000U)
	/* Trace allocations and frees */
	#define SLAB_TRACE				((slab_flags_t __force)0x00200000U)

	/* Flag to prevent checks on free */
	#define SLAB_DEBUG_OBJECTS		((slab_flags_t __force)0x00400000U)

	/* Avoid kmemleak tracing */
	#define SLAB_NOLEAKTRACE		((slab_flags_t __force)0x00800000U)


	/*
	 * Ignore user specified debugging flags.
	 * Intended for caches created for self-tests so they have only flags
	 * specified in the code and other flags are ignored.
	 */
	#define SLAB_NO_USER_FLAGS		((slab_flags_t __force)0x10000000U)

	/* The following flags affect the page allocator grouping pages by mobility */
	/* Objects are reclaimable */
	#define SLAB_RECLAIM_ACCOUNT	((slab_flags_t __force)0)
	#define SLAB_TEMPORARY			SLAB_RECLAIM_ACCOUNT	/* Objects are short-lived */

#endif /* _SLUB_CONST_H_ */
