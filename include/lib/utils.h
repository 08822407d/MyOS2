#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

#include <sys/types.h>
	// bitmap.c
	void bm_set_bit(bitmap_t *, unsigned long);
	void bm_clear_bit(bitmap_t *, unsigned long);
	unsigned long bm_get_freebit_idx(bitmap_t *, unsigned long);

	#define container_of(member_ptr, container_type, member_name)									\
	({																		\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);							\
		(container_type *)((unsigned long)p - (unsigned long)&(((container_type *)0)->member_name));	\
	})

	// list.c
	typedef struct List
	{
		struct List * prev;
		struct List * next;
	} List_s;
	void list_init(List_s *);
	List_s * get_prev(List_s *);
	List_s * get_next(List_s *);
	void list_insert_front(List_s *, List_s *);
	void list_insert_back(List_s *, List_s *);
	void list_delete(List_s *);
	

#endif /* _LIB_UTILS_H_ */