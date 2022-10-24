#include <sys/types.h>
#include <stddef.h>

#include <../../include/utils.h>
#include <stdbool.h>

/*==============================================================================================*/
/*										list operations											*/
/*==============================================================================================*/
void list_init(List_s * src, void * owner_p)
{
	src->prev = src;
	src->next = src;
	src->owner_p = owner_p;
}

inline List_s * list_get_prev(List_s * src)
{
	return src->prev;
}

inline List_s * list_get_next(List_s * src)
{
	return src->next;
}

// insert src to dst->prev
void list_insert_prev(List_s * dst, List_s * src)
{
	list_delete(src);

	src->next = dst;
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}

// insert src to dst->next
void list_insert_next(List_s * dst, List_s * src)
{
	list_delete(src);

	src->next = dst->next;
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}

void list_delete(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	list_init(src, src->owner_p);
}

bool list_is_empty(List_s *l_p)
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
	list_init(&lhdr_p->header, NULL);
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
		List_s * ret_val = list_get_next(&lhdr_p->header);
		list_delete(ret_val);
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
		List_s * ret_val = list_get_prev(&lhdr_p->header);
		list_delete(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

	return NULL;
}

List_s * list_hdr_delete(List_hdr_s * lhdr_p, List_s * l_p)
{
	if (list_in_lhdr(lhdr_p, l_p))
	{
		list_delete(l_p);
		lhdr_p->count--;
		return l_p;
	}

	return NULL;
}

void list_foreach_do(List_hdr_s * lhdr, List_s * target,
						list_traverse_do do_hdlr)
{
	if (do_hdlr == NULL)
	{
		while (1);
	}

	List_s * tmp;
	for (tmp = lhdr->header.next; tmp != &lhdr->header; tmp = tmp->next)
	{
		do_hdlr(tmp, target);
	}
}

void list_search_and_do(List_hdr_s * lhdr, List_s * target,
					list_traverse_check chk_hdlr,
					list_traverse_do do_hdlr,
					list_traverse_end end_hdlr)
{
	if (chk_hdlr == NULL || do_hdlr == NULL)
	{
		while (1);
	}

	List_s * tmp;
	for (tmp = lhdr->header.next; tmp != &lhdr->header; tmp = tmp->next)
	{
		int rv = chk_hdlr(tmp, target);
		if (rv != 0)
		{
			do_hdlr(tmp, target);
			break;
		}
	}
	if (end_hdlr != NULL && tmp == &lhdr->header)
			end_hdlr(tmp, target);
}