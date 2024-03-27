#include <linux/init/init.h>
#include <linux/kernel/nodemask.h>

#include "kmalloc.h"
#include "slub.h"
#include "slub_types.h"


#include "../kmalloc_api.h"


/* Structure holding parameters for get_partial() call chain */
typedef struct partial_context {
	page_s	**slab;
	gfp_t	flags;
	uint	orig_size;
} partial_ctx_s;


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


/* Internal SLUB flags */
/* Poison object */
#define __OBJECT_POISON		((slab_flags_t __force)0x80000000U)
/* Use cmpxchg_double */
#define __CMPXCHG_DOUBLE	((slab_flags_t __force)0x40000000U)



static kmem_cache_s *kmem_cache_node;



static inline void
*get_freepointer(struct kmem_cache *s, void *object) {
	return object + s->offset;
}

static inline void
set_freepointer(struct kmem_cache *s, void *object, void *fp) {
	unsigned long freeptr_addr = (unsigned long)object + s->offset;
	*(void **)freeptr_addr = fp;
}

/* Loop over all objects in a slab */
#define for_each_object(__p, __s, __addr, __objects)		\
			for (__p = __addr;								\
				__p < (__addr) + (__objects) * (__s)->size;	\
				__p += (__s)->size)

static inline unsigned int
order_objects(unsigned int order, unsigned int size) {
	return ((unsigned int)PAGE_SIZE << order) / size;
}

static inline struct kmem_cache_order_objects
oo_make(unsigned int order, unsigned int size) {
	struct kmem_cache_order_objects x = {
		.order		= order,
		.obj_nr		= order_objects(order, size),
	};
	return x;
}

#define oo_order(x)		((x).order)
#define oo_objects(x)	((x).obj_nr)



/*
 * Slab allocation and freeing
 */
static inline page_s
*alloc_slab_page(gfp_t flags, struct kmem_cache_order_objects oo) {
	return alloc_pages(flags, oo_order(oo));
}


static page_s
*new_slab(struct kmem_cache *s, gfp_t flags) {
	flags = flags & (GFP_RECLAIM_MASK | GFP_CONSTRAINT_MASK);
// static struct slab *allocate_slab(struct kmem_cache *s, gfp_t flags, int node)
// {
	page_s *slab;
	struct kmem_cache_order_objects oo = s->oo;
	gfp_t alloc_gfp;
	void *start, *p, *next;
	int idx;

	flags &= gfp_allowed_mask;
	flags |= s->allocflags;

	/*
	 * Let the initial higher-order allocation fail under memory pressure
	 * so we fall-back to the minimum order allocation.
	 */
	alloc_gfp = (flags | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_NOFAIL;

	slab = alloc_slab_page(alloc_gfp, oo);
	if (unlikely(!slab)) {
		oo = s->min;
		/*
		 * Allocation may have failed due to fragmentation.
		 * Try a lower order alloc if possible
		 */
		slab = alloc_slab_page(flags, oo);
		if (unlikely(!slab))
			return NULL;
	}

	slab->objects = oo_objects(oo);
	slab->inuse = 0;
	slab->frozen = 0;
	slab->slab_cache = s;

	start = (void *)page_to_virt(slab);
	slab->freelist = start;
	for (idx = 0, p = start; idx < slab->objects - 1; idx++) {
		next = p + s->size;
		set_freepointer(s, p, next);
		p = next;
	}
	set_freepointer(s, p, NULL);

	return slab;
// }
}


/*
 * Tracking of fully allocated slabs for debugging purposes.
 */
static void add_full(struct kmem_cache *s,
	struct kmem_cache_node *n, page_s *slab)
{
	if (!(s->flags & SLAB_STORE_USER))
		return;

	// lockdep_assert_held(&n->list_lock);
	list_header_push(&n->full, &slab->slab_list);
}

static void remove_full(struct kmem_cache *s, struct kmem_cache_node *n, page_s *slab)
{
	if (!(s->flags & SLAB_STORE_USER))
		return;

	// lockdep_assert_held(&n->list_lock);
	list_header_delete_node(&n->full, &slab->slab_list);
}

/*
 * Management of partially allocated slabs.
 */
static inline void
__add_partial(struct kmem_cache_node *n, page_s *slab, int tail)
{
	n->nr_partial++;
	if (tail == DEACTIVATE_TO_TAIL)
		list_header_enqueue(&n->partial, &slab->slab_list);
	else
		list_header_push(&n->partial, &slab->slab_list);
}

static inline void add_partial(struct kmem_cache_node *n,
				page_s *slab, int tail)
{
	// lockdep_assert_held(&n->list_lock);
	__add_partial(n, slab, tail);
}

static inline void remove_partial(struct kmem_cache_node *n,
					page_s *slab)
{
	// lockdep_assert_held(&n->list_lock);
	list_header_delete_node(&n->partial, &slab->slab_list);
	n->nr_partial--;
}

/*
 * Called only for kmem_cache_debug() caches instead of acquire_slab(), with a
 * slab from the n->partial list. Remove only a single object from the slab, do
 * the alloc_debug_processing() checks and leave the slab on the list, or move
 * it to full list if it was the last free object.
 */
static void *alloc_single_from_partial(struct kmem_cache *s,
		struct kmem_cache_node *n, page_s *slab, int orig_size)
{
	void *object;

	// lockdep_assert_held(&n->list_lock);

	object = slab->freelist;
	slab->freelist = get_freepointer(s, object);
	slab->inuse++;

	if (slab->inuse == slab->objects) {
		remove_partial(n, slab);
		add_full(s, n, slab);
	}

	return object;
}
/*
 * Called only for kmem_cache_debug() caches to allocate from a freshly
 * allocated slab. Allocate a single object instead of whole freelist
 * and put the slab to the partial (or full) list.
 */
static void *alloc_single_from_new_slab(struct kmem_cache *s,
					page_s *slab, int orig_size)
{
	// int nid = slab_nid(slab);
	// struct kmem_cache_node *n = get_node(s, nid);
	unsigned long flags;
	void *object;


	// object = slab->freelist;
	// slab->freelist = get_freepointer(s, object);
	// slab->inuse = 1;

	// if (!alloc_debug_processing(s, slab, object, orig_size))
	// 	/*
	// 	 * It's not really expected that this would fail on a
	// 	 * freshly allocated slab, but a concurrent memory
	// 	 * corruption in theory could cause that.
	// 	 */
	// 	return NULL;

	// spin_lock_irqsave(&n->list_lock, flags);

	// if (slab->inuse == slab->objects)
	// 	add_full(s, n, slab);
	// else
	// 	add_partial(n, slab, DEACTIVATE_TO_HEAD);

	// inc_slabs_node(s, nid, slab->objects);
	// spin_unlock_irqrestore(&n->list_lock, flags);

	return object;
}

/*
 * Get a partial slab, lock it and return it.
 */
// static void *get_partial(struct kmem_cache *s, int node, struct partial_context *pc)
static void
*get_partial(struct kmem_cache *s, struct partial_context *pc) {
// /*
//  * Try to allocate a partial slab from a specific node.
//  */
// static void *get_partial_node(struct kmem_cache *s, struct partial_context *pc)
// {
	page_s *slab, *slab_next;
	void *object = NULL;
	unsigned long flags;
	unsigned int partial_slabs = 0;
	kmem_cache_node_s *n = &s->node;

	spin_lock_irqsave(&n->list_lock, flags);
	// list_for_each_entry_safe(slab, slab_next, &n->partial, slab_list) {
	list_header_for_each_container_safe(slab, slab_next, &n->partial, slab_list) {
		object = alloc_single_from_partial(s, n, slab, pc->orig_size);
		if (object)
			break;
		continue;
	}
	spin_unlock_irqrestore(&n->list_lock, flags);
	return object;
// }
}



// static void *__slab_alloc_node(struct kmem_cache *s,
// 		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
static void
*slab_alloc(struct kmem_cache *s, gfp_t gfpflags, size_t orig_size) {
	struct partial_context pc;
	page_s *slab;
	void *object;

	pc.flags = gfpflags;
	pc.slab = &slab;
	pc.orig_size = orig_size;
	object = get_partial(s, &pc);

	if (object)
		return object;

	slab = new_slab(s, gfpflags);
	if (unlikely(!slab)) {
		// slab_out_of_memory(s, gfpflags, node);
		return NULL;
	}

	object = alloc_single_from_new_slab(s, slab, orig_size);

	return object;
}

void *kmem_cache_alloc(struct kmem_cache *s, gfp_t gfpflags)
{
// static __fastpath_inline
// void *__kmem_cache_alloc_lru(struct kmem_cache *s, struct list_lru *lru,
// 				gfp_t gfpflags)
// {
	void *ret = slab_alloc(s, gfpflags, s->object_size);
	// trace_kmem_cache_alloc(_RET_IP_, ret, s, gfpflags, NUMA_NO_NODE);
	return ret;
// }
}
EXPORT_SYMBOL(kmem_cache_alloc);




/*
 * Object placement in a slab is made very easy because we always start at
 * offset 0. If we tune the size of the object to the alignment then we can
 * get the required alignment by putting one properly sized object after
 * another.
 *
 * Notice that the allocation order determines the sizes of the per cpu
 * caches. Each processor has always one slab available for allocations.
 * Increasing the allocation order reduces the number of times that slabs
 * must be moved on and off the partial lists and is therefore a factor in
 * locking overhead.
 */

/*
 * Minimum / Maximum order of slab pages. This influences locking overhead
 * and slab fragmentation. A higher order reduces the number of partial slabs
 * and increases the number of allocations possible without having to
 * take the list_lock.
 */
static uint slub_min_order		= 0;
static uint slub_max_order		= PAGE_ALLOC_COSTLY_ORDER;
static uint slub_min_objects	= 4;

// static inline int calculate_order(unsigned int size)
static inline int
simple_calculate_order(unsigned int size) {
	int
		order,
		min_order = (int)slub_min_order;
	uint
		fraction = 64,
		min_objnr = slub_min_objects;

	// make sure @min_order can hold at least 1 object
	for (order = max(min_order, get_order(min_objnr * size));
			order <= MAX_ORDER; order++) {

		uint slab_size = (uint)PAGE_SIZE << order;
		uint rem = slab_size % size;
		if (rem <= slab_size / fraction)
			break;
	}

	return order;
}
#define calculate_order simple_calculate_order

/*
 * calculate_sizes() determines the order and the distribution of data within
 * a slab object.
 */
// static int calculate_sizes(struct kmem_cache *s)
static int simple_calculate_sizes(struct kmem_cache *s)
{
	slab_flags_t flags = s->flags;
	uint size = s->object_size;
	uint order;

	/*
	 * Round up object size to the next word boundary. We can only
	 * place the free pointer at word boundaries and this determines
	 * the possible location of the free pointer.
	 */
	size = ALIGN(size, sizeof(void *));
	/*
	 * With that we have determined the number of bytes in actual use
	 * by the object and redzoning.
	 */
	s->inuse = size;

	/*
	 * SLUB stores one object immediately after another beginning from
	 * offset 0. In order to align the objects we have to simply size
	 * each object to conform to the alignment.
	 */
	size = ALIGN(size, s->align);
	s->size = size;
	order = calculate_order(size);

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
#define calculate_sizes simple_calculate_sizes

// static int kmem_cache_open(struct kmem_cache *s, slab_flags_t flags)
static int simple_kmem_cache_open(struct kmem_cache *s, slab_flags_t flags)
{
	s->flags = flags;

	if (calculate_sizes(s) <= 0)
		goto error;

	/*
	 * The larger the object size is, the more slabs we want on the partial
	 * list to avoid pounding the page allocator excessively.
	 */
	s->min_partial = min_t(ulong, MAX_PARTIAL, ilog2(s->size) / 2);
	s->min_partial = max_t(ulong, MIN_PARTIAL, s->min_partial);

	// /* Initialize the pre-computed randomized freelist if slab is up */
	// if (slab_state >= UP) {
	// 	if (init_cache_random_seq(s))
	// 		goto error;
	// }

// static int init_kmem_cache_nodes(struct kmem_cache *s)
// {
	kmem_cache_node_s *node_cache = &kmem_cache->node;
	INIT_LIST_HEADER_S(&node_cache->partial);
	INIT_LIST_HEADER_S(&node_cache->full);
// }
	return 0;

error:
	// __kmem_cache_release(s);
	return -EINVAL;
}
#define kmem_cache_open simple_kmem_cache_open




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
	struct kmem_cache *s = kmem_cache_zalloc(kmem_cache, GFP_NOWAIT);
	memcpy(s, static_cache, kmem_cache->object_size);
	list_header_push(&slab_caches, &s->list);
	return s;
}

void __init kmem_cache_init(void)
{
	static __initdata kmem_cache_s
		boot_kmem_cache;
	int node;

	kmem_cache = &boot_kmem_cache;

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	create_boot_cache(kmem_cache, "kmem_cache",
			offsetof(struct kmem_cache, node) +
				nr_node_ids * sizeof(struct kmem_cache_node *),
			SLAB_HWCACHE_ALIGN, 0, 0);

	kmem_cache = bootstrap(&boot_kmem_cache);

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