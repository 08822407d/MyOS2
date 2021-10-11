#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

#include <sys/types.h>
	// bitmap.c
	void bm_set_bit(bitmap_t *, unsigned long idx);
	void bm_clear_bit(bitmap_t *, unsigned long idx);
	unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit);
	unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx);

	// list.c
	#define container_of(member_ptr, container_type, member_name)										\
	({																									\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);									\
		(container_type *)((unsigned long)p - (unsigned long)&(((container_type *)0)->member_name));	\
	})

	struct List;
	typedef struct List List_s;
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
		void *		owner_p;
	} List_s;

	typedef struct List_hdr
	{
		unsigned long	count;
		List_s			header;
	} List_hdr_s;
	

	void list_init(List_s * src, void * owner_p);
	List_s * list_get_prev(List_s * src);
	List_s * list_get_next(List_s * src);
	void list_insert_prev(List_s * src, List_s * dst);
	void list_insert_next(List_s * src, List_s * dst);
	void list_delete(List_s * src);
	
	void list_hdr_init(List_hdr_s * lh_p);
	void list_hdr_push(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_pop(List_hdr_s * lhdr_p);
	void list_hdr_append(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_dequeue(List_hdr_s * lhdr_p);


#endif /* _LIB_UTILS_H_ */