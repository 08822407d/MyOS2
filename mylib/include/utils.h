#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

// #include <linux/kernel/types.h>
// #include <linux/kernel/stddef.h>
#include <sys/types.h>
#include <stdbool.h>

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
	bool list_is_empty(List_s *l_p);
	
	bool list_in_lhdr(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_init(List_hdr_s * lh_p);
	void list_hdr_push(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_pop(List_hdr_s * lhdr_p);
	void list_hdr_append(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_dequeue(List_hdr_s * lhdr_p);
	List_s * list_hdr_delete(List_hdr_s * lhdr_p, List_s * l_p);

	typedef int(* list_traverse_do)(List_s * curr, List_s * target);
	typedef	list_traverse_do	list_traverse_check;
	typedef	list_traverse_do	list_traverse_end;
	void list_foreach_do(List_hdr_s * lhdr, List_s * target,
						list_traverse_do do_hdlr);
	void list_search_and_do(List_hdr_s * lhdr, List_s * target,
						list_traverse_check chk_hdlr,
						list_traverse_do do_hdlr,
						list_traverse_end end_hdlr);

#endif /* _LIB_UTILS_H_ */