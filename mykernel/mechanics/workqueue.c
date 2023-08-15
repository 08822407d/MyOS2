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

// #include <linux/export.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/sched.h>
#include <linux/init/init.h>
#include <linux/kernel/signal.h>
#include <linux/kernel/completion.h>
#include <linux/kernel/workqueue.h>
#include <linux/kernel/slab.h>
#include <linux/kernel/cpu.h>
// #include <linux/notifier.h>
#include <linux/kernel/kthread.h>
// #include <linux/hardirq.h>
// #include <linux/mempolicy.h>
// #include <linux/freezer.h>
// #include <linux/debug_locks.h>
// #include <linux/lockdep.h>
// #include <linux/idr.h>
// #include <linux/jhash.h>
// #include <linux/hashtable.h>
// #include <linux/rculist.h>
// #include <linux/nodemask.h>
// #include <linux/moduleparam.h>
#include <linux/kernel/uaccess.h>
// #include <linux/sched/isolation.h>
#include <linux/kernel/nmi.h>
// #include <linux/kvm_para.h>

// #include "workqueue_internal.h"

// enum {
// 	/*
// 	 * worker_pool flags
// 	 *
// 	 * A bound pool is either associated or disassociated with its CPU.
// 	 * While associated (!DISASSOCIATED), all workers are bound to the
// 	 * CPU and none has %WORKER_UNBOUND set and concurrency management
// 	 * is in effect.
// 	 *
// 	 * While DISASSOCIATED, the cpu may be offline and all workers have
// 	 * %WORKER_UNBOUND set and concurrency management disabled, and may
// 	 * be executing on any CPU.  The pool behaves as an unbound one.
// 	 *
// 	 * Note that DISASSOCIATED should be flipped only while holding
// 	 * wq_pool_attach_mutex to avoid changing binding state while
// 	 * worker_attach_to_pool() is in progress.
// 	 */
// 	POOL_MANAGER_ACTIVE		= 1 << 0,	/* being managed */
// 	POOL_DISASSOCIATED		= 1 << 2,	/* cpu can't serve workers */

// 	/* worker flags */
// 	WORKER_DIE				= 1 << 1,	/* die die die */
// 	WORKER_IDLE				= 1 << 2,	/* is idle */
// 	WORKER_PREP				= 1 << 3,	/* preparing to run works */
// 	WORKER_CPU_INTENSIVE	= 1 << 6,	/* cpu intensive */
// 	WORKER_UNBOUND			= 1 << 7,	/* worker is unbound */
// 	WORKER_REBOUND			= 1 << 8,	/* worker was rebound */

// 	WORKER_NOT_RUNNING		= WORKER_PREP | WORKER_CPU_INTENSIVE |
// 				  WORKER_UNBOUND | WORKER_REBOUND,

// 	NR_STD_WORKER_POOLS		= 2,		/* # standard pools per cpu */

// 	UNBOUND_POOL_HASH_ORDER	= 6,		/* hashed by pool->attrs */
// 	BUSY_WORKER_HASH_ORDER	= 6,		/* 64 pointers */

// 	MAX_IDLE_WORKERS_RATIO	= 4,		/* 1/4 of busy can be idle */
// 	IDLE_WORKER_TIMEOUT		= 300 * HZ,	/* keep idle ones for 5 mins */

// 	MAYDAY_INITIAL_TIMEOUT  = HZ / 100 >= 2 ? HZ / 100 : 2,
// 						/* call for help after 10ms
// 						   (min two ticks) */
// 	MAYDAY_INTERVAL			= HZ / 10,	/* and then every 100ms */
// 	CREATE_COOLDOWN			= HZ,		/* time to breath after fail */

// 	/*
// 	 * Rescue workers are used only on emergencies and shared by
// 	 * all cpus.  Give MIN_NICE.
// 	 */
// 	RESCUER_NICE_LEVEL		= MIN_NICE,
// 	HIGHPRI_NICE_LEVEL		= MIN_NICE,

// 	WQ_NAME_LEN				= 24,
// };


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