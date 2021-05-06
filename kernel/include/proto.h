#ifndef _PROTO_H_
#define _PROTO_H_

#include <sys/types.h>
#include "ktypes.h"
		
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

	typedef struct softirq
	{
		void (*action)(void * data);
		void * data;
	} softirq_s;

	extern uint64_t		softirq_status;
	extern softirq_s	softirq_vector[64];
	extern timer_list_s timer_list_head;


	/* main.c */
	// #ifndef UNPAGED
	// #define kmain __k_unpaged_kmain
	// #endif
	void kmain(void);

	/* start.c */
	void cstart(void);

	/* memory.c */
	void mem_init(void);
	void slab_init();
	Page_s * page_alloc(void);
	void page_free(Page_s *);
	void * kmalloc(size_t size);
	void kfree(void * obj_p);

	/* video.c */
	void video_init(void);

	/* functions defined in architecture-dependent files. */
	void prot_init(void);

	/* softirq.c */
	void register_softirq(unsigned sirq, void (*action)(void * data), void * data);
	void unregister_softirq(unsigned sirq);
	void set_softirq_status(unsigned sirq);
	uint64_t get_softirq_status(void);
	void softirq_init(void);

	/* timer.c */
	void init_timer(timer_list_s * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies);
	void add_timer(timer_list_s * timer);
	void del_timer(timer_list_s * timer);
	void timer_init(void);
	void do_timer(void * data);

#endif /* _PROTO_H_ */