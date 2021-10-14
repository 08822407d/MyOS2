#ifndef _WAIT_QUEUE_H_
#define _WAIT_QUEUE_H_

#include <lib/utils.h>

	struct task;
	typedef struct task task_s;

	typedef struct wait_queue
	{
		List_s		wq_list;
		task_s *	task;
	} wait_queue_T;

	typedef List_hdr_s wait_queue_hdr_s;
	
	void wq_init(wait_queue_T * wq, task_s * tsk);
	void wq_sleep_on(wait_queue_hdr_s * wqhdr);
	void wq_sleep_on_intrable(wait_queue_hdr_s * wqhdr);
	void wq_wakeup(wait_queue_hdr_s * wqhdr);


#endif /* _WAIT_QUEUE_H_ */