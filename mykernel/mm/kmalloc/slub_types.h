/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SLUB_TYPES_H_
#define _SLUB_TYPES_H_
/*
 * SLUB : A Slab allocator without object queues.
 *
 * (C) 2007 SGI, Christoph Lameter
 */

	#include <linux/kernel/lock_ipc.h>
	#include <linux/kernel/kobject.h>
	#include <linux/lib/list.h>

	/*
	 * The slab lists for all objects.
	 */
	typedef struct kmem_cache_node {
		spinlock_t		list_lock;
		unsigned long	nr_partial;
		List_s			partial;
		atomic_long_t	nr_slabs;
		atomic_long_t	total_objects;
		List_s			full;
	} kmem_cache_node_s;

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
	 * When changing the layout, make sure freelist and tid are still compatible
	 * with this_cpu_cmpxchg_double() alignment requirements.
	 */
	typedef struct kmem_cache_cpu {
	// 	void **freelist;	/* Pointer to next available object */
	// 	unsigned long tid;	/* Globally unique transaction id */
	// 	struct slab *slab;	/* The slab from which we are allocating */
	// 	struct slab *partial;	/* Partially allocated frozen slabs */
	// 	local_lock_t lock;	/* Protects the fields above */
	// #ifdef CONFIG_SLUB_STATS
	// 	unsigned stat[NR_SLUB_STAT_ITEMS];
	// #endif
	} kmem_cache_cpu_s;

	/*
	 * Word size structure that can be atomically updated or read and that
	 * contains both the order and the number of objects that a slab of the
	 * given order would contain.
	 */
	typedef struct kmem_cache_order_objects {
		unsigned int x;
	} kmem_cache_order_obj_s;

	/*
	 * Slab cache management.
	 */
	typedef struct kmem_cache {
		// kmem_cache_cpu_s	__percpu *cpu_slab;
		/* Used for retrieving partial slabs, etc. */
		slab_flags_t	flags;
		unsigned long	min_partial;
		unsigned int	size;			/* The size of an object including metadata */
		unsigned int	object_size;	/* The size of an object without metadata */
		// struct reciprocal_value reciprocal_size;
		unsigned int	offset;			/* Free pointer offset */
		// /* Number of per cpu partial objects to keep around */
		// unsigned int cpu_partial;
		// /* Number of per cpu partial slabs to keep around */
		// unsigned int cpu_partial_slabs;
		kmem_cache_order_obj_s	oo;

		/* Allocation and freeing of slabs */
		kmem_cache_order_obj_s	min;
		gfp_t			allocflags;		/* gfp flags to use on each alloc */
		int				refcount;		/* Refcount for slab cache destroy */
		// void			(*ctor)(void *);
		unsigned int	inuse;			/* Offset to metadata */
		unsigned int	align;			/* Alignment */
		unsigned int	red_left_pad;	/* Left redzone padding size */
		const char		*name;			/* Name (only for display!) */
		List_s			list;			/* List of slab caches */
		kobj_s			kobj;			/* For sysfs */

	#ifdef CONFIG_SLAB_FREELIST_HARDENED
		unsigned long	random;
	#endif
	#ifdef CONFIG_SLAB_FREELIST_RANDOM
		unsigned int	*random_seq;
	#endif
	#ifdef CONFIG_KASAN_GENERIC
		struct kasan_cache	kasan_info;
	#endif
	#ifdef CONFIG_HARDENED_USERCOPY
		unsigned int	useroffset;	/* Usercopy region offset */
		unsigned int	usersize;		/* Usercopy region size */
	#endif

		kmem_cache_node_s	*node[MAX_NUMNODES];
	} kmem_cache_s;

#endif /* _SLUB_TYPES_H_ */
