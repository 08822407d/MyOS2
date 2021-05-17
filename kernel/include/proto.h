#ifndef _PROTO_H_
#define _PROTO_H_

#include <sys/types.h>

#include "ktypes.h"
#include "../klib/data_structure.h"
		
	struct timer;
	typedef struct timer timer_s;	

	define_list_header(timer);

	typedef struct timer
	{
		timer_s *		prev;
		timer_s *		next;
		
		timer_list_s *	list_header;

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
	extern timer_s timer_list_head;


	/* main.c */
	void kmain(void);

	/* mod_test.c */
	unsigned long module_test(unsigned long);

	/* start.c */
	void cstart(void);

	/* memory.c */
	void init_page_manage(void);
	void init_slab();
	Page_s * page_alloc(void);
	void page_free(Page_s *);
	void * kmalloc(size_t size);
	void kfree(void * obj_p);

	/* video.c */
	void init_video(void);

	/* functions defined in architecture-dependent files. */
	void prot_bsp_init(void);

	/* softirq.c */
	void register_softirq(unsigned sirq, void (*action)(void * data), void * data);
	void unregister_softirq(unsigned sirq);
	void set_softirq_status(unsigned sirq);
	uint64_t get_softirq_status(void);
	void softirq_init(void);

	/* timer.c */
	void init_timer(timer_s * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies);
	void add_timer(timer_s * timer);
	void del_timer(timer_s * timer);
	void timer_init(void);
	void do_timer(void * data);

#endif /* _PROTO_H_ */