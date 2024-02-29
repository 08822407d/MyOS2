#ifndef _LINUX_VM_MAP_API_H_
#define _LINUX_VM_MAP_API_H_

    #include <linux/mm/mm_types.h>

	// /* generic vm_area_ops exported for stackable file systems */
	// extern vm_fault_t filemap_fault(vm_fault_s *vmf);
	// extern vm_fault_t filemap_map_pages(vm_fault_s *vmf,
	// 		pgoff_t start_pgoff, pgoff_t end_pgoff);
	// // extern vm_fault_t filemap_page_mkwrite(vm_fault_s *vmf);

	extern int generic_file_mmap(file_s *, vma_s *);

	// /* mmap.c */
	extern int __myos_vma_adjust(vma_s *vma, unsigned long start,
			unsigned long end, pgoff_t pgoff, vma_s *insert, vma_s *expand);
	vma_s *myos_vma_merge(mm_s *mm, vma_s *prev, unsigned long addr,
            unsigned long end, unsigned long vm_flags, file_s *file, pgoff_t pgoff);
	extern int insert_vm_struct(mm_s *, vma_s *);
	extern unsigned long myos_mmap_region(file_s *file, unsigned long addr,
			unsigned long len, vm_flags_t vm_flags, unsigned long pgoff);
	extern unsigned long do_mmap(file_s *file, unsigned long addr,
			unsigned long len, unsigned long prot, unsigned long flags,
			unsigned long pgoff, unsigned long *populate);
	extern int __do_munmap(mm_s *, unsigned long, size_t, bool downgrade);

	// /* These take the mm semaphore themselves */
	extern int __must_check vm_brk_flags(unsigned long, unsigned long, unsigned long);
	extern int vm_munmap(unsigned long, size_t);
	int __vm_munmap(unsigned long start, size_t len, bool downgrade);
	extern unsigned long __must_check vm_mmap(file_s *,
			unsigned long, unsigned long, unsigned long,
			unsigned long, unsigned long);

	extern unsigned long stack_guard_gap;
	/* Generic expand stack which grows the stack according to GROWS{UP,DOWN} */
	extern int expand_stack(vma_s *vma, unsigned long address);
	extern vma_s * myos_find_vma(mm_s * mm, unsigned long addr);

	/* mm/util.c */
	void __vma_link_list(mm_s *mm, vma_s *vma, vma_s *prev);
	void __vma_unlink_list(mm_s *mm, vma_s *vma);
	extern unsigned long  __must_check vm_mmap_pgoff(file_s *, unsigned long,
			unsigned long, unsigned long, unsigned long, unsigned long);


	int do_brk_flags(unsigned long addr, unsigned long len, unsigned long flags);

#endif /* _LINUX_VM_MAP_API_H_ */