/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HASH_LIST_H_
#define _LINUX_HASH_LIST_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "hash_list_types.h"

	/*
	 * Double linked lists with a single pointer list head.
	 * Mostly useful for hash tables where the two pointer list head is
	 * too wasteful.
	 * You lose the ability to access the tail in O(1).
	 */

	#ifdef DEBUG



	#endif

	#include "hash_list_macro.h"
	
	#if defined(HLIST_DEFINATION) || !(DEBUG)

		// #define HLIST_HEAD_INIT { .first = NULL }
		// #define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
		// #define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
		// static inline void INIT_HLIST_NODE(struct hlist_node *h)
		// {
		// 	h->next = NULL;
		// 	h->pprev = NULL;
		// }

		// /**
		//  * hlist_unhashed - Has node been removed from list and reinitialized?
		//  * @h: Node to be checked
		//  *
		//  * Not that not all removal functions will leave a node in unhashed
		//  * state.  For example, hlist_nulls_del_init_rcu() does leave the
		//  * node in unhashed state, but hlist_nulls_del() does not.
		//  */
		// static inline int hlist_unhashed(const struct hlist_node *h)
		// {
		// 	return !h->pprev;
		// }

		// /**
		//  * hlist_unhashed_lockless - Version of hlist_unhashed for lockless use
		//  * @h: Node to be checked
		//  *
		//  * This variant of hlist_unhashed() must be used in lockless contexts
		//  * to avoid potential load-tearing.  The READ_ONCE() is paired with the
		//  * various WRITE_ONCE() in hlist helpers that are defined below.
		//  */
		// static inline int hlist_unhashed_lockless(const struct hlist_node *h)
		// {
		// 	return !READ_ONCE(h->pprev);
		// }

		// /**
		//  * hlist_empty - Is the specified hlist_head structure an empty hlist?
		//  * @h: Structure to check.
		//  */
		// static inline int hlist_empty(const struct hlist_head *h)
		// {
		// 	return !READ_ONCE(h->first);
		// }

		// static inline void __hlist_del(struct hlist_node *n)
		// {
		// 	struct hlist_node *next = n->next;
		// 	struct hlist_node **pprev = n->pprev;

		// 	WRITE_ONCE(*pprev, next);
		// 	if (next)
		// 		WRITE_ONCE(next->pprev, pprev);
		// }

		// /**
		//  * hlist_del - Delete the specified hlist_node from its list
		//  * @n: Node to delete.
		//  *
		//  * Note that this function leaves the node in hashed state.  Use
		//  * hlist_del_init() or similar instead to unhash @n.
		//  */
		// static inline void hlist_del(struct hlist_node *n)
		// {
		// 	__hlist_del(n);
		// 	n->next = LIST_POISON1;
		// 	n->pprev = LIST_POISON2;
		// }

		// /**
		//  * hlist_del_init - Delete the specified hlist_node from its list and initialize
		//  * @n: Node to delete.
		//  *
		//  * Note that this function leaves the node in unhashed state.
		//  */
		// static inline void hlist_del_init(struct hlist_node *n)
		// {
		// 	if (!hlist_unhashed(n)) {
		// 		__hlist_del(n);
		// 		INIT_HLIST_NODE(n);
		// 	}
		// }

		// /**
		//  * hlist_add_head - add a new entry at the beginning of the hlist
		//  * @n: new entry to be added
		//  * @h: hlist head to add it after
		//  *
		//  * Insert a new entry after the specified head.
		//  * This is good for implementing stacks.
		//  */
		// static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
		// {
		// 	struct hlist_node *first = h->first;
		// 	WRITE_ONCE(n->next, first);
		// 	if (first)
		// 		WRITE_ONCE(first->pprev, &n->next);
		// 	WRITE_ONCE(h->first, n);
		// 	WRITE_ONCE(n->pprev, &h->first);
		// }

		// /**
		//  * hlist_add_before - add a new entry before the one specified
		//  * @n: new entry to be added
		//  * @next: hlist node to add it before, which must be non-NULL
		//  */
		// static inline void hlist_add_before(struct hlist_node *n,
		// 					struct hlist_node *next)
		// {
		// 	WRITE_ONCE(n->pprev, next->pprev);
		// 	WRITE_ONCE(n->next, next);
		// 	WRITE_ONCE(next->pprev, &n->next);
		// 	WRITE_ONCE(*(n->pprev), n);
		// }

		// /**
		//  * hlist_add_behind - add a new entry after the one specified
		//  * @n: new entry to be added
		//  * @prev: hlist node to add it after, which must be non-NULL
		//  */
		// static inline void hlist_add_behind(struct hlist_node *n,
		// 					struct hlist_node *prev)
		// {
		// 	WRITE_ONCE(n->next, prev->next);
		// 	WRITE_ONCE(prev->next, n);
		// 	WRITE_ONCE(n->pprev, &prev->next);

		// 	if (n->next)
		// 		WRITE_ONCE(n->next->pprev, &n->next);
		// }

		// /**
		//  * hlist_add_fake - create a fake hlist consisting of a single headless node
		//  * @n: Node to make a fake list out of
		//  *
		//  * This makes @n appear to be its own predecessor on a headless hlist.
		//  * The point of this is to allow things like hlist_del() to work correctly
		//  * in cases where there is no list.
		//  */
		// static inline void hlist_add_fake(struct hlist_node *n)
		// {
		// 	n->pprev = &n->next;
		// }

		// /**
		//  * hlist_fake: Is this node a fake hlist?
		//  * @h: Node to check for being a self-referential fake hlist.
		//  */
		// static inline bool hlist_fake(struct hlist_node *h)
		// {
		// 	return h->pprev == &h->next;
		// }

		// /**
		//  * hlist_is_singular_node - is node the only element of the specified hlist?
		//  * @n: Node to check for singularity.
		//  * @h: Header for potentially singular list.
		//  *
		//  * Check whether the node is the only node of the head without
		//  * accessing head, thus avoiding unnecessary cache misses.
		//  */
		// static inline bool
		// hlist_is_singular_node(struct hlist_node *n, struct hlist_head *h)
		// {
		// 	return !n->next && n->pprev == &h->first;
		// }

		// /**
		//  * hlist_move_list - Move an hlist
		//  * @old: hlist_head for old list.
		//  * @new: hlist_head for new list.
		//  *
		//  * Move a list from one list head to another. Fixup the pprev
		//  * reference of the first entry if it exists.
		//  */
		// static inline void hlist_move_list(struct hlist_head *old,
		// 				struct hlist_head *new)
		// {
		// 	new->first = old->first;
		// 	if (new->first)
		// 		new->first->pprev = &new->first;
		// 	old->first = NULL;
		// }

	#endif

#endif /* _LINUX_HASH_LIST_H_ */