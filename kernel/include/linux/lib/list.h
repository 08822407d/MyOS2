/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_LIST_H

	#define _LINUX_LIST_H

	// #include <linux/container_of.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/stddef.h>
	// #include <linux/poison.h>
	#include <linux/kernel/const.h>

	// #include <asm/barrier.h>


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
	void list_hdr_dump(List_hdr_s *lhdr_p);

	typedef int(* list_traverse_do)(List_s * curr, List_s * target);
	typedef	list_traverse_do	list_traverse_check;
	typedef	list_traverse_do	list_traverse_end;
	void list_foreach_do(List_hdr_s * lhdr, List_s * target,
						list_traverse_do do_hdlr);
	void list_search_and_do(List_hdr_s * lhdr, List_s * target,
						list_traverse_check chk_hdlr,
						list_traverse_do do_hdlr,
						list_traverse_end end_hdlr);

#endif /* _LINUX_LIST_H */