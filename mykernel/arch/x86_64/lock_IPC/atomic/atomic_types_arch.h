#ifndef _ASM_X86_ATOMIC_TYPES_H_
#define _ASM_X86_ATOMIC_TYPES_H_

	#include "../lock_ipc_type_declaration_arch.h"


	struct atomic {
		int		counter;
	};

	struct atomic64 {
		s64		counter;
	};

#endif /* _ASM_X86_ATOMIC_TYPES_H_ */