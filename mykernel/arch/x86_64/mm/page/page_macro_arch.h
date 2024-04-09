/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PAGE_MACRO_H_
#define _ASM_X86_PAGE_MACRO_H_

	#	ifndef __pa
	#		define __pa(x)	((unsigned long)(x) - (unsigned long)&_k_offset)
	#	endif

	// #		define __pa_nodebug(x)	__phys_addr_nodebug((unsigned long)(x))
	// 		/* __pa_symbol should be used for C visible symbols.
	// 		   This seems to be the official gcc blessed way to do such arithmetic. */
	// 		/*
	// 		 * We need __phys_reloc_hide() here because gcc may assume that there is no
	// 		 * overflow during __pa() calculation and can optimize it unexpectedly.
	// 		 * Newer versions of gcc provide -fno-strict-overflow switch to handle this
	// 		 * case properly. Once all supported versions of gcc understand it, we can
	// 		 * remove this Voodoo magic stuff. (i.e. once gcc3.x is deprecated)
	// 		 */
	// #		define __pa_symbol(x) \
	// 				__phys_addr_symbol(__phys_reloc_hide((unsigned long)(x)))

	#		ifndef __va
	#			define __va(x)	((unsigned long)(x) + (unsigned long)&_k_offset)
	#		endif

	// #		define __boot_va(x)	__va(x)
	// #		define __boot_pa(x)	__pa(x)

	#		define phys_to_page(kaddr) pfn_to_page(kaddr >> PAGE_SHIFT)
			/*
			 * virt_to_page(kaddr) returns a valid pointer if and only if
			 * virt_addr_valid(kaddr) returns true.
			 */
	#		define virt_to_page(kaddr) pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
			// #define pfn_to_kaddr(pfn) __va((pfn) << PAGE_SHIFT)
			// extern bool __virt_addr_valid(unsigned long kaddr);
			// #define virt_addr_valid(kaddr) __virt_addr_valid((unsigned long)(kaddr))

#endif /* _ASM_X86_PAGE_MACRO_H_ */