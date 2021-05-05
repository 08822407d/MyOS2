#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <sys/types.h>

#include "arch_proto.h"
#include "interrupt.h"

#include "../../../include/param.h"
		
	struct timer_list;
	typedef struct timer_list timer_list_s;
	typedef struct timer_list
	{
		timer_list_s *	prev;
		timer_list_s *	next;
		unsigned long 	expire_jiffies;
		void 			(* func)(void * data);
		void *			data;
	} timer_list_s;


	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];
	extern timer_list_s timer_list_head;

	/* device.c */
	void devices_init(void);

	/* keyboard.c */
	void keyboard_init(void);
	void keyboard_handler(unsigned long param, stack_frame_s * sf_regs);
	void analysis_keycode(void);

	/* timer.c, clock.c */
	void get_cmos_time(time_s *time);
	void init_timer(timer_list_s * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies);
	void add_timer(timer_list_s * timer);
	void del_timer(timer_list_s * timer);
	void timer_init(void);
	void do_timer(void * data);
	void HPET_init(void);

#endif /* _AMD64_PC_DEVICE_H_ */