#ifndef _WORKQUEUE_MACRO_H_
#define _WORKQUEUE_MACRO_H_


	// /*
	//  * The first word is the work queue pointer and the flags rolled into
	//  * one
	//  */
	// #define work_data_bits(work) ((unsigned long *)(&(work)->data))

	// #define for_each_bh_worker_pool(pool, cpu)										\
	// 		for ((pool) = &per_cpu(bh_worker_pools, cpu)[0];						\
	// 				(pool) < &per_cpu(bh_worker_pools, cpu)[NR_STD_WORKER_POOLS];	\
	// 				(pool)++)

	#define for_each_cpu_worker_pool(pool, cpu)					\
			for ((pool) = &per_cpu(cpu_worker_pools, cpu)[0];	\
					(pool) < &per_cpu(cpu_worker_pools,			\
								cpu)[NR_STD_WORKER_POOLS];		\
					(pool)++)

	// /**
	//  * for_each_pool - iterate through all worker_pools in the system
	//  * @pool: iteration cursor
	//  * @pi: integer used for iteration
	//  *
	//  * This must be called either with wq_pool_mutex held or RCU read
	//  * locked.  If the pool needs to be used beyond the locking in effect, the
	//  * caller is responsible for guaranteeing that the pool stays online.
	//  *
	//  * The if/else clause exists only for the lockdep assertion and can be
	//  * ignored.
	//  */
	// #define for_each_pool(pool, pi)						\
	// 	idr_for_each_entry(&worker_pool_idr, pool, pi)			\
	// 		if (({ assert_rcu_or_pool_mutex(); false; })) { }	\
	// 		else

	// /**
	//  * for_each_pool_worker - iterate through all workers of a worker_pool
	//  * @worker: iteration cursor
	//  * @pool: worker_pool to iterate workers of
	//  *
	//  * This must be called with wq_pool_attach_mutex.
	//  *
	//  * The if/else clause exists only for the lockdep assertion and can be
	//  * ignored.
	//  */
	// #define for_each_pool_worker(worker, pool)				\
	// 	list_for_each_entry((worker), &(pool)->workers, node)		\
	// 		if (({ lockdep_assert_held(&wq_pool_attach_mutex); false; })) { } \
	// 		else

	// /**
	//  * for_each_pwq - iterate through all pool_workqueues of the specified workqueue
	//  * @pwq: iteration cursor
	//  * @wq: the target workqueue
	//  *
	//  * This must be called either with wq->mutex held or RCU read locked.
	//  * If the pwq needs to be used beyond the locking in effect, the caller is
	//  * responsible for guaranteeing that the pwq stays online.
	//  *
	//  * The if/else clause exists only for the lockdep assertion and can be
	//  * ignored.
	//  */
	// #define for_each_pwq(pwq, wq)						\
	// 	list_for_each_entry_rcu((pwq), &(wq)->pwqs, pwqs_node,		\
	// 				 lockdep_is_held(&(wq->mutex)))

#endif /* _WORKQUEUE_MACRO_H_ */