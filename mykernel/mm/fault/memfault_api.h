#ifndef _MEM_FAULT_API_H_
#define _MEM_FAULT_API_H_

	#include "pgtable.h"
	#include "pgalloc.h"

	extern pgd_t init_top_pgt[];
	#define swapper_pg_dir init_top_pgt


	page_s *vm_normal_page(vma_s *vma, ulong addr, pte_t pte);

	int copy_page_range(vma_s *dst_vma, vma_s *src_vma);

	vm_fault_t finish_fault(vm_fault_s *vmf);

	extern vm_fault_t myos_handle_mm_fault(vma_s *vma,
            ulong address, uint flags);

	int __pud_alloc(mm_s *mm, p4d_t *p4d_entp);
	int __pmd_alloc(mm_s *mm, pud_t *pud_entp);
	int __pgtbl_alloc(mm_s *mm, pmd_t *pmd_entp);
	#define __pte_alloc __pgtbl_alloc

	extern pte_t *myos_creat_one_page_mapping(mm_s *mm,
			virt_addr_t addr, page_s *page);
	extern page_s *myos_get_one_page_from_mapping(mm_s *mm, virt_addr_t addr);
	extern int myos_map_range(mm_s *mm, virt_addr_t start, virt_addr_t end);
	extern vm_fault_s myos_dump_pagetable(ulong address);


	extern pte_t *__pte_offset_map(pmd_t *pmd,
			ulong addr, pmd_t *pmdvalp);
	extern pte_t *pte_offset_map_nolock(mm_s *mm,
			pmd_t *pmdp, ulong addr, spinlock_t **ptlp);
	extern pte_t *__pte_offset_map_lock(mm_s *mm,
			pmd_t *pmdp, ulong addr, spinlock_t **ptlp);
	extern pte_t *pte_offset_map_lock(mm_s *mm,
			pmd_t *pmdp, ulong addr, spinlock_t **ptlp);


#endif /* _MEM_FAULT_API_H_ */