#ifndef _GLO_H_
#define _GLO_H_

#include "param.h"

	extern unsigned long volatile	jiffies;

	extern kinfo_s 			kparam;
	extern framebuffer_s	framebuffer;
	extern percpu_data_s **	smp_info;

	extern kbd_inbuf_s *	p_kb;
	extern time_s 			time;

	extern task_queue_s	idle_queue;
	extern task_list_s global_ready_task;
	extern task_list_s global_blocked_task;

#endif /* _GLO_H_ */