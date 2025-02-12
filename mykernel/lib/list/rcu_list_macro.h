/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RCU_LIST_MACROS_H_
#define _LINUX_RCU_LIST_MACROS_H_

	/*
	 * return the ->next pointer of a list_head in an rcu safe
	 * way, we must not access it directly
	 */
	#define list_next_rcu(list)	(					\
				*((List_s __rcu **)(&(list)->next))	\
			)

	/**
	 * list_tail_rcu - returns the prev pointer of the head of the list
	 * @head: the head of the list
	 *
	 * Note: This should only be used with the list header, and even then
	 * only if list_del() and similar primitives are not also used on the
	 * list header.
	 */
	#define list_tail_rcu(head)	(					\
				*((List_s __rcu **)(&(head)->prev))	\
			)

	/**
	 * rcu_assign_pointer() - assign to RCU-protected pointer
	 * @p: pointer to assign to
	 * @v: value to assign (publish)
	 *
	 * Assigns the specified value to the specified RCU-protected
	 * pointer, ensuring that any concurrent RCU readers will see
	 * any prior initialization.
	 *
	 * Inserts memory barriers on architectures that require them
	 * (which is most of them), and also prevents the compiler from
	 * reordering the code that initializes the structure after the pointer
	 * assignment.  More importantly, this call documents which pointers
	 * will be dereferenced by RCU read-side code.
	 *
	 * In some special cases, you may use RCU_INIT_POINTER() instead
	 * of rcu_assign_pointer().  RCU_INIT_POINTER() is a bit faster due
	 * to the fact that it does not constrain either the CPU or the compiler.
	 * That said, using RCU_INIT_POINTER() when you should have used
	 * rcu_assign_pointer() is a very bad thing that results in
	 * impossible-to-diagnose memory corruption.  So please be careful.
	 * See the RCU_INIT_POINTER() comment header for details.
	 *
	 * Note that rcu_assign_pointer() evaluates each of its arguments only
	 * once, appearances notwithstanding.  One of the "extra" evaluations
	 * is in typeof() and the other visible only to sparse (__CHECKER__),
	 * neither of which actually execute the argument.  As with most cpp
	 * macros, this execute-arguments-only-once property is important, so
	 * please be careful when making changes to rcu_assign_pointer() and the
	 * other macros that it invokes.
	 */
	// #define rcu_assign_pointer(p, v)					      \
	// do {									      \
	// 	uintptr_t _r_a_p__v = (uintptr_t)(v);				      \
	// 	rcu_check_sparse(p, __rcu);					      \
	// 										\
	// 	if (__builtin_constant_p(v) && (_r_a_p__v) == (uintptr_t)NULL)	      \
	// 		WRITE_ONCE((p), (typeof(p))(_r_a_p__v));		      \
	// 	else								      \
	// 		smp_store_release(&p, RCU_INITIALIZER((typeof(p))_r_a_p__v)); \
	// } while (0)
	#define rcu_assign_pointer(p, v)	((p) = (v))

	/**
	 * rcu_replace_pointer() - replace an RCU pointer, returning its old value
	 * @rcu_ptr: RCU pointer, whose old value is returned
	 * @ptr: regular pointer
	 * @c: the lockdep conditions under which the dereference will take place
	 *
	 * Perform a replacement, where @rcu_ptr is an RCU-annotated
	 * pointer and @c is the lockdep argument that is passed to the
	 * rcu_dereference_protected() call used to read that pointer.  The old
	 * value of @rcu_ptr is returned, and @rcu_ptr is set to @ptr.
	 */
	// #define rcu_replace_pointer(rcu_ptr, ptr, c)				\
	// ({									\
	// 	typeof(ptr) __tmp = rcu_dereference_protected((rcu_ptr), (c));	\
	// 	rcu_assign_pointer((rcu_ptr), (ptr));				\
	// 	__tmp;								\
	// })


	/**
	 * rcu_dereference() - fetch RCU-protected pointer for dereferencing
	 * @p: The pointer to read, prior to dereferencing
	 *
	 * This is a simple wrapper around rcu_dereference_check().
	 */
	// #define rcu_dereference(p) rcu_dereference_check(p, 0)
	#define rcu_dereference(p)	(p)




	/**
	 * list_entry_rcu - get the struct for this entry
	 * @ptr:        the &struct list_head pointer.
	 * @type:       the type of the struct this is embedded in.
	 * @member:     the name of the list_head within the struct.
	 *
	 * This primitive may safely run concurrently with the _rcu list-mutation
	 * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
	 */
	// #define list_entry_rcu(ptr, type, member)	\
	// 			container_of(READ_ONCE(ptr), type, member)
	#define list_entry_rcu(ptr, type, member)	\
				container_of(ptr, type, member)

	/*
	 * Where are list_empty_rcu() and list_first_entry_rcu()?
	 *
	 * They do not exist because they would lead to subtle race conditions:
	 *
	 * if (!list_empty_rcu(mylist)) {
	 *	struct foo *bar = list_first_entry_rcu(mylist, struct foo, list_member);
	 *	do_something(bar);
	 * }
	 *
	 * The list might be non-empty when list_empty_rcu() checks it, but it
	 * might have become empty by the time that list_first_entry_rcu() rereads
	 * the ->next pointer, which would result in a SEGV.
	 *
	 * When not using RCU, it is OK for list_first_entry() to re-read that
	 * pointer because both functions should be protected by some lock that
	 * blocks writers.
	 *
	 * When using RCU, list_empty() uses READ_ONCE() to fetch the
	 * RCU-protected ->next pointer and then compares it to the address of the
	 * list head.  However, it neither dereferences this pointer nor provides
	 * this pointer to its caller.  Thus, READ_ONCE() suffices (that is,
	 * rcu_dereference() is not needed), which means that list_empty() can be
	 * used anywhere you would want to use list_empty_rcu().  Just don't
	 * expect anything useful to happen if you do a subsequent lockless
	 * call to list_first_entry_rcu()!!!
	 *
	 * See list_first_or_null_rcu for an alternative.
	 */

	/**
	 * list_first_or_null_rcu - get the first element from a list
	 * @ptr:        the list head to take the element from.
	 * @type:       the type of the struct this is embedded in.
	 * @member:     the name of the list_head within the struct.
	 *
	 * Note that if the list is empty, it returns NULL.
	 *
	 * This primitive may safely run concurrently with the _rcu list-mutation
	 * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
	 */
	#define list_first_or_null_rcu(ptr, type, member)	({		\
				List_s *__ptr = (ptr);							\
				List_s *__next = READ_ONCE(__ptr->next);		\
				likely(__ptr != __next) ?						\
					list_entry_rcu(__next, type, member) : NULL;\
			})

	/**
	 * list_next_or_null_rcu - get the next element from a list
	 * @head:	the head for the list.
	 * @ptr:        the list head to take the next element from.
	 * @type:       the type of the struct this is embedded in.
	 * @member:     the name of the list_head within the struct.
	 *
	 * Note that if the ptr is at the end of the list, NULL is returned.
	 *
	 * This primitive may safely run concurrently with the _rcu list-mutation
	 * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
	 */
	#define list_next_or_null_rcu(head, ptr, type, member)	({	\
				List_s *__head = (head);						\
				List_s *__ptr = (ptr);							\
				List_s *__next = READ_ONCE(__ptr->next);		\
				likely(__next != __head) ?						\
					list_entry_rcu(__next, type, member) : NULL;\
			})

	/**
	 * list_for_each_entry_rcu	-	iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 * @cond:	optional lockdep expression if called from non-RCU protection.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as list_add_rcu()
	 * as long as the traversal is guarded by rcu_read_lock().
	 */
	#define list_for_each_entry_rcu(pos, head, member, cond...)	\
			for (__list_check_rcu(dummy, ## cond, 0),			\
				pos = list_entry_rcu((head)->next,				\
							typeof(*pos), member);				\
				&pos->member != (head);							\
				pos = list_entry_rcu(pos->member.next,			\
						typeof(*pos), member))

	/**
	 * list_for_each_entry_srcu	-	iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 * @cond:	lockdep expression for the lock required to traverse the list.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as list_add_rcu()
	 * as long as the traversal is guarded by srcu_read_lock().
	 * The lockdep expression srcu_read_lock_held() can be passed as the
	 * cond argument from read side.
	 */
	#define list_for_each_entry_srcu(pos, head, member, cond)	\
			for (__list_check_srcu(cond),						\
				pos = list_entry_rcu((head)->next,				\
						typeof(*pos), member);					\
				&pos->member != (head);							\
				pos = list_entry_rcu(pos->member.next,			\
						typeof(*pos), member))

	/**
	 * list_entry_lockless - get the struct for this entry
	 * @ptr:        the &struct list_head pointer.
	 * @type:       the type of the struct this is embedded in.
	 * @member:     the name of the list_head within the struct.
	 *
	 * This primitive may safely run concurrently with the _rcu
	 * list-mutation primitives such as list_add_rcu(), but requires some
	 * implicit RCU read-side guarding.  One example is running within a special
	 * exception-time environment where preemption is disabled and where lockdep
	 * cannot be invoked.  Another example is when items are added to the list,
	 * but never deleted.
	 */
	#define list_entry_lockless(ptr, type, member) \
				container_of((typeof(ptr))READ_ONCE(ptr), type, member)

	/**
	 * list_for_each_entry_lockless - iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_struct within the struct.
	 *
	 * This primitive may safely run concurrently with the _rcu
	 * list-mutation primitives such as list_add_rcu(), but requires some
	 * implicit RCU read-side guarding.  One example is running within a special
	 * exception-time environment where preemption is disabled and where lockdep
	 * cannot be invoked.  Another example is when items are added to the list,
	 * but never deleted.
	 */
	#define list_for_each_entry_lockless(pos, head, member) \
			for (pos = list_entry_lockless((head)->next,	\
						typeof(*pos), member);				\
				&pos->member != (head);						\
				pos = list_entry_lockless(pos->member.next,	\
						typeof(*pos), member))

	/**
	 * list_for_each_entry_continue_rcu - continue iteration over list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_head within the struct.
	 *
	 * Continue to iterate over list of given type, continuing after
	 * the current position which must have been in the list when the RCU read
	 * lock was taken.
	 * This would typically require either that you obtained the node from a
	 * previous walk of the list in the same RCU read-side critical section, or
	 * that you held some sort of non-RCU reference (such as a reference count)
	 * to keep the node alive *and* in the list.
	 *
	 * This iterator is similar to list_for_each_entry_from_rcu() except
	 * this starts after the given position and that one starts at the given
	 * position.
	 */
	#define list_for_each_entry_continue_rcu(pos, head, member)	\
			for (pos = list_entry_rcu(pos->member.next,			\
						typeof(*pos), member);					\
				&pos->member != (head);							\
				pos = list_entry_rcu(pos->member.next,			\
						typeof(*pos), member))

	/**
	 * list_for_each_entry_from_rcu - iterate over a list from current point
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the list_node within the struct.
	 *
	 * Iterate over the tail of a list starting from a given position,
	 * which must have been in the list when the RCU read lock was taken.
	 * This would typically require either that you obtained the node from a
	 * previous walk of the list in the same RCU read-side critical section, or
	 * that you held some sort of non-RCU reference (such as a reference count)
	 * to keep the node alive *and* in the list.
	 *
	 * This iterator is similar to list_for_each_entry_continue_rcu() except
	 * this starts from the given position and that one starts from the position
	 * after the given position.
	 */
	#define list_for_each_entry_from_rcu(pos, head, member)	\
			for (; &(pos)->member != (head);				\
				pos = list_entry_rcu(pos->member.next,		\
						typeof(*(pos)), member))



	/*
	 * return the first or the next element in an RCU protected hlist
	 */
	#define hlist_first_rcu(head)	(					\
				*((HList_s __rcu **)(&(head)->first))	\
			)
	#define hlist_next_rcu(node)	(					\
				*((HList_s __rcu **)(&(node)->next))	\
			)
	#define hlist_pprev_rcu(node)	(					\
				*((HList_s __rcu **)((node)->pprev))	\
			)

	#define __hlist_for_each_rcu(pos, head)				\
			for (pos = rcu_dereference(hlist_first_rcu(head));	\
				pos;						\
				pos = rcu_dereference(hlist_next_rcu(pos)))

	/**
	 * hlist_for_each_entry_rcu - iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the hlist_node within the struct.
	 * @cond:	optional lockdep expression if called from non-RCU protection.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as hlist_add_head_rcu()
	 * as long as the traversal is guarded by rcu_read_lock().
	 */
	#define hlist_for_each_entry_rcu(pos, head, member, cond...)		\
			for (__list_check_rcu(dummy, ## cond, 0),			\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_first_rcu(head)),\
					typeof(*(pos)), member);			\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_srcu - iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the hlist_node within the struct.
	 * @cond:	lockdep expression for the lock required to traverse the list.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as hlist_add_head_rcu()
	 * as long as the traversal is guarded by srcu_read_lock().
	 * The lockdep expression srcu_read_lock_held() can be passed as the
	 * cond argument from read side.
	 */
	#define hlist_for_each_entry_srcu(pos, head, member, cond)		\
			for (__list_check_srcu(cond),					\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_first_rcu(head)),\
					typeof(*(pos)), member);			\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_rcu_notrace - iterate over rcu list of given type (for tracing)
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the hlist_node within the struct.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as hlist_add_head_rcu()
	 * as long as the traversal is guarded by rcu_read_lock().
	 *
	 * This is the same as hlist_for_each_entry_rcu() except that it does
	 * not do any RCU debugging or tracing.
	 */
	#define hlist_for_each_entry_rcu_notrace(pos, head, member)			\
			for (pos = hlist_entry_safe(rcu_dereference_raw_check(hlist_first_rcu(head)),\
					typeof(*(pos)), member);			\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_raw_check(hlist_next_rcu(\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_rcu_bh - iterate over rcu list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the hlist_node within the struct.
	 *
	 * This list-traversal primitive may safely run concurrently with
	 * the _rcu list-mutation primitives such as hlist_add_head_rcu()
	 * as long as the traversal is guarded by rcu_read_lock().
	 */
	#define hlist_for_each_entry_rcu_bh(pos, head, member)			\
			for (pos = hlist_entry_safe(rcu_dereference_bh(hlist_first_rcu(head)),\
					typeof(*(pos)), member);			\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_continue_rcu - iterate over a hlist continuing after current point
	 * @pos:	the type * to use as a loop cursor.
	 * @member:	the name of the hlist_node within the struct.
	 */
	#define hlist_for_each_entry_continue_rcu(pos, member)			\
			for (pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu( \
					&(pos)->member)), typeof(*(pos)), member);	\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(	\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_continue_rcu_bh - iterate over a hlist continuing after current point
	 * @pos:	the type * to use as a loop cursor.
	 * @member:	the name of the hlist_node within the struct.
	 */
	#define hlist_for_each_entry_continue_rcu_bh(pos, member)		\
			for (pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(  \
					&(pos)->member)), typeof(*(pos)), member);	\
				pos;							\
				pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(	\
					&(pos)->member)), typeof(*(pos)), member))

	/**
	 * hlist_for_each_entry_from_rcu - iterate over a hlist continuing from current point
	 * @pos:	the type * to use as a loop cursor.
	 * @member:	the name of the hlist_node within the struct.
	 */
	#define hlist_for_each_entry_from_rcu(pos, member)			\
			for (; pos;							\
				pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(	\
					&(pos)->member)), typeof(*(pos)), member))


	/**
	 * RCU_INIT_POINTER() - initialize an RCU protected pointer
	 * @p: The pointer to be initialized.
	 * @v: The value to initialized the pointer to.
	 *
	 * Initialize an RCU-protected pointer in special cases where readers
	 * do not need ordering constraints on the CPU or the compiler.  These
	 * special cases are:
	 *
	 * 1.	This use of RCU_INIT_POINTER() is NULLing out the pointer *or*
	 * 2.	The caller has taken whatever steps are required to prevent
	 *	RCU readers from concurrently accessing this pointer *or*
	 * 3.	The referenced data structure has already been exposed to
	 *	readers either at compile time or via rcu_assign_pointer() *and*
	 *
	 *	a.	You have not made *any* reader-visible changes to
	 *		this structure since then *or*
	 *	b.	It is OK for readers accessing this structure from its
	 *		new location to see the old state of the structure.  (For
	 *		example, the changes were to statistical counters or to
	 *		other state where exact synchronization is not required.)
	 *
	 * Failure to follow these rules governing use of RCU_INIT_POINTER() will
	 * result in impossible-to-diagnose memory corruption.  As in the structures
	 * will look OK in crash dumps, but any concurrent RCU readers might
	 * see pre-initialized values of the referenced data structure.  So
	 * please be very careful how you use RCU_INIT_POINTER()!!!
	 *
	 * If you are creating an RCU-protected linked structure that is accessed
	 * by a single external-to-structure RCU-protected pointer, then you may
	 * use RCU_INIT_POINTER() to initialize the internal RCU-protected
	 * pointers, but you must use rcu_assign_pointer() to initialize the
	 * external-to-structure pointer *after* you have completely initialized
	 * the reader-accessible portions of the linked structure.
	 *
	 * Note that unlike rcu_assign_pointer(), RCU_INIT_POINTER() provides no
	 * ordering guarantees for either the CPU or the compiler.
	 */
	// #define RCU_INIT_POINTER(p, v)					\
	// 		do {									\
	// 			rcu_check_sparse(p, __rcu);			\
	// 			WRITE_ONCE(p, RCU_INITIALIZER(v));	\
	// 		} while (0)
	#define RCU_INIT_POINTER(p, v)		\
			do {						\
				WRITE_ONCE(p, v);		\
			} while (0)

	/**
	 * RCU_POINTER_INITIALIZER() - statically initialize an RCU protected pointer
	 * @p: The pointer to be initialized.
	 * @v: The value to initialized the pointer to.
	 *
	 * GCC-style initialization for an RCU-protected pointer in a structure field.
	 */
	#define RCU_POINTER_INITIALIZER(p, v) \
				.p = RCU_INITIALIZER(v)

#endif /* _LINUX_RCU_LIST_MACROS_H_ */