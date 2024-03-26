#define LIST_DEFINATION
#include "double_linked_list.h"


#ifdef CONFIG_DEBUG_LIST
	#include <linux/kernel/export.h>
	/*
	 * Check that the data structures for the list manipulations are reasonably
	 * valid. Failures here indicate memory corruption (and possibly an exploit
	 * attempt).
	 */

	bool __list_add_valid(List_s *new, List_s *prev, List_s *next)
	{
		// if (CHECK_DATA_CORRUPTION(prev == NULL,
		// 		"list_add corruption. prev is NULL.\n") ||
		//     CHECK_DATA_CORRUPTION(next == NULL,
		// 		"list_add corruption. next is NULL.\n") ||
		//     CHECK_DATA_CORRUPTION(next->prev != prev,
		// 		"list_add corruption. next->prev should be prev (%px), but was %px. (next=%px).\n",
		// 		prev, next->prev, next) ||
		//     CHECK_DATA_CORRUPTION(prev->next != next,
		// 		"list_add corruption. prev->next should be next (%px), but was %px. (prev=%px).\n",
		// 		next, prev->next, prev) ||
		//     CHECK_DATA_CORRUPTION(new == prev || new == next,
		// 		"list_add double add: new=%px, prev=%px, next=%px.\n",
		// 		new, prev, next))
		// 	return false;

		return true;
	}
	EXPORT_SYMBOL(__list_add_valid);

	bool __list_del_entry_valid(List_s *entry)
	{
		List_s *prev, *next;

		prev = entry->prev;
		next = entry->next;

		// if (CHECK_DATA_CORRUPTION(next == NULL,
		// 		"list_del_init corruption, %px->next is NULL\n", entry) ||
		//     CHECK_DATA_CORRUPTION(prev == NULL,
		// 		"list_del_init corruption, %px->prev is NULL\n", entry) ||
		//     CHECK_DATA_CORRUPTION(next == LIST_POISON1,
		// 		"list_del_init corruption, %px->next is LIST_POISON1 (%px)\n",
		// 		entry, LIST_POISON1) ||
		//     CHECK_DATA_CORRUPTION(prev == LIST_POISON2,
		// 		"list_del_init corruption, %px->prev is LIST_POISON2 (%px)\n",
		// 		entry, LIST_POISON2) ||
		//     CHECK_DATA_CORRUPTION(prev->next != entry,
		// 		"list_del_init corruption. prev->next should be %px, but was %px. (prev=%px)\n",
		// 		entry, prev->next, prev) ||
		//     CHECK_DATA_CORRUPTION(next->prev != entry,
		// 		"list_del_init corruption. next->prev should be %px, but was %px. (next=%px)\n",
		// 		entry, next->prev, next))
		// 	return false;

		return true;
	}
	EXPORT_SYMBOL(__list_del_entry_valid);
#endif



#include <linux/kernel/mm.h>

/*==============================================================================================*/
/*										list operations											*/
/*==============================================================================================*/
// insert src to dst->prev
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}

// insert src to dst->next
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}

bool list_in_lhdr(List_hdr_s * lhdr_p, List_s * l_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * next = lhdr_p->header.next;
		while (next != &lhdr_p->header)
		{
			if (next == l_p)
			{
				return true;
			}
			next = next->next;
		}
	}

	return false;
}

/*==============================================================================================*/
/*										list header operations									*/
/*==============================================================================================*/
void list_hdr_init(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->header);
}

void list_hdr_push(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->header, l_p);
	lhdr_p->count++;
}

List_s * list_hdr_pop(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->header.next;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

	return NULL;
}

void list_hdr_append(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->header, l_p);
	lhdr_p->count++;
}

inline __always_inline void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_hdr_append(lhdr_p, l_p);
}

void list_hdr_dump(List_hdr_s *lhdr_p)
{
	List_s *lp;
	while ((lp = list_hdr_pop(lhdr_p)) != NULL)
		kfree(lp);
}

List_s * list_hdr_dequeue(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->header.prev;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

	return NULL;
}

List_s * list_hdr_delete(List_hdr_s * lhdr_p, List_s * l_p)
{
	if (list_in_lhdr(lhdr_p, l_p))
	{
		list_del_init(l_p);
		lhdr_p->count--;
		return l_p;
	}

	return NULL;
}