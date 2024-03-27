#define LIST_DEFINATION
#include "double_list.h"


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
		// 		"list_add_to_next corruption. prev is NULL.\n") ||
		//     CHECK_DATA_CORRUPTION(next == NULL,
		// 		"list_add_to_next corruption. next is NULL.\n") ||
		//     CHECK_DATA_CORRUPTION(next->prev != prev,
		// 		"list_add_to_next corruption. next->prev should be prev (%px), but was %px. (next=%px).\n",
		// 		prev, next->prev, next) ||
		//     CHECK_DATA_CORRUPTION(prev->next != next,
		// 		"list_add_to_next corruption. prev->next should be next (%px), but was %px. (prev=%px).\n",
		// 		next, prev->next, prev) ||
		//     CHECK_DATA_CORRUPTION(new == prev || new == next,
		// 		"list_add_to_next double add: new=%px, prev=%px, next=%px.\n",
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