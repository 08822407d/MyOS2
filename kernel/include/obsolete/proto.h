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

	extern uint64_t		softirq_status;
	extern softirq_s	softirq_vector[64];
	extern List_hdr_s	timer_lhdr;

	/* main.c */
	void start_kernel(void);

	/* mod_test.c */
	unsigned long module_test(unsigned long);

	/* start.c */
	void cstart(void);

	/* memory.c */
	page_s * alloc_pages(enum zone_type ,unsigned int order);
	page_s * paddr_to_page(phys_addr_t paddr);
	phys_addr_t page_to_paddr(page_s * page);
	void page_free(page_s *);

	/* video.c */
	void myos_init_video(void);

	/* functions defined in architecture-dependent files. */
	void prot_bsp_init(void);

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