#ifndef _SWAIT_TYPES_H_
#define _SWAIT_TYPES_H_

	#include "../kactive_type_declaration.h"


	struct swait_queue_head {
		spinlock_t	lock;
		List_hdr_s	task_list_hdr;
	};

	struct swait_queue {
		task_s		*task;
		List_s		task_list;
	};

#endif /* _SWAIT_TYPES_H_ */