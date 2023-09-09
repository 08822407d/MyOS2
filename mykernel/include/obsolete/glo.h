#ifndef _GLO_H_
#define _GLO_H_

#include <linux/kernel/sched.h>

#include <obsolete/ktypes.h>

	extern unsigned long volatile	jiffies;

	extern PCB_u			idletsk;
	extern unsigned			nr_lcpu;

	extern kbd_inbuf_s		*p_kb;
	extern time_s 			time;

#endif /* _GLO_H_ */