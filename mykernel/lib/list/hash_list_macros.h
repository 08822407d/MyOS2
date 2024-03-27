/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HASH_LIST_MACROS_H_
#define _LINUX_HASH_LIST_MACROS_H_

	// #define hlist_entry(ptr, type, member) container_of(ptr,type,member)

	// #define hlist_for_each(pos, head) \
	// 	for (pos = (head)->first; pos ; pos = pos->next)

	// #define hlist_for_each_safe(pos, n, head) \
	// 	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	// 		pos = n)

	// #define hlist_entry_safe(ptr, type, member) \
	// 	({ typeof(ptr) ____ptr = (ptr); \
	// 	____ptr ? hlist_entry(____ptr, type, member) : NULL; \
	// 	})

	// /**
	//  * hlist_for_each_entry	- iterate over list of given type
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the hlist_node within the struct.
	//  */
	// #define hlist_for_each_entry(pos, head, member)				\
	// 	for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member);\
	// 		pos;							\
	// 		pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

	// /**
	//  * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
	//  * @pos:	the type * to use as a loop cursor.
	//  * @member:	the name of the hlist_node within the struct.
	//  */
	// #define hlist_for_each_entry_continue(pos, member)			\
	// 	for (pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member);\
	// 		pos;							\
	// 		pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

	// /**
	//  * hlist_for_each_entry_from - iterate over a hlist continuing from current point
	//  * @pos:	the type * to use as a loop cursor.
	//  * @member:	the name of the hlist_node within the struct.
	//  */
	// #define hlist_for_each_entry_from(pos, member)				\
	// 	for (; pos;							\
	// 		pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

	// /**
	//  * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
	//  * @pos:	the type * to use as a loop cursor.
	//  * @n:		a &struct hlist_node to use as temporary storage
	//  * @head:	the head for your list.
	//  * @member:	the name of the hlist_node within the struct.
	//  */
	// #define hlist_for_each_entry_safe(pos, n, head, member) 		\
	// 	for (pos = hlist_entry_safe((head)->first, typeof(*pos), member);\
	// 		pos && ({ n = pos->member.next; 1; });			\
	// 		pos = hlist_entry_safe(n, typeof(*pos), member))

#endif /* _LINUX_HASH_LIST_MACROS_H_ */