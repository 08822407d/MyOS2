#ifndef _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_
#define _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>


	/* swait */
	struct swait_queue_head;
	typedef struct swait_queue_head swqueue_hdr_s;
	struct swait_queue;
	typedef struct swait_queue swqueue_s;

	/* completion */
	struct completion;
	typedef struct completion completion_s;

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


	#include <linux/kernel/lib_type_declaration.h>
	#include <linux/kernel/lock_ipc_type_declaration.h>

#endif /* _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_ */