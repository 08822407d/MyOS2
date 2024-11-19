#ifndef _WORKQUEUE_API_H_
#define _WORKQUEUE_API_H_

	#include "workqueue.h"


	// extern worker_pool_s bh_worker_pools[NR_STD_WORKER_POOLS];
	extern worker_pool_s cpu_worker_pools[NR_STD_WORKER_POOLS];

	void __init workqueue_init_early(void);
	void __init workqueue_init(void);

#endif /* _WORKQUEUE_API_H_ */