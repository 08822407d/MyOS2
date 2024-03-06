#ifndef _PROTO_H_
#define _PROTO_H_

#include <linux/kernel/types.h>

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


	/* video.c */
	void myos_init_video(void);

	/* softirq.c */
	void register_softirq(unsigned sirq, void (*action)(void * data), void * data);
	void unregister_softirq(unsigned sirq);
	void set_softirq_status(unsigned sirq);
	uint64_t get_softirq_status(void);


#endif /* _PROTO_H_ */