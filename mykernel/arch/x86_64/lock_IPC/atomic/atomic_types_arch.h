#ifndef _ATOMIC_TYPES_H_
#define _ATOMIC_TYPES_H_

	#include "../lock_ipc_type_declaration_arch.h"


	typedef struct atomic {
		int counter;
	} atomic_t;

	typedef struct atomic64 {
		s64 counter;
	} atomic64_t;

#endif /* _ATOMIC_TYPES_H_ */