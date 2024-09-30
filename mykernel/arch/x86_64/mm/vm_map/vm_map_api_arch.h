#ifndef _ASM_X86_VM_MAP_API_H_
#define _ASM_X86_VM_MAP_API_H_

	#include "vm_map_arch.h"

	void arch_pick_mmap_layout(mm_s * mm, rlimit_s *rlim_stack);

	ulong arch_get_unmapped_area(file_s *filp, ulong addr,
			ulong len, ulong pgoff, ulong flags);

	ulong arch_get_unmapped_area_topdown(file_s *filp, const ulong addr0,
			const ulong len, const ulong pgoff, const ulong flags);

	pgprot_t vm_get_page_prot(ulong vm_flags);

#endif /* _ASM_X86_VM_MAP_API_H_ */