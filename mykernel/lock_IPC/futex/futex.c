#define FUTEX_DEFINATION
#include "futex.h"


long do_futex(u32 __user *uaddr, int op, u32 val, ktime_t *timeout,
		u32 __user *uaddr2, u32 val2, u32 val3)
{
	uint flags = futex_to_flags(op);
	int cmd = op & FUTEX_CMD_MASK;

	if (flags & FLAGS_CLOCKRT) {
		if (cmd != FUTEX_WAIT_BITSET &&
			cmd != FUTEX_WAIT_REQUEUE_PI &&
			cmd != FUTEX_LOCK_PI2)
			return -ENOSYS;
	}

	switch (cmd) {
	case FUTEX_WAIT:
		val3 = FUTEX_BITSET_MATCH_ANY;
		fallthrough;
	// case FUTEX_WAIT_BITSET:
	// 	return futex_wait(uaddr, flags, val, timeout, val3);
	// case FUTEX_WAKE:
	// 	val3 = FUTEX_BITSET_MATCH_ANY;
	// 	fallthrough;
	// case FUTEX_WAKE_BITSET:
	// 	return futex_wake(uaddr, flags, val, val3);
	// case FUTEX_REQUEUE:
	// 	return futex_requeue(uaddr, flags, uaddr2, flags, val, val2, NULL, 0);
	// case FUTEX_CMP_REQUEUE:
	// 	return futex_requeue(uaddr, flags, uaddr2, flags, val, val2, &val3, 0);
	// case FUTEX_WAKE_OP:
	// 	return futex_wake_op(uaddr, flags, uaddr2, val, val2, val3);
	case FUTEX_LOCK_PI:
		flags |= FLAGS_CLOCKRT;
		// fallthrough;
	// case FUTEX_LOCK_PI2:
	// 	return futex_lock_pi(uaddr, flags, timeout, 0);
	// case FUTEX_UNLOCK_PI:
	// 	return futex_unlock_pi(uaddr, flags);
	// case FUTEX_TRYLOCK_PI:
	// 	return futex_lock_pi(uaddr, flags, NULL, 1);
	// case FUTEX_WAIT_REQUEUE_PI:
	// 	val3 = FUTEX_BITSET_MATCH_ANY;
	// 	return futex_wait_requeue_pi(uaddr, flags, val, timeout, val3,
	// 					 uaddr2);
	// case FUTEX_CMP_REQUEUE_PI:
	// 	return futex_requeue(uaddr, flags, uaddr2, flags, val, val2, &val3, 1);
	}
	return -ENOSYS;
}
