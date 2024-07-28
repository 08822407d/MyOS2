#include <sys/types.h>
#include <stddef.h>

#include <../../include/utils.h>
#include <stdbool.h>

/*==============================================================================================*/
/*										list operations											*/
/*==============================================================================================*/
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}

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

void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
}

bool list_is_empty_entry(List_s *l_p)
{
	return (l_p == l_p->next);
}

bool list_header_contains(List_hdr_s * lhdr_p, List_s * l_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * next = lhdr_p->anchor.next;
		while (next != &lhdr_p->anchor)
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
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}

void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}

List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

	return NULL;
}

void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}

List_s * list_header_remove_tail(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.prev;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

	return NULL;
}

List_s * list_header_delete_node(List_hdr_s * lhdr_p, List_s * l_p)
{
	if (list_header_contains(lhdr_p, l_p))
	{
		list_del_init(l_p);
		lhdr_p->count--;
		return l_p;
	}

	return NULL;
}
