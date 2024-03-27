/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DOUBLE_LIST_H_
#define _LINUX_DOUBLE_LIST_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "double_list_types.h"

	/*
	 * Circular doubly linked list implementation.
	 *
	 * Some of the internal functions ("__xxx") are useful when
	 * manipulating whole lists rather than single entries, as
	 * sometimes we already know the next/prev entries and we can
	 * generate better code by using them directly rather than
	 * using the generic single-entry routines.
	 */

	#ifdef DEBUG

		extern void
		INIT_LIST_S(List_s *list);
		extern void
		INIT_LIST_HEADER_S(List_hdr_s *lhdr);

		extern bool
		__list_add_valid(List_s *new, List_s *prev, List_s *next);
		extern bool
		__list_del_entry_valid(List_s *entry);

		extern void
		__list_add_between(List_s *new, List_s *prev, List_s *next);
		extern void
		list_add_to_next(List_s *new, List_s *head);
		extern void
		list_add_to_prev(List_s *new, List_s *head);

		extern void
		__list_del(List_s * prev, List_s * next);
		extern void
		__list_del_clearprev(List_s *entry);
		extern void
		__list_del_entry(List_s *entry);
		extern void
		list_del_init(List_s *entry);
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
		list_is_first_entry(const List_s *list, const List_s *head);
		extern bool
		list_is_last_entry(const List_s *list, const List_s *head);
		extern bool
		list_is_head_anchor(const List_s *list, const List_s *head);
		extern bool
		list_is_empty_entry(const List_s *head);
		extern bool
		list_is_empty_entry_careful(const List_s *head);
		extern bool
		list_is_singular(const List_s *head);

		extern void
		list_rotate_left(List_s *head);
		extern void
		list_rotate_to_front(List_s *list, List_s *head);

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
		list_header_is_empty(const List_hdr_s *lhdr_p);
		extern bool
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p);

		extern void
		list_header_push(List_hdr_s *lhdr_p, List_s *l_p);
		extern List_s
		*list_header_pop(List_hdr_s *lhdr_p);
		
		extern void
		list_header_enqueue(List_hdr_s *lhdr_p, List_s *l_p);
		extern List_s
		*list_header_dequeue(List_hdr_s *lhdr_p);

		extern List_s
		*list_delete_from_header(List_hdr_s *lhdr_p, List_s *l_p);

	#endif

		#define INIT_LIST_HEAD(list) INIT_LIST_S(list)

		#define list_add list_add_to_next
		#define list_add_tail list_add_to_prev

		#define list_is_first list_is_first_entry
		#define list_is_last list_is_last_entry
		#define list_is_head list_is_head_anchor
		#define list_empty list_is_empty_entry
		#define list_empty_careful list_is_empty_entry_careful

	#include "double_list_macros.h"
	
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
		INIT_LIST_HEADER_S(List_hdr_s *lhdr) {
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
		__list_add_between(List_s *new, List_s *prev, List_s *next) {
			if (!__list_add_valid(new, prev, next))
				return;

			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
		}
		/**
		 * list_add_to_next - add a new entry
		 * @new: new entry to be added
		 * @head: list head to add it after
		 *
		 * Insert a new entry after the specified head.
		 * This is good for implementing stacks.
		 */
		PREFIX_STATIC_INLINE
		void
		list_add_to_next(List_s *new, List_s *head) {
			__list_add_between(new, head, head->next);
		}
		/**
		 * list_add_to_prev - add a new entry
		 * @new: new entry to be added
		 * @head: list head to add it before
		 *
		 * Insert a new entry before the specified head.
		 * This is useful for implementing queues.
		 */
		PREFIX_STATIC_INLINE
		void
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
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
		 * needs to check the node 'prev' pointer instead of calling list_is_empty_entry().
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
		 * Note: list_is_empty_entry() on entry does not return true after this, the entry is
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
		//  * together with list_is_empty_entry_careful() in a way to guarantee ordering
		//  * of other memory operations.
		//  *
		//  * Any memory operations done before a list_del_init_careful() are
		//  * guaranteed to be visible after a list_is_empty_entry_careful() test.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_del_init_careful(List_s *entry) {
		// 	__list_del_entry(entry);
		// 	entry->prev = entry;
		// 	smp_store_release(&entry->next, entry);
		// }


		// /**
		//  * list_replace - replace old entry by new one
		//  * @old : the element to be replaced
		//  * @new : the new element to insert
		//  *
		//  * If @old was empty, it will be overwritten.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_replace(List_s *old, List_s *new) {
		// 	new->next = old->next;
		// 	new->next->prev = new;
		// 	new->prev = old->prev;
		// 	new->prev->next = new;
		// }
		// /**
		//  * list_replace_init - replace old entry by new one and initialize the old one
		//  * @old : the element to be replaced
		//  * @new : the new element to insert
		//  *
		//  * If @old was empty, it will be overwritten.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_replace_init(List_s *old, List_s *new) {
		// 	list_replace(old, new);
		// 	INIT_LIST_HEAD(old);
		// }

		// /**
		//  * list_swap - replace entry1 with entry2 and re-add entry1 at entry2's position
		//  * @entry1: the location to place entry2
		//  * @entry2: the location to place entry1
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_swap(List_s *entry1, List_s *entry2) {
		// 	List_s *pos = entry2->prev;

		// 	list_del_init(entry2);
		// 	list_replace(entry1, entry2);
		// 	if (pos == entry1)
		// 		pos = entry2;
		// 	list_add_to_next(entry1, pos);
		// }

		/**
		 * list_move - delete from one list and add as another's head
		 * @list: the entry to move
		 * @head: the head that will precede our entry
		 */
		PREFIX_STATIC_INLINE
		void
		list_move(List_s *list, List_s *head) {
			__list_del_entry(list);
			list_add_to_next(list, head);
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
			list_add_to_prev(list, head);
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
		 * list_is_first_entry -- tests whether @list is the first entry in list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_first_entry(const List_s *list, const List_s *head) {
			return list->prev == head;
		}
		/**
		 * list_is_last_entry - tests whether @list is the last entry in list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_last_entry(const List_s *list, const List_s *head) {
			return list->next == head;
		}
		/**
		 * list_is_head_anchor - tests whether @list is the list @head
		 * @list: the entry to test
		 * @head: the head of the list
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_head_anchor(const List_s *list, const List_s *head) {
			return list == head;
		}
		/**
		 * list_is_empty_entry - tests whether a list is empty
		 * @head: the list to test.
		 */
		PREFIX_STATIC_INLINE
		bool
		list_is_empty_entry(const List_s *head) {
			return READ_ONCE(head->next) == head;
		}
		// /**
		//  * list_is_empty_entry_careful - tests whether a list is empty and not being modified
		//  * @head: the list to test
		//  *
		//  * Description:
		//  * tests whether a list is empty _and_ checks that no other CPU might be
		//  * in the process of modifying either member (next or prev)
		//  *
		//  * NOTE: using list_is_empty_entry_careful() without synchronization
		//  * can only be safe if the only activity that can happen
		//  * to the list entry is list_del_init(). Eg. it cannot be used
		//  * if another CPU could re-list_add_to_next() it.
		//  */
		// PREFIX_STATIC_INLINE
		// bool
		// list_is_empty_entry_careful(const List_s *head) {
		// 	List_s *next = smp_load_acquire(&head->next);
		// 	return list_is_head_anchor(next, head) && (next == head->prev);
		// }
		// /**
		//  * list_is_singular - tests whether a list has just one entry.
		//  * @head: the list to test.
		//  */
		// PREFIX_STATIC_INLINE
		// bool
		// list_is_singular(const List_s *head) {
		// 	return !list_is_empty_entry(head) && (head->next == head->prev);
		// }

		// /**
		//  * list_rotate_left - rotate the list to the left
		//  * @head: the head of the list
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_rotate_left(List_s *head) {
		// 	List_s *first;

		// 	if (!list_is_empty_entry(head)) {
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
		// 	if (list_is_empty_entry(head))
		// 		return;
		// 	if (list_is_singular(head) && !list_is_head_anchor(entry, head) && (entry != head->next))
		// 		return;
		// 	if (list_is_head_anchor(entry, head))
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
		// 	if (!list_is_empty_entry(list))
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
		// 	if (!list_is_empty_entry(list))
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
		// 	if (!list_is_empty_entry(list)) {
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
		// 	if (!list_is_empty_entry(list)) {
		// 		__list_splice(list, head->prev, head);
		// 		INIT_LIST_HEAD(list);
		// 	}
		// }



		PREFIX_STATIC_INLINE
		bool
		list_header_is_empty(const List_hdr_s *lhdr_p) {
			return READ_ONCE(lhdr_p->count) == 0;
		}
		PREFIX_STATIC_INLINE
		bool
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
			}
			return false;
		}

		PREFIX_STATIC_INLINE
		void
		list_header_push(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_next(l_p, &lhdr_p->header);
			lhdr_p->count++;
		}
		PREFIX_STATIC_INLINE
		List_s
		*list_header_pop(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->header.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
			}
			return NULL;
		}

		PREFIX_STATIC_INLINE
		void
		list_header_enqueue(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->header);
			lhdr_p->count++;
		}
		PREFIX_STATIC_INLINE
		List_s
		*list_header_dequeue(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->header.prev;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
			}

			return NULL;
		}

		PREFIX_STATIC_INLINE
		List_s
		*list_delete_from_header(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
			}

			return NULL;
		}

	#endif

#endif /* _LINUX_DOUBLE_LIST_H_ */