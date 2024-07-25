#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

// #include <linux/kernel/types.h>
// #include <linux/kernel/stddef.h>
#include <sys/types.h>
#include <stdbool.h>

	// #define container_of(ptr, type, member)
	// ({const typeof(((type *)0)->member) *__mptr = (ptr);
	// (type *)((char *)__mptr - offsetof(type,member));})
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
	})

	typedef __UINT64_TYPE__ bitmap_t;
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
	// indicate the param for input
	#define IN
	// indicate the param for output
	#define OUT

	// bitmap.c
	void bm_set_bit(bitmap_t *, unsigned long idx);
	void bm_clear_bit(bitmap_t *, unsigned long idx);
	unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit);
	unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx);

	// list.c
	// struct List;
	typedef struct List List_s;
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;

	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
	

	void INIT_LIST_S(List_s * src);
	void list_insert_prev(List_s * src, List_s * dst);
	void list_insert_next(List_s * src, List_s * dst);
	void list_del_init(List_s * src);
	bool list_is_empty_entry(List_s *l_p);
	
	bool list_header_contains(List_hdr_s * lhdr_p, List_s * l_p);
	void INIT_LIST_HEADER_S(List_hdr_s * lh_p);
	void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_header_remove_head(List_hdr_s * lhdr_p);
	void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p);
	void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_header_remove_tail(List_hdr_s * lhdr_p);
	List_s * list_header_delete_node(List_hdr_s * lhdr_p, List_s * l_p);

#endif /* _LIB_UTILS_H_ */