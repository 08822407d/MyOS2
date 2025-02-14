#ifndef _SWAIT_MACRO_H_
#define _SWAIT_MACRO_H_


	#define __SWAITQUEUE_INITIALIZER(name) {					\
				.task			= current,						\
				.task_list		= LIST_INIT((name).task_list),	\
			}

	#define DECLARE_SWAITQUEUE(name)	\
				swqueue_s name = __SWAITQUEUE_INITIALIZER(name)

    #define __SWAIT_QUEUE_HEAD_INITIALIZER(name) {							\
				.lock			= __ARCH_SPIN_LOCK_UNLOCKED,				\
				.task_list_hdr	= LIST_HEADER_INIT((name).task_list_hdr),	\
			}

	#define DECLARE_SWAIT_QUEUE_HEAD(name)	\
				swqueue_hdr_s name = __SWAIT_QUEUE_HEAD_INITIALIZER(name)

	#define init_swait_queue_head(q)			\
			do {								\
				__init_swait_queue_head((q));	\
			} while (0)

#endif /* _SWAIT_MACRO_H_ */