#ifndef _WAIT_QUEUE_H_
#define _WAIT_QUEUE_H_

	struct task_struct;
	typedef struct task_struct task_s;

	typedef struct wait_queue
	{
		List_s		wq_list;
		task_s *	task;
	} wait_queue_T;
	
	void wq_init(wait_queue_T * wq, task_s * tsk);
	void wq_sleep_on(List_hdr_s * wqhdr);
	void wq_sleep_on_intrable(List_hdr_s * wqhdr);
	void wq_wakeup(List_hdr_s * wqhdr, unsigned long pstate);


#endif /* _WAIT_QUEUE_H_ */