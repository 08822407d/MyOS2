#ifndef _GLO_H_
#define _GLO_H_

#include <linux/kernel/sched_types.h>
#include <obsolete/ktypes.h>

	extern unsigned long volatile	jiffies;

	extern task_s		idles;
	extern task_s		*idle_threads;
	extern unsigned		nr_cpu_ids;

	extern kbd_inbuf_s	*p_kb;
	extern time_s 		time;

#endif /* _GLO_H_ */