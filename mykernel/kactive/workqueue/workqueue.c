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
	// struct workqueue_struct *wq;
	// struct worker_pool *pool;
	// int cpu, bkt;

	// /*
	//  * It'd be simpler to initialize NUMA in workqueue_init_early() but
	//  * CPU to node mapping may not be available that early on some
	//  * archs such as power and arm64.  As per-cpu pools created
	//  * previously could be missing node hint and unbound pools NUMA
	//  * affinity, fix them up.
	//  *
	//  * Also, while iterating workqueues, create rescuers if requested.
	//  */
	// wq_numa_init();

	// mutex_lock(&wq_pool_mutex);

	// for_each_possible_cpu(cpu) {
	// 	for_each_cpu_worker_pool(pool, cpu) {
	// 		pool->node = cpu_to_node(cpu);
	// 	}
	// }

	// list_for_each_entry(wq, &workqueues, list) {
	// 	wq_update_unbound_numa(wq, smp_processor_id(), true);
	// 	WARN(init_rescuer(wq),
	// 	     "workqueue: failed to create early rescuer for %s",
	// 	     wq->name);
	// }

	// mutex_unlock(&wq_pool_mutex);

	// /* create the initial workers */
	// for_each_online_cpu(cpu) {
	// 	for_each_cpu_worker_pool(pool, cpu) {
	// 		pool->flags &= ~POOL_DISASSOCIATED;
	// 		BUG_ON(!create_worker(pool));
	// 	}
	// }

	// hash_for_each(unbound_pool_hash, bkt, pool, hash_node)
	// 	BUG_ON(!create_worker(pool));

	// wq_online = true;
	// wq_watchdog_init();
}