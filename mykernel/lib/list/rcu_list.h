/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RCU_LIST_H_
#define _LINUX_RCU_LIST_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../lib_const.h"
	#include "../lib_types.h"
	#include "../lib_api.h"


	/*
	 * Double linked lists with a single pointer list head.
	 * Mostly useful for hash tables where the two pointer list head is
	 * too wasteful.
	 * You lose the ability to access the tail in O(1).
	 */

	#ifdef DEBUG

		extern void
		INIT_LIST_HEAD_RCU(List_s *list);

		extern void
		__list_add_rcu(List_s *new, List_s *prev, List_s *next);

		extern void
		list_add_rcu(List_s *new, List_s *head);

		extern void
		list_add_tail_rcu(List_s *new, List_s *head);

		extern void
		list_del_rcu(List_s *entry);

		extern void
		list_replace_rcu(List_s *old, List_s *new);



		extern void
		hlist_del_init_rcu(HList_s *n);

		extern void
		hlist_del_rcu(HList_s *n);

		extern void
		hlist_replace_rcu(HList_s *old, HList_s *new);

		extern void
		hlists_swap_heads_rcu(HList_hdr_s *left, HList_hdr_s *right);

		extern void
		hlist_add_head_rcu(HList_s *n, HList_hdr_s *h);

		extern void
		hlist_add_tail_rcu(HList_s *n, HList_hdr_s *h);

		extern void
		hlist_add_before_rcu(HList_s *n, HList_s *next);

		extern void
		hlist_add_behind_rcu(HList_s *n, HList_s *prev);

	#endif

	#include "rcu_list_macro.h"
	
	#if defined(RCULIST_DEFINATION) || !(DEBUG)

		/*
		 * INIT_LIST_HEAD_RCU - Initialize a list_head visible to RCU readers
		 * @list: list to be initialized
		 *
		 * You should instead use INIT_LIST_HEAD() for normal initialization and
		 * cleanup tasks, when readers have no access to the list being initialized.
		 * However, if the list being initialized is visible to readers, you
		 * need to keep the compiler from being too mischievous.
		 */
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_HEAD_RCU(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}

		/*
		 * Insert a new entry between two known consecutive entries.
		 *
		 * This is only for internal list manipulation where we know
		 * the prev/next entries already!
		 */
		PREFIX_STATIC_INLINE
		void
		__list_add_rcu(List_s *new, List_s *prev, List_s *next) {
			if (!__list_add_valid(new, prev, next))
				return;

			new->next = next;
			new->prev = prev;
			rcu_assign_pointer(list_next_rcu(prev), new);
			next->prev = new;
		}

		/**
		 * list_add_rcu - add a new entry to rcu-protected list
		 * @new: new entry to be added
		 * @head: list head to add it after
		 *
		 * Insert a new entry after the specified head.
		 * This is good for implementing stacks.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as list_add_rcu()
		 * or list_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * list_for_each_entry_rcu().
		 */
		PREFIX_STATIC_INLINE
		void
		list_add_rcu(List_s *new, List_s *head) {
			__list_add_rcu(new, head, head->next);
		}

		/**
		 * list_add_tail_rcu - add a new entry to rcu-protected list
		 * @new: new entry to be added
		 * @head: list head to add it before
		 *
		 * Insert a new entry before the specified head.
		 * This is useful for implementing queues.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as list_add_tail_rcu()
		 * or list_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * list_for_each_entry_rcu().
		 */
		PREFIX_STATIC_INLINE
		void
		list_add_tail_rcu(List_s *new, List_s *head) {
			__list_add_rcu(new, head->prev, head);
		}

		/**
		 * list_del_rcu - deletes entry from list without re-initialization
		 * @entry: the element to delete from the list.
		 *
		 * Note: list_empty() on entry does not return true after this,
		 * the entry is in an undefined state. It is useful for RCU based
		 * lockfree traversal.
		 *
		 * In particular, it means that we can not poison the forward
		 * pointers that may still be used for walking the list.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as list_del_rcu()
		 * or list_add_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * list_for_each_entry_rcu().
		 *
		 * Note that the caller is not permitted to immediately free
		 * the newly deleted entry.  Instead, either synchronize_rcu()
		 * or call_rcu() must be used to defer freeing until an RCU
		 * grace period has elapsed.
		 */
		PREFIX_STATIC_INLINE
		void
		list_del_rcu(List_s *entry) {
			__list_del_entry(entry);
			entry->prev = LIST_POISON2;
		}

		/**
		 * list_replace_rcu - replace old entry by new one
		 * @old : the element to be replaced
		 * @new : the new element to insert
		 *
		 * The @old entry will be replaced with the @new entry atomically.
		 * Note: @old should not be empty.
		 */
		PREFIX_STATIC_INLINE
		void
		list_replace_rcu(List_s *old, List_s *new) {
			new->next = old->next;
			new->prev = old->prev;
			rcu_assign_pointer(list_next_rcu(new->prev), new);
			new->next->prev = new;
			old->prev = LIST_POISON2;
		}

		// /**
		//  * __list_splice_init_rcu - join an RCU-protected list into an existing list.
		//  * @list:	the RCU-protected list to splice
		//  * @prev:	points to the last element of the existing list
		//  * @next:	points to the first element of the existing list
		//  * @sync:	synchronize_rcu, synchronize_rcu_expedited, ...
		//  *
		//  * The list pointed to by @prev and @next can be RCU-read traversed
		//  * concurrently with this function.
		//  *
		//  * Note that this function blocks.
		//  *
		//  * Important note: the caller must take whatever action is necessary to prevent
		//  * any other updates to the existing list.  In principle, it is possible to
		//  * modify the list as soon as sync() begins execution. If this sort of thing
		//  * becomes necessary, an alternative version based on call_rcu() could be
		//  * created.  But only if -really- needed -- there is no shortage of RCU API
		//  * members.
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// __list_splice_init_rcu(List_s *list,
		// 		List_s *prev, List_s *next, void (*sync)(void)) {

		// 	List_s *first = list->next;
		// 	List_s *last = list->prev;

		// 	/*
		// 	 * "first" and "last" tracking list, so initialize it.  RCU readers
		// 	 * have access to this list, so we must use INIT_LIST_HEAD_RCU()
		// 	 * instead of INIT_LIST_HEAD().
		// 	 */

		// 	INIT_LIST_HEAD_RCU(list);

		// 	/*
		// 	 * At this point, the list body still points to the source list.
		// 	 * Wait for any readers to finish using the list before splicing
		// 	 * the list body into the new list.  Any new readers will see
		// 	 * an empty list.
		// 	 */

		// 	sync();
		// 	ASSERT_EXCLUSIVE_ACCESS(*first);
		// 	ASSERT_EXCLUSIVE_ACCESS(*last);

		// 	/*
		// 	 * Readers are finished with the source list, so perform splice.
		// 	 * The order is important if the new list is global and accessible
		// 	 * to concurrent RCU readers.  Note that RCU readers are not
		// 	 * permitted to traverse the prev pointers without excluding
		// 	 * this function.
		// 	 */

		// 	last->next = next;
		// 	rcu_assign_pointer(list_next_rcu(prev), first);
		// 	first->prev = prev;
		// 	next->prev = last;
		// }

		// /**
		//  * list_splice_init_rcu - splice an RCU-protected list into an existing list,
		//  *                        designed for stacks.
		//  * @list:	the RCU-protected list to splice
		//  * @head:	the place in the existing list to splice the first list into
		//  * @sync:	synchronize_rcu, synchronize_rcu_expedited, ...
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice_init_rcu(List_s *list,
		// 		List_s *head, void (*sync)(void)) {

		// 	if (!list_empty(list))
		// 		__list_splice_init_rcu(list, head, head->next, sync);
		// }

		// /**
		//  * list_splice_tail_init_rcu - splice an RCU-protected list into an existing
		//  *                             list, designed for queues.
		//  * @list:	the RCU-protected list to splice
		//  * @head:	the place in the existing list to splice the first list into
		//  * @sync:	synchronize_rcu, synchronize_rcu_expedited, ...
		//  */
		// PREFIX_STATIC_INLINE
		// void
		// list_splice_tail_init_rcu(List_s *list,
		// 		List_s *head, void (*sync)(void)) {

		// 	if (!list_empty(list))
		// 		__list_splice_init_rcu(list, head->prev, head, sync);
		// }




		/**
		 * hlist_del_init_rcu - deletes entry from hash list with re-initialization
		 * @n: the element to delete from the hash list.
		 *
		 * Note: list_unhashed() on the node return true after this. It is
		 * useful for RCU based read lockfree traversal if the writer side
		 * must know if the list entry is still hashed or already unhashed.
		 *
		 * In particular, it means that we can not poison the forward pointers
		 * that may still be used for walking the hash list and we can only
		 * zero the pprev pointer so list_unhashed() will return true after
		 * this.
		 *
		 * The caller must take whatever precautions are necessary (such as
		 * holding appropriate locks) to avoid racing with another
		 * list-mutation primitive, such as hlist_add_head_rcu() or
		 * hlist_del_rcu(), running on this same list.  However, it is
		 * perfectly legal to run concurrently with the _rcu list-traversal
		 * primitives, such as hlist_for_each_entry_rcu().
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_del_init_rcu(HList_s *n) {
			if (!hlist_unhashed(n)) {
				__hlist_del(n);
				WRITE_ONCE(n->pprev, NULL);
			}
		}

		/**
		 * hlist_del_rcu - deletes entry from hash list without re-initialization
		 * @n: the element to delete from the hash list.
		 *
		 * Note: list_unhashed() on entry does not return true after this,
		 * the entry is in an undefined state. It is useful for RCU based
		 * lockfree traversal.
		 *
		 * In particular, it means that we can not poison the forward
		 * pointers that may still be used for walking the hash list.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as hlist_add_head_rcu()
		 * or hlist_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * hlist_for_each_entry().
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_del_rcu(HList_s *n) {
			__hlist_del(n);
			WRITE_ONCE(n->pprev, LIST_POISON2);
		}

		/**
		 * hlist_replace_rcu - replace old entry by new one
		 * @old : the element to be replaced
		 * @new : the new element to insert
		 *
		 * The @old entry will be replaced with the @new entry atomically.
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_replace_rcu(HList_s *old, HList_s *new) {
			HList_s *next = old->next;

			new->next = next;
			WRITE_ONCE(new->pprev, old->pprev);
			rcu_assign_pointer(*(HList_s __rcu **)new->pprev, new);
			if (next)
				WRITE_ONCE(new->next->pprev, &new->next);
			WRITE_ONCE(old->pprev, LIST_POISON2);
		}

		/**
		 * hlists_swap_heads_rcu - swap the lists the hlist heads point to
		 * @left:  The hlist head on the left
		 * @right: The hlist head on the right
		 *
		 * The lists start out as [@left  ][node1 ... ] and
		 *                        [@right ][node2 ... ]
		 * The lists end up as    [@left  ][node2 ... ]
		 *                        [@right ][node1 ... ]
		 */
		PREFIX_STATIC_INLINE
		void
		hlists_swap_heads_rcu(HList_hdr_s *left, HList_hdr_s *right) {
			HList_s *node1 = left->first;
			HList_s *node2 = right->first;

			rcu_assign_pointer(left->first, node2);
			rcu_assign_pointer(right->first, node1);
			WRITE_ONCE(node2->pprev, &left->first);
			WRITE_ONCE(node1->pprev, &right->first);
		}

		/**
		 * hlist_add_head_rcu
		 * @n: the element to add to the hash list.
		 * @h: the list to add to.
		 *
		 * Description:
		 * Adds the specified element to the specified hlist,
		 * while permitting racing traversals.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as hlist_add_head_rcu()
		 * or hlist_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * hlist_for_each_entry_rcu(), used to prevent memory-consistency
		 * problems on Alpha CPUs.  Regardless of the type of CPU, the
		 * list-traversal primitive must be guarded by rcu_read_lock().
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_add_head_rcu(HList_s *n, HList_hdr_s *h) {
			HList_s *first = h->first;

			n->next = first;
			WRITE_ONCE(n->pprev, &h->first);
			rcu_assign_pointer(hlist_first_rcu(h), n);
			if (first)
				WRITE_ONCE(first->pprev, &n->next);
		}

		/**
		 * hlist_add_tail_rcu
		 * @n: the element to add to the hash list.
		 * @h: the list to add to.
		 *
		 * Description:
		 * Adds the specified element to the specified hlist,
		 * while permitting racing traversals.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as hlist_add_head_rcu()
		 * or hlist_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * hlist_for_each_entry_rcu(), used to prevent memory-consistency
		 * problems on Alpha CPUs.  Regardless of the type of CPU, the
		 * list-traversal primitive must be guarded by rcu_read_lock().
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_add_tail_rcu(HList_s *n, HList_hdr_s *h) {
			HList_s *i, *last = NULL;

			/* Note: write side code, so rcu accessors are not needed. */
			for (i = h->first; i; i = i->next)
				last = i;

			if (last) {
				n->next = last->next;
				WRITE_ONCE(n->pprev, &last->next);
				rcu_assign_pointer(hlist_next_rcu(last), n);
			} else {
				hlist_add_head_rcu(n, h);
			}
		}

		/**
		 * hlist_add_before_rcu
		 * @n: the new element to add to the hash list.
		 * @next: the existing element to add the new element before.
		 *
		 * Description:
		 * Adds the specified element to the specified hlist
		 * before the specified node while permitting racing traversals.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as hlist_add_head_rcu()
		 * or hlist_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * hlist_for_each_entry_rcu(), used to prevent memory-consistency
		 * problems on Alpha CPUs.
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_add_before_rcu(HList_s *n, HList_s *next) {
			WRITE_ONCE(n->pprev, next->pprev);
			n->next = next;
			rcu_assign_pointer(hlist_pprev_rcu(n), n);
			WRITE_ONCE(next->pprev, &n->next);
		}

		/**
		 * hlist_add_behind_rcu
		 * @n: the new element to add to the hash list.
		 * @prev: the existing element to add the new element after.
		 *
		 * Description:
		 * Adds the specified element to the specified hlist
		 * after the specified node while permitting racing traversals.
		 *
		 * The caller must take whatever precautions are necessary
		 * (such as holding appropriate locks) to avoid racing
		 * with another list-mutation primitive, such as hlist_add_head_rcu()
		 * or hlist_del_rcu(), running on this same list.
		 * However, it is perfectly legal to run concurrently with
		 * the _rcu list-traversal primitives, such as
		 * hlist_for_each_entry_rcu(), used to prevent memory-consistency
		 * problems on Alpha CPUs.
		 */
		PREFIX_STATIC_INLINE
		void
		hlist_add_behind_rcu(HList_s *n, HList_s *prev) {
			n->next = prev->next;
			WRITE_ONCE(n->pprev, &prev->next);
			rcu_assign_pointer(hlist_next_rcu(prev), n);
			if (n->next)
				WRITE_ONCE(n->next->pprev, &n->next);
		}

	#endif

#endif /* _LINUX_RCU_LIST_H_ */