// /* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ARCH_X86_PROCESSOR_H_
#define _ARCH_X86_PROCESSOR_H_

	#include <processor/desc_arch_api.h>

	#define INIT_THREAD {							\
				.sp	= (unsigned long)				\
						&__top_init_kernel_stack,	\
			}

#endif /* _ARCH_X86_PROCESSOR_H_ */
