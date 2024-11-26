#ifndef _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_
#define _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>
	#include <linux/kernel/lib_type_declaration.h>
	#include <linux/kernel/lock_ipc_type_declaration.h>


	/* workqueue */
	struct work_struct;
	typedef struct work_struct work_s;
    typedef void (*work_func_t)(work_s *work);
	struct worker;
	typedef struct worker worker_s;
	struct workqueue_attrs;
	typedef struct workqueue_attrs wq_attrs_s;
	struct worker_pool;
	typedef struct worker_pool worker_pool_s;
	struct pool_workqueue;
	typedef struct pool_workqueue pool_workqueue_s;
	struct workqueue_struct;
	typedef struct workqueue_struct workqueue_s;

#endif /* _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_ */