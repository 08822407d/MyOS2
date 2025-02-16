#ifndef _MM_TYPE_DECLARATION_H_
#define _MM_TYPE_DECLARATION_H_

	struct address_space;
	typedef struct address_space addr_spc_s;
	struct file;
	typedef struct file file_s;
	struct pt_regs;
	typedef struct pt_regs pt_regs_s;
	struct linux_binprm;
	typedef struct linux_binprm linux_bprm_s;
	struct linux_binfmt;
	typedef struct linux_binfmt linux_bfmt_s;


	#include <asm/mm_types.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/lib_types.h>
	#include <linux/kernel/lock_ipc_type_declaration.h>
	#include <uapi/linux/auxvec.h>


	/* memblock */
	struct memblock_region;
	typedef struct memblock_region mmblk_rgn_s;
	struct memblock_type;
	typedef struct memblock_type mmblk_type_s;
	struct memblock;
	typedef struct memblock memblock_s;

	/* memfault */
	typedef ulong		vm_flags_t;
	/**
	 * typedef vm_fault_t - Return type for page fault handlers.
	 *
	 * Page fault handlers return a bitmask of %VM_FAULT values.
	 */
	typedef __bitwise	uint vm_fault_t;
	struct vm_fault;
	typedef struct vm_fault vm_fault_s;

	/* kmalloc */
	struct slab;
	typedef struct slab slab_s;
	struct kmalloc_info_struct;
	typedef struct kmalloc_info_struct kmalloc_info_s;
	struct kmem_cache_node;
	typedef struct kmem_cache_node kmem_cache_node_s;
	struct kmem_cache_order_objects;
	typedef struct kmem_cache_order_objects kmem_cache_order_obj_s;
	struct kmem_cache;
	typedef struct kmem_cache kmem_cache_s;

	/* mm_misc */
	struct mm_struct;
	typedef struct mm_struct mm_s;

	/* page_alloc */
	struct zone;
	typedef struct zone zone_s;
	struct pglist_data;
	typedef struct pglist_data pg_data_t;
	struct page;
	typedef struct page page_s;
	struct folio;
	typedef struct folio folio_s;
	struct pgflag_defs;
	typedef struct pgflag_defs pgflag_defs_s;
	struct ptdesc;
	typedef struct ptdesc ptdesc_s;
	struct mempolicy;
	typedef struct mempolicy mpol_s;

	typedef page_s *pgtable_t;
	
	/* vm_map */
	struct vm_area_struct;
	typedef struct vm_area_struct vma_s;
	struct vm_operations_struct;
	typedef struct vm_operations_struct vm_ops_s;
	struct anon_vma;
	typedef struct anon_vma anon_vma_s;
	struct anon_vma_chain;
	typedef struct anon_vma_chain anon_vma_chain_s;
	struct vm_unmapped_area_info;
	typedef struct vm_unmapped_area_info unmapped_vma_info_s;


#endif /* _MM_TYPE_DECLARATION_H_ */