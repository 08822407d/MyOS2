#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

#include <sys/types.h>
	// bitmap.c
	void bm_set_bit(bitmap_t *, unsigned long idx);
	void bm_clear_bit(bitmap_t *, unsigned long idx);
	unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit);
	unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx);

	#define container_of(member_ptr, container_type, member_name)										\
	({																									\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);									\
		(container_type *)((unsigned long)p - (unsigned long)&(((container_type *)0)->member_name));	\
	})

	// list.c
	typedef struct List
	{
		struct List * prev;
		struct List * next;
	} List_s;
	void list_init(List_s * src);
	List_s * list_get_prev(List_s * src);
	List_s * list_get_next(List_s * src);
	void list_insert_front(List_s * src, List_s * dst);
	void list_insert_back(List_s * src, List_s * dst);
	void list_delete(List_s * src);
	

#endif /* _LIB_UTILS_H_ */