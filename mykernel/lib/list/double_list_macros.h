/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DOUBLE_LIST_MACROS_H_
#define _LINUX_DOUBLE_LIST_MACROS_H_

	#define LIST_INIT(name) { &(name), &(name) }
	#define LIST_HEADER_INIT(name) {			\
				.anchor	= { &(name.anchor), &(name.anchor) },	\
				.count	= 0,					\
			}

	#define LIST_S(name) \
				List_s name = LIST_INIT(name)
	#define LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)

	/**
	 * list_container - get the struct for this entry
	 * @ptr:	the &List_s pointer.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_container(ptr, type, member) \
				container_of(ptr, type, member)
	#define list_entry list_container

	/**
	 * list_first_entry - get the first element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)

	/**
	 * list_last_entry - get the last element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the list_head within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
	#define list_last_entry(ptr, type, member) \
				list_entry((ptr)->prev, type, member)

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
	// 	pos__ != head__ ? list_container(pos__, type, member) : NULL; \
	// })

	/**
	 * list_next_container - get the next element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_next_container(pos, member) \
				list_container((pos)->member.next, typeof(*(pos)), member)
	#define list_next_entry list_next_container

	/**
	 * list_prev_container - get the prev element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_prev_container(pos, member) \
				list_container((pos)->member.prev, typeof(*(pos)), member)
	#define list_prev_entry list_prev_container

	/**
	 * list_for_each	-	iterate over a list
	 * @pos:	the &struct list_head to use as a loop cursor.
	 * @head:	the head for your list.
	 */
	#define list_for_each(pos, head)			\
				for (pos = (head)->next;		\
					!list_is_head(pos, (head));	\
					pos = pos->next)

	// /**
	//  * list_for_each_rcu - Iterate over a list in an RCU-safe fashion
	//  * @pos:	the &struct list_head to use as a loop cursor.
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_rcu(pos, head)		  \
	// 	for (pos = rcu_dereference((head)->next); \
	// 		 !list_is_head(pos, (head)); \
	// 		 pos = rcu_dereference(pos->next))

	// /**
	//  * list_for_each_continue - continue iteration over a list
	//  * @pos:	the &struct list_head to use as a loop cursor.
	//  * @head:	the head for your list.
	//  *
	//  * Continue to iterate over a list, continuing after the current position.
	//  */
	// #define list_for_each_continue(pos, head) \
	// 	for (pos = pos->next; !list_is_head(pos, (head)); pos = pos->next)

	// /**
	//  * list_for_each_prev	-	iterate over a list backwards
	//  * @pos:	the &struct list_head to use as a loop cursor.
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_prev(pos, head) \
	// 	for (pos = (head)->prev; !list_is_head(pos, (head)); pos = pos->prev)

	// /**
	//  * list_for_each_safe - iterate over a list safe against removal of list entry
	//  * @pos:	the &struct list_head to use as a loop cursor.
	//  * @n:		another &struct list_head to use as temporary storage
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_safe(pos, n, head) \
	// 	for (pos = (head)->next, n = pos->next; \
	// 		 !list_is_head(pos, (head)); \
	// 		 pos = n, n = pos->next)

	// /**
	//  * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
	//  * @pos:	the &struct list_head to use as a loop cursor.
	//  * @n:		another &struct list_head to use as temporary storage
	//  * @head:	the head for your list.
	//  */
	// #define list_for_each_prev_safe(pos, n, head) \
	// 	for (pos = (head)->prev, n = pos->prev; \
	// 		 !list_is_head(pos, (head)); \
	// 		 pos = n, n = pos->prev)

	// /**
	//  * list_count_nodes - count nodes in the list
	//  * @head:	the head for your list.
	//  */
	// static inline size_t list_count_nodes(struct list_head *head)
	// {
	// 	struct list_head *pos;
	// 	size_t count = 0;

	// 	list_for_each(pos, head)
	// 		count++;

	// 	return count;
	// }

	/**
	 * list_entry_is_head - test if the entry points to the head of the list
	 * @pos:	the type * to cursor
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_entry_is_head(pos, head, member)				\
				(&pos->member == (head))

	/**
	 * list_for_each_entry	-	iterate over list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 */
	#define list_for_each_entry(pos, head, member)							\
				for (pos = list_first_entry(head, typeof(*pos), member);	\
					!list_entry_is_head(pos, head, member);					\
					pos = list_next_entry(pos, member))

	// /**
	//  * list_for_each_entry_reverse - iterate backwards over list of given type.
	//  * @pos:	the type * to use as a loop cursor.
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  */
	// #define list_for_each_entry_reverse(pos, head, member)			\
	// 	for (pos = list_last_entry(head, typeof(*pos), member);		\
	// 		 !list_entry_is_head(pos, head, member); 			\
	// 		 pos = list_prev_entry(pos, member))

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
	// 		 !list_entry_is_head(pos, head, member);			\
	// 		 pos = list_next_entry(pos, member))

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
	// 		 !list_entry_is_head(pos, head, member);			\
	// 		 pos = list_prev_entry(pos, member))

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
	// 		 pos = list_next_entry(pos, member))

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
	// 		 pos = list_prev_entry(pos, member))

	// /**
	//  * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
	//  * @pos:	the type * to use as a loop cursor.
	//  * @n:		another type * to use as temporary storage
	//  * @head:	the head for your list.
	//  * @member:	the name of the list_head within the struct.
	//  */
	// #define list_for_each_entry_safe(pos, n, head, member)			\
	// 	for (pos = list_first_entry(head, typeof(*pos), member),	\
	// 		n = list_next_entry(pos, member);			\
	// 		 !list_entry_is_head(pos, head, member); 			\
	// 		 pos = n, n = list_next_entry(n, member))

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
	// 		 !list_entry_is_head(pos, head, member);				\
	// 		 pos = n, n = list_next_entry(n, member))

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
	// 		 !list_entry_is_head(pos, head, member);				\
	// 		 pos = n, n = list_next_entry(n, member))

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
	// 		 !list_entry_is_head(pos, head, member); 			\
	// 		 pos = n, n = list_prev_entry(n, member))

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


	#define list_headr_first_container(ptr, type, member) \
				list_first_entry(&((ptr)->anchor), type, member)

	#define list_headr_last_container(ptr, type, member) \
				list_last_entry(&((ptr)->anchor), type, member)

	#define list_header_foreach(pos, lhdr)	\
				list_for_each(pos, &((lhdr)->anchor))

	#define list_container_is_header_anchor(pos, head, member)	\
				(&pos->member == &((head)->anchor))

	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)

#endif /* _LINUX_DOUBLE_LIST_MACROS_H_ */