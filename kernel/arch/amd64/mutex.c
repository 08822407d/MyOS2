#include <sys/cdefs.h>

#include "include/mutex.h"

/*==============================================================================================*
 *											spin lock											*
 *==============================================================================================*/
inline __always_inline void init_spinlock(spinlock_T * lock)
{
	lock->lock.value = 1;
}

inline __always_inline void lock_spinlock(spinlock_T * lock)
{
	__asm__	__volatile__(	"1:						\n\t"
							"lock	decq	%0		\n\t"
							"jns	3f				\n\t"
							"2:						\n\t"
							"pause					\n\t"
							"cmpq	$0,		%0		\n\t"
							"jle	2b				\n\t"
							"jmp	1b				\n\t"
							"3:						\n\t"
						:	"=m"(lock->lock.value)
						:
						:	"memory"
						);
}

inline __always_inline void unlock_spinlock(spinlock_T * lock)
{
	// if (lock->lock == 1)
	// 	return;

	__asm__	__volatile__(	"movq	$1,		%0		\n\t"
						:	"=m"(lock->lock.value)
						:
						:	"memory"
						);
}

/*==============================================================================================*
 *										recursive spin lock										*
 *==============================================================================================*/
inline __always_inline void init_recursivelock(recursive_lock_T * lock)
{
	lock->counter = 0;
	lock->owner = NULL;
	init_spinlock(&lock->selflock);
}

void lock_recursivelock(recursive_lock_T * lock)
{
	task_s * owner = NULL;
	task_s * curr = curr_tsk;
	do
	{
		lock_spinlock(&lock->selflock);
		owner = lock->owner;
		if (lock->owner == NULL)
			lock->owner = curr;
		unlock_spinlock(&lock->selflock);
	} while(owner != curr);
	lock->counter++;
}

void unlock_recursivelock(recursive_lock_T * lock)
{
	task_s * curr = curr_tsk;
	while (lock->owner != curr);
	lock->counter--;
	if (lock->counter == 0)
		lock->owner = NULL;
}

/*==============================================================================================*
 *										atomic operations										*
 *==============================================================================================*/
inline __always_inline void atomic_add(atomic_T * atomic, long value)
{
	__asm__ __volatile__(	"lock	addq	%1,	%0	\n\t"
						:	"=m"(atomic->value)
						:	"r"(value)
						:	"memory"
						);
}

inline __always_inline void atomic_sub(atomic_T * atomic, long value)
{
	__asm__ __volatile__(	"lock	subq	%1,	%0	\n\t"
						:	"=m"(atomic->value)
						:	"r"(value)
						:	"memory"
						);
}

inline __always_inline void atomic_inc(atomic_T * atomic)
{
	__asm__ __volatile__(	"lock	incq	%0		\n\t"
						:	"=m"(atomic->value)
						:	"m"(atomic->value)
						:	"memory"
						);
}

inline __always_inline void atomic_dec(atomic_T * atomic)
{
	__asm__ __volatile__(	"lock	decq	%0		\n\t"
						:	"=m"(atomic->value)
						:	"m"(atomic->value)
						:	"memory"
						);
}

inline __always_inline void atomic_set_mask(atomic_T * atomic, long mask)
{
	__asm__ __volatile__(	"lock	orq		%1,	%0	\n\t"
						:	"=m"(atomic->value)
						:	"r"(mask)
						:	"memory"
						);
}

inline __always_inline void atomic_clear_mask(atomic_T * atomic, long mask)
{
	__asm__ __volatile__(	"lock	andq	%1,	%0	\n\t"
						:	"=m"(atomic->value)
						:	"r"(~(mask))
						:	"memory"
						);
}