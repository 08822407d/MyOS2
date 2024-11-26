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
workqueue_s *system_bh_wq;
EXPORT_SYMBOL_GPL(system_bh_wq);
// workqueue_s *system_bh_highpri_wq;
// EXPORT_SYMBOL_GPL(system_bh_highpri_wq);

static int simple_worker_thread(void *__worker);



// static void __queue_work(int cpu, struct workqueue_struct *wq,
// 		struct work_struct *work)
static void
__simple_queue_work(int cpu, workqueue_s *wq, work_s*work) {
	pool_workqueue_s *pwq;
	worker_pool_s *pool;

retry:
	/* pwq which will be used unless @work is executing elsewhere */
	if (cpu == WORK_CPU_UNBOUND) {
		// if (wq->flags & WQ_UNBOUND)
		// 	cpu = wq_select_unbound_cpu(raw_smp_processor_id());
		// else
			cpu = raw_smp_processor_id();
	}

	pwq = *per_cpu_ptr(wq->cpu_pwq, cpu);
	pool = pwq->pool;

	spin_lock(&pool->lock);

	if (WARN_ON(!list_empty(&work->entry)))
		goto out;

	list_header_add_to_tail(&pool->worklist, &work->entry);
	// 	kick_pool(pool);

out:
	spin_unlock_no_resched(&pool->lock);
	// rcu_read_unlock();
}



/**
 * queue_work_on - queue work on specific cpu
 * @cpu: CPU number to execute work on
 * @wq: workqueue to use
 * @work: work to queue
 *
 * We queue the work to a specific CPU, the caller must ensure it
 * can't go away.  Callers that fail to ensure that the specified
 * CPU cannot go away will execute on a randomly chosen CPU.
 * But note well that callers specifying a CPU that never has been
 * online will get a splat.
 *
 * Return: %false if @work was already on a queue, %true otherwise.
 */
bool queue_work_on(int cpu, workqueue_s*wq, work_s*work)
{
	bool ret = false;
	ulong irq_flags;

	local_irq_save(irq_flags);

	// if (!test_and_set_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work)) &&
	// 		!clear_pending_if_disabled(work)) {
		__simple_queue_work(cpu, wq, work);
		ret = true;
	// }

	local_irq_restore(irq_flags);
	return ret;
}
EXPORT_SYMBOL(queue_work_on);




static worker_s *alloc_worker(int node) {
	worker_s *worker;

	worker = kzalloc_node(sizeof(*worker), GFP_KERNEL, 0);
	if (worker) {
		INIT_LIST_HEAD(&worker->entry);
		INIT_LIST_HEADER_S(&worker->scheduled);
		INIT_LIST_HEAD(&worker->node);
		/* on creation a worker is in !idle && prep state */
		worker->flags = WORKER_PREP;
	}
	return worker;
}

/**
 * worker_attach_to_pool() - attach a worker to a pool
 * @worker: worker to be attached
 * @pool: the target pool
 *
 * Attach @worker to @pool.  Once attached, the %WORKER_UNBOUND flag and
 * cpu-binding of @worker are kept coordinated with the pool across
 * cpu-[un]hotplugs.
 */
static void
worker_attach_to_pool(worker_s *worker, worker_pool_s *pool) {
	// mutex_lock(&wq_pool_attach_mutex);

	/*
	 * The wq_pool_attach_mutex ensures %POOL_DISASSOCIATED remains stable
	 * across this function. See the comments above the flag definition for
	 * details. BH workers are, while per-CPU, always DISASSOCIATED.
	 */
	if (pool->flags & POOL_DISASSOCIATED) {
		worker->flags |= WORKER_UNBOUND;
	} else {
		WARN_ON_ONCE(pool->flags & POOL_BH);
		// kthread_set_per_cpu(worker->task, pool->cpu);
	}

	// if (worker->rescue_wq)
	// 	set_cpus_allowed_ptr(worker->task, pool_allowed_cpus(pool));

	// list_add_tail(&worker->node, &pool->workers);
	list_header_add_to_tail(&pool->workers, &worker->node);
	worker->pool = pool;

	// mutex_unlock(&wq_pool_attach_mutex);
}


/**
 * assign_work - assign a work item and its linked work items to a worker
 * @work: work to assign
 * @worker: worker to assign to
 * @nextp: out parameter for nested worklist walking
 *
 * Assign @work and its linked work items to @worker. If @work is already being
 * executed by another worker in the same pool, it'll be punted there.
 *
 * If @nextp is not NULL, it's updated to point to the next work of the last
 * scheduled work. This allows assign_work() to be nested inside
 * list_for_each_entry_safe().
 *
 * Returns %true if @work was successfully assigned to @worker. %false if @work
 * was punted to another worker already executing it.
 */
// static bool assign_work(struct work_struct *work,
// 		struct worker *worker, struct work_struct **nextp)
static bool
simple_assign_work(work_s*work, worker_s *worker) {
	worker_pool_s *pool = worker->pool;

	list_header_delete_node(&pool->worklist, &work->entry);
	list_header_add_to_tail(&worker->scheduled, &work->entry);

	return true;
}

/**
 * process_one_work - process single work
 * @worker: self
 * @work: work to process
 *
 * Process @work.  This function contains all the logics necessary to
 * process a single work including synchronization against and
 * interaction with other workers on the same cpu, queueing and
 * flushing.  As long as context requirement is met, any worker can
 * call this function to process a work.
 *
 * CONTEXT:
 * raw_spin_lock_irq(pool->lock) which is released and regrabbed.
 */
static void
process_one_work(worker_s *worker, work_s *work)
__releases(&pool->lock)
__acquires(&pool->lock)
{
	pool_workqueue_s *pwq = get_work_pwq(work);
	worker_pool_s *pool = worker->pool;
	ulong work_data;
	// int lockdep_start_depth, rcu_start_depth;
	// bool bh_draining = pool->flags & POOL_BH_DRAINING;
// #ifdef CONFIG_LOCKDEP
// 	/*
// 	 * It is permissible to free the struct work_struct from
// 	 * inside the function that is called from it, this we need to
// 	 * take into account for lockdep too.  To avoid bogus "held
// 	 * lock freed" warnings as well as problems when looking into
// 	 * work->lockdep_map, make a copy and use that here.
// 	 */
// 	struct lockdep_map lockdep_map;

// 	lockdep_copy_map(&lockdep_map, &work->lockdep_map);
// #endif
	/* ensure we're on the correct CPU */
	WARN_ON_ONCE(!(pool->flags & POOL_DISASSOCIATED) &&
			raw_smp_processor_id() != pool->cpu);

	/* claim and dequeue */
	// debug_work_deactivate(work);
	// hash_add(pool->busy_hash, &worker->hentry, (unsigned long)work);
	worker->current_work = work;
	worker->current_func = work->func;
	worker->current_pwq = pwq;
	// if (worker->task)
	// 	worker->current_at = worker->task->se.sum_exec_runtime;
	// work_data = *work_data_bits(work);
	// worker->current_color = get_work_color(work_data);

	/*
	 * Record wq name for cmdline and debug reporting, may get
	 * overridden through set_worker_desc().
	 */
	strscpy(worker->desc, pwq->wq->name, WORKER_DESC_LEN);

	// list_del_init(&work->entry);
	list_header_delete_node(&worker->scheduled, &work->entry);

	/*
	 * CPU intensive works don't participate in concurrency management.
	 * They're the scheduler's responsibility.  This takes @worker out
	 * of concurrency management and the next code block will chain
	 * execution of the pending work items.
	 */
	if (unlikely(pwq->wq->flags & WQ_CPU_INTENSIVE))
		worker_set_flags(worker, WORKER_CPU_INTENSIVE);

	// /*
	//  * Kick @pool if necessary. It's always noop for per-cpu worker pools
	//  * since nr_running would always be >= 1 at this point. This is used to
	//  * chain execution of the pending work items for WORKER_NOT_RUNNING
	//  * workers such as the UNBOUND and CPU_INTENSIVE ones.
	//  */
	// kick_pool(pool);

	// /*
	//  * Record the last pool and clear PENDING which should be the last
	//  * update to @work.  Also, do this inside @pool->lock so that
	//  * PENDING and queued state changes happen together while IRQ is
	//  * disabled.
	//  */
	// set_work_pool_and_clear_pending(work, pool->id, pool_offq_flags(pool));

	// pwq->stats[PWQ_STAT_STARTED]++;
	// raw_spin_unlock_irq(&pool->lock);
	spin_unlock_no_resched(&pool->lock);

	// rcu_start_depth = rcu_preempt_depth();
	// lockdep_start_depth = lockdep_depth(current);
	// /* see drain_dead_softirq_workfn() */
	// if (!bh_draining)
	// 	lock_map_acquire(&pwq->wq->lockdep_map);
	// lock_map_acquire(&lockdep_map);
	// /*
	//  * Strictly speaking we should mark the invariant state without holding
	//  * any locks, that is, before these two lock_map_acquire()'s.
	//  *
	//  * However, that would result in:
	//  *
	//  *   A(W1)
	//  *   WFC(C)
	//  *		A(W1)
	//  *		C(C)
	//  *
	//  * Which would create W1->C->W1 dependencies, even though there is no
	//  * actual deadlock possible. There are two solutions, using a
	//  * read-recursive acquire on the work(queue) 'locks', but this will then
	//  * hit the lockdep limitation on recursive locks, or simply discard
	//  * these locks.
	//  *
	//  * AFAICT there is no possible deadlock scenario between the
	//  * flush_work() and complete() primitives (except for single-threaded
	//  * workqueues), so hiding them isn't a problem.
	//  */
	// lockdep_invariant_state(true);
	// trace_workqueue_execute_start(work);
	worker->current_func(work);
	// /*
	//  * While we must be careful to not use "work" after this, the trace
	//  * point will only record its address.
	//  */
	// trace_workqueue_execute_end(work, worker->current_func);
	// pwq->stats[PWQ_STAT_COMPLETED]++;
	// lock_map_release(&lockdep_map);
	// if (!bh_draining)
	// 	lock_map_release(&pwq->wq->lockdep_map);

	// if (unlikely((worker->task && in_atomic()) ||
	// 	     lockdep_depth(current) != lockdep_start_depth ||
	// 	     rcu_preempt_depth() != rcu_start_depth)) {
	// 	pr_err("BUG: workqueue leaked atomic, lock or RCU: %s[%d]\n"
	// 	       "     preempt=0x%08x lock=%d->%d RCU=%d->%d workfn=%ps\n",
	// 	       current->comm, task_pid_nr(current), preempt_count(),
	// 	       lockdep_start_depth, lockdep_depth(current),
	// 	       rcu_start_depth, rcu_preempt_depth(),
	// 	       worker->current_func);
	// 	debug_show_held_locks(current);
	// 	dump_stack();
	// }

	// /*
	//  * The following prevents a kworker from hogging CPU on !PREEMPTION
	//  * kernels, where a requeueing work item waiting for something to
	//  * happen could deadlock with stop_machine as such work item could
	//  * indefinitely requeue itself while all other CPUs are trapped in
	//  * stop_machine. At the same time, report a quiescent RCU state so
	//  * the same condition doesn't freeze RCU.
	//  */
	// if (worker->task)
	// 	cond_resched();

	spin_lock_irq(&pool->lock);

	/*
	 * In addition to %WQ_CPU_INTENSIVE, @worker may also have been marked
	 * CPU intensive by wq_worker_tick() if @work hogged CPU longer than
	 * wq_cpu_intensive_thresh_us. Clear it.
	 */
	worker_clr_flags(worker, WORKER_CPU_INTENSIVE);

	// /* tag the worker for identification in schedule() */
	// worker->last_func = worker->current_func;

	/* we're done with it, release */
	// hash_del(&worker->hentry);
	worker->current_work = NULL;
	worker->current_func = NULL;
	worker->current_pwq = NULL;
	// worker->current_color = INT_MAX;

	// /* must be the last step, see the function comment */
	// pwq_dec_nr_in_flight(pwq, work_data);
}

/**
 * process_scheduled_works - process scheduled works
 * @worker: self
 *
 * Process all scheduled works.  Please note that the scheduled list
 * may change while processing a work, so this function repeatedly
 * fetches a work from the top and executes it.
 *
 * CONTEXT:
 * raw_spin_lock_irq(pool->lock) which may be released and regrabbed
 * multiple times.
 */
static void
process_scheduled_works(worker_s *worker) {
	work_s *work;
	bool first = true;

	while ((work = list_headr_first_container(
			&worker->scheduled, work_s, entry))) {
		// if (first) {
		// 	worker->pool->watchdog_ts = jiffies;
		// 	first = false;
		// }
		process_one_work(worker, work);
	}
}

/**
 * worker_thread - the worker thread function
 * @__worker: self
 *
 * The worker thread function.  All workers belong to a worker_pool -
 * either a per-cpu one or dynamic unbound one.  These workers process all
 * work items regardless of their specific target workqueue.  The only
 * exception is work items which belong to workqueues with a rescuer which
 * will be explained in rescuer_thread().
 *
 * Return: 0
 */
static int
simple_worker_thread(void *__worker) {
	worker_s *worker = __worker;
	worker_pool_s *pool = worker->pool;

	/* tell the scheduler that this is a workqueue worker */
	// set_pf_worker(true);
woke_up:
	spin_lock_irq(&pool->lock);

	worker_leave_idle(worker);
	/*
	 * ->scheduled list can only be filled while a worker is
	 * preparing to process a work or actually processing it.
	 * Make sure nobody diddled with it while I was sleeping.
	 */
	WARN_ON_ONCE(!list_header_is_empty(&worker->scheduled));

	/*
	 * Finish PREP stage.  We're guaranteed to have at least one idle
	 * worker or that someone else has already assumed the manager
	 * role.  This is where @worker starts participating in concurrency
	 * management if applicable and concurrency management is restored
	 * after being rebound.  See rebind_workers() for details.
	 */
	worker_clr_flags(worker, WORKER_PREP | WORKER_REBOUND);

	while (keep_working(pool)) {
		work_s *work = list_headr_first_container(
						&pool->worklist, work_s, entry);
		if (simple_assign_work(work, worker))
			process_scheduled_works(worker);
	};

	worker_set_flags(worker, WORKER_PREP);
sleep:
	/*
	 * pool->lock is held and there's no work to process and no need to
	 * manage, sleep.  Workers are woken up only while holding
	 * pool->lock or from local cpu, so setting the current state
	 * before releasing pool->lock is enough to prevent losing any
	 * event.
	 */
	worker_enter_idle(worker);
	__set_current_state(TASK_IDLE);
	spin_unlock_irq_no_resched(&pool->lock);
	schedule();
	goto woke_up;
}

static int
format_worker_id(char *buf, size_t size,
		worker_s *worker, worker_pool_s *pool) {

	if (pool) {
		if (pool->cpu >= 0)
			return scnprintf(buf, size, "kworker/%d:%d%s",
						pool->cpu, worker->id,
						pool->attrs->nice < 0  ? "H" : "");
		else
			return scnprintf(buf, size, "kworker/u%d:%d",
						pool->id, worker->id);
	} else {
		return scnprintf(buf, size, "kworker/dying");
	}
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

	worker = alloc_worker(pool->node);
	if (!worker) {
		pr_err_once("workqueue: Failed to allocate a worker\n");
		goto fail;
	}

	// worker->id = id;

	if (!(pool->flags & POOL_BH)) {
		char id_buf[WORKER_ID_LEN];

		format_worker_id(id_buf, sizeof(id_buf), worker, pool);
		worker->task = kthread_create_on_node(simple_worker_thread,
							worker, pool->node, "%s", id_buf);
		if (IS_ERR(worker->task)) {
			if (PTR_ERR(worker->task) == -EINTR) {
				pr_err("workqueue: Interrupted when creating "
					"a worker thread \"%s\"\n", id_buf);
			} else {
				pr_err_once("workqueue: Failed to create a "
					"worker thread: %pe", worker->task);
			}
			goto fail;
		}

		set_user_nice(worker->task, pool->attrs->nice);
		// kthread_bind_mask(worker->task, pool_allowed_cpus(pool));
	}

	/* successful, attach the worker to the pool */
	worker_attach_to_pool(worker, pool);

	/* start the newly created worker */
	spin_lock_irq(&pool->lock);

	worker->pool->nr_workers++;
	worker_enter_idle(worker);

	/*
	 * @worker is waiting on a completion in kthread() and will trigger hung
	 * check if not woken up soon. As kick_pool() is noop if @pool is empty,
	 * wake it up explicitly.
	 */
	if (worker->task)
		wake_up_process(worker->task);
	spin_unlock_irq_no_resched(&pool->lock);

	return worker;

fail:
	// ida_free(&pool->worker_ida, id);
	kfree(worker);
	return NULL;
}


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
	pool->node = NUMA_NO_NODE;
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
	pool->attrs = kzalloc(sizeof(*(pool->attrs)), GFP_KERNEL);
	if (!pool->attrs)
		return -ENOMEM;

	// wqattrs_clear_for_pool(pool->attrs);

	return 0;
}



static int
simple_alloc_and_link_pwqs(workqueue_s *wq) {
	bool highpri = wq->flags & WQ_HIGHPRI;
	int cpu, ret;

	wq->cpu_pwq = alloc_percpu(pool_workqueue_s *);
	if (!wq->cpu_pwq)
		goto enomem;

	if (!(wq->flags & WQ_UNBOUND)) {
		worker_pool_s __percpu *pools;
		if (wq->flags & WQ_BH)
			pools = bh_worker_pools;
		else
			pools = cpu_worker_pools;

		for_each_possible_cpu(cpu) {
			pool_workqueue_s **pwq_p;
			pool_workqueue_s *pwq;
			worker_pool_s *pool;

			pool = &(per_cpu_ptr(pools, cpu)[highpri]);
			pwq_p = per_cpu_ptr(wq->cpu_pwq, cpu);

			*pwq_p = kmem_cache_alloc(pwq_cache, GFP_KERNEL);
			if (!*pwq_p)
				goto enomem;

			pwq = *pwq_p;
			pwq->pool = pool;
			pwq->wq = wq;

			// mutex_lock(&wq->mutex);
			list_header_add_to_tail(&wq->pwqs, &pwq->pwqs_node);
			// mutex_unlock(&wq->mutex);
		}
		return 0;
	}

	return ret;
enomem:
	if (wq->cpu_pwq) {
		for_each_possible_cpu(cpu) {
			pool_workqueue_s *pwq = *per_cpu_ptr(wq->cpu_pwq, cpu);
			if (pwq)
				kmem_cache_free(pwq_cache, pwq);
		}
		// free_percpu(wq->cpu_pwq);
		wq->cpu_pwq = NULL;
	}
	return -ENOMEM;
}

__printf(1, 4)
workqueue_s
*simple_alloc_workqueue(const char *fmt, uint flags, int max_active, ...)
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

	/* allocate wq and format name */
	// if (flags & WQ_UNBOUND)
	// 	wq_size = struct_size(wq, node_nr_active, nr_node_ids + 1);
	// else
		wq_size = sizeof(*wq);

	wq = kzalloc(wq_size, GFP_KERNEL);
	if (!wq)
		return NULL;

	va_start(args, max_active);
	name_len = vsnprintf(wq->name, sizeof(wq->name), fmt, args);
	va_end(args);

	if (name_len >= WQ_NAME_LEN)
		pr_warn_once("workqueue: name exceeds WQ_NAME_LEN."
				" Truncating to: %s\n", wq->name);

	/* init wq */
	wq->flags = flags;
	INIT_LIST_HEADER_S(&wq->pwqs);
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

	// if (alloc_and_link_pwqs(wq) < 0)
	// 	goto err_unlock_free_node_nr_active;
	simple_alloc_and_link_pwqs(wq);

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
}
EXPORT_SYMBOL_GPL(alloc_workqueue);


static void __init
init_cpu_worker_pool(worker_pool_s *pool, int cpu, int nice) {
	BUG_ON(init_worker_pool(pool));
	pool->cpu = cpu;
	// cpumask_copy(pool->attrs->cpumask, cpumask_of(cpu));
	// cpumask_copy(pool->attrs->__pod_cpumask, cpumask_of(cpu));
	pool->attrs->nice = nice;
	// pool->attrs->affn_strict = true;
	pool->node = 0;

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
	int std_nice[NR_STD_WORKER_POOLS] = { 0, HIGHPRI_NICE_LEVEL };
	int i, cpu;

	pwq_cache = KMEM_CACHE(pool_workqueue, SLAB_PANIC);

	/* initialize BH and CPU pools */
	for_each_possible_cpu(cpu) {
		worker_pool_s *pool;

		i = 0;
		for_each_bh_worker_pool(pool, cpu) {
			init_cpu_worker_pool(pool, cpu, std_nice[i]);
			pool->flags |= POOL_BH;
			// init_irq_work(bh_pool_irq_work(pool), irq_work_fns[i]);
			i++;
		}

		i = 0;
		for_each_cpu_worker_pool(pool, cpu) {
			init_cpu_worker_pool(pool, cpu, std_nice[i]);
			i++;
		}
	}

	system_wq =
			simple_alloc_workqueue("events", 0, 0);
	// system_highpri_wq =
	// 		alloc_workqueue("events_highpri", WQ_HIGHPRI, 0);
	// system_long_wq =
	// 		alloc_workalloc_workqueuequeue("events_power_efficient", WQ_POWER_EFFICIENT, 0);
	// system_freezable_power_efficient_wq =
	// 		alloc_workqueue("events_freezable_pwr_efficient", WQ_FREEZABLE | WQ_POWER_EFFICIENT, 0);
	system_bh_wq =
			simple_alloc_workqueue("events_bh", WQ_BH, 0);
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

	// mutex_lock(&wq_pool_mutex);

	/*
	 * Per-cpu pools created earlier could be missing node hint. Fix them
	 * up. Also, create a rescuer for workqueues that requested it.
	 */
	for_each_possible_cpu(cpu) {
		for_each_bh_worker_pool(pool, cpu)
			pool->node = 0;
		for_each_cpu_worker_pool(pool, cpu)
			pool->node = 0;
	}

	// list_for_each_entry(wq, &workqueues, list) {
	// 	WARN(init_rescuer(wq),
	// 	     "workqueue: failed to create early rescuer for %s",
	// 	     wq->name);
	// }

	// mutex_unlock(&wq_pool_mutex);

	/*
	 * Create the initial workers. A BH pool has one pseudo worker that
	 * represents the shared BH execution context and thus doesn't get
	 * affected by hotplug events. Create the BH pseudo workers for all
	 * possible CPUs here.
	 */
	for_each_possible_cpu(cpu)
		for_each_bh_worker_pool(pool, cpu)
			BUG_ON(!create_worker(pool));

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
