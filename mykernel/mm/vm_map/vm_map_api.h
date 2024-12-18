#ifndef _VM_MAP_API_H_
#define _VM_MAP_API_H_

	#include "vm_map.h"


	extern kmem_cache_s *vm_area_cachep;

	/* generic vm_area_ops exported for stackable file systems */
	extern vm_fault_t simple_filemap_fault(vm_fault_s *vmf);
	extern vm_fault_t filemap_map_pages(vm_fault_s *vmf,
			pgoff_t start_pgoff, pgoff_t end_pgoff);
	extern vm_fault_t simple_filemap_map_page(vm_fault_s *vmf, pgoff_t pgoff);
	// extern vm_fault_t filemap_page_mkwrite(vm_fault_s *vmf);

	extern int generic_file_mmap(file_s *, vma_s *);

	/*
	 * Linux kernel virtual memory manager primitives.
	 * The idea being to have a "virtual" mm in the same way
	 * we have a virtual fs - giving a cleaner interface to the
	 * mm details, and allowing different kinds of memory mappings
	 * (from shared memory to executable loading to arbitrary
	 * mmap() functions).
	 */
	vma_s *vm_area_alloc(mm_s *);
	vma_s *vm_area_creat_dup(vma_s *);
	void vm_area_free(vma_s *);
	vma_s *vma_prev_vma(vma_s *vma);
	vma_s *vma_next_vma(vma_s *vma);
	vma_s *mm_next_vma(mm_s *mm, vma_s *vma);
	vma_s *mm_prev_vma(mm_s *mm, vma_s *vma);
	void unlink_anon_vmas(vma_s *);
	int anon_vma_clone(vma_s *, vma_s *);

	// /* mmap.c */
	extern void __simple_vma_adjust(vma_s *vma, ulong start, ulong end);
	vma_s *simple_vma_merge(mm_s *mm, vma_s *prev, ulong addr,
			ulong end, ulong vm_flags, file_s *file, pgoff_t pgoff);
	extern int insert_vm_struct(mm_s *, vma_s *);
	extern ulong simple_mmap_region(file_s *file, ulong addr,
			ulong len, vm_flags_t vm_flags, ulong pgoff);
	extern ulong do_mmap(file_s *file, ulong addr, ulong len,
			ulong prot, ulong flags, vm_flags_t vm_flags, ulong pgoff);
	extern int simple_do_vma_munmap(mm_s *, ulong start, ulong end);

	// /* These take the mm semaphore themselves */
	extern int __must_check vm_brk_flags(ulong, ulong, ulong);
	// extern int vm_munmap(ulong, size_t);
	extern int __vm_munmap(ulong start, size_t len);
	#define vm_munmap __vm_munmap
	extern ulong __must_check vm_mmap(file_s *,
			ulong, ulong, ulong, ulong, ulong);

	extern ulong stack_guard_gap;
	/* Generic expand stack which grows the stack according to GROWS{UP,DOWN} */
	extern int expand_stack(vma_s *vma, ulong address);
	extern ulong simple_find_vm_unmapped_area_topdown(mm_s * mm, unmapped_vma_info_s *info);
	ulong simple_get_unmapped_area(file_s *filp, const ulong addr,
			const ulong len, const ulong pgoff, const ulong flags);
	extern ulong __get_unmapped_area(file_s *, ulong, ulong, ulong, ulong, vm_flags_t);
	extern vma_s * simple_find_vma(mm_s * mm, ulong addr);

	/* mm/util.c */
	void __vma_link_to_list(mm_s *mm, vma_s *vma, vma_s *prev);
	void __vma_unlink_from_list(mm_s *mm, vma_s *vma);
	extern ulong  __must_check vm_mmap_pgoff(file_s *,
			ulong, ulong, ulong, ulong, ulong);

	extern ulong randomize_stack_top(ulong stack_top);

	int do_brk_flags(ulong addr, ulong len, ulong flags);




	extern void __init mmap_init(void);

#endif /* _VM_MAP_API_H_ */