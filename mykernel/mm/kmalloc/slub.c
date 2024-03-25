#include <linux/init/init.h>

#include "kmalloc.h"
#include "slub_types.h"


#include "../kmalloc_api.h"


/*
 * Minimum number of partial slabs. These will be left on the partial
 * lists even if they are empty. kmem_cache_shrink may reclaim them.
 */
#define MIN_PARTIAL 5

/*
 * Maximum number of desirable partial slabs.
 * The existence of more partial slabs makes kmem_cache_shrink
 * sort the partial list by the number of objects in use.
 */
#define MAX_PARTIAL 10


#define DEBUG_DEFAULT_FLAGS (SLAB_CONSISTENCY_CHECKS | SLAB_RED_ZONE | \
				SLAB_POISON | SLAB_STORE_USER)

/*
 * These debug flags cannot use CMPXCHG because there might be consistency
 * issues when checking or reading debug information
 */
#define SLAB_NO_CMPXCHG (SLAB_CONSISTENCY_CHECKS | SLAB_STORE_USER | \
				SLAB_TRACE)

/*
 * Debugging flags that require metadata to be stored in the slab.  These get
 * disabled when slub_debug=O is used and a cache's min order increases with
 * metadata.
 */
#define DEBUG_METADATA_FLAGS (SLAB_RED_ZONE | SLAB_POISON | SLAB_STORE_USER)

#define OO_SHIFT			16
#define OO_MASK				((1 << OO_SHIFT) - 1)
#define MAX_OBJS_PER_PAGE	32767 /* since slab.objects is u15 */

/* Internal SLUB flags */
/* Poison object */
#define __OBJECT_POISON		((slab_flags_t __force)0x80000000U)
/* Use cmpxchg_double */
#define __CMPXCHG_DOUBLE	((slab_flags_t __force)0x40000000U)



static kmem_cache_s *kmem_cache_node;


static inline unsigned int
order_objects(unsigned int order, unsigned int size) {
	return ((unsigned int)PAGE_SIZE << order) / size;
}

static inline struct kmem_cache_order_objects
oo_make(unsigned int order, unsigned int size) {
	struct kmem_cache_order_objects x = {
		(order << OO_SHIFT) + order_objects(order, size)
	};
	return x;
}

static inline unsigned int
oo_order(struct kmem_cache_order_objects x) {
	return x.x >> OO_SHIFT;
}

static inline unsigned int
oo_objects(struct kmem_cache_order_objects x) {
	return x.x & OO_MASK;
}



/*
 * calculate_sizes() determines the order and the distribution of data within
 * a slab object.
 */
static int calculate_sizes(struct kmem_cache *s)
{
	slab_flags_t flags = s->flags;
	unsigned int size = s->object_size;
	unsigned int order;

	/*
	 * Round up object size to the next word boundary. We can only
	 * place the free pointer at word boundaries and this determines
	 * the possible location of the free pointer.
	 */
	size = ALIGN(size, sizeof(void *));

	// /*
	//  * Determine if we can poison the object itself. If the user of
	//  * the slab may touch the object after free or before allocation
	//  * then we should never poison the object itself.
	//  */
	// if ((flags & SLAB_POISON) && !(flags & SLAB_TYPESAFE_BY_RCU) &&
	// 		!s->ctor)
	// 	s->flags |= __OBJECT_POISON;
	// else
	// 	s->flags &= ~__OBJECT_POISON;


	/*
	 * If we are Redzoning then check if there is some space between the
	 * end of the object and the free pointer. If not then add an
	 * additional word to have some bytes to store Redzone information.
	 */
	if ((flags & SLAB_RED_ZONE) && size == s->object_size)
		size += sizeof(void *);

	/*
	 * With that we have determined the number of bytes in actual use
	 * by the object and redzoning.
	 */
	s->inuse = size;

	// if (slub_debug_orig_size(s) ||
	//     (flags & (SLAB_TYPESAFE_BY_RCU | SLAB_POISON)) ||
	//     ((flags & SLAB_RED_ZONE) && s->object_size < sizeof(void *)) ||
	//     s->ctor) {
	// 	/*
	// 	 * Relocate free pointer after the object if it is not
	// 	 * permitted to overwrite the first word of the object on
	// 	 * kmem_cache_free.
	// 	 *
	// 	 * This is the case if we do RCU, have a constructor or
	// 	 * destructor, are poisoning the objects, or are
	// 	 * redzoning an object smaller than sizeof(void *).
	// 	 *
	// 	 * The assumption that s->offset >= s->inuse means free
	// 	 * pointer is outside of the object is used in the
	// 	 * freeptr_outside_object() function. If that is no
	// 	 * longer true, the function needs to be modified.
	// 	 */
	// 	s->offset = size;
	// 	size += sizeof(void *);
	// } else {
	// 	/*
	// 	 * Store freelist pointer near middle of object to keep
	// 	 * it away from the edges of the object to avoid small
	// 	 * sized over/underflows from neighboring allocations.
	// 	 */
	// 	s->offset = ALIGN_DOWN(s->object_size / 2, sizeof(void *));
	// }

	// if (flags & SLAB_STORE_USER) {
	// 	/*
	// 	 * Need to store information about allocs and frees after
	// 	 * the object.
	// 	 */
	// 	size += 2 * sizeof(struct track);

	// 	/* Save the original kmalloc request size */
	// 	if (flags & SLAB_KMALLOC)
	// 		size += sizeof(unsigned int);
	// }

	// kasan_cache_create(s, &size, &s->flags);
	// if (flags & SLAB_RED_ZONE) {
	// 	/*
	// 	 * Add some empty padding so that we can catch
	// 	 * overwrites from earlier objects rather than let
	// 	 * tracking information or the free pointer be
	// 	 * corrupted if a user writes before the start
	// 	 * of the object.
	// 	 */
	// 	size += sizeof(void *);

	// 	s->red_left_pad = sizeof(void *);
	// 	s->red_left_pad = ALIGN(s->red_left_pad, s->align);
	// 	size += s->red_left_pad;
	// }

	/*
	 * SLUB stores one object immediately after another beginning from
	 * offset 0. In order to align the objects we have to simply size
	 * each object to conform to the alignment.
	 */
	size = ALIGN(size, s->align);
	s->size = size;
	// s->reciprocal_size = reciprocal_value(size);
	// order = calculate_order(size);

	if ((int)order < 0)
		return 0;

	s->allocflags = 0;
	if (order)
		s->allocflags |= __GFP_COMP;

	if (s->flags & SLAB_CACHE_DMA)
		s->allocflags |= GFP_DMA;

	if (s->flags & SLAB_CACHE_DMA32)
		s->allocflags |= GFP_DMA32;

	if (s->flags & SLAB_RECLAIM_ACCOUNT)
		s->allocflags |= __GFP_RECLAIMABLE;

	/*
	 * Determine the number of objects per slab
	 */
	s->oo = oo_make(order, size);
	s->min = oo_make(get_order(size), size);

	return !!oo_objects(s->oo);
}

static int kmem_cache_open(struct kmem_cache *s, slab_flags_t flags)
{
	// s->flags = kmem_cache_flags(s->size, flags, s->name);
	s->flags = flags;

	if (!calculate_sizes(s))
		goto error;
	// if (disable_higher_order_debug) {
	// 	/*
	// 	 * Disable debugging flags that store metadata if the min slab
	// 	 * order increased.
	// 	 */
	// 	if (get_order(s->size) > get_order(s->object_size)) {
	// 		s->flags &= ~DEBUG_METADATA_FLAGS;
	// 		s->offset = 0;
	// 		if (!calculate_sizes(s))
	// 			goto error;
	// 	}
	// }

	// /*
	//  * The larger the object size is, the more slabs we want on the partial
	//  * list to avoid pounding the page allocator excessively.
	//  */
	// s->min_partial = min_t(unsigned long, MAX_PARTIAL, ilog2(s->size) / 2);
	// s->min_partial = max_t(unsigned long, MIN_PARTIAL, s->min_partial);

	// set_cpu_partial(s);

	// /* Initialize the pre-computed randomized freelist if slab is up */
	// if (slab_state >= UP) {
	// 	if (init_cache_random_seq(s))
	// 		goto error;
	// }

	// if (!init_kmem_cache_nodes(s))
	// 	goto error;

	// if (alloc_kmem_cache_cpus(s))
	// 	return 0;

error:
	// __kmem_cache_release(s);
	return -EINVAL;
}




/********************************************************************
 *			Basic setup of slabs
 *******************************************************************/

/*
 * Used for early kmem_cache structures that were allocated using
 * the page allocator. Allocate them properly then fix up the pointers
 * that may be pointing to the wrong kmem_cache structure.
 */

static struct kmem_cache * __init bootstrap(struct kmem_cache *static_cache)
{
	int node;
	// struct kmem_cache *s = kmem_cache_zalloc(kmem_cache, GFP_NOWAIT);
	// struct kmem_cache_node *n;

	// memcpy(s, static_cache, kmem_cache->object_size);

	// /*
	//  * This runs very early, and only the boot processor is supposed to be
	//  * up.  Even if it weren't true, IRQs are not up so we couldn't fire
	//  * IPIs around.
	//  */
	// __flush_cpu_slab(s, smp_processor_id());
	// for_each_kmem_cache_node(s, node, n) {
	// 	struct slab *p;

	// 	list_for_each_entry(p, &n->partial, slab_list)
	// 		p->slab_cache = s;

	// 	list_for_each_entry(p, &n->full, slab_list)
	// 		p->slab_cache = s;
	// }
	// list_add(&s->list, &slab_caches);
	// return s;
}

void __init kmem_cache_init(void)
{
	static __initdata kmem_cache_s boot_kmem_cache,
		boot_kmem_cache_node;
	int node;

	// if (debug_guardpage_minorder())
	// 	slub_max_order = 0;

	// /* Print slub debugging pointers without hashing */
	// if (__slub_debug_enabled())
	// 	no_hash_pointers_enable(NULL);

	kmem_cache_node = &boot_kmem_cache_node;
	kmem_cache = &boot_kmem_cache;

	// /*
	//  * Initialize the nodemask for which we will allocate per node
	//  * structures. Here we don't need taking slab_mutex yet.
	//  */
	// for_each_node_state(node, N_NORMAL_MEMORY)
	// 	node_set(node, slab_nodes);

	create_boot_cache(kmem_cache_node, "kmem_cache_node",
		sizeof(struct kmem_cache_node), SLAB_HWCACHE_ALIGN, 0, 0);

	// hotplug_memory_notifier(slab_memory_callback, SLAB_CALLBACK_PRI);

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	create_boot_cache(kmem_cache, "kmem_cache",
			offsetof(struct kmem_cache, node) +
				sizeof(struct kmem_cache_node *),
			SLAB_HWCACHE_ALIGN, 0, 0);

	kmem_cache = bootstrap(&boot_kmem_cache);
	kmem_cache_node = bootstrap(&boot_kmem_cache_node);

	// /* Now we can use the kmem_cache to allocate kmalloc slabs */
	// setup_kmalloc_cache_index_table();
	// create_kmalloc_caches(0);

	// /* Setup random freelists for each cache */
	// init_freelist_randomization();

	// cpuhp_setup_state_nocalls(CPUHP_SLUB_DEAD, "slub:dead", NULL,
	// 			  slub_cpu_dead);

	// pr_info("SLUB: HWalign=%d, Order=%u-%u, MinObjects=%u, CPUs=%u, Nodes=%u\n",
	// 	cache_line_size(),
	// 	slub_min_order, slub_max_order, slub_min_objects,
	// 	nr_cpu_ids, nr_node_ids);
}


int __kmem_cache_create(struct kmem_cache *s, slab_flags_t flags)
{
	int err;

	err = kmem_cache_open(s, flags);
	if (err)
		return err;

	/* Mutex is not taken during early boot */
	if (slab_state <= UP)
		return 0;

	// err = sysfs_slab_add(s);
	// if (err) {
	// 	__kmem_cache_release(s);
	// 	return err;
	// }

	// if (s->flags & SLAB_STORE_USER)
	// 	debugfs_slab_add(s);

	return 0;
}