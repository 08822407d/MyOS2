/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PAGE_H_
#define _ASM_X86_PAGE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const_arch.h"
	#include "../mm_types_arch.h"
	#include "../mm_api_arch.h"


	#ifdef DEBUG

		extern void
		clear_user_page(void *page, ulong vaddr, page_s *pg);

		extern void
		copy_user_page(void *to, void *from, ulong vaddr, page_s *topage);

	#endif

	#include "page_macro_arch.h"
	
	#if defined(ARCH_PAGE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		clear_user_page(void *page,
				ulong vaddr, page_s *pg) {
			clear_page(page);
		}

		PREFIX_STATIC_INLINE
		void
		copy_user_page(void *to, void *from,
				ulong vaddr, page_s *topage) {
			copy_page(to, from);
		}

		// static __always_inline u64
		// __canonical_address(u64 vaddr, u8 vaddr_bits) {
		// 	return ((s64)vaddr << (64 - vaddr_bits)) >> (64 - vaddr_bits);
		// }

		// static __always_inline u64
		// __is_canonical_address(u64 vaddr, u8 vaddr_bits) {
		// 	return __canonical_address(vaddr, vaddr_bits) == vaddr;
		// }

	#endif

#endif /* _ASM_X86_PAGE_H_ */