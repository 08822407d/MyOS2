#ifndef _GLO_H_
#define _GLO_H_

#include "param.h"

	extern kinfo_s kparam;
	extern framebuffer_s framebuffer;
	extern percpu_data_s ** smp_info;
	extern kbd_inbuf_s * p_kb;
	extern time_s time;
	extern unsigned long volatile jiffies;
	extern proc_s * proc_waiting_list;


#endif /* _GLO_H_ */