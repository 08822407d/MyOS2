#ifndef _KLIB_DATA_STRUCTURE_
#define _KLIB_DATA_STRUCTURE_

#include <lib/stddef.h>

	#define m_list_init(src)						\
			({										\
				if ((src) != NULL)					\
				{									\
					(src)->prev = (src);			\
					(src)->next = (src);			\
				}									\
			})

	#define m_list_get_next(src)					\
			({										\
				(src)->next							\
			};)

	#define m_list_get_prev(src)					\
			({										\
				(src)->prev							\
			};)

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

	#define	m_list_delete(src)						\
			({										\
				if ((src) != NULL)					\
				{									\
					(src)->prev->next = (src)->next;\
					(src)->next->prev = (src)->prev;\
													\
					(src)->prev = (src);			\
					(src)->next = (src);			\
				}									\
			})

#endif /* _KLIB_DATA_STRUCTURE_ */