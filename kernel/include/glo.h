#ifndef _GLO_H_
#define _GLO_H_

#include "param.h"

	extern unsigned long volatile	jiffies;

	extern kinfo_s 			kparam;
	extern framebuffer_s	framebuffer;
	extern percpu_data_s **	smp_info;

	extern kbd_inbuf_s *	p_kb;
	extern time_s 			time;

	extern idle_task_queue_s	idle_queue;
	extern task_s *			task_waiting_list;
	extern unsigned long	waiting_task_count;

#endif /* _GLO_H_ */