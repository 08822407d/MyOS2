#ifndef _GLO_H_
#define _GLO_H_

#include <sys/param.h>

	extern unsigned long volatile	jiffies;

	extern PCB_u			task0_PCB;
	extern kinfo_s 			kparam;
	extern framebuffer_s	framebuffer;
	extern PCB_u **			idle_tasks;
	extern cpudata_u **		percpu_data;

	extern kbd_inbuf_s *	p_kb;
	extern time_s 			time;

	extern size_t			cpustack_off;
	extern task_queue_s		idle_queue;

#endif /* _GLO_H_ */