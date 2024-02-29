#ifndef _LINUX_ATOMIC_LONG_H
#define _LINUX_ATOMIC_LONG_H

	#include <asm/lock_ipc.h>

	typedef atomic64_t atomic_long_t;
	#define ATOMIC_LONG_INIT(i)		ATOMIC64_INIT(i)

#endif /* _LINUX_ATOMIC_LONG_H */
