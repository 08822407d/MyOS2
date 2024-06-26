// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CMPXCHG_64_API_H_
#define _ASM_X86_CMPXCHG_64_API_H_

	#define arch_cmpxchg64(ptr, o, n) ({				\
				BUILD_BUG_ON(sizeof(*(ptr)) != 8);		\
				arch_cmpxchg((ptr), (o), (n));			\
			})

	#define arch_cmpxchg64_local(ptr, o, n) ({			\
				BUILD_BUG_ON(sizeof(*(ptr)) != 8);		\
				arch_cmpxchg_local((ptr), (o), (n));	\
			})
		
	#define arch_try_cmpxchg64(ptr, po, n) ({			\
				BUILD_BUG_ON(sizeof(*(ptr)) != 8);		\
				arch_try_cmpxchg((ptr), (po), (n));		\
			})

#endif /* _ASM_X86_CMPXCHG_64_API_H_ */
