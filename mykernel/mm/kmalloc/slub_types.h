/* SPDX-License-Identifier: GPL-2.0 */
/*
 * SLUB : A Slab allocator without object queues.
 *
 * (C) 2007 SGI, Christoph Lameter
 */
#ifndef _SLUB_TYPES_H_
#define _SLUB_TYPES_H_

	#include "../mm_type_declaration.h"


	struct slab {
		ulong				__page_flags;

		kmem_cache_s		*slab_cache;
		struct {
			union {
				List_s		slab_list;
				struct {
					slab_s	*next;
					int		slabs;	/* Nr of slabs left */
				};
			};
			/* Double-word boundary */
			void			*freelist;		/* first free object */
			union {
				ulong		counters;
				struct {
					uint	inuse	: 16;
					uint	objects	: 15;
					uint	frozen	: 1;
				} __attribute__((packed));
			};
		};
		uint				__unused;

		atomic_t			__page_refcount;
		ulong				memcg_data;
	};


	/* A table of kmalloc cache names and sizes */
	struct kmalloc_info_struct {
		const char		*name;
		uint			size;
	};

	/*
	 * The slab lists for all objects.
	 */
	struct kmem_cache_node {
		spinlock_t		list_lock;
		List_hdr_s		partial;
		atomic_long_t	nr_slabs;
		atomic_long_t	total_objects;
		List_hdr_s		full;
	};

	/*
	 * Word size structure that can be atomically updated or read and that
	 * contains both the order and the number of objects that a slab of the
	 * given order would contain.
	 */
	struct kmem_cache_order_objects {
		uint
			obj_nr	: 16,
			order	: 16;
	} __attribute__((packed));

	/*
	 * Slab cache management.
	 */
	struct kmem_cache {
		// kmem_cache_cpu_s	__percpu *cpu_slab;
		/* Used for retrieving partial slabs, etc. */
		slab_flags_t		flags;
		ulong				min_partial;
		uint				size;			/* The size of an object including metadata */
		uint				object_size;	/* The size of an object without metadata */
		uint				offset;			/* Free pointer offset */
		// /* Number of per cpu partial objects to keep around */
		// uint			cpu_partial;
		// /* Number of per cpu partial slabs to keep around */
		// uint			cpu_partial_slabs;
		kmem_cache_order_obj_s	oo;

		// /* Allocation and freeing of slabs */
		gfp_t				allocflags;		/* gfp flags to use on each alloc */
		int					refcount;		/* Refcount for slab cache destroy */
		uint				inuse;			/* Offset to metadata */
		uint				align;			/* Alignment */
		const char			*name;			/* Name (only for display!) */
		List_s				list;			/* List of slab caches */
		// kobj_s			kobj;			/* For sysfs */

		// kmem_cache_node_s	*node[MAX_NUMNODES];
		kmem_cache_node_s	node;
	};

#endif /* _SLUB_TYPES_H_ */
