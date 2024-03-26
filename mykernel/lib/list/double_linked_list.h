/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_LIST_H_
#define _LINUX_LIST_H_

	// #include <linux/container_of.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/stddef.h>
	// #include <linux/poison.h>
	#include <linux/kernel/const.h>

	#include <asm/insns.h>

	/*
	 * Circular doubly linked list implementation.
	 *
	 * Some of the internal functions ("__xxx") are useful when
	 * manipulating whole lists rather than single entries, as
	 * sometimes we already know the next/prev entries and we can
	 * generate better code by using them directly rather than
	 * using the generic single-entry routines.
	 */

	#define LIST_INIT(name) { &(name), &(name) }
	#define LIST_HEADER_INIT(name) {			\
				.header	= { &(name.header), &(name.header) },	\
				.count	= 0,					\
			}

	#define LIST_S(name) \
				List_s name = LIST_INIT(name)
	#define LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)



	#include <linux/compiler/myos_optimize_option.h>
	#ifdef DEBUG

		extern void
		INIT_LIST_S(List_s *list);
		#define INIT_LIST_HEAD(list) INIT_LIST_S(list)
		extern void
		INIT_LIST_HDR_S(List_hdr_s *lhdr);

		extern bool
		__list_add_valid(List_s *new, List_s *prev, List_s *next);
		extern bool
		__list_del_entry_valid(List_s *entry);

		extern void
		__list_add(List_s *new, List_s *prev, List_s *next);
		extern void
		list_add(List_s *new, List_s *head);
		extern void
		list_add_tail(List_s *new, List_s *head);

		extern void
		__list_del(List_s * prev, List_s * next);
		extern void
		__list_del_clearprev(List_s *entry);
		extern void
		__list_del_entry(List_s *entry);
		extern void
		list_del(List_s *entry);
		extern void
		list_del_init(List_s *entry);
		extern void
		list_del_init_careful(List_s *entry);

		extern void
		list_replace(List_s *old, List_s *new);
		extern void
		list_replace_init(List_s *old, List_s *new);

		extern void
		list_swap(List_s *entry1, List_s *entry2);

		extern void
		list_move(List_s *list, List_s *head);
		extern void
		list_move_tail(List_s *list, List_s *head);
		extern void
		list_bulk_move_tail(List_s *head, List_s *first, List_s *last);

		extern bool
		list_is_first(const List_s *list, const List_s *head);
		extern bool
		list_is_last(const List_s *list, const List_s *head);
		extern bool
		list_is_head(const List_s *list, const List_s *head);
		extern bool
		list_empty(const List_s *head);
		extern bool
		list_empty_careful(const List_s *head);

		extern void
		list_rotate_left(List_s *head);
		extern void
		list_rotate_to_front(List_s *list, List_s *head);

		extern bool
		list_is_singular(const List_s *head);

		extern void
		__list_cut_position(List_s *list, List_s *head, List_s *entry);
		extern void
		list_cut_position(List_s *list, List_s *head, List_s *entry);
		extern void
		list_cut_before(List_s *list, List_s *head, List_s *entry);

		extern void
		__list_splice(const List_s *list, List_s *prev, List_s *next);
		extern void
		list_splice(const List_s *list, List_s *head);
		extern void
		list_splice_tail(List_s *list, List_s *head);
		extern void
		list_splice_init(List_s *list, List_s *head);
		extern void
		list_splice_tail_init(List_s *list, List_s *head);


		extern bool
		list_hdr_empty(const List_hdr_s *header);

	#endif
	
	#if defined(LIST_DEFINATION) || !(DEBUG)

		/**
		 * INIT_LIST_HEAD - Initialize a list_head structure
		 * @list: list_head structure to be initialized.
		 *
		 * Initializes the list_head to point to itself.  If it is a list header,
		 * the result is an empty list.
		 */
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_S(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_HDR_S(List_hdr_s *lhdr) {
			WRITE_ONCE(lhdr->count, 0);
			INIT_LIST_S(&lhdr->header);
		}

		PREFIX_STATIC_INLINE
		bool
		__list_add_valid(List_s *new, List_s *prev, List_s *next) {
			return true;
		}
		PREFIX_STATIC_INLINE
		bool
		__list_del_entry_valid(List_s *entry) {
			return true;
		}

		/*
		 * Insert a new entry between two known consecutive entries.
		 *
		 * This is only for internal list manipulation where we know
		 * the prev/next entries already!
		 */
		PREFIX_STATIC_INLINE
		void
		__list_add(List_s *new, List_s *prev, List_s *next) {
			if (!__list_add_valid(new, prev, next))
				return;

			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
		}
		/**
		 * list_add - add a new entry
		 * @new: new entry to be added
		 * @head: list head to add it after
		 *
		 * Insert a new entry after the specified head.
		 * This is good for implementing stacks.
		 */
		PREFIX_STATIC_INLINE
		void
		list_add(List_s *new, List_s *head) {
			__list_add(new, head, head->next);
		}
		/**
		 * list_add_tail - add a new entry
		 * @new: new entry to be added
		 * @head: list head to add it before
		 *
		 * Insert a new entry before the specified head.
		 * This is useful for implementing queues.
		 */
		PREFIX_STATIC_INLINE
		void
		list_add_tail(List_s *new, List_s *head) {
			__list_add(new, head->prev, head);
		}


		/*
		 * Delete a list entry by making the prev/next entries
		 * point to each other.
		 *
		 * This is only for internal list manipulation where we know
		 * the prev/next entries already!
		 */
		PREFIX_STATIC_INLINE
		void
		__list_del(List_s * prev, List_s * next) {
			next->prev = prev;
			WRITE_ONCE(prev->next, next);
		}
		/*
		 * Delete a list entry and clear the 'prev' pointer.
		 *
		 * This is a special-purpose list clearing method used in the networking code
		 * for lists allocated as per-cpu, where we don't want to incur the extra
		 * WRITE_ONCE() overhead of a regular list_del_init(). The code that uses this
		 * needs to check the node 'prev' pointer instead of calling list_empty().
		 */
		PREFIX_STATIC_INLINE
		void
		__list_del_clearprev(List_s *entry) {
			__list_del(entry->prev, entry->next);
			entry->prev = NULL;
		}
		PREFIX_STATIC_INLINE
		void
		__list_del_entry(List_s *entry) {
			if (!__list_del_entry_valid(entry))
				return;

			__list_del(entry->prev, entry->next);
		}
		/**
		 * list_del - deletes entry from list.
		 * @entry: the element to delete from the list.
		 * Note: list_empty() on entry does not return true after this, the entry is
		 * in an undefined state.
		 */
		PREFIX_STATIC_INLINE
		void
		list_del(List_s *entry) {
			__list_del_entry(entry);
			// entry->next = LIST_POISON1;
			// entry->prev = LIST_POISON2;
		}
		/**
		 * list_del_init - deletes entry from list and reinitialize it.
		 * @entry: the element to delete from the list.
		 */
		PREFIX_STATIC_INLINE
		void
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
		// /**
		//  * list_del_init_careful - deletes entry from list and reinitialize it.
		//  * @entry: the element to delete from the list.
		//  *
		//  * This is the same as list_del_init(), except designed to be used
		//  * together with list_empty_careful() in a way to guarantee ordering
		//  * of other memory operations.
		//  *
		//  * Any memory operations done before a list_del_init_careful() are
		//  * guaranteed to be visible after a list_empty_careful() test.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_del_init_careful(List_s *entry) {
		// 	__list_del_entry(entry);
		// 	entry->prev = entry;
		// 	smp_store_release(&entry->next, entry);
		// }


		/**
		 * list_replace - replace old entry by new one
		 * @old : the element to be replaced
		 * @new : the new element to insert
		 *
		 * If @old was empty, it will be overwritten.
		 */
		PREFIX_STATIC_INLINE
		void
		list_replace(List_s *old, List_s *new) {
			new->next = old->next;
			new->next->prev = new;
			new->prev = old->prev;
			new->prev->next = new;
		}
		/**
		 * list_replace_init - replace old entry by new one and initialize the old one
		 * @old : the element to be replaced
		 * @new : the new element to insert
		 *
		 * If @old was empty, it will be overwritten.
		 */
		PREFIX_STATIC_INLINE
		void
		list_replace_init(List_s *old, List_s *new) {
			list_replace(old, new);
			INIT_LIST_HEAD(old);
		}

		/**
		 * list_swap - replace entry1 with entry2 and re-add entry1 at entry2's position
		 * @entry1: the location to place entry2
		 * @entry2: the location to place entry1
		 */
		PREFIX_STATIC_INLINE
		void
		list_swap(List_s *entry1, List_s *entry2) {
			List_s *pos = entry2->prev;

			list_del(entry2);
			list_replace(entry1, entry2);
			if (pos == entry1)
				pos = entry2;
			list_add(entry1, pos);
		}

		/**
		 * list_move - delete from one list and add as another's head
		 * @list: the entry to move
		 * @head: the head that will precede our entry
		 */
		PREFIX_STATIC_INLINE
		void
		list_move(List_s *list, List_s *head) {
			__list_del_entry(list);
			list_add(list, head);
		}
		/**
		 * list_move_tail - delete from one list and add as another's tail
		 * @list: the entry to move
		 * @head: the head that will follow our entry
		 */
		PREFIX_STATIC_INLINE
		void
		list_move_tail(List_s *list, List_s *head) {
			__list_del_entry(list);
			list_add_tail(list, head);
		}
		/**
		 * list_bulk_move_tail - move a subsection of a list to its tail
		 * @head: the head that will follow our entry
		 * @first: first entry to move
		 * @last: last entry to move, can be the same as first
		 *
		 * Move all entries between @first and including @last before @head.
		 * All three entries must belong to the same linked list.
		 */
		PREFIX_STATIC_INLINE
		void
		list_bulk_move_tail(List_s *head, List_s *first, List_s *last) {
			first->prev->next = last->next;
			last->next->prev = first->prev;

			head->prev->next = first;
			first->prev = head->prev;

			last->next = head;
			head->prev = last;
		}

		/**
		 * list_is_first -- tests whether @list is the first entry in list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_first(const List_s *list, const List_s *head) {
			return list->prev == head;
		}
		/**
		 * list_is_last - tests whether @list is the last entry in list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_last(const List_s *list, const List_s *head) {
			return list->next == head;
		}
		/**
		 * list_is_head - tests whether @list is the list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_head(const List_s *list, const List_s *head) {
			return list == head;
		}
		/**
		 * list_empty - tests whether a list is empty
		 * @head: the list to test.
		 */
		PREFIX_STATIC_INLINE
		bool
		list_empty(const List_s *head) {
			return READ_ONCE(head->next) == head;
		}
		// /**
		//  * list_empty_careful - tests whether a list is empty and not being modified
		//  * @head: the list to test
		//  *
		//  * Description:
		//  * tests whether a list is empty _and_ checks that no other CPU might be
		//  * in the process of modifying either member (next or prev)
		//  *
		//  * NOTE: using list_empty_careful() without synchronization
		//  * can only be safe if the only activity that can happen
		//  * to the list entry is list_del_init(). Eg. it cannot be used
		//  * if another CPU could re-list_add() it.
		//  */
		// PREFIX_STATIC_INLINE
		// bool
		// list_empty_careful(const List_s *head) {
		// 	List_s *next = smp_load_acquire(&head->next);
		// 	return list_is_head(next, head) && (next == head->prev);
		// }

		// /**
		//  * list_rotate_left - rotate the list to the left
		//  * @head: the head of the list
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_rotate_left(List_s *head) {
		// 	List_s *first;

		// 	if (!list_empty(head)) {
		// 		first = head->next;
		// 		list_move_tail(first, head);
		// 	}
		// }
		// /**
		//  * list_rotate_to_front() - Rotate list to specific item.
		//  * @list: The desired new front of the list.
		//  * @head: The head of the list.
		//  *
		//  * Rotates list so that @list becomes the new front of the list.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_rotate_to_front(List_s *list, List_s *head) {
		// 	/*
		// 	 * Deletes the list head from the list denoted by @head and
		// 	 * places it as the tail of @list, this effectively rotates the
		// 	 * list so that @list is at the front.
		// 	 */
		// 	list_move_tail(head, list);
		// }

		// /**
		//  * list_is_singular - tests whether a list has just one entry.
		//  * @head: the list to test.
		//  */
		// PREFIX_STATIC_INLINE
		// bool
		// list_is_singular(const List_s *head) {
		// 	return !list_empty(head) && (head->next == head->prev);
		// }

		// PREFIX_STATIC_INLINE
		// void
		// __list_cut_position(List_s *list, List_s *head, List_s *entry) {
		// 	List_s *new_first = entry->next;
		// 	list->next = head->next;
		// 	list->next->prev = list;
		// 	list->prev = entry;
		// 	entry->next = list;
		// 	head->next = new_first;
		// 	new_first->prev = head;
		// }
		// /**
		//  * list_cut_position - cut a list into two
		//  * @list: a new list to add all removed entries
		//  * @head: a list with entries
		//  * @entry: an entry within head, could be the head itself
		//  *	and if so we won't cut the list
		//  *
		//  * This helper moves the initial part of @head, up to and
		//  * including @entry, from @head to @list. You should
		//  * pass on @entry an element you know is on @head. @list
		//  * should be an empty list or a list you do not care about
		//  * losing its data.
		//  *
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_cut_position(List_s *list, List_s *head, List_s *entry) {
		// 	if (list_empty(head))
		// 		return;
		// 	if (list_is_singular(head) && !list_is_head(entry, head) && (entry != head->next))
		// 		return;
		// 	if (list_is_head(entry, head))
		// 		INIT_LIST_HEAD(list);
		// 	else
		// 		__list_cut_position(list, head, entry);
		// }
		// /**
		//  * list_cut_before - cut a list into two, before given entry
		//  * @list: a new list to add all removed entries
		//  * @head: a list with entries
		//  * @entry: an entry within head, could be the head itself
		//  *
		//  * This helper moves the initial part of @head, up to but
		//  * excluding @entry, from @head to @list.  You should pass
		//  * in @entry an element you know is on @head.  @list should
		//  * be an empty list or a list you do not care about losing
		//  * its data.
		//  * If @entry == @head, all entries on @head are moved to
		//  * @list.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_cut_before(List_s *list, List_s *head, List_s *entry) {
		// 	if (head->next == entry) {
		// 		INIT_LIST_HEAD(list);
		// 		return;
		// 	}
		// 	list->next = head->next;
		// 	list->next->prev = list;
		// 	list->prev = entry->prev;
		// 	list->prev->next = list;
		// 	head->next = entry;
		// 	entry->prev = head;
		// }

		// PREFIX_STATIC_INLINE
		// void
		// __list_splice(const List_s *list, List_s *prev, List_s *next) {
		// 	List_s *first = list->next;
		// 	List_s *last = list->prev;

		// 	first->prev = prev;
		// 	prev->next = first;

		// 	last->next = next;
		// 	next->prev = last;
		// }
		// /**
		//  * list_splice - join two lists, this is designed for stacks
		//  * @list: the new list to add.
		//  * @head: the place to add it in the first list.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice(const List_s *list, List_s *head) {
		// 	if (!list_empty(list))
		// 		__list_splice(list, head, head->next);
		// }
		// /**
		//  * list_splice_tail - join two lists, each list being a queue
		//  * @list: the new list to add.
		//  * @head: the place to add it in the first list.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice_tail(List_s *list, List_s *head) {
		// 	if (!list_empty(list))
		// 		__list_splice(list, head->prev, head);
		// }
		// /**
		//  * list_splice_init - join two lists and reinitialise the emptied list.
		//  * @list: the new list to add.
		//  * @head: the place to add it in the first list.
		//  *
		//  * The list at @list is reinitialised
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice_init(List_s *list, List_s *head) {
		// 	if (!list_empty(list)) {
		// 		__list_splice(list, head, head->next);
		// 		INIT_LIST_HEAD(list);
		// 	}
		// }
		// /**
		//  * list_splice_tail_init - join two lists and reinitialise the emptied list
		//  * @list: the new list to add.
		//  * @head: the place to add it in the first list.
		//  *
		//  * Each of the lists is a queue.
		//  * The list at @list is reinitialised
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice_tail_init(List_s *list, List_s *head) {
		// 	if (!list_empty(list)) {
		// 		__list_splice(list, head->prev, head);
		// 		INIT_LIST_HEAD(list);
		// 	}
		// }



		PREFIX_STATIC_INLINE
		bool
		list_hdr_empty(const List_hdr_s *header) {
			return READ_ONCE(header->count) == 0;
		}

	#endif


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
			for (pos = (head)->next; !list_is_head(pos, (head)); pos = pos->next)

		// /**
		//  * list_for_each_continue - continue iteration over a list
		//  * @pos:	the &List_s to use as a loop cursor.
		//  * @head:	the head for your list.
		//  *
		//  * Continue to iterate over a list, continuing after the current position.
		//  */
		// #define list_for_each_continue(pos, head) \
		// 	for (pos = pos->next; !list_is_head(pos, (head)); pos = pos->next)

		// /**
		//  * list_for_each_prev	-	iterate over a list backwards
		//  * @pos:	the &List_s to use as a loop cursor.
		//  * @head:	the head for your list.
		//  */
		// #define list_for_each_prev(pos, head) \
		// 	for (pos = (head)->prev; !list_is_head(pos, (head)); pos = pos->prev)

		// /**
		//  * list_for_each_safe - iterate over a list safe against removal of list entry
		//  * @pos:	the &List_s to use as a loop cursor.
		//  * @n:		another &List_s to use as temporary storage
		//  * @head:	the head for your list.
		//  */
		// #define list_for_each_safe(pos, n, head) \
		// 	for (pos = (head)->next, n = pos->next; \
		// 		!list_is_head(pos, (head)); \
		// 		pos = n, n = pos->next)

		// /**
		//  * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
		//  * @pos:	the &List_s to use as a loop cursor.
		//  * @n:		another &List_s to use as temporary storage
		//  * @head:	the head for your list.
		//  */
		// #define list_for_each_prev_safe(pos, n, head) \
		// 	for (pos = (head)->prev, n = pos->prev; \
		// 		!list_is_head(pos, (head)); \
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



	void list_init(List_s * src);
	List_s * list_get_prev(List_s * src);
	List_s * list_get_next(List_s * src);
	void list_insert_prev(List_s * src, List_s * dst);
	void list_insert_next(List_s * src, List_s * dst);
	void list_delete(List_s * src);
	
	bool list_in_lhdr(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_init(List_hdr_s * lh_p);
	void list_hdr_push(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_pop(List_hdr_s * lhdr_p);
	void list_hdr_append(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_enqueue(List_hdr_s * lhdr_p, List_s * l_p);
	List_s * list_hdr_dequeue(List_hdr_s * lhdr_p);
	List_s * list_hdr_delete(List_hdr_s * lhdr_p, List_s * l_p);
	void list_hdr_dump(List_hdr_s *lhdr_p);

	typedef int(* list_traverse_do)(List_s * curr, List_s * target);
	typedef	list_traverse_do	list_traverse_check;
	typedef	list_traverse_do	list_traverse_end;
	void list_foreach_do(List_hdr_s * lhdr,
			List_s * target, list_traverse_do do_hdlr);
	void list_search_and_do(List_hdr_s * lhdr, List_s * target,
			list_traverse_check chk_hdlr, list_traverse_do do_hdlr,
			list_traverse_end end_hdlr);

	#define list_hdr_foreach(pos, lhdr)	\
				list_for_each(pos, (&(lhdr->header)))

	#define list_hdr_foreach_entry(lhdr, member)		\
			for ((member) = (lhdr)->header.next;		\
				(member) != &(lhdr)->header;		\
				(member) = (member)->next)

	static inline bool
	list_hdr_contains(List_hdr_s * lhdr, List_s * target) {
		List_s *tmp = NULL;
		list_hdr_foreach(tmp, lhdr)
		{
			if (tmp == target)
				return true;
		}
		return false;
	}

#endif /* _LINUX_LIST_H_ */