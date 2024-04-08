#include <linux/init/init.h>
#include <linux/kernel/nodemask.h>

#include "kmalloc.h"


static inline uint
order_objects(uint order, uint size) {
	return ((uint)PAGE_SIZE << order) / size;
}

static inline kmem_cache_order_obj_s
oo_make(uint order, uint size) {
	kmem_cache_order_obj_s x = {
		.order		= order,
		.obj_nr		= order_objects(order, size),
	};
	return x;
}

#define oo_order(x)		((x).order)
#define oo_objects(x)	((x).obj_nr)


static inline void
*get_freepointer(kmem_cache_s *s, void *object) {
	while (IS_INVAL_PTR((const void *)(object + s->offset)));
	return (void *)*(ulong *)(object + s->offset);
}

static inline void
set_freepointer(kmem_cache_s *s, void *this_obj, void *next_obj) {
	ulong freeptr_addr = (ulong)this_obj + s->offset;
	while (IS_INVAL_PTR((const void *)freeptr_addr));
	*(void **)freeptr_addr = next_obj;
}

/*
 * Tracking of fully allocated slabs for debugging purposes.
 */
// static void
// add_to_full(kmem_cache_node_s *n, slab_s *slab) {
// 	list_header_push(&n->full, &slab->slab_list);
// }
#define add_to_full(n, slab)	\
			list_header_push(&((n)->full), &((slab)->slab_list))
// static void
// remove_from_full(kmem_cache_node_s *n, slab_s *slab) {
// 	list_header_delete_node(&n->full, &slab->slab_list);
// }
#define remove_from_full(n, slab)	\
			list_header_delete_node(&((n)->full), &((slab)->slab_list))

/*
 * Management of partially allocated slabs.
 */
// static inline void
// add_to_partial(kmem_cache_node_s *n, slab_s *slab) {
// 	list_header_push(&n->partial, &slab->slab_list);
// }
#define add_to_partial(n, slab)	\
			list_header_push(&((n)->partial), &((slab)->slab_list))
// static inline void
// remove_from_partial(kmem_cache_node_s *n, slab_s *slab) {
// 	list_header_delete_node(&n->partial, &slab->slab_list);
// }
#define remove_from_partial(n, slab)	\
			list_header_delete_node(&((n)->partial), &((slab)->slab_list))

/********************************************************************
 *						slub alloc functions
 *******************************************************************/
static inline gfp_t
prepare_alloc_flags(kmem_cache_s *s, gfp_t flags) {
	flags &= (GFP_RECLAIM_MASK | GFP_CONSTRAINT_MASK) & gfp_allowed_mask;
	flags |= s->allocflags;
	/*
	 * Let the initial higher-order allocation fail under memory pressure
	 * so we fall-back to the minimum order allocation.
	 */
	return (flags | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_NOFAIL;
}

/*
 * Slab allocation and freeing
 */
static inline slab_s
*alloc_slab(kmem_cache_s *s, gfp_t flags) {
	gfp_t alloc_gfp = prepare_alloc_flags(s, flags);
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
	slab_s *slab = (slab_s *)folio;
	if (!folio)
		return NULL;

	__folio_set_slab(folio);
	/* Make the flag visible before any changes to folio->mapping */
	smp_wmb();
	// if (folio_is_pfmemalloc(folio))
	// 	slab_set_pfmemalloc(slab);
	return slab;
}

static inline slab_s
*alloc_init_new_slab(kmem_cache_s *s, gfp_t flags) {
	slab_s *slab = alloc_slab(s, flags);
	if (unlikely(!slab))
		return NULL;
	
	/* init new slab */
	slab->objects = oo_objects(s->oo);
	slab->inuse = 0;
	slab->frozen = 0;
	slab->slab_cache = s;

	/* init_slab_objects in new slab */
	int idx;
	void *start, *p, *next;
	// start = slab_address(slab);
	start = (void *)page_to_virt(&((folio_s *)slab)->page);
	slab->freelist = start;
	for (idx = 0, p = start; idx < slab->objects - 1; idx++) {
		next = p + s->size;
		set_freepointer(s, p, next);
		p = next;
	}
	set_freepointer(s, p, NULL);

	return slab;
}

static noinline void
online_new_empty_slab(kmem_cache_s *s,
		kmem_cache_node_s *n, slab_s *slab) {
	ulong irqflags;

	spin_lock_irqsave(&n->list_lock, irqflags);

	add_to_partial(n, slab);
	atomic_long_inc(&n->nr_slabs);
	atomic_long_add(slab->objects, &n->total_objects);

	spin_unlock_irqrestore(&n->list_lock, irqflags);
}

/*
 * Called only for kmem_cache_debug() caches instead of acquire_slab(), with a
 * slab from the n->partial list. Remove only a single object from the slab, do
 * the alloc_debug_processing() checks and leave the slab on the list, or move
 * it to full list if it was the last free object.
 */
static inline void
*alloc_single_from_partial(kmem_cache_s *s, slab_s *slab) {
	void *object = slab->freelist;
	slab->freelist = get_freepointer(s, object);
	slab->inuse++;
	return object;
}

/*
 * Get a partial slab, lock it and return it.
 */
// static void *get_partial(kmem_cache_s *s, int node, partial_ctx_s *pc)
#define iter_partial_list list_header_for_each_container_safe
static noinline void
*get_from_partial_list(kmem_cache_s *s, kmem_cache_node_s *n) {
	void *object = NULL;
	ulong flags;
	slab_s *slab, *slab_next;

	spin_lock_irqsave(&n->list_lock, flags);

	iter_partial_list(slab, slab_next, &n->partial, slab_list)
		if ((object = alloc_single_from_partial(s, slab)) != NULL) {
			if (slab->inuse == slab->objects) {
				remove_from_partial(n, slab);
				add_to_full(n, slab);
			}
			break;
		}

	spin_unlock_irqrestore(&n->list_lock, flags);

	return object;
}

// static void *__slab_alloc_node(kmem_cache_s *s,
// 		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
static void
*slab_alloc(kmem_cache_s *s, gfp_t gfpflags) {
	void *object;
	kmem_cache_node_s *n = &s->node;

	while ((object = get_from_partial_list(s, n)) == NULL) {
		slab_s *slab = alloc_init_new_slab(s, gfpflags);
		if (unlikely(!slab))
			return NULL;

		online_new_empty_slab(s, n, slab);
	}

	if (gfpflags & __GFP_ZERO)
		memset(object, 0, s->size);
	return object;
}

void *kmem_cache_alloc(kmem_cache_s *s, gfp_t gfpflags) {
	return slab_alloc(s, gfpflags);
}
EXPORT_SYMBOL(kmem_cache_alloc);


/********************************************************************
 *						slub alloc functions
 *******************************************************************/
static void
free_slab(kmem_cache_s *s, slab_s *slab) {
	folio_s *folio = (folio_s *)(slab);
	int order = folio->_folio_order;
	int pages = 1 << order;

	// __slab_clear_pfmemalloc(slab);
	folio->mapping = NULL;
	/* Make the mapping reset visible before clearing the flag */
	smp_wmb();
	__folio_clear_slab(folio);
	// mm_account_reclaimed_pages(pages);
	// unaccount_slab(slab, order, s);
	__free_pages(&folio->page, order);
}

static void
offline_exceeded_empty_slab(kmem_cache_s *s,
		kmem_cache_node_s *n, slab_s *slab) {
	ulong flags;

	spin_lock_irqsave(&n->list_lock, flags);

	remove_from_partial(n, slab);
	atomic_long_dec(&n->nr_slabs);
	atomic_long_sub(slab->objects, &n->total_objects);

	spin_unlock_irqrestore(&n->list_lock, flags);
}

/* Perform the actual freeing while we still hold the locks */
static void
free_single_to_partial(kmem_cache_s *s,
		slab_s *slab, void *object) {
	slab->inuse--;
	set_freepointer(s, object, slab->freelist);
	slab->freelist = object;
}

static noinline void
free_to_partial_list(kmem_cache_s *s, kmem_cache_node_s *n,
		slab_s *slab, void *object) {

	ulong flags;
	spin_lock_irqsave(&n->list_lock, flags);
	/* If object is in a full slab, move this slab to free list */
	if (slab->inuse == slab->objects) {
		remove_from_full(n, slab);
		add_to_partial(n, slab);
	}
	free_single_to_partial(s, slab, object);
	spin_unlock_irqrestore(&n->list_lock, flags);
}

static void
slab_free(kmem_cache_s *s, slab_s *slab, void *object) {
	kmem_cache_node_s *n = &s->node;
	free_to_partial_list(s, n, slab, object);

	/*
	 * If the slab is empty, and node's partial list is full,
	 * it should be discarded anyway no matter it's on full or
	 * partial list.
	 */
	if (slab->inuse == 0 ||
		n->partial.count >= s->min_partial) {

	    offline_exceeded_empty_slab(s, n, slab);
	    free_slab(s, slab);
	}
}

void kmem_cache_free(kmem_cache_s *s, void *x) {
	// s = cache_from_obj(s, x);
	// if (!s)
	// 	return;
	slab_free(s, virt_to_slab(x), x);
}
EXPORT_SYMBOL(kmem_cache_free);


/********************************************************************
 *					create kmem_cache functions
 *******************************************************************/

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
simple_calculate_order(uint size) {
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
// static int calculate_sizes(kmem_cache_s *s)
static int
simple_calculate_sizes(kmem_cache_s *s) {
	slab_flags_t flags = s->flags;
	uint order, size = s->object_size;
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

	return !!oo_objects(s->oo);
}
#define calculate_sizes simple_calculate_sizes

// static int kmem_cache_open(kmem_cache_s *s, slab_flags_t flags)
static int
simple_kmem_cache_open(kmem_cache_s *s, slab_flags_t flags) {
	s->flags = flags;
	if (calculate_sizes(s) <= 0)
		goto error;

	/*
	 * The larger the object size is, the more slabs we want on the partial
	 * list to avoid pounding the page allocator excessively.
	 */
	s->min_partial = min_t(ulong, MAX_PARTIAL, ilog2(s->size) / 2);
	s->min_partial = max_t(ulong, MIN_PARTIAL, s->min_partial);

	// static int init_kmem_cache_nodes(kmem_cache_s *s)
	// {
		kmem_cache_node_s *node_cache = &s->node;
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
static void kmalloc_test() {
	int test_count = 64;
	void *kmalloc_testp[test_count];
	for (int i = 0; i < test_count; i++)
		kmalloc_testp[i] = kmalloc(4096, GFP_KERNEL);
	for (int i = 0; i < test_count; i++)
		kfree(kmalloc_testp[i]);
}

/*
 * Used for early kmem_cache structures that were allocated using
 * the page allocator. Allocate them properly then fix up the pointers
 * that may be pointing to the wrong kmem_cache structure.
 */
static kmem_cache_s __init
*bootstrap(kmem_cache_s *static_cache) {
	kmem_cache_s *s = kmem_cache_zalloc(kmem_cache, GFP_NOWAIT);

	kmem_cache_node_s *tmp_n = &static_cache->node;
	kmem_cache_node_s *n = &s->node;
	slab_s *slab = container_of(tmp_n->partial.anchor.next, slab_s, slab_list);

	remove_from_partial(tmp_n, slab);
	memcpy(s, static_cache, sizeof(kmem_cache_s));
	INIT_LIST_HEADER_S(&n->partial);
	INIT_LIST_HEADER_S(&n->full);
	add_to_partial(n, slab);
	list_header_push(&slab_caches, &s->list);
	return s;
}

void __init kmem_cache_init(void)
{
	static __initdata kmem_cache_s
		boot_kmem_cache;

	kmem_cache = &boot_kmem_cache;

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	// create_boot_cache(kmem_cache, "kmem_cache",
	// 		offsetof(struct kmem_cache, node) +
	// 			nr_node_ids * sizeof(struct kmem_cache_node *),
	// 		SLAB_HWCACHE_ALIGN);
	create_boot_cache(kmem_cache, "kmem_cache",
			sizeof(kmem_cache_s), SLAB_HWCACHE_ALIGN);

	kmem_cache = bootstrap(&boot_kmem_cache);

	/* Now we can use the kmem_cache to allocate kmalloc slabs */
	// setup_kmalloc_cache_index_table();
	create_kmalloc_caches(0);

	pr_info("SLUB: HWalign=%d, Order=%u-%u, MinObjects=%u, CPUs=%u, Nodes=%u\n",
			cache_line_size(), slub_min_order, slub_max_order, slub_min_objects,
			nr_cpu_ids, nr_node_ids);

	// kmalloc_test();
}


int __kmem_cache_create(kmem_cache_s *s, slab_flags_t flags)
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