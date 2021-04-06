#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>
#include <lib/stddef.h>

void list_init(List_s * current)
{
	current->prev = current;
	current->next = current;
}

inline __always_inline List_s * get_prev(List_s * current)
{
	return current->prev;
}

inline __always_inline List_s * get_next(List_s * current)
{
	return current->next;
}

void list_insert_front(List_s * curr, List_s * new)
{
	new->next = curr;
	new->prev = curr->prev;

	new->next->prev = new;
	new->prev->next = new;
}

void list_insert_back(List_s * curr, List_s * new)
{
	new->next = curr->next;
	new->prev = curr;

	new->next->prev = new;
	new->prev->next = new;
}

void list_delete(List_s * curr)
{
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
}