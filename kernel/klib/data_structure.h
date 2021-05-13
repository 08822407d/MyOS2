#ifndef _KLIB_DATA_STRUCTURE_
#define _KLIB_DATA_STRUCTURE_

#include <lib/stddef.h>

/*==============================================================================================*
 *											LIST												*
 *==============================================================================================*/
	#define define_list_header(TYPE)				\
			struct TYPE ## _list;					\
			typedef struct TYPE ## _list			\
			{										\
				unsigned		count;				\
				TYPE ## _s *	head_p;				\
			} TYPE ## _list_s;
	#define init_list_header(lh)					\
			({										\
				(lh)->count = 0;					\
				(lh)->head_p = NULL;				\
			})

	#define m_list_init(src)						\
			({										\
				if ((src) != NULL)					\
				{									\
					(src)->prev = (src);			\
					(src)->next = (src);			\
					(src)->list_header = NULL;		\
				}									\
			})

	#define m_list_get_next(src)					\
			({										\
				(src)->next;						\
			})

	#define m_list_get_prev(src)					\
			({										\
				(src)->prev;						\
			})

	#define m_list_insert_front(src, dst)			\
			({										\
				if ((dst) != NULL)					\
				{									\
					(src)->next = (dst);			\
					(src)->prev = (dst)->prev;		\
													\
					(src)->next->prev = (src);		\
					(src)->prev->next = (src);		\
				}									\
			})

	#define m_list_insert_back(src, dst)			\
			({										\
				if ((dst) != NULL)					\
				{									\
					(src)->prev = (dst);			\
					(src)->next = (dst)->next;		\
													\
					(src)->next->prev = (src);		\
					(src)->prev->next = (src);		\
				}									\
			})

	#define	m_list_delete(src)							\
			({											\
				if ((src) != NULL)						\
				{										\
					(src)->prev->next = (src)->next;	\
					(src)->next->prev = (src)->prev;	\
														\
					m_list_init((src));					\
				}										\
			})

	#define m_push_list(unit, list_head)									\
			({																\
				if ((unit) != NULL || (list_head) != NULL)					\
				{															\
					m_list_init((unit));									\
					if ((list_head)->count == 0)							\
						(list_head)->head_p = unit;							\
					else													\
						m_list_insert_front((unit), (list_head)->head_p);	\
																			\
					(list_head)->head_p = (list_head)->head_p->prev;		\
					(unit)->list_header = list_head;						\
					(list_head)->count++;									\
				}															\
			})

	#define m_pop_list(ret_val, list_head)									\
			({																\
				if ((list_head) != NULL)									\
				{															\
					(ret_val) = (list_head)->head_p;						\
					if ((list_head)->count == 1)							\
						(list_head)->head_p = NULL;							\
					else													\
						(list_head)->head_p = (list_head)->head_p->next;	\
																			\
					m_list_delete((ret_val));								\
					(ret_val)->list_header = NULL;							\
					(list_head)->count--;									\
				}															\
			})

/*==============================================================================================*
 *											BTREE												*
 *==============================================================================================*/

#endif /* _KLIB_DATA_STRUCTURE_ */