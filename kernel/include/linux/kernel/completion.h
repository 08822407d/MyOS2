/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPLETION_H
#define __LINUX_COMPLETION_H

	/*
	 * (C) Copyright 2001 Linus Torvalds
	 *
	 * Atomic wait-for-completion handler data structures.
	 * See kernel/sched/completion.c for details.
	 */

	#include <linux/kernel/swait.h>

	/*
	 * completion_s - structure used to maintain state for a "completion"
	 *
	 * This is the opaque structure used to maintain the state for a "completion".
	 * Completions currently use a FIFO to queue threads that have to wait for
	 * the "completion" event.
	 *
	 * See also:  complete(), wait_for_completion() (and friends _timeout,
	 * _interruptible, _interruptible_timeout, and _killable), init_completion(),
	 * reinit_completion(), and macros DECLARE_COMPLETION(),
	 * DECLARE_COMPLETION_ONSTACK().
	 */
	typedef struct completion {
		unsigned int	done;
		swqueue_hdr_s	wait;
	} completion_s;

	// #define init_completion_map(x, m)	init_completion(x)
	static inline void complete_acquire(completion_s *x) {}
	static inline void complete_release(completion_s *x) {}

	#define COMPLETION_INITIALIZER(work)	\
				{ 0, __SWAIT_QUEUE_HEAD_INITIALIZER((work).wait) }

	// #define COMPLETION_INITIALIZER_ONSTACK_MAP(work, map)	\
	// 			(*({ init_completion_map(&(work), &(map)); &(work); }))

	// #define COMPLETION_INITIALIZER_ONSTACK(work)	\
	// 			(*({ init_completion(&work); &work; }))

	/**
	 * DECLARE_COMPLETION - declare and initialize a completion structure
	 * @work:  identifier for the completion structure
	 *
	 * This macro declares and initializes a completion structure. Generally used
	 * for static declarations. You should use the _ONSTACK variant for automatic
	 * variables.
	 */
	#define DECLARE_COMPLETION(work)	\
				completion_s work = COMPLETION_INITIALIZER(work)

	// /*
	// * Lockdep needs to run a non-constant initializer for on-stack
	// * completions - so we use the _ONSTACK() variant for those that
	// * are on the kernel stack:
	// */
	// /**
	//  * DECLARE_COMPLETION_ONSTACK - declare and initialize a completion structure
	//  * @work:  identifier for the completion structure
	//  *
	//  * This macro declares and initializes a completion structure on the kernel
	//  * stack.
	//  */
	// #ifdef CONFIG_LOCKDEP
	// #	define DECLARE_COMPLETION_ONSTACK(work)	\
	// 		completion_s work = COMPLETION_INITIALIZER_ONSTACK(work)
	// #	define DECLARE_COMPLETION_ONSTACK_MAP(work, map)	\
	// 		completion_s work = COMPLETION_INITIALIZER_ONSTACK_MAP(work, map)
	// #else
	#define DECLARE_COMPLETION_ONSTACK(work)	DECLARE_COMPLETION(work)
	// #define DECLARE_COMPLETION_ONSTACK_MAP(work, map)	DECLARE_COMPLETION(work)
	// #endif

	/**
	 * init_completion - Initialize a dynamically allocated completion
	 * @x:  pointer to completion structure that is to be initialized
	 *
	 * This inline function will initialize a dynamically created completion
	 * structure.
	 */
	static inline void init_completion(completion_s *x) {
		x->done = 0;
		// init_swait_queue_head(&x->wait);
	}

	// /**
	//  * reinit_completion - reinitialize a completion structure
	//  * @x:  pointer to completion structure that is to be reinitialized
	//  *
	//  * This inline function should be used to reinitialize a completion structure so it can
	//  * be reused. This is especially important after complete_all() is used.
	//  */
	// static inline void reinit_completion(completion_s *x) {
	// 	x->done = 0;
	// }

	extern void wait_for_completion(completion_s *);
	// extern void wait_for_completion_io(completion_s *);
	// extern int wait_for_completion_interruptible(completion_s *x);
	// extern int wait_for_completion_killable(completion_s *x);
	// extern unsigned long
	// wait_for_completion_timeout(completion_s *x, unsigned long timeout);
	// extern unsigned long
	// wait_for_completion_io_timeout(completion_s *x, unsigned long timeout);
	// extern long
	// wait_for_completion_interruptible_timeout( completion_s *x, unsigned long timeout);
	// extern long
	// wait_for_completion_killable_timeout(completion_s *x, unsigned long timeout);
	// extern bool try_wait_for_completion(completion_s *x);
	// extern bool completion_done(completion_s *x);

	extern void complete(completion_s *);
	// extern void complete_all(completion_s *);

#endif
