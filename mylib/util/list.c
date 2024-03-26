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

bool list_node_empty(List_s *l_p)
{
	return (l_p == l_p->next);
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

inline void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_hdr_append(lhdr_p, l_p);
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
