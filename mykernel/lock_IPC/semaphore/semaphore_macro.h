#ifndef _LINUX_SEMAPHORE_MACRO_H_
#define _LINUX_SEMAPHORE_MACRO_H_

	#define __SEMAPHORE_INITIALIZER(name, n)	{					\
				.lock		= __SPIN_LOCK_UNLOCKED((name).lock),	\
				.count		= n,									\
				.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
			}

	/*
	 * Unlike mutexes, binary semaphores do not have an owner, so up() can
	 * be called in a different thread from the one which called down().
	 * It is also safe to call down_trylock() and up() from interrupt
	 * context.
	 */
	#define DEFINE_SEMAPHORE(_name, _n)	\
				sema_t _name = __SEMAPHORE_INITIALIZER(_name, _n)

#endif /* _LINUX_SEMAPHORE_MACRO_H_ */