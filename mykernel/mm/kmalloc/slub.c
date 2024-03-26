#include <linux/init/init.h>
#include <linux/kernel/nodemask.h>

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
		.order		= OO_SHIFT,
		.obj_nr		= order_objects(order, size),
	};
	return x;
}

#define oo_order(x)		((x).order)
#define oo_objects(x)	((x).obj_nr)



// /*
//  * Slow path. The lockless freelist is empty or we need to perform
//  * debugging duties.
//  *
//  * Processing is still very fast if new objects have been freed to the
//  * regular freelist. In that case we simply take over the regular freelist
//  * as the lockless freelist and zap the regular freelist.
//  *
//  * If that is not working then we fall back to the partial lists. We take the
//  * first element of the freelist as the object to allocate now and move the
//  * rest of the freelist to the lockless freelist.
//  *
//  * And if we were unable to get a new slab from the partial slab lists then
//  * we need to allocate a new slab. This is the slowest path since it involves
//  * a call to the page allocator and the setup of a new slab.
//  *
//  * Version of __slab_alloc to use when we know that preemption is
//  * already disabled (which is the case for bulk allocation).
//  */
// static void *___slab_alloc(struct kmem_cache *s, gfp_t gfpflags, int node,
// 			  unsigned long addr, struct kmem_cache_cpu *c, unsigned int orig_size)
// {
// 	void *freelist;
// 	struct slab *slab;
// 	unsigned long flags;
// 	struct partial_context pc;

// 	stat(s, ALLOC_SLOWPATH);

// reread_slab:

// 	slab = READ_ONCE(c->slab);
// 	if (!slab) {
// 		/*
// 		 * if the node is not online or has no normal memory, just
// 		 * ignore the node constraint
// 		 */
// 		if (unlikely(node != NUMA_NO_NODE &&
// 			     !node_isset(node, slab_nodes)))
// 			node = NUMA_NO_NODE;
// 		goto new_slab;
// 	}
// redo:

// 	if (unlikely(!node_match(slab, node))) {
// 		/*
// 		 * same as above but node_match() being false already
// 		 * implies node != NUMA_NO_NODE
// 		 */
// 		if (!node_isset(node, slab_nodes)) {
// 			node = NUMA_NO_NODE;
// 		} else {
// 			stat(s, ALLOC_NODE_MISMATCH);
// 			goto deactivate_slab;
// 		}
// 	}

// 	/*
// 	 * By rights, we should be searching for a slab page that was
// 	 * PFMEMALLOC but right now, we are losing the pfmemalloc
// 	 * information when the page leaves the per-cpu allocator
// 	 */
// 	if (unlikely(!pfmemalloc_match(slab, gfpflags)))
// 		goto deactivate_slab;

// 	/* must check again c->slab in case we got preempted and it changed */
// 	local_lock_irqsave(&s->cpu_slab->lock, flags);
// 	if (unlikely(slab != c->slab)) {
// 		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 		goto reread_slab;
// 	}
// 	freelist = c->freelist;
// 	if (freelist)
// 		goto load_freelist;

// 	freelist = get_freelist(s, slab);

// 	if (!freelist) {
// 		c->slab = NULL;
// 		c->tid = next_tid(c->tid);
// 		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 		stat(s, DEACTIVATE_BYPASS);
// 		goto new_slab;
// 	}

// 	stat(s, ALLOC_REFILL);

// load_freelist:

// 	lockdep_assert_held(this_cpu_ptr(&s->cpu_slab->lock));

// 	/*
// 	 * freelist is pointing to the list of objects to be used.
// 	 * slab is pointing to the slab from which the objects are obtained.
// 	 * That slab must be frozen for per cpu allocations to work.
// 	 */
// 	VM_BUG_ON(!c->slab->frozen);
// 	c->freelist = get_freepointer(s, freelist);
// 	c->tid = next_tid(c->tid);
// 	local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 	return freelist;

// deactivate_slab:

// 	local_lock_irqsave(&s->cpu_slab->lock, flags);
// 	if (slab != c->slab) {
// 		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 		goto reread_slab;
// 	}
// 	freelist = c->freelist;
// 	c->slab = NULL;
// 	c->freelist = NULL;
// 	c->tid = next_tid(c->tid);
// 	local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 	deactivate_slab(s, slab, freelist);

// new_slab:

// 	if (slub_percpu_partial(c)) {
// 		local_lock_irqsave(&s->cpu_slab->lock, flags);
// 		if (unlikely(c->slab)) {
// 			local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 			goto reread_slab;
// 		}
// 		if (unlikely(!slub_percpu_partial(c))) {
// 			local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 			/* we were preempted and partial list got empty */
// 			goto new_objects;
// 		}

// 		slab = c->slab = slub_percpu_partial(c);
// 		slub_set_percpu_partial(c, slab);
// 		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
// 		stat(s, CPU_PARTIAL_ALLOC);
// 		goto redo;
// 	}

// new_objects:

// 	pc.flags = gfpflags;
// 	pc.slab = &slab;
// 	pc.orig_size = orig_size;
// 	freelist = get_partial(s, node, &pc);
// 	if (freelist)
// 		goto check_new_slab;

// 	slub_put_cpu_ptr(s->cpu_slab);
// 	slab = new_slab(s, gfpflags, node);
// 	c = slub_get_cpu_ptr(s->cpu_slab);

// 	if (unlikely(!slab)) {
// 		slab_out_of_memory(s, gfpflags, node);
// 		return NULL;
// 	}

// 	stat(s, ALLOC_SLAB);

// 	if (kmem_cache_debug(s)) {
// 		freelist = alloc_single_from_new_slab(s, slab, orig_size);

// 		if (unlikely(!freelist))
// 			goto new_objects;

// 		if (s->flags & SLAB_STORE_USER)
// 			set_track(s, freelist, TRACK_ALLOC, addr);

// 		return freelist;
// 	}

// 	/*
// 	 * No other reference to the slab yet so we can
// 	 * muck around with it freely without cmpxchg
// 	 */
// 	freelist = slab->freelist;
// 	slab->freelist = NULL;
// 	slab->inuse = slab->objects;
// 	slab->frozen = 1;

// 	inc_slabs_node(s, slab_nid(slab), slab->objects);

// check_new_slab:

// 	if (kmem_cache_debug(s)) {
// 		/*
// 		 * For debug caches here we had to go through
// 		 * alloc_single_from_partial() so just store the tracking info
// 		 * and return the object
// 		 */
// 		if (s->flags & SLAB_STORE_USER)
// 			set_track(s, freelist, TRACK_ALLOC, addr);

// 		return freelist;
// 	}

// 	if (unlikely(!pfmemalloc_match(slab, gfpflags))) {
// 		/*
// 		 * For !pfmemalloc_match() case we don't load freelist so that
// 		 * we don't make further mismatched allocations easier.
// 		 */
// 		deactivate_slab(s, slab, get_freepointer(s, freelist));
// 		return freelist;
// 	}

// retry_load_slab:

// 	local_lock_irqsave(&s->cpu_slab->lock, flags);
// 	if (unlikely(c->slab)) {
// 		void *flush_freelist = c->freelist;
// 		struct slab *flush_slab = c->slab;

// 		c->slab = NULL;
// 		c->freelist = NULL;
// 		c->tid = next_tid(c->tid);

// 		local_unlock_irqrestore(&s->cpu_slab->lock, flags);

// 		deactivate_slab(s, flush_slab, flush_freelist);

// 		stat(s, CPUSLAB_FLUSH);

// 		goto retry_load_slab;
// 	}
// 	c->slab = slab;

// 	goto load_freelist;
// }

// /*
//  * A wrapper for ___slab_alloc() for contexts where preemption is not yet
//  * disabled. Compensates for possible cpu changes by refetching the per cpu area
//  * pointer.
//  */
// static void *__slab_alloc(struct kmem_cache *s, gfp_t gfpflags, int node,
// 			  unsigned long addr, struct kmem_cache_cpu *c, unsigned int orig_size)
// {
// 	void *p;

// #ifdef CONFIG_PREEMPT_COUNT
// 	/*
// 	 * We may have been preempted and rescheduled on a different
// 	 * cpu before disabling preemption. Need to reload cpu area
// 	 * pointer.
// 	 */
// 	c = slub_get_cpu_ptr(s->cpu_slab);
// #endif

// 	p = ___slab_alloc(s, gfpflags, node, addr, c, orig_size);
// #ifdef CONFIG_PREEMPT_COUNT
// 	slub_put_cpu_ptr(s->cpu_slab);
// #endif
// 	return p;
// }

// static __always_inline void *__slab_alloc_node(struct kmem_cache *s,
// 		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
// {
// 	struct kmem_cache_cpu *c;
// 	struct slab *slab;
// 	unsigned long tid;
// 	void *object;

// redo:
// 	/*
// 	 * Must read kmem_cache cpu data via this cpu ptr. Preemption is
// 	 * enabled. We may switch back and forth between cpus while
// 	 * reading from one cpu area. That does not matter as long
// 	 * as we end up on the original cpu again when doing the cmpxchg.
// 	 *
// 	 * We must guarantee that tid and kmem_cache_cpu are retrieved on the
// 	 * same cpu. We read first the kmem_cache_cpu pointer and use it to read
// 	 * the tid. If we are preempted and switched to another cpu between the
// 	 * two reads, it's OK as the two are still associated with the same cpu
// 	 * and cmpxchg later will validate the cpu.
// 	 */
// 	c = raw_cpu_ptr(s->cpu_slab);
// 	tid = READ_ONCE(c->tid);

// 	/*
// 	 * Irqless object alloc/free algorithm used here depends on sequence
// 	 * of fetching cpu_slab's data. tid should be fetched before anything
// 	 * on c to guarantee that object and slab associated with previous tid
// 	 * won't be used with current tid. If we fetch tid first, object and
// 	 * slab could be one associated with next tid and our alloc/free
// 	 * request will be failed. In this case, we will retry. So, no problem.
// 	 */
// 	barrier();

// 	/*
// 	 * The transaction ids are globally unique per cpu and per operation on
// 	 * a per cpu queue. Thus they can be guarantee that the cmpxchg_double
// 	 * occurs on the right processor and that there was no operation on the
// 	 * linked list in between.
// 	 */

// 	object = c->freelist;
// 	slab = c->slab;

// 	if (!USE_LOCKLESS_FAST_PATH() ||
// 	    unlikely(!object || !slab || !node_match(slab, node))) {
// 		object = __slab_alloc(s, gfpflags, node, addr, c, orig_size);
// 	} else {
// 		void *next_object = get_freepointer_safe(s, object);

// 		/*
// 		 * The cmpxchg will only match if there was no additional
// 		 * operation and if we are on the right processor.
// 		 *
// 		 * The cmpxchg does the following atomically (without lock
// 		 * semantics!)
// 		 * 1. Relocate first pointer to the current per cpu area.
// 		 * 2. Verify that tid and freelist have not been changed
// 		 * 3. If they were not changed replace tid and freelist
// 		 *
// 		 * Since this is without lock semantics the protection is only
// 		 * against code executing on this cpu *not* from access by
// 		 * other cpus.
// 		 */
// 		if (unlikely(!this_cpu_cmpxchg_double(
// 				s->cpu_slab->freelist, s->cpu_slab->tid,
// 				object, tid,
// 				next_object, next_tid(tid)))) {

// 			note_cmpxchg_failure("slab_alloc", s, tid);
// 			goto redo;
// 		}
// 		prefetch_freepointer(s, next_object);
// 		stat(s, ALLOC_FASTPATH);
// 	}

// 	return object;
// }
// #else /* CONFIG_SLUB_TINY */
// static void *__slab_alloc_node(struct kmem_cache *s,
// 		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
// {
// 	struct partial_context pc;
// 	struct slab *slab;
// 	void *object;

// 	pc.flags = gfpflags;
// 	pc.slab = &slab;
// 	pc.orig_size = orig_size;
// 	object = get_partial(s, node, &pc);

// 	if (object)
// 		return object;

// 	slab = new_slab(s, gfpflags, node);
// 	if (unlikely(!slab)) {
// 		slab_out_of_memory(s, gfpflags, node);
// 		return NULL;
// 	}

// 	object = alloc_single_from_new_slab(s, slab, orig_size);

// 	return object;
// }
// #endif /* CONFIG_SLUB_TINY */

// /*
//  * If the object has been wiped upon free, make sure it's fully initialized by
//  * zeroing out freelist pointer.
//  */
// static __always_inline void maybe_wipe_obj_freeptr(struct kmem_cache *s,
// 						   void *obj)
// {
// 	if (unlikely(slab_want_init_on_free(s)) && obj)
// 		memset((void *)((char *)kasan_reset_tag(obj) + s->offset),
// 			0, sizeof(void *));
// }

// /*
//  * Inlined fastpath so that allocation functions (kmalloc, kmem_cache_alloc)
//  * have the fastpath folded into their functions. So no function call
//  * overhead for requests that can be satisfied on the fastpath.
//  *
//  * The fastpath works by first checking if the lockless freelist can be used.
//  * If not then __slab_alloc is called for slow processing.
//  *
//  * Otherwise we can simply pick the next object from the lockless free list.
//  */
// static __fastpath_inline void *slab_alloc_node(struct kmem_cache *s, struct list_lru *lru,
// 		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
// {
// 	void *object;
// 	struct obj_cgroup *objcg = NULL;
// 	bool init = false;

// 	s = slab_pre_alloc_hook(s, lru, &objcg, 1, gfpflags);
// 	if (!s)
// 		return NULL;

// 	object = kfence_alloc(s, orig_size, gfpflags);
// 	if (unlikely(object))
// 		goto out;

// 	object = __slab_alloc_node(s, gfpflags, node, addr, orig_size);

// 	maybe_wipe_obj_freeptr(s, object);
// 	init = slab_want_init_on_alloc(gfpflags, s);

// out:
// 	/*
// 	 * When init equals 'true', like for kzalloc() family, only
// 	 * @orig_size bytes might be zeroed instead of s->object_size
// 	 */
// 	slab_post_alloc_hook(s, objcg, gfpflags, 1, &object, init, orig_size);

// 	return object;
// }

// static __fastpath_inline void *slab_alloc(struct kmem_cache *s, struct list_lru *lru,
// 		gfp_t gfpflags, unsigned long addr, size_t orig_size)
// {
// 	return slab_alloc_node(s, lru, gfpflags, NUMA_NO_NODE, addr, orig_size);
// }

// static __always_inline void
// *__kmem_cache_alloc_lru(struct kmem_cache *s, struct list_lru *lru, gfp_t gfpflags)
// {
// 	void *ret = slab_alloc(s, lru, gfpflags, _RET_IP_, s->object_size);

// 	trace_kmem_cache_alloc(_RET_IP_, ret, s, gfpflags, NUMA_NO_NODE);

// 	return ret;
// }

void *kmem_cache_alloc(struct kmem_cache *s, gfp_t gfpflags)
{
	// return __kmem_cache_alloc_lru(s, NULL, gfpflags);
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
static unsigned int slub_min_order = 0;
static unsigned int slub_max_order = PAGE_ALLOC_COSTLY_ORDER;
static unsigned int slub_min_objects = 4;

// static inline int calculate_order(unsigned int size)
static inline int
simple_calculate_order(unsigned int size) {
	int order,
		min_order = (int)slub_min_order;
	unsigned int
		min_objnr = slub_min_objects,
		fraction = 64;

	// make sure @min_order can hold at least 1 object
	for (order = max(min_order, get_order(min_objnr * size));
			order <= MAX_ORDER; order++) {

		unsigned int slab_size = (unsigned int)PAGE_SIZE << order;
		unsigned int rem;

		rem = slab_size % size;

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
	unsigned int size = s->object_size;
	unsigned int order;

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

	if (!calculate_sizes(s))
		goto error;

	/*
	 * The larger the object size is, the more slabs we want on the partial
	 * list to avoid pounding the page allocator excessively.
	 */
	s->min_partial = min_t(unsigned long, MAX_PARTIAL, ilog2(s->size) / 2);
	s->min_partial = max_t(unsigned long, MIN_PARTIAL, s->min_partial);

	// /* Initialize the pre-computed randomized freelist if slab is up */
	// if (slab_state >= UP) {
	// 	if (init_cache_random_seq(s))
	// 		goto error;
	// }

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
	static __initdata kmem_cache_s
		boot_kmem_cache,
		boot_kmem_cache_node;
	int node;

	kmem_cache_node = &boot_kmem_cache_node;
	kmem_cache = &boot_kmem_cache;

	// create_boot_cache(kmem_cache_node, "kmem_cache_node",
	// 	sizeof(struct kmem_cache_node), SLAB_HWCACHE_ALIGN, 0, 0);

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	create_boot_cache(kmem_cache, "kmem_cache",
			offsetof(struct kmem_cache, node) +
				nr_node_ids * sizeof(struct kmem_cache_node *),
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