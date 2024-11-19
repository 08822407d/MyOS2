#ifndef _WORKQUEUE_MACRO_H_
#define _WORKQUEUE_MACRO_H_


	/*
	 * The first word is the work queue pointer and the flags rolled into
	 * one
	 */
	#define work_data_bits(work) ((unsigned long *)(&(work)->data))

#endif /* _WORKQUEUE_MACRO_H_ */