/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RUNQUEUE_API_H_
#define _LINUX_RUNQUEUE_API_H_

	#include <linux/kernel/linkage.h>
	#include "runqueue.h"


	extern void set_user_nice(task_s *p, long nice);

	extern void update_rq_clock(rq_s *rq);

#endif /* _LINUX_RUNQUEUE_API_H_ */