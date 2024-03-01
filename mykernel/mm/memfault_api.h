#ifndef _LINUX_MEM_FAULT_API_H_
#define _LINUX_MEM_FAULT_API_H_

    #include "fault/memfault_const.h"
    #include "fault/memfault_types.h"
	#include "fault/pgtable.h"

	page_s *vm_normal_page(vma_s *vma, unsigned long addr, pte_t pte);

	int copy_page_range(vma_s *dst_vma, vma_s *src_vma);

	vm_fault_t finish_fault(vm_fault_s *vmf);

	extern vm_fault_t myos_handle_mm_fault(vma_s *vma,
            pt_regs_s *regs, unsigned long address, unsigned int flags);

	int __myos_pud_alloc(mm_s *mm, p4d_t *p4d, unsigned long address);
	int __myos_pmd_alloc(mm_s *mm, pud_t *pud, unsigned long address);
	int __myos_pte_alloc(mm_s *mm, pmd_t *pmd, unsigned long address);

	extern int myos_map_range(mm_s *mm, unsigned long start, unsigned long end);
	extern vm_fault_s myos_dump_pagetable(unsigned long address);

#endif /* _LINUX_MEM_FAULT_API_H_ */