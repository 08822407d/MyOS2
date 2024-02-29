/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_H
#define _LINUX_MM_H

	#include <linux/lib/errno.h>
	#include <linux/mm/gfp.h>
	#include <linux/debug/bug.h>
	#include <linux/lib/list.h>
	#include <linux/mm/mmzone.h>
	#include <linux/mm/mm_types.h>
	#include <linux/mm/pfn.h>
	#include <linux/kernel/resource.h>
	#include <linux/kernel/err.h>
	#include <linux/mm/page-flags.h>
	#include <linux/mm/page_ref.h>
	#include <linux/kernel/overflow.h>
	#include <linux/kernel/sizes.h>
	#include <linux/kernel/sched.h>
	#include <linux/mm/pgtable.h>
	#include <linux/kernel/slab.h>


	#include <mm/memblock_api.h>
	#include <mm/page_alloc_api.h>
	#include <mm/kmalloc_api.h>
	#include <mm/memfault_api.h>
	#include <mm/vm_map_api.h>



	// struct mempolicy;
	// anon_vma_s;
	// anon_vma_chain_s;
	// struct user_struct;
	struct pt_regs;
	typedef struct pt_regs pt_regs_s;

	// extern int sysctl_page_lock_unfairness;
	
	void mm_core_init(void);
	void init_mm_internals(void);

	extern void *high_memory;

	#include <asm/page.h>
	#include <asm/processor.h>

	// /*
	// * Architectures that support memory tagging (assigning tags to memory regions,
	// * embedding these tags into addresses that point to these memory regions, and
	// * checking that the memory and the pointer tags match on memory accesses)
	// * redefine this macro to strip tags from pointers.
	// * It's defined as noop for architectures that don't support memory tagging.
	// */
	// #ifndef untagged_addr
	// #define untagged_addr(addr) (addr)
	// #endif

	#ifndef __pa_symbol
	#	define __pa_symbol(x)  __pa(RELOC_HIDE((unsigned long)(x), 0))
	#endif


	/*
	 * Default maximum number of active map areas, this limits the number of vmas
	 * per mm struct. Users can overwrite this number by sysctl but there is a
	 * problem.
	 *
	 * When a program's coredump is generated as ELF format, a section is created
	 * per a vma. In ELF, the number of sections is represented in unsigned short.
	 * This means the number of sections should be smaller than 65535 at coredump.
	 * Because the kernel adds some informative sections to a image of program at
	 * generating coredump, we need some margin. The number of extra sections is
	 * 1-3 now and depends on arch. We use "5" as safe margin, here.
	 *
	 * ELF extended numbering allows more than 65535 sections, so 16-bit bound is
	 * not a hard limit any more. Although some userspace tools can be surprised by
	 * that.
	 */
	#define MAPCOUNT_ELF_CORE_MARGIN	(5)
	#define DEFAULT_MAX_MAP_COUNT		(USHRT_MAX - MAPCOUNT_ELF_CORE_MARGIN)

	extern int sysctl_max_map_count;

	extern unsigned long sysctl_user_reserve_kbytes;
	extern unsigned long sysctl_admin_reserve_kbytes;

	/* to align the pointer to the (next) page boundary */
	#define PAGE_ALIGN(addr) ALIGN(addr, PAGE_SIZE)

	void setup_initial_init_mm(void *start_code,
			void *end_code, void *end_data, void *brk);

	// /*
	// * mapping from the currently active vm_flags protection bits (the
	// * low four bits) to a page protection mask..
	// */
	// extern pgprot_t protection_map[16];

	/*
	* The default fault flags that should be used by most of the
	* arch-specific page fault handlers.
	*/
	#define FAULT_FLAG_DEFAULT  (FAULT_FLAG_ALLOW_RETRY | \
					FAULT_FLAG_KILLABLE | \
					FAULT_FLAG_INTERRUPTIBLE)

	static inline void vma_init(vma_s *vma, mm_s *mm) {
		static const vm_ops_s dummy_vm_ops = {};

		memset(vma, 0, sizeof(*vma));
		vma->vm_mm = mm;
		vma->vm_ops = &dummy_vm_ops;
		// INIT_LIST_HEAD(&vma->anon_vma_chain);
	}

	static inline void vma_set_anonymous(vma_s *vma) {
		vma->vm_ops = NULL;
	}

	static inline bool vma_is_anonymous(vma_s *vma) {
		return !vma->vm_ops;
	}

	static inline void
	set_compound_order(page_s *page, unsigned int order) {
		page[1].compound_order = order;
		page[1].compound_nr = 1U << order;
	}

	/*
	 * Do pte_mkwrite, but only if the vma says VM_WRITE.  We do this when
	 * servicing faults for write access.  In the normal case, do always want
	 * pte_mkwrite.  But get_user_pages can cause write faults for mappings
	 * that do not have writing enabled, when used by access_process_vm.
	 */
	static inline pte_t maybe_mkwrite(pte_t pte, vma_s *vma) {
		if (likely(vma->vm_flags & VM_WRITE))
			pte = pte_mkwrite(pte);
		return pte;
	}

	static inline void get_page(page_s *page) {
		atomic_inc(&page->_refcount);
	}

	static inline void put_page(page_s *page) {
		atomic_dec(&page->_refcount);
	}

	static inline bool is_cow_mapping(vm_flags_t flags) {
		return (flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;
	}

	static inline bool
	page_is_guard(page_s *page) { return false; }

	static inline void clear_page_pfmemalloc(page_s *page) {
		// page->lru.next = NULL;
		list_init(&page->lru, page);
	}

	#define offset_in_page(p)		((unsigned long)(p) & ~PAGE_MASK)

	#define p4d_alloc(mm, pgd, addr)	p4d_ent_offset(pgd, addr)

	static inline pud_t *pud_alloc(mm_s *mm,
			p4d_t *p4d, unsigned long address) {
		return (arch_p4d_none(*p4d)) && __myos_pud_alloc(mm, p4d, address) ?
				NULL : pud_ent_offset(p4d, address);
	}

	static inline pmd_t *pmd_alloc(mm_s *mm,
			pud_t *pud, unsigned long address) {
		return (arch_pud_none(*pud)) && __myos_pmd_alloc(mm, pud, address)?
				NULL: pmd_ent_offset(pud, address);
	}

	static inline pte_t *pte_alloc(mm_s *mm,
			pmd_t *pmd, unsigned long address) {
		return (arch_pmd_none(*pmd)) && __myos_pte_alloc(mm, pmd, address) ?
				NULL : pte_ent_offset(pmd, address);
	}


	extern void
	reserve_bootmem_region(phys_addr_t start, phys_addr_t end);

	/*
	* Using memblock node mappings, an architecture may initialise its
	* zones, allocate the backing mem_map and account for memory holes in an
	* architecture independent manner.
	*
	* An architecture is expected to register range of page frames backed by
	* physical memory with simple_mmblk_add[_node]() before calling
	* free_area_init() passing in the PFN each zone ends at. At a basic
	* usage, an architecture is expected to do something like
	*
	* unsigned long max_zone_pfns[MAX_NR_ZONES] = {max_dma, max_normal_pfn,
	* 							 max_highmem_pfn};
	* for_each_valid_physical_page_range()
	*	memblock_add_node(base, size, nid, MEMBLOCK_NONE)
	* free_area_init(max_zone_pfns);
	*/
	void free_area_init(unsigned long *max_zone_pfn);

	extern void mem_init(void);

	extern int set_mm_exe_file(mm_s *mm, file_s *new_exe_file);


	/**
	 * find_vma_intersection() - Look up the first VMA which intersects the interval
	 * @mm: The process address space.
	 * @start_addr: The inclusive start user address.
	 * @end_addr: The exclusive end user address.
	 *
	 * Returns: The first VMA within the provided range, %NULL otherwise.  Assumes
	 * start_addr < end_addr.
	 */
	static inline vma_s
	*find_vma_intersection(mm_s *mm,
			unsigned long start_addr, unsigned long end_addr) {
		vma_s *vma = myos_find_vma(mm, start_addr);

		if (vma && end_addr <= vma->vm_start)
			vma = NULL;
		return vma;
	}

	/**
	 * vma_lookup() - Find a VMA at a specific address
	 * @mm: The process address space.
	 * @addr: The user address.
	 *
	 * Return: The vm_area_struct at the given address, %NULL otherwise.
	 */
	static inline
	vma_s *vma_lookup(mm_s *mm, unsigned long addr) {
		vma_s *vma = myos_find_vma(mm, addr);

		if (vma && addr < vma->vm_start)
			vma = NULL;

		return vma;
	}

	static inline unsigned long vm_start_gap(vma_s *vma) {
		unsigned long vm_start = vma->vm_start;

		if (vma->vm_flags & VM_GROWSDOWN) {
			vm_start -= stack_guard_gap;
			if (vm_start > vma->vm_start)
				vm_start = 0;
		}
		return vm_start;
	}

	static inline unsigned long vm_end_gap(vma_s *vma) {
		unsigned long vm_end = vma->vm_end;

		if (vma->vm_flags & VM_GROWSUP) {
			vm_end += stack_guard_gap;
			if (vm_end < vma->vm_end)
				vm_end = -PAGE_SIZE;
		}
		return vm_end;
	}

	static inline unsigned long vma_pages(vma_s *vma) {
		return (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
	}

	/* Look up the first VMA which exactly match the interval vm_start ... vm_end */
	static inline vma_s
	*find_exact_vma(mm_s *mm, unsigned long vm_start, unsigned long vm_end) {
		vma_s *vma = myos_find_vma(mm, vm_start);

		if (vma && (vma->vm_start != vm_start || vma->vm_end != vm_end))
			vma = NULL;

		return vma;
	}

	// from <linux/mm/internal.h>
	// {
		extern unsigned long highest_memmap_pfn;
		extern char *const zone_names[MAX_NR_ZONES];
	// }

	static inline zone_s *myos_page_zone(const page_s * page)
	{
		unsigned long pfn = page_to_pfn(page);
		for (int i = 0 ; i < MAX_NR_ZONES; i ++)
		{
			zone_s * zone = &(NODE_DATA(0)->node_zones[i]);
			if (pfn >= zone->zone_start_pfn && 
				pfn < (zone->zone_start_pfn + zone->spanned_pages))
				return zone;
		}
	}


	extern unsigned long __meminit
	myos_kernel_mapping_normal(size_t paddr_start, size_t paddr_end);
	extern void __iomem
	*myos_ioremap(size_t paddr_start, unsigned long size);

	extern vm_fault_s myos_dump_pagetable(unsigned long address);

#endif /* _LINUX_MM_H */