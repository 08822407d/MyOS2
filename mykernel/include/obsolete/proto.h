#ifndef _PROTO_H_
#define _PROTO_H_

#include <linux/kernel/types.h>
#include <linux/mm/mmzone.h>

#include "ktypes.h"
		
	struct timer;
	typedef struct timer timer_s;	

	typedef struct timer
	{
		List_s			tmr_list;
		unsigned long 	expire_jiffies;
		void 			(* func)(void * data);
		void *			data;
	} timer_s;

	typedef struct softirq
	{
		void (*action)(void * data);
		void * data;
	} softirq_s;

	extern List_hdr_s	timer_lhdr;

	/* memory.c */
	page_s * paddr_to_page(phys_addr_t paddr);

	/* video.c */
	void myos_init_video(void);

	/* softirq.c */
	void register_softirq(unsigned sirq, void (*action)(void * data), void * data);
	void unregister_softirq(unsigned sirq);
	void set_softirq_status(unsigned sirq);
	uint64_t get_softirq_status(void);
	void myos_softirq_init(void);

	/* timer.c */
	void init_timer(timer_s * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies);
	void add_timer(timer_s * timer);
	void del_timer(timer_s * timer);
	void myos_timer_init(void);
	void do_timer(void * data);


#endif /* _PROTO_H_ */