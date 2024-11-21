#define WORKQUEUE_DEFINATION
#include "workqueue.h"

// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/workqueue.c - generic async execution with shared worker pool
 *
 * Copyright (C) 2002		Ingo Molnar
 *
 *   Derived from the taskqueue/keventd code by:
 *     David Woodhouse <dwmw2@infradead.org>
 *     Andrew Morton
 *     Kai Petzke <wpp@marie.physik.tu-berlin.de>
 *     Theodore Ts'o <tytso@mit.edu>
 *
 * Made to use alloc_percpu by Christoph Lameter.
 *
 * Copyright (C) 2010		SUSE Linux Products GmbH
 * Copyright (C) 2010		Tejun Heo <tj@kernel.org>
 *
 * This is the generic async execution mechanism.  Work items as are
 * executed in process context.  The worker pool is shared and
 * automatically managed.  There are two worker pools for each CPU (one for
 * normal work items and the other for high priority ones) and some extra
 * pools for workqueues which are not bound to any specific CPU - the
 * number of these backing pools is dynamic.
 *
 * Please read Documentation/core-api/workqueue.rst for details.
 */
#include <linux/init/init.h>
#include <linux/kernel/mm_api.h>



static kmem_cache_s *pwq_cache;


static DECLARE_LIST_HDR_S(workqueues);	/* PR: list of all workqueues */
static bool workqueue_freezing;			/* PL: have wqs started freezing? */

// static DEFINE_IDR(worker_pool_idr);	/* PR: idr of all pools */

// /* PL: hash of all unbound pools keyed by pool->attrs */
// static DEFINE_HASHTABLE(unbound_pool_hash, UNBOUND_POOL_HASH_ORDER);

// /* I: attributes used when instantiating standard unbound pools on demand */
// static struct workqueue_attrs *unbound_std_wq_attrs[NR_STD_WORKER_POOLS];

// /* I: attributes used when instantiating ordered pools on demand */
// static struct workqueue_attrs *ordered_wq_attrs[NR_STD_WORKER_POOLS];

// /*
//  * I: kthread_worker to release pwq's. pwq release needs to be bounced to a
//  * process context while holding a pool lock. Bounce to a dedicated kthread
//  * worker to avoid A-A deadlocks.
//  */
// static struct kthread_worker *pwq_release_worker __ro_after_init;

workqueue_s *system_wq __ro_after_init;
EXPORT_SYMBOL(system_wq);
// workqueue_s *system_highpri_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_highpri_wq);
// workqueue_s *system_long_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_long_wq);
// workqueue_s *system_unbound_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_unbound_wq);
// workqueue_s *system_freezable_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_freezable_wq);
// workqueue_s *system_power_efficient_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_power_efficient_wq);
// workqueue_s *system_freezable_power_efficient_wq __ro_after_init;
// EXPORT_SYMBOL_GPL(system_freezable_power_efficient_wq);
// workqueue_s *system_bh_wq;
// EXPORT_SYMBOL_GPL(system_bh_wq);
// workqueue_s *system_bh_highpri_wq;
// EXPORT_SYMBOL_GPL(system_bh_highpri_wq);




// static struct worker *alloc_worker(int node)
static worker_s *alloc_worker() {
	worker_s *worker;

	worker = kzalloc_node(sizeof(*worker), GFP_KERNEL, 0);
	if (worker) {
		INIT_LIST_HEAD(&worker->entry);
		// INIT_LIST_HEAD(&worker->scheduled);
		INIT_LIST_HEAD(&worker->node);
		/* on creation a worker is in !idle && prep state */
		// worker->flags = WORKER_PREP;
	}
	return worker;
}


/**
 * create_worker - create a new workqueue worker
 * @pool: pool the new worker will belong to
 *
 * Create and start a new worker which is attached to @pool.
 *
 * CONTEXT:
 * Might sleep.  Does GFP_KERNEL allocations.
 *
 * Return:
 * Pointer to the newly created worker.
 */
static worker_s
*create_worker(worker_pool_s *pool) {
	worker_s *worker;
	// int id;

	// /* ID is needed to determine kthread name */
	// id = ida_alloc(&pool->worker_ida, GFP_KERNEL);
	// if (id < 0) {
	// 	pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
	// 		    ERR_PTR(id));
	// 	return NULL;
	// }

	// worker = alloc_worker(pool->node);
	worker = alloc_worker();
	if (!worker) {
		pr_err_once("workqueue: Failed to allocate a worker\n");
		goto fail;
	}

	// worker->id = id;

	if (!(pool->flags & POOL_BH)) {
		char id_buf[WORKER_ID_LEN];

		// format_worker_id(id_buf, sizeof(id_buf), worker, pool);
		// worker->task = kthread_create_on_node(worker_thread, worker,
		// 				      pool->node, "%s", id_buf);
		// if (IS_ERR(worker->task)) {
		// 	if (PTR_ERR(worker->task) == -EINTR) {
		// 		pr_err("workqueue: Interrupted when creating a worker thread \"%s\"\n",
		// 		       id_buf);
		// 	} else {
		// 		pr_err_once("workqueue: Failed to create a worker thread: %pe",
		// 			    worker->task);
		// 	}
		// 	goto fail;
		// }

		// set_user_nice(worker->task, pool->attrs->nice);
		// kthread_bind_mask(worker->task, pool_allowed_cpus(pool));
	}

	// /* successful, attach the worker to the pool */
	// worker_attach_to_pool(worker, pool);

	// /* start the newly created worker */
	// raw_spin_lock_irq(&pool->lock);

	// worker->pool->nr_workers++;
	// worker_enter_idle(worker);

	// /*
	//  * @worker is waiting on a completion in kthread() and will trigger hung
	//  * check if not woken up soon. As kick_pool() is noop if @pool is empty,
	//  * wake it up explicitly.
	//  */
	// if (worker->task)
	// 	wake_up_process(worker->task);

	// raw_spin_unlock_irq(&pool->lock);

	return worker;

fail:
	// ida_free(&pool->worker_ida, id);
	kfree(worker);
	return NULL;
}



static int
wq_clamp_max_active(int max_active, uint flags, const char *name) {
	if (max_active < 1 || max_active > WQ_MAX_ACTIVE)
		pr_warn("workqueue: max_active %d requested for %s is out of range,"
			"clamping between %d and %d\n", max_active, name, 1, WQ_MAX_ACTIVE);

	return clamp_val(max_active, 1, WQ_MAX_ACTIVE);
}


static int
alloc_and_link_pwqs(workqueue_s *wq) {
	bool highpri = wq->flags & WQ_HIGHPRI;
	int cpu, ret;

	// lockdep_assert_held(&wq_pool_mutex);

	wq->cpu_pwq = alloc_percpu(pool_workqueue_s *);
	if (!wq->cpu_pwq)
		goto enomem;

	if (!(wq->flags & WQ_UNBOUND)) {
		worker_pool_s __percpu *pools;

		// if (wq->flags & WQ_BH)
		// 	pools = bh_worker_pools;
		// else
			pools = cpu_worker_pools;

		for_each_possible_cpu(cpu) {
			pool_workqueue_s **pwq_p;
			worker_pool_s *pool;

			pool = &(per_cpu_ptr(pools, cpu)[highpri]);
			pwq_p = per_cpu_ptr(wq->cpu_pwq, cpu);

			// *pwq_p = kmem_cache_alloc_node(pwq_cache, GFP_KERNEL,
			// 			       pool->node);
			*pwq_p = kmem_cache_alloc(pwq_cache, GFP_KERNEL);
			if (!*pwq_p)
				goto enomem;

			// init_pwq(*pwq_p, wq, pool);

			// mutex_lock(&wq->mutex);
			// link_pwq(*pwq_p);
			// mutex_unlock(&wq->mutex);
		}
		return 0;
	}

	// if (wq->flags & __WQ_ORDERED) {
	// 	struct pool_workqueue *dfl_pwq;

	// 	ret = apply_workqueue_attrs_locked(wq, ordered_wq_attrs[highpri]);
	// 	/* there should only be single pwq for ordering guarantee */
	// 	dfl_pwq = rcu_access_pointer(wq->dfl_pwq);
	// 	WARN(!ret && (wq->pwqs.next != &dfl_pwq->pwqs_node ||
	// 			wq->pwqs.prev != &dfl_pwq->pwqs_node),
	// 			"ordering guarantee broken for workqueue %s\n", wq->name);
	// } else {
	// 	ret = apply_workqueue_attrs_locked(wq, unbound_std_wq_attrs[highpri]);
	// }

	return ret;

enomem:
	if (wq->cpu_pwq) {
		for_each_possible_cpu(cpu) {
			pool_workqueue_s *pwq = *per_cpu_ptr(wq->cpu_pwq, cpu);

			if (pwq)
				kmem_cache_free(pwq_cache, pwq);
		}
		// free_percpu(wq->cpu_pwq);
		// wq->cpu_pwq = NULL;
	}
	return -ENOMEM;
}


__printf(1, 4) workqueue_s
*alloc_workqueue(const char *fmt, uint flags, int max_active, ...)
{
	va_list args;
	workqueue_s *wq;
	size_t wq_size;
	int name_len;

	if (flags & WQ_BH) {
		if (WARN_ON_ONCE(flags & ~__WQ_BH_ALLOWS))
			return NULL;
		if (WARN_ON_ONCE(max_active))
			return NULL;
	}

	// /* see the comment above the definition of WQ_POWER_EFFICIENT */
	// if ((flags & WQ_POWER_EFFICIENT) && wq_power_efficient)
	// 	flags |= WQ_UNBOUND;

	// /* allocate wq and format name */
	// if (flags & WQ_UNBOUND)
	// 	wq_size = struct_size(wq, node_nr_active, nr_node_ids + 1);
	// else
		wq_size = sizeof(*wq);

	wq = kzalloc(wq_size, GFP_KERNEL);
	if (!wq)
		return NULL;

	// if (flags & WQ_UNBOUND) {
	// 	wq->unbound_attrs = alloc_workqueue_attrs();
	// 	if (!wq->unbound_attrs)
	// 		goto err_free_wq;
	// }

	va_start(args, max_active);
	name_len = vsnprintf(wq->name, sizeof(wq->name), fmt, args);
	va_end(args);

	if (name_len >= WQ_NAME_LEN)
		pr_warn_once("workqueue: name exceeds WQ_NAME_LEN. Truncating to: %s\n", wq->name);

	if (flags & WQ_BH) {
		/*
		 * BH workqueues always share a single execution context per CPU
		 * and don't impose any max_active limit.
		 */
		max_active = INT_MAX;
	} else {
		max_active = max_active ?: WQ_DFL_ACTIVE;
		max_active = wq_clamp_max_active(max_active, flags, wq->name);
	}

	// /* init wq */
	// wq->flags = flags;
	// wq->max_active = max_active;
	// wq->min_active = min(max_active, WQ_DFL_MIN_ACTIVE);
	// wq->saved_max_active = wq->max_active;
	// wq->saved_min_active = wq->min_active;
	// mutex_init(&wq->mutex);
	// atomic_set(&wq->nr_pwqs_to_flush, 0);
	INIT_LIST_HEADER_S(&wq->pwqs);
	// INIT_LIST_HEAD(&wq->flusher_queue);
	// INIT_LIST_HEAD(&wq->flusher_overflow);
	// INIT_LIST_HEAD(&wq->maydays);

	// wq_init_lockdep(wq);
	INIT_LIST_HEAD(&wq->list);

	// if (flags & WQ_UNBOUND) {
	// 	if (alloc_node_nr_active(wq->node_nr_active) < 0)
	// 		goto err_unreg_lockdep;
	// }

	// /*
	//  * wq_pool_mutex protects the workqueues list, allocations of PWQs,
	//  * and the global freeze state.
	//  */
	// apply_wqattrs_lock();

	if (alloc_and_link_pwqs(wq) < 0)
		goto err_unlock_free_node_nr_active;

	// mutex_lock(&wq->mutex);
	// wq_adjust_max_active(wq);
	// mutex_unlock(&wq->mutex);

	list_header_add_to_tail(&workqueues, &wq->list);

	// if (wq_online && init_rescuer(wq) < 0)
	// 	goto err_unlock_destroy;

	// apply_wqattrs_unlock();

	// if ((wq->flags & WQ_SYSFS) && workqueue_sysfs_register(wq))
	// 	goto err_destroy;

	return wq;

err_unlock_free_node_nr_active:
	// apply_wqattrs_unlock();
	// /*
	//  * Failed alloc_and_link_pwqs() may leave pending pwq->release_work,
	//  * flushing the pwq_release_worker ensures that the pwq_release_workfn()
	//  * completes before calling kfree(wq).
	//  */
	// if (wq->flags & WQ_UNBOUND) {
	// 	kthread_flush_worker(pwq_release_worker);
	// 	free_node_nr_active(wq->node_nr_active);
	// }
err_unreg_lockdep:
	// wq_unregister_lockdep(wq);
	// wq_free_lockdep(wq);
err_free_wq:
	// free_workqueue_attrs(wq->unbound_attrs);
	kfree(wq);
	return NULL;
err_unlock_destroy:
	// apply_wqattrs_unlock();
err_destroy:
	// destroy_workqueue(wq);
	return NULL;
}
EXPORT_SYMBOL_GPL(alloc_workqueue);


/**
 * init_worker_pool - initialize a newly zalloc'd worker_pool
 * @pool: worker_pool to initialize
 *
 * Initialize a newly zalloc'd @pool.  It also allocates @pool->attrs.
 *
 * Return: 0 on success, -errno on failure.  Even on failure, all fields
 * inside @pool proper are initialized and put_unbound_pool() can be called
 * on @pool safely to release it.
 */
static int
init_worker_pool(worker_pool_s *pool) {
	spin_lock_init(&pool->lock);
	pool->id = -1;
	pool->cpu = -1;
	// pool->node = NUMA_NO_NODE;
	pool->flags |= POOL_DISASSOCIATED;
	// pool->watchdog_ts = jiffies;
	INIT_LIST_HEADER_S(&pool->worklist);
	INIT_LIST_HEADER_S(&pool->idle_list);
	// hash_init(pool->busy_hash);

	// timer_setup(&pool->idle_timer, idle_worker_timeout, TIMER_DEFERRABLE);
	// INIT_WORK(&pool->idle_cull_work, idle_cull_fn);

	// timer_setup(&pool->mayday_timer, pool_mayday_timeout, 0);

	INIT_LIST_HEADER_S(&pool->workers);

	// ida_init(&pool->worker_ida);
	// INIT_HLIST_NODE(&pool->hash_node);
	// pool->refcnt = 1;

	// /* shouldn't fail above this point */
	// pool->attrs = alloc_workqueue_attrs();
	// if (!pool->attrs)
	// 	return -ENOMEM;

	// wqattrs_clear_for_pool(pool->attrs);

	return 0;
}


static void __init
init_cpu_worker_pool(worker_pool_s *pool, int cpu, int nice) {
	BUG_ON(init_worker_pool(pool));
	pool->cpu = cpu;
	// cpumask_copy(pool->attrs->cpumask, cpumask_of(cpu));
	// cpumask_copy(pool->attrs->__pod_cpumask, cpumask_of(cpu));
	// pool->attrs->nice = nice;
	// pool->attrs->affn_strict = true;
	// pool->node = cpu_to_node(cpu);

	// /* alloc pool ID */
	// mutex_lock(&wq_pool_mutex);
	// BUG_ON(worker_pool_assign_id(pool));
	// mutex_unlock(&wq_pool_mutex);
}

/**
 * workqueue_init_early - early init for workqueue subsystem
 *
 * This is the first step of three-staged workqueue subsystem initialization and
 * invoked as soon as the bare basics - memory allocation, cpumasks and idr are
 * up. It sets up all the data structures and system workqueues and allows early
 * boot code to create workqueues and queue/cancel work items. Actual work item
 * execution starts only after kthreads can be created and scheduled right
 * before early initcalls.
 */
void __init workqueue_init_early(void)
{
	// struct wq_pod_type *pt = &wq_pod_types[WQ_AFFN_SYSTEM];
	int std_nice[NR_STD_WORKER_POOLS] = { 0, HIGHPRI_NICE_LEVEL };
	// void (*irq_work_fns[2])(struct irq_work *) = { bh_pool_kick_normal,
	// 						   bh_pool_kick_highpri };
	int i, cpu;

	BUILD_BUG_ON(__alignof__(pool_workqueue_s) < __alignof__(long long));

	// BUG_ON(!alloc_cpumask_var(&wq_online_cpumask, GFP_KERNEL));
	// BUG_ON(!alloc_cpumask_var(&wq_unbound_cpumask, GFP_KERNEL));
	// BUG_ON(!alloc_cpumask_var(&wq_requested_unbound_cpumask, GFP_KERNEL));
	// BUG_ON(!zalloc_cpumask_var(&wq_isolated_cpumask, GFP_KERNEL));

	// cpumask_copy(wq_online_cpumask, cpu_online_mask);
	// cpumask_copy(wq_unbound_cpumask, cpu_possible_mask);
	// restrict_unbound_cpumask("HK_TYPE_WQ", housekeeping_cpumask(HK_TYPE_WQ));
	// restrict_unbound_cpumask("HK_TYPE_DOMAIN", housekeeping_cpumask(HK_TYPE_DOMAIN));
	// if (!cpumask_empty(&wq_cmdline_cpumask))
	// 	restrict_unbound_cpumask("workqueue.unbound_cpus", &wq_cmdline_cpumask);

	// cpumask_copy(wq_requested_unbound_cpumask, wq_unbound_cpumask);

	pwq_cache = KMEM_CACHE(pool_workqueue, SLAB_PANIC);

	// unbound_wq_update_pwq_attrs_buf = alloc_workqueue_attrs();
	// BUG_ON(!unbound_wq_update_pwq_attrs_buf);

	// /*
	//  * If nohz_full is enabled, set power efficient workqueue as unbound.
	//  * This allows workqueue items to be moved to HK CPUs.
	//  */
	// if (housekeeping_enabled(HK_TYPE_TICK))
	// 	wq_power_efficient = true;

	// /* initialize WQ_AFFN_SYSTEM pods */
	// pt->pod_cpus = kcalloc(1, sizeof(pt->pod_cpus[0]), GFP_KERNEL);
	// pt->pod_node = kcalloc(1, sizeof(pt->pod_node[0]), GFP_KERNEL);
	// pt->cpu_pod = kcalloc(nr_cpu_ids, sizeof(pt->cpu_pod[0]), GFP_KERNEL);
	// BUG_ON(!pt->pod_cpus || !pt->pod_node || !pt->cpu_pod);

	// BUG_ON(!zalloc_cpumask_var_node(&pt->pod_cpus[0], GFP_KERNEL, NUMA_NO_NODE));

	// pt->nr_pods = 1;
	// cpumask_copy(pt->pod_cpus[0], cpu_possible_mask);
	// pt->pod_node[0] = NUMA_NO_NODE;
	// pt->cpu_pod[0] = 0;

	/* initialize BH and CPU pools */
	for_each_possible_cpu(cpu) {
		worker_pool_s *pool;

		// i = 0;
		// for_each_bh_worker_pool(pool, cpu) {
		// 	init_cpu_worker_pool(pool, cpu, std_nice[i]);
		// 	pool->flags |= POOL_BH;
		// 	// init_irq_work(bh_pool_irq_work(pool), irq_work_fns[i]);
		// 	i++;
		// }

		i = 0;
		for_each_cpu_worker_pool(pool, cpu)
			init_cpu_worker_pool(pool, cpu, std_nice[i++]);
	}

	// /* create default unbound and ordered wq attrs */
	// for (i = 0; i < NR_STD_WORKER_POOLS; i++) {
	// 	// struct workqueue_attrs *attrs;

	// 	// BUG_ON(!(attrs = alloc_workqueue_attrs()));
	// 	// attrs->nice = std_nice[i];
	// 	// unbound_std_wq_attrs[i] = attrs;

	// 	// /*
	// 	//  * An ordered wq should have only one pwq as ordering is
	// 	//  * guaranteed by max_active which is enforced by pwqs.
	// 	//  */
	// 	// BUG_ON(!(attrs = alloc_workqueue_attrs()));
	// 	// attrs->nice = std_nice[i];
	// 	// attrs->ordered = true;
	// 	// ordered_wq_attrs[i] = attrs;
	// }

	system_wq =
			alloc_workqueue("events", 0, 0);
	// system_highpri_wq =
	// 		alloc_workqueue("events_highpri", WQ_HIGHPRI, 0);
	// system_long_wq =
	// 		alloc_workqueue("events_long", 0, 0);
	// system_unbound_wq =
	// 		alloc_workqueue("events_unbound", WQ_UNBOUND, WQ_MAX_ACTIVE);
	// system_freezable_wq =
	// 		alloc_workqueue("events_freezable", WQ_FREEZABLE, 0);
	// system_power_efficient_wq =
	// 		alloc_workqueue("events_power_efficient", WQ_POWER_EFFICIENT, 0);
	// system_freezable_power_efficient_wq =
	// 		alloc_workqueue("events_freezable_pwr_efficient", WQ_FREEZABLE | WQ_POWER_EFFICIENT, 0);
	// system_bh_wq =
	// 		alloc_workqueue("events_bh", WQ_BH, 0);
	// system_bh_highpri_wq =
	// 		alloc_workqueue("events_bh_highpri", WQ_BH | WQ_HIGHPRI, 0);
	// BUG_ON(
	// 	!system_wq || !system_highpri_wq || !system_long_wq ||
	// 	!system_unbound_wq || !system_freezable_wq ||
	// 	!system_power_efficient_wq ||
	// 	!system_freezable_power_efficient_wq ||
	// 	!system_bh_wq || !system_bh_highpri_wq
	// );
}


/**
 * workqueue_init - bring workqueue subsystem fully online
 *
 * This is the latter half of two-staged workqueue subsystem initialization
 * and invoked as soon as kthreads can be created and scheduled.
 * Workqueues have been created and work items queued on them, but there
 * are no kworkers executing the work items yet.  Populate the worker pools
 * with the initial workers and enable future kworker creations.
 */
void __init workqueue_init(void)
{
	workqueue_s *wq;
	worker_pool_s *pool;
	int cpu, bkt;

	// wq_cpu_intensive_thresh_init();

	// mutex_lock(&wq_pool_mutex);

	// /*
	//  * Per-cpu pools created earlier could be missing node hint. Fix them
	//  * up. Also, create a rescuer for workqueues that requested it.
	//  */
	// for_each_possible_cpu(cpu) {
	// 	for_each_bh_worker_pool(pool, cpu)
	// 		pool->node = cpu_to_node(cpu);
	// 	for_each_cpu_worker_pool(pool, cpu)
	// 		pool->node = cpu_to_node(cpu);
	// }

	// list_for_each_entry(wq, &workqueues, list) {
	// 	WARN(init_rescuer(wq),
	// 	     "workqueue: failed to create early rescuer for %s",
	// 	     wq->name);
	// }

	// mutex_unlock(&wq_pool_mutex);

	// /*
	//  * Create the initial workers. A BH pool has one pseudo worker that
	//  * represents the shared BH execution context and thus doesn't get
	//  * affected by hotplug events. Create the BH pseudo workers for all
	//  * possible CPUs here.
	//  */
	// for_each_possible_cpu(cpu)
	// 	for_each_bh_worker_pool(pool, cpu)
	// 		BUG_ON(!create_worker(pool));

	for_each_online_cpu(cpu) {
		for_each_cpu_worker_pool(pool, cpu) {
			pool->flags &= ~POOL_DISASSOCIATED;
			BUG_ON(!create_worker(pool));
		}
	}

	// hash_for_each(unbound_pool_hash, bkt, pool, hash_node)
	// 	BUG_ON(!create_worker(pool));

	// wq_online = true;
	// wq_watchdog_init();
}