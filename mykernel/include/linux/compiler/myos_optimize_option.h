#ifndef MYOS_OPTIMIZE_OPTION_H
#define MYOS_OPTIMIZE_OPTION_H

	#include "compiler.h"

	#ifndef DEBUG

	#	define PREFIX_STATIC_INLINE static inline
	#	define PREFIX_STATIC_AWLWAYS_INLINE static __always_inline

	#else /* DEBUG */

	#	define PREFIX_STATIC_INLINE
	#	define PREFIX_STATIC_AWLWAYS_INLINE

	#endif /* DEBUG */


#endif /* MYOS_OPTIMIZE_OPTION_H */