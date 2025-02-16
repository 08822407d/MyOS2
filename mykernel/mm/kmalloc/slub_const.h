/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SLUB_CONST_H_
#define _SLUB_CONST_H_


	/*
	 * State of the slab allocator.
	 *
	 * This is used to describe the states of the allocator during bootup.
	 * Allocators use this to gradually bootstrap themselves. Most allocators
	 * have the problem that the structures used for managing slab caches are
	 * allocated from slab caches themselves.
	 */
	enum slab_state {
		DOWN,			/* No slab functionality yet */
		PARTIAL,		/* SLUB: kmem_cache_node available */
		PARTIAL_NODE,	/* SLAB: kmalloc size for node struct available */
		UP,				/* Slab caches usable but not all extras yet */
		FULL			/* Everything is working */
	};

	enum stat_item {
		ALLOC_FASTPATH,				/* Allocation from cpu slab */
		ALLOC_SLOWPATH,				/* Allocation by getting a new cpu slab */
		FREE_FASTPATH,				/* Free to cpu slab */
		FREE_SLOWPATH,				/* Freeing not to cpu slab */
		FREE_FROZEN,				/* Freeing to frozen slab */
		FREE_ADD_PARTIAL,			/* Freeing moves slab to partial list */
		FREE_REMOVE_PARTIAL,		/* Freeing removes last object */
		ALLOC_FROM_PARTIAL,			/* Cpu slab acquired from node partial list */
		ALLOC_SLAB,					/* Cpu slab acquired from page allocator */
		ALLOC_REFILL,				/* Refill cpu slab from slab freelist */
		ALLOC_NODE_MISMATCH,		/* Switching cpu slab */
		FREE_SLAB,					/* Slab freed to the page allocator */
		CPUSLAB_FLUSH,				/* Abandoning of the cpu slab */
		DEACTIVATE_FULL,			/* Cpu slab was full when deactivated */
		DEACTIVATE_EMPTY,			/* Cpu slab was empty when deactivated */
		DEACTIVATE_TO_HEAD,			/* Cpu slab was moved to the head of partials */
		DEACTIVATE_TO_TAIL,			/* Cpu slab was moved to the tail of partials */
		DEACTIVATE_REMOTE_FREES,	/* Slab contained remotely freed objects */
		DEACTIVATE_BYPASS,			/* Implicit deactivation */
		ORDER_FALLBACK,				/* Number of times fallback was necessary */
		CMPXCHG_DOUBLE_CPU_FAIL,	/* Failure of this_cpu_cmpxchg_double */
		CMPXCHG_DOUBLE_FAIL,		/* Number of times that cmpxchg double did not match */
		CPU_PARTIAL_ALLOC,			/* Used cpu partial on alloc */
		CPU_PARTIAL_FREE,			/* Refill cpu partial on free */
		CPU_PARTIAL_NODE,			/* Refill cpu partial from node partial */
		CPU_PARTIAL_DRAIN,			/* Drain cpu partial to node partial */
		NR_SLUB_STAT_ITEMS
	};


	/*
	 * Minimum number of partial slabs. These will be left on the partial
	 * lists even if they are empty. kmem_cache_shrink may reclaim them.
	 */
	#define MIN_PARTIAL 4
	/*
	 * Maximum number of desirable partial slabs.
	 * The existence of more partial slabs makes kmem_cache_shrink
	 * sort the partial list by the number of objects in use.
	 */
	#define MAX_PARTIAL 8
	#define MAX_OBJS_PER_PAGE	32767 /* since slab.objects is u15 */


	/*
	 * SLUB directly allocates requests fitting in to an order-1 page
	 * (PAGE_SIZE*2).  Larger requests are passed to the page allocator.
	 */
	#define KMALLOC_SHIFT_HIGH		(PAGE_SHIFT + 1)
	// #define KMALLOC_SHIFT_MAX		(MAX_ORDER + PAGE_SHIFT)
	// #ifndef KMALLOC_SHIFT_LOW
	// #  define KMALLOC_SHIFT_LOW		3
	// #endif

	/* Maximum allocatable size */
	// #define KMALLOC_MAX_SIZE		(1UL << KMALLOC_SHIFT_MAX)
	#define KMALLOC_MAX_CACHE_SIZE	PAGE_SIZE
	/* Maximum order allocatable via the slab allocator */
	// #define KMALLOC_MAX_ORDER		(KMALLOC_SHIFT_MAX - PAGE_SHIFT)

	// /*
	//  * Kmalloc subsystem.
	//  */
	// #ifndef KMALLOC_MIN_SIZE
	// #  define KMALLOC_MIN_SIZE		(1 << KMALLOC_SHIFT_LOW)
	// #endif

	// /*
	//  * This restriction comes from byte sized index implementation.
	//  * Page size is normally 2^12 bytes and, in this case, if we want to use
	//  * byte sized index which can represent 2^8 entries, the size of the object
	//  * should be equal or greater to 2^12 / 2^8 = 2^4 = 16.
	//  * If minimum size of kmalloc is less than 16, we use it as minimum object
	//  * size and give up to use byte sized index.
	//  */
	// #define SLAB_OBJ_MIN_SIZE		(KMALLOC_MIN_SIZE < 16 ? (KMALLOC_MIN_SIZE) : 16)

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

	/* Account to memcg */
	#ifdef CONFIG_MEMCG_KMEM
	#  define SLAB_ACCOUNT			((slab_flags_t __force)0x04000000U)
	#else
	#  define SLAB_ACCOUNT			0
	#endif


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

	/* Legal flag mask for kmem_cache_create(), for various configurations */
	#define SLAB_CORE_FLAGS (SLAB_HWCACHE_ALIGN | SLAB_CACHE_DMA | \
				SLAB_CACHE_DMA32 | SLAB_PANIC | \
				SLAB_TYPESAFE_BY_RCU | SLAB_DEBUG_OBJECTS )

	#if defined(CONFIG_DEBUG_SLAB)
	#  define SLAB_DEBUG_FLAGS (SLAB_RED_ZONE | SLAB_POISON | SLAB_STORE_USER)
	#elif defined(CONFIG_SLUB_DEBUG)
	#  define SLAB_DEBUG_FLAGS (SLAB_RED_ZONE | SLAB_POISON | SLAB_STORE_USER | \
					SLAB_TRACE | SLAB_CONSISTENCY_CHECKS)
	#else
	#  define SLAB_DEBUG_FLAGS (0)
	#endif

	#if defined(CONFIG_SLAB)
	#  define SLAB_CACHE_FLAGS (SLAB_MEM_SPREAD | SLAB_NOLEAKTRACE | \
					SLAB_RECLAIM_ACCOUNT | SLAB_TEMPORARY)
	#elif defined(CONFIG_SLUB)
	#  define SLAB_CACHE_FLAGS (SLAB_NOLEAKTRACE | SLAB_RECLAIM_ACCOUNT | \
					SLAB_TEMPORARY | SLAB_NO_USER_FLAGS | SLAB_KMALLOC)
	#else
	#  define SLAB_CACHE_FLAGS (SLAB_NOLEAKTRACE)
	#endif

	/* Common flags available with current configuration */
	#define CACHE_CREATE_MASK (SLAB_CORE_FLAGS | SLAB_DEBUG_FLAGS | SLAB_CACHE_FLAGS)

	/* Common flags permitted for kmem_cache_create */
	#define SLAB_FLAGS_PERMITTED (SLAB_CORE_FLAGS | \
					SLAB_RED_ZONE | \
					SLAB_POISON | \
					SLAB_STORE_USER | \
					SLAB_TRACE | \
					SLAB_CONSISTENCY_CHECKS | \
					SLAB_MEM_SPREAD | \
					SLAB_NOLEAKTRACE | \
					SLAB_RECLAIM_ACCOUNT | \
					SLAB_TEMPORARY | \
					SLAB_KMALLOC | \
					SLAB_NO_USER_FLAGS)

#endif /* _SLUB_CONST_H_ */
