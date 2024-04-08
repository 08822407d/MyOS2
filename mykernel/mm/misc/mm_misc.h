#ifndef _MM_MISC_H_
#define _MM_MISC_H_

	#include <linux/compiler/myos_optimize_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern void
		mmget(mm_s *mm);

	#endif
	
	#if defined(MM_MISC_DEFINATION) || !(DEBUG)

		/**
		 * mmget() - Pin the address space associated with a &mm_s.
		 * @mm: The address space to pin.
		 *
		 * Make sure that the address space of the given &mm_s doesn't
		 * go away. This does not protect against parts of the address space being
		 * modified or freed, however.
		 *
		 * Never use this function to pin this address space for an
		 * unbounded/indefinite amount of time.
		 *
		 * Use mmput() to release the reference acquired by mmget().
		 *
		 * See also <Documentation/vm/active_mm.rst> for an in-depth explanation
		 * of &mm_struct.mm_count vs &mm_struct.mm_users.
		 */
		PREFIX_STATIC_INLINE
		void
		mmget(mm_s *mm) {
			atomic_inc(&mm->mm_users);
		}

	#endif

#endif /* _MM_MISC_H_ */