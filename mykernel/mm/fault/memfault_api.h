#ifndef _MEM_FAULT_API_H_
#define _MEM_FAULT_API_H_

	#include "pgtable.h"


	page_s *vm_normal_page(vma_s *vma, ulong addr, pte_t pte);

	int copy_page_range(vma_s *dst_vma, vma_s *src_vma);

	vm_fault_t finish_fault(vm_fault_s *vmf);

	extern vm_fault_t myos_handle_mm_fault(vma_s *vma,
            pt_regs_s *regs, ulong address, uint flags);

	int __myos_pud_alloc(mm_s *mm, p4d_t *p4d, ulong address);
	int __myos_pmd_alloc(mm_s *mm, pud_t *pud, ulong address);
	int __myos_pte_alloc(mm_s *mm, pmd_t *pmd, ulong address);

	extern pte_t *myos_creat_one_page_mapping(mm_s *mm,
			virt_addr_t addr, page_s *page);
	extern page_s *myos_get_one_page_from_mapping(mm_s *mm, virt_addr_t addr);
	extern int myos_map_range(mm_s *mm, virt_addr_t start, virt_addr_t end);
	extern vm_fault_s myos_dump_pagetable(ulong address);

#endif /* _MEM_FAULT_API_H_ */