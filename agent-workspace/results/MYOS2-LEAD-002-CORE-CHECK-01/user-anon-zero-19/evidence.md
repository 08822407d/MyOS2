---
task_id: CORE-USER-ANON-ZERO-19
track_id: MYOS2-LEAD-002
record_type: fixed_time_source_evidence
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra
source_branch: time
source_pin_short12: a039d9803ade
instruction_short12: d4bffc1a4816
results_previous_head_short12: 15a162f13089
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
quote_count: 90
date: 2026-11-01
---

# 第19批固定源码锚点库

证据标签格式为路径::符号；每条引文直接从冻结time代码读取1–5行，原样保留缩进和特殊字符。注释只能作为该动作未活动的证据；头注释或外部Linux语义不自动成为MyOS2的运行事实。

### AZ19-A001 · none入口
[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault]
```c
handle_pte_fault(vm_fault_s *vmf) {
	pte_t entry;

	if (pte_none(vmf->orig_pte))
		return do_pte_missing(vmf);
```

### AZ19-A002 · 匿名分派
[VERIFIED mykernel/mm/fault/fault.c::do_pte_missing]
```c
do_pte_missing(vm_fault_s *vmf) {
	if (vma_is_anonymous(vmf->vma))
		return do_anonymous_page(vmf);
	else
		return do_fault(vmf);
```

### AZ19-A003 · PTE与NULL出口
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	vmf.pte_ptr = pte_offset_map_nolock(vmf.vma->vm_mm,
					vmf.pmd_entp, vmf.address, &(vmf.ptl));
	if (unlikely(vmf.pte_ptr == NULL))
		return 0;
```

### AZ19-A004 · 保存原PTE并分派
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	vmf.orig_pte = ptep_get_lockless(vmf.pte_ptr);
	vmf.flags |= FAULT_FLAG_ORIG_PTE_VALID;


	ret = handle_pte_fault(&vmf);
```

### AZ19-A005 · vmf初始参数
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	vm_fault_s vmf = {
		.vma		= vma,
		.address	= address & PAGE_MASK,
		.flags		= flags,
		.pgoff		= linear_page_index(vma, address),
```

### AZ19-A006 · 上层先pte_alloc
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	if (pte_alloc(mm, vmf.pmd_entp)) {
		goto fail;
	}
	barrier();
```

### AZ19-A007 · ret初始化
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	folio_s *folio;
	vm_fault_t ret = 0;
	// int nr_pages = 1;
	pte_t entry;
```

### AZ19-A008 · 共享拒绝
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	/* File mapping without ->vm_ops ? */
	if (vma->vm_flags & VM_SHARED)
		return VM_FAULT_SIGBUS;
```

### AZ19-A009 · 第二次pte_alloc门
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	if (pte_alloc(vma->vm_mm, vmf->pmd_entp))
		return VM_FAULT_OOM;
```

### AZ19-A010 · 读零页PTE
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	/* Use the zero-page for reads */
	if (!(vmf->flags & FAULT_FLAG_WRITE)) {
		entry = pte_mkspecial(pfn_pte(my_zero_pfn(vmf->address),
					vma->vm_page_prot));
```

### AZ19-A011 · 读页PTE指针门
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
		// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd,
		// 		vmf->address, &vmf->ptl);
		if (!vmf->pte_ptr)
			goto unlock;
```

### AZ19-A012 · 读页复核注释
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
		// if (vmf_pte_changed(vmf)) {
		// 	update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
		// 	goto unlock;
		// }
```

### AZ19-A013 · 读页直接goto setpte
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
		// ret = check_stable_address_space(vma->vm_mm);
		// if (ret)
		// 	goto unlock;
		goto setpte;
```

### AZ19-A014 · 匿名写分配失败分支
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	folio = alloc_anon_folio(vmf);
	if (IS_ERR(folio))
		return 0;
	if (!folio)
		goto oom;
```

### AZ19-A015 · 匿名写uptodate注释
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	//  * The memory barrier inside __folio_mark_uptodate makes sure that
	//  * preceding stores to the page contents become visible before
	//  * the set_pte_at() write.
	//  */
	// __folio_mark_uptodate(folio);
```

### AZ19-A016 · 写PTE标记条件
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	entry = mk_pte(&folio->page, vma->vm_page_prot);
	if (vma->vm_flags & VM_WRITE)
		entry = pte_mkwrite(pte_mkdirty(entry));
```

### AZ19-A017 · 写PTE指针NULL去release
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	// vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd, addr, &vmf->ptl);
	if (!vmf->pte_ptr)
		goto release;
```

### AZ19-A018 · 写PTE重检注释
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	// if (vmf_pte_changed(vmf)) {
	// 	update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
	// 	goto release;
	// }
```

### AZ19-A019 · 写引用/rmap/LRU注释
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
	// folio_ref_add(folio, nr_pages - 1);
	// add_mm_counter(vma->vm_mm, MM_ANONPAGES, nr_pages);
	// folio_add_new_anon_rmap(folio, vma, vmf->address);
	// folio_add_lru_vma(folio, vma);
```

### AZ19-A020 · 共同set_pte_at
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
setpte:
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte_ptr, entry);
```

### AZ19-A021 · unlock与正常返回
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
unlock:
	// if (vmf->pte)
	// 	pte_unmap_unlock(vmf->pte, vmf->ptl);
	return ret;
```

### AZ19-A022 · 释放及OOM出口
[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page]
```c
release:
	folio_put(folio);
	goto unlock;
oom:
	return VM_FAULT_OOM;
```

### AZ19-A023 · 申请need_zero=true
[VERIFIED mykernel/mm/fault/fault.c::alloc_anon_folio]
```c
static folio_s
*alloc_anon_folio(vm_fault_s *vmf) {
	vma_s *vma = vmf->vma;
	return folio_prealloc(vma->vm_mm, vma, vmf->address, true);
```

### AZ19-A024 · 清零分配候选注释
[VERIFIED mykernel/mm/fault/fault.c::folio_prealloc]
```c
	// if (need_zero)
	// 	new_folio = vma_alloc_zeroed_movable_folio(vma, addr);
	// else
	// 	new_folio = vma_alloc_folio(GFP_HIGHUSER_MOVABLE, 0, vma,
	// 				    addr, false);
```

### AZ19-A025 · 活动预分配忽略清零
[VERIFIED mykernel/mm/fault/fault.c::folio_prealloc]
```c
	// return new_folio;

	return folio_alloc(GFP_HIGHUSER_MOVABLE, 0);
}
```

### AZ19-A026 · 零页PFN变量
[VERIFIED mykernel/mm/fault/fault.c::zero_pfn]
```c
ulong	zero_pfn __read_mostly;
EXPORT_SYMBOL(zero_pfn);
```

### AZ19-A027 · 计算PFN
[VERIFIED mykernel/mm/fault/fault.c::init_zero_pfn]
```c
int __init
init_zero_pfn(void) {
	zero_pfn = page_to_pfn(ZERO_PAGE(0));
	return 0;
```

### AZ19-A028 · initcall标记
[VERIFIED mykernel/mm/fault/fault.c::init_zero_pfn]
```c
}
early_initcall(init_zero_pfn);
```

### AZ19-A029 · 重检helper被提供
[VERIFIED mykernel/mm/fault/fault.c::vmf_pte_changed]
```c
vmf_pte_changed(struct vm_fault *vmf) {
	if (vmf->flags & FAULT_FLAG_ORIG_PTE_VALID)
		return !pte_same(ptep_get(vmf->pte_ptr), vmf->orig_pte);

	return !pte_none(ptep_get(vmf->pte_ptr));
```

### AZ19-A030 · 已有PTE写保护入口
[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault]
```c
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
		else if (likely(vmf->flags & FAULT_FLAG_WRITE))
			entry = pte_mkdirty(entry);
```

### AZ19-A031 · 匿名谓词
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_is_anonymous]
```c
		PREFIX_STATIC_INLINE
		bool
		vma_is_anonymous(vma_s *vma) {
			return !vma->vm_ops;
		}
```

### AZ19-A032 · 匿名赋值
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_set_anonymous]
```c
		void
		vma_set_anonymous(vma_s *vma) {
			vma->vm_ops = NULL;
		}
```

### AZ19-A033 · VMA初始清零
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
		vma_init(vma_s *vma, mm_s *mm) {
			static const vm_ops_s dummy_vm_ops = {};
			memset(vma, 0, sizeof(*vma));
			INIT_LIST_S(&vma->list);
```

### AZ19-A034 · 默认dummy vm_ops
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
			// INIT_LIST_HEAD(&vma->anon_vma_chain);
			vma->vm_mm = mm;
			vma->vm_ops = &dummy_vm_ops;
		}
```

### AZ19-A035 · VMA申请源
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_alloc]
```c
{
	vma_s *vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (vma)
		vma_init(vma, mm);
	return vma;
```

### AZ19-A036 · VMA保护初始化
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;
```

### AZ19-A037 · 共享shmem入口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	} else if (vm_flags & VM_SHARED) {
		error = shmem_zero_setup(vma);
		if (error)
```

### AZ19-A038 · 私有匿名入口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	} else {
		vma_set_anonymous(vma);
	}
```

### AZ19-A039 · brk设置匿名
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	vma_set_anonymous(vma);
	vma->vm_start = addr;
	vma->vm_end = addr + len;
```

### AZ19-A040 · brk保护源
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	vma->vm_pgoff = pgoff;
	vma->vm_flags = flags;
	vma->vm_page_prot = vm_get_page_prot(flags);
	simple_vma_link(mm, vma, prev);
```

### AZ19-A041 · CPU错误码写标志
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (err_code & X86_PF_SHSTK)
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_WRITE)
		flags |= FAULT_FLAG_WRITE;
```

### AZ19-A042 · 访问错误门注释
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// if (unlikely(access_error(error_code, vma))) {
	// 	bad_area_access_error(regs, error_code, address, vma);
	// 	return;
	// }
```

### AZ19-A043 · 上层非零结果忙等
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);
```

### AZ19-A044 · pte_alloc绑定
[VERIFIED mykernel/mm/fault/pgalloc_macro.h::pte_alloc]
```c
	#define pte_alloc				pgtble_alloc
	#define pte_alloc_map			pgtble_alloc_map
	#define pte_alloc_map_lock		pgtble_alloc_map_lock
```

### AZ19-A045 · pte_alloc的实际条件
[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc]
```c
		PREFIX_STATIC_INLINE
		int
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp) {
			return (unlikely(pmd_none(*(pmd_entp))) && __pte_alloc(mm, pmd_entp));
		}
```

### AZ19-A046 · 错误位定义
[VERIFIED mykernel/mm/fault/fault_const.h::VM_FAULT_OOM]
```c
	enum vm_fault_reason {
		VM_FAULT_OOM			= 0x000001,
		VM_FAULT_SIGBUS			= 0x000002,
		VM_FAULT_MAJOR			= 0x000004,
		VM_FAULT_HWPOISON		= 0x000010,
```

### AZ19-A047 · WRITE标志
[VERIFIED mykernel/mm/fault/fault_const.h::FAULT_FLAG_WRITE]
```c
	 */
	enum fault_flag {
		FAULT_FLAG_WRITE			= 1 << 0,
		FAULT_FLAG_MKWRITE			= 1 << 1,
```


### AZ19-A048 · 私有读写模板为PAGE_COPY
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
static pgprot_t protection_map[16] __ro_after_init = {
	[VM_NONE]									= PAGE_NONE,
	[VM_READ]									= PAGE_READONLY,
	[VM_WRITE]									= PAGE_COPY,
	[VM_WRITE | VM_READ]						= PAGE_COPY,
```

### AZ19-A049 · 私有执行/写模板
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_EXEC]									= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_READ]							= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_WRITE]						= PAGE_COPY_EXEC,
	[VM_EXEC | VM_WRITE | VM_READ]				= PAGE_COPY_EXEC,
```

### AZ19-A050 · 共享模板另行映射
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_SHARED]									= PAGE_NONE,
	[VM_SHARED | VM_READ]						= PAGE_READONLY,
	[VM_SHARED | VM_WRITE]						= PAGE_SHARED,
	[VM_SHARED | VM_WRITE | VM_READ]			= PAGE_SHARED,
```

### AZ19-A051 · 四种VM flag决定初始保护
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
pgprot_t vm_get_page_prot(ulong vm_flags)
{
	ulong val = pgprot_val(protection_map[vm_flags &
					(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]);
```

### AZ19-A052 · 支持掩码只清位
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
	// val = __sme_set(val);
	if (val & _PAGE_PRESENT)
		val &= __supported_pte_mask;
	return __pgprot(val);
```

### AZ19-A053 · 私有PAGE_COPY及READONLY无RW
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_COPY]
```c
	#define PAGE_COPY_NOEXEC			__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_COPY_EXEC				__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY					__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY				__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY_EXEC			__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
```

### AZ19-A054 · 共享PAGE_SHARED含RW
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_SHARED]
```c
	#define PAGE_NONE					__pg(   0|   0|   0|___A|   0|   0|   0|___G)
	#define PAGE_SHARED					__pg(__PP|__RW|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_SHARED_EXEC			__pg(__PP|__RW|_USR|___A|   0|   0|   0|   0)
```

### AZ19-A055 · PRESENT/RW/USER位分别定义
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_RW]
```c
	#define _PAGE_PRESENT				(_AT(pteval_t, 1) << _PAGE_BIT_PRESENT)
	#define _PAGE_RW					(_AT(pteval_t, 1) << _PAGE_BIT_RW)
	#define _PAGE_USER					(_AT(pteval_t, 1) << _PAGE_BIT_USER)
```

### AZ19-A056 · SPECIAL独立标记
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_SPECIAL]
```c
	#define _PAGE_PAT					(_AT(pteval_t, 1) << _PAGE_BIT_PAT)
	#define _PAGE_PAT_LARGE				(_AT(pteval_t, 1) << _PAGE_BIT_PAT_LARGE)
	#define _PAGE_SPECIAL				(_AT(pteval_t, 1) << _PAGE_BIT_SPECIAL)
	#define _PAGE_CPA_TEST				(_AT(pteval_t, 1) << _PAGE_BIT_CPA_TEST)
```

### AZ19-A057 · PTE置位操作使用OR
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_set_flags]
```c
		pte_t
		pte_set_flags(pte_t pte, pteval_t set) {
			pteval_t v = arch_pte_val(pte);
			return arch_make_pte(v | set);
		}
```

### AZ19-A058 · special不会主动清RW
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_mkspecial]
```c
	static inline pte_t pte_mkspecial(pte_t pte)
	{
		return pte_set_flags(pte, _PAGE_SPECIAL);
	}
```

### AZ19-A059 · pfn_pte直接带入原pgprot
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pte]
```c
		pfn_pte(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PTE_PFN_MASK;
			return __pte(pfn | check_pgprot(pgprot));
```

### AZ19-A060 · RW判断真实实现
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_writable]
```c
		PREFIX_STATIC_INLINE
		int
		pte_writable(pte_t pte) {
			return arch_pte_flags(pte) & _PAGE_RW;
		}
```

### AZ19-A061 · mkwrite增加RW
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_mkwrite]
```c
		pte_t
		pte_mkwrite(pte_t pte) {
			return pte_set_flags(pte, _PAGE_RW);
		}
```

### AZ19-A062 · mkdirty增加脏位宏
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_mkdirty]
```c
		pte_t
		pte_mkdirty(pte_t pte) {
			return pte_set_flags(pte,
						_PAGE_DIRTY | _PAGE_SOFT_DIRTY);
		}
```

### AZ19-A063 · set_pte_at调用set_pte
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_at]
```c
		void
		set_pte_at(mm_s *mm, ulong addr, pte_t *ptep, pte_t pte) {
			// page_table_check_pte_set(mm, addr, ptep, pte);
			set_pte(ptep, pte);
		}
```

### AZ19-A064 · 实际WRITE_ONCE
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte]
```c
		void
		set_pte(pte_t *pte_ptr, pte_t pte) {
			WRITE_ONCE(*(pte_ptr), (pte));
		}
```

### AZ19-A065 · mk_pte由page转PFN
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::mk_pte]
```c
	 * to linux/mm.h:page_to_nid())
	 */
	#define mk_pte(page, pgprot)   pfn_pte(page_to_pfn(page), (pgprot))
```

### AZ19-A066 · none软件掩码
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_is_none]
```c
		PREFIX_STATIC_INLINE
		int
		pte_is_none(pte_t pte) {
			return !(pte.val & ~(_PAGE_KNL_ERRATUM_MASK));
		}
```

### AZ19-A067 · present软件谓词含PROTNONE
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_is_present]
```c
		int
		pte_is_present(pte_t pte) {
			return arch_pte_flags(pte) &
						(_PAGE_PRESENT | _PAGE_PROTNONE);
		}
```

### AZ19-A068 · 比较val
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_same]
```c
		int
		pte_same(pte_t a, pte_t b) {
			return a.val == b.val;
		}
```

### AZ19-A069 · ZERO_PAGE转换BSS对象
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::ZERO_PAGE]
```c
	extern ulong empty_zero_page[PAGE_SIZE / sizeof(ulong)] __visible;
	#define ZERO_PAGE(vaddr) ((void)(vaddr),virt_to_page(empty_zero_page))
```

### AZ19-A070 · pte_write宏绑定架构判断
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pte_write]
```c
	#define pte_write(ptep)		pte_writable(ptep)
	#define pte_present			pte_is_present
	#define pte_none			pte_is_none
```

### AZ19-A071 · 零页PFN助手不消费addr
[VERIFIED mykernel/mm/fault/pgtable.h::my_zero_pfn]
```c
		ulong
		my_zero_pfn(ulong addr) {
			extern ulong zero_pfn;
			return zero_pfn;
```

### AZ19-A072 · 空零页在BSS占PAGE_SIZE
[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::empty_zero_page]
```c
	__PAGE_ALIGNED_BSS
SYM_DATA_START_PAGE_ALIGNED(empty_zero_page)
	.skip PAGE_SIZE
SYM_DATA_END(empty_zero_page)
```

### AZ19-A073 · 链接器BSS区域
[VERIFIED mykernel/arch/x86_64/kernel.lds::__bss_start]
```c
	.bss ALIGN(_Page_Size) : AT(ADDR(.bss) - __START_KERNEL_map)
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
```

### AZ19-A074 · clear_bss调用memset
[VERIFIED mykernel/arch/x86_64/kernel/head64.c::clear_bss]
```c
static void __init
clear_bss(void) {
	memset(__bss_start, 0,
		(unsigned long) __bss_stop - (unsigned long) __bss_start);
}
```

### AZ19-A075 · 架构入口调用clear_bss
[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
asmlinkage __visible void __init
x86_64_start_kernel(char * real_mode_data)
{
	clear_bss();
```

### AZ19-A076 · 入口进入start_kernel
[VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
	// void __init x86_64_start_reservations(char *real_mode_data)
	// {
		/* version is always not zero if it is copied */
		start_kernel();
	// }
```

### AZ19-A077 · 显式调用init_zero_pfn
[VERIFIED mykernel/init/main.c::do_pre_smp_initcalls]
```c
extern int init_zero_pfn(void);
static void __init
do_pre_smp_initcalls(void) {
	init_zero_pfn();
}
```

### AZ19-A078 · 内核init调用pre_smp
[VERIFIED mykernel/init/main.c::kernel_init_freeable]
```c
	// rcu_init_tasks_generic();
	do_pre_smp_initcalls();
	// lockup_detector_init();
```

### AZ19-A079 · kernel_init转入kernel_init_freeable
[VERIFIED mykernel/init/main.c::kernel_init]
```c
extern void do_name(void);
	do_name();
	kernel_init_freeable();
```


### AZ19-A080 · 分配包装直接page_folio且忽略参数
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::folio_alloc_noprof]
```c
folio_s *folio_alloc_noprof(gfp_t gfp, uint order)
{
	return page_folio(alloc_page(GFP_HIGHUSER_MOVABLE));
```

### AZ19-A081 · alloc_page绑定order0
[VERIFIED mykernel/mm/highmem/highmem_macro.h::alloc_page]
```c
	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)

	#define folio_alloc				folio_alloc_noprof
```

### AZ19-A082 · page_folio经过compound_head
[VERIFIED mykernel/mm/page_alloc/page-flags_macro.h::page_folio]
```c
	#define page_folio(p)		(_Generic((p),							\
				const page_s *:		(const folio_s *)_compound_head(p), \
				page_s *:			(folio_s *)_compound_head(p))		\
			)
```

### AZ19-A083 · compound_head读取传入页字段
[VERIFIED mykernel/mm/page_alloc/page-flags.h::_compound_head]
```c
		_compound_head(const page_s *page) {
			ulong head = READ_ONCE(page->compound_head);
			if (unlikely(head & 1))
				return head - 1;
			return (ulong)page;
```

### AZ19-A084 · 分配GFP无__GFP_ZERO
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_HIGHUSER_MOVABLE]
```c
	#define GFP_HIGHUSER			(GFP_USER | __GFP_HIGHMEM)
	#define GFP_HIGHUSER_MOVABLE	(GFP_HIGHUSER | __GFP_MOVABLE | \
										__GFP_SKIP_KASAN_POISON)
```

### AZ19-A085 · 清页动作仅__GFP_ZERO时活动
[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	// if (init)
	// kernel_init_pages(page, 1 << order);
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```

### AZ19-A086 · 分配页引用递增
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

```

### AZ19-A087 · folio_put归零调用后端
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_put]
```c
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
		}
```

### AZ19-A088 · 后端仅注释
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
void __folio_put(folio_s *folio)
{
	// if (unlikely(folio_is_zone_device(folio)))
	// 	free_zone_device_page(&folio->page);
	// else if (unlikely(folio_test_large(folio)))
```

### AZ19-A089 · 后端无活动free
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
	// else if (unlikely(folio_test_large(folio)))
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
}
```

### AZ19-A090 · IS_ERR是分类而非错误生产者
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```


## 证据范围与反证

- 匿名判定：vm_ops==NULL，至少两条VMA生产路径有明确vma_set_anonymous赋值；不推导全部来源或其它访问控制。
- 零页身份：empty_zero_page由BSS页对齐空间占位，链接脚本BSS范围与x86_64_start_kernel调用clear_bss可作条件性零填充源，init_zero_pfn按ZERO_PAGE描述符换算PFN。未对实际内存字节、物理PFN数值或硬件引导完成情况取证。
- 保护：pte_mkspecial仅按位OR特殊标记。普通VMA的PAGE_COPY等默认保护为不可写，但输入vm_page_prot的其它生产者或改变尚未穷尽。
- 状态：读写缺页都会把局部entry传到set_pte_at；被注释的vmf_pte_changed、pte_offset_map_lock、rmap、LRU及通知不能补为活动指令。
- 分配：alloc_anon_folio的need_zero=true不被folio_prealloc活动主体消费，GFP_HIGHUSER_MOVABLE未含__GFP_ZERO；值为0、NULL和编码错误指针都按来源与条件区分。
- 应合读18主线R18-S01/S02/S03、17主线R17-S01/S02/S03、16主线R16-S01/S02及12主线缺页返回限定；本次不改变历史文件或旧问题状态。
- 编译、预处理、C/ASM执行、QEMU、真实内核和CPU/TLB运行验证均NOT_RUN；全局锁、全量符号绑定、所有特殊页引用及数据隔离均未认证。

