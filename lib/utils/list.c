#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>
#include <lib/stddef.h>

void list_init(List_s * src)
{
	src->prev = src;
	src->next = src;
}

inline __always_inline List_s * list_get_prev(List_s * src)
{
	return src->prev;
}

inline __always_inline List_s * list_get_next(List_s * src)
{
	return src->next;
}

void list_insert_front(List_s * src, List_s * dst)
{
	src->next = dst;
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}

void list_insert_back(List_s * src, List_s * dst)
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
}