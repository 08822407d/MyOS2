#ifndef _LINUX_LIST_API_H_
#define _LINUX_LIST_API_H_

	#include "list/double_linked_list.h"

	#define LIST_INIT(name) { &(name), &(name) }
	#define LIST_HEADER_INIT(name) {			\
				.header	= { &(name.header), &(name.header) },	\
				.count	= 0,					\
			}

	#define LIST_S(name) \
				List_s name = LIST_INIT(name)
	#define LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)

	/**
	 * list_entry - get the struct for this entry
	 * @ptr:	the &List_s pointer.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_entry(ptr, type, member) \
				container_of(ptr, type, member)

	/**
	 * list_first_entry - get the first element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->header.next, type, member)

	/**
	 * list_last_entry - get the last element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
	#define list_last_entry(ptr, type, member) \
				list_entry((ptr)->header.prev, type, member)

	// /**
	//  * list_first_entry_or_null - get the first element from a list
	//  * @ptr:	the list head to take the element from.
	//  * @type:	the type of the struct this is embedded in.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Note that if the list is empty, it returns NULL.
	//  */
	// #define list_first_entry_or_null(ptr, type, member) ({ \
	// 	List_s *head__ = (ptr); \
	// 	List_s *pos__ = READ_ONCE(head__->next); \
	// 	pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
	// })

	/**
	 * list_next_entry - get the next element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_next_entry(pos, member) \
		list_entry((pos)->member.next, typeof(*(pos)), member)

	/**
	 * list_prev_entry - get the prev element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_prev_entry(pos, member) \
		list_entry((pos)->member.prev, typeof(*(pos)), member)

	/**
	 * list_for_each	-	iterate over a list
	 * @pos:	the &List_s to use as a loop cursor.
	 * @head:	the head for your list.
	 */
	#define list_for_each(pos, head) \
		for (pos = (head)->next; !list_node_is_head(pos, (head)); pos = pos->next)

	// /**
	//  * list_for_each_continue - continue iteration over a list
	//  * @pos:	the &List_s to use as a loop cursor.
	//  * @head:	the head for your list.
	//  *
	//  * Continue to iterate over a list, continuing after the current position.
	//  */
	// #define list_for_each_continue(pos, head) \
	// 	for (pos = pos->next; !list_node_is_head(pos, (head)); pos = pos->next)

	// /**
	//  * list_for_each_prev	-	iterate over a list backwards
	//  * @pos:	the &List_s to use as a loop cursor.
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_prev(pos, head) \
	// 	for (pos = (head)->prev; !list_node_is_head(pos, (head)); pos = pos->prev)

	// /**
	//  * list_for_each_safe - iterate over a list safe against removal of list entry
	//  * @pos:	the &List_s to use as a loop cursor.
	//  * @n:		another &List_s to use as temporary storage
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_safe(pos, n, head) \
	// 	for (pos = (head)->next, n = pos->next; \
	// 		!list_node_is_head(pos, (head)); \
	// 		pos = n, n = pos->next)

	// /**
	//  * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
	//  * @pos:	the &List_s to use as a loop cursor.
	//  * @n:		another &List_s to use as temporary storage
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_prev_safe(pos, n, head) \
	// 	for (pos = (head)->prev, n = pos->prev; \
	// 		!list_node_is_head(pos, (head)); \
	// 		pos = n, n = pos->prev)

	/**
	 * list_entry_is_head - test if the entry points to the head of the list
	 * @pos:	the type * to cursor
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_entry_is_head(pos, head, member)				\
				(&pos->member == &((head)->header))

	/**
	 * list_for_each_entry	-	iterate over list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_for_each_entry(pos, head, member)				\
				for (pos = list_first_entry(head, typeof(*pos), member);	\
					!list_entry_is_head(pos, head, member);			\
					pos = list_next_entry(pos, member))

	// /**
	//  * list_for_each_entry_reverse - iterate backwards over list of given type.
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  */
	// #define list_for_each_entry_reverse(pos, head, member)			\
	// 	for (pos = list_last_entry(head, typeof(*pos), member);		\
	// 		!list_entry_is_head(pos, head, member); 			\
	// 		pos = list_prev_entry(pos, member))

	// /**
	//  * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
	//  * @pos:	the type * to use as a start point
	//  * @head:	the head of the list
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
	//  */
	// #define list_prepare_entry(pos, head, member) \
	// 	((pos) ? : list_entry(head, typeof(*pos), member))

	// /**
	//  * list_for_each_entry_continue - continue iteration over list of given type
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Continue to iterate over list of given type, continuing after
	//  * the current position.
	//  */
	// #define list_for_each_entry_continue(pos, head, member) 		\
	// 	for (pos = list_next_entry(pos, member);			\
	// 		!list_entry_is_head(pos, head, member);			\
	// 		pos = list_next_entry(pos, member))

	// /**
	//  * list_for_each_entry_continue_reverse - iterate backwards from the given point
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Start to iterate over list of given type backwards, continuing after
	//  * the current position.
	//  */
	// #define list_for_each_entry_continue_reverse(pos, head, member)		\
	// 	for (pos = list_prev_entry(pos, member);			\
	// 		!list_entry_is_head(pos, head, member);			\
	// 		pos = list_prev_entry(pos, member))

	// /**
	//  * list_for_each_entry_from - iterate over list of given type from the current point
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Iterate over list of given type, continuing from current position.
	//  */
	// #define list_for_each_entry_from(pos, head, member) 			\
	// 	for (; !list_entry_is_head(pos, head, member);			\
	// 		pos = list_next_entry(pos, member))

	// /**
	//  * list_for_each_entry_from_reverse - iterate backwards over list of given type
	//  *                                    from the current point
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Iterate backwards over list of given type, continuing from current position.
	//  */
	// #define list_for_each_entry_from_reverse(pos, head, member)		\
	// 	for (; !list_entry_is_head(pos, head, member);			\
	// 		pos = list_prev_entry(pos, member))

	/**
	 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
	 * @pos:	the type * to use as a loop cursor.
	 * @n:		another type * to use as temporary storage
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_for_each_entry_safe(pos, n, head, member)			\
		for (pos = list_first_entry(head, typeof(*pos), member),	\
			n = list_next_entry(pos, member);			\
			!list_entry_is_head(pos, head, member); 			\
			pos = n, n = list_next_entry(n, member))

	// /**
	//  * list_for_each_entry_safe_continue - continue list iteration safe against removal
	//  * @pos:	the type * to use as a loop cursor.
	//  * @n:		another type * to use as temporary storage
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Iterate over list of given type, continuing after current point,
	//  * safe against removal of list entry.
	//  */
	// #define list_for_each_entry_safe_continue(pos, n, head, member) 		\
	// 	for (pos = list_next_entry(pos, member), 				\
	// 		n = list_next_entry(pos, member);				\
	// 		!list_entry_is_head(pos, head, member);				\
	// 		pos = n, n = list_next_entry(n, member))

	// /**
	//  * list_for_each_entry_safe_from - iterate over list from current point safe against removal
	//  * @pos:	the type * to use as a loop cursor.
	//  * @n:		another type * to use as temporary storage
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Iterate over list of given type from current point, safe against
	//  * removal of list entry.
	//  */
	// #define list_for_each_entry_safe_from(pos, n, head, member) 			\
	// 	for (n = list_next_entry(pos, member);					\
	// 		!list_entry_is_head(pos, head, member);				\
	// 		pos = n, n = list_next_entry(n, member))

	// /**
	//  * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
	//  * @pos:	the type * to use as a loop cursor.
	//  * @n:		another type * to use as temporary storage
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * Iterate backwards over list of given type, safe against removal
	//  * of list entry.
	//  */
	// #define list_for_each_entry_safe_reverse(pos, n, head, member)		\
	// 	for (pos = list_last_entry(head, typeof(*pos), member),		\
	// 		n = list_prev_entry(pos, member);			\
	// 		!list_entry_is_head(pos, head, member); 			\
	// 		pos = n, n = list_prev_entry(n, member))

	// /**
	//  * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
	//  * @pos:	the loop cursor used in the list_for_each_entry_safe loop
	//  * @n:		temporary storage used in list_for_each_entry_safe
	//  * @member:	the name of the list_head within the struct.
	//  *
	//  * list_safe_reset_next is not safe to use in general if the list may be
	//  * modified concurrently (eg. the lock is dropped in the loop body). An
	//  * exception to this is if the cursor element (pos) is pinned in the list,
	//  * and list_safe_reset_next is called after re-taking the lock and before
	//  * completing the current iteration of the loop body.
	//  */
	// #define list_safe_reset_next(pos, n, member)				\
	// 	n = list_next_entry(pos, member)


	// MyOS defined protos
	#define LIST_HEAD_INIT(name)	LIST_INIT(name)
	#define LIST_HEAD(name)			LIST_S(name)

	void list_insert_prev(List_s * src, List_s * dst);
	void list_insert_next(List_s * src, List_s * dst);
	
	bool list_in_lhdr(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_init(List_hdr_s * lh_p);
	void list_hdr_push(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_pop(List_hdr_s * lhdr_p);
	void list_hdr_append(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_dequeue(List_hdr_s * lhdr_p);
	List_s * list_hdr_delete(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_dump(List_hdr_s *lhdr_p);


	#define list_hdr_foreach(pos, lhdr)	\
				list_for_each(pos, (&(lhdr->header)))

	#define list_hdr_foreach_entry(lhdr, member)	\
			for ((member) = (lhdr)->header.next;	\
				(member) != &(lhdr)->header;		\
				(member) = (member)->next)

	static inline bool
	list_hdr_contains(List_hdr_s * lhdr, List_s * target) {
		List_s *tmp = NULL;
		list_hdr_foreach(tmp, lhdr) {
			if (tmp == target)
				return true;
		}
		return false;
	}




	#include "list/hash_list.h"

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

#endif /* _LINUX_LIST_API_H_ */