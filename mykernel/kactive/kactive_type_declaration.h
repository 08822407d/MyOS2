#ifndef _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_
#define _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_

	#include <linux/kernel/types.h>
	#include <linux/kernel/lib_type_declaration.h>
	#include <linux/kernel/lock_ipc_type_declaration.h>


	/* workqueue */
	struct work_struct;
	typedef struct work_struct work_s;
    typedef void (*work_func_t)(work_s *work);

#endif /* _KERNEL_ACTIVE_TYPE_DECLARATIONS_H_ */