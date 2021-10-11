#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>
#include <lib/stddef.h>

/*==============================================================================================*/
/*										list operations											*/
/*==============================================================================================*/
void list_init(List_s * src, void * owner_p)
{
	src->prev = src;
	src->next = src;
	src->owner_p = owner_p;
}

inline __always_inline List_s * list_get_prev(List_s * src)
{
	return src->prev;
}

inline __always_inline List_s * list_get_next(List_s * src)
{
	return src->next;
}

// insert src to dst->prev
void list_insert_prev(List_s * dst, List_s * src)
{
	src->next = dst;
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}

// insert src to dst->next
void list_insert_next(List_s * dst, List_s * src)
{
	src->next = dst->next;
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}

void list_delete(List_s * src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	src->owner_p = NULL;
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
	List_s * ret_val = list_get_next(&lhdr_p->header);
	list_delete(ret_val);
	lhdr_p->count--;
	return ret_val;
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

List_s * list_hdr_dequeue(List_hdr_s * lhdr_p)
{
	List_s * ret_val = list_get_prev(&lhdr_p->header);
	list_delete(ret_val);
	lhdr_p->count--;
	return ret_val;
}