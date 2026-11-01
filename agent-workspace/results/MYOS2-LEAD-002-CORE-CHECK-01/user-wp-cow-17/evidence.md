---
task_id: "CORE-USER-WP-COW-17"
track_id: "MYOS2-LEAD-002"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra（Owner报告，非后端证明）"
source_pin_short12: "a039d9803ade"
results_initial_head_short12: "38a23958a355"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
date: "2026-11-01"
record_type: "source_anchor_ledger"
---

# 第17批源码证据集

每条 `W17-Axx` 都直接取自固定 `time @ a039d9803ade` 文件，对应定义体1–5行。`[VERIFIED]` 只证明源文本字节和所列局部C分支，不证明全局完整性、代码已运行、CPU行为或缺陷已复现。注释只作为未活动范围证据。

### W17-A01 — none走missing路径
[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault]
```c

	if (pte_none(vmf->orig_pte))
		return do_pte_missing(vmf);
```

### W17-A02 — 现存PTE第一次快照重检无活动锁
[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault]
```c
	// spin_lock(vmf->ptl);
	entry = vmf->orig_pte;
	if (unlikely(!pte_same(ptep_get(vmf->pte_ptr), entry))) {
		update_mmu_tlb(vmf->vma, vmf->address, vmf->pte_ptr);
		goto unlock;
```

### W17-A03 — WRITE且不可写进入do_wp_page
[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault]
```c
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
		else if (likely(vmf->flags & FAULT_FLAG_WRITE))
			entry = pte_mkdirty(entry);
```

### W17-A04 — 旧页解析
[VERIFIED mykernel/mm/fault/fault.c::do_wp_page]
```c
	vmf->page = vm_normal_page(vma, vmf->address, *vmf->pte_ptr);
	if (vmf->page)
		folio = page_folio(vmf->page);
```

### W17-A05 — 共享MAYSHARE无限循环
[VERIFIED mykernel/mm/fault/fault.c::do_wp_page]
```c
	if (vma->vm_flags & (VM_SHARED | VM_MAYSHARE)) {
		// catch this case, handler temporarily not implemented
		while (1);
		
```

### W17-A06 — old_folio get再复制
[VERIFIED mykernel/mm/fault/fault.c::do_wp_page]
```c
	if (folio)
		folio_get(folio);

	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return wp_page_copy(vmf);
```

### W17-A07 — PFN过上界无page
[VERIFIED mykernel/mm/fault/fault.c::vm_normal_page]
```c
check_pfn:
	if (unlikely(pfn > highest_memmap_pfn)) {
		// print_bad_pte(vma, addr, pte, NULL);
		return NULL;
	}
```

### W17-A08 — PFN映射page
[VERIFIED mykernel/mm/fault/fault.c::vm_normal_page]
```c
out:
	return pfn_to_page(pfn);
```

### W17-A09 — zero PFN和预分配
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	pfn_is_zero = is_zero_pfn(pte_pfn(vmf->orig_pte));
	new_folio = folio_prealloc(mm, vma, vmf->address, pfn_is_zero);
	if (!new_folio)
		goto oom;
```

### W17-A10 — need_zero参数未使用
[VERIFIED mykernel/mm/fault/fault.c::folio_prealloc]
```c
	// return new_folio;

	return folio_alloc(GFP_HIGHUSER_MOVABLE, 0);
}
```

### W17-A11 — 分配器封装不检查alloc_page结果
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::folio_alloc_noprof]
```c
folio_s *folio_alloc_noprof(gfp_t gfp, uint order)
{
	return page_folio(alloc_page(GFP_HIGHUSER_MOVABLE));
```

### W17-A12 — page_folio宏
[VERIFIED mykernel/mm/page_alloc/page-flags_macro.h::page_folio]
```c
	#define page_folio(p)		(_Generic((p),							\
				const page_s *:		(const folio_s *)_compound_head(p), \
				page_s *:			(folio_s *)_compound_head(p))		\
			)
```

### W17-A13 — NULL输入将触及page字段
[VERIFIED mykernel/mm/page_alloc/page-flags.h::_compound_head]
```c
		_compound_head(const page_s *page) {
			ulong head = READ_ONCE(page->compound_head);
			if (unlikely(head & 1))
				return head - 1;
			return (ulong)page;
```

### W17-A14 — 物理页出队计数增加
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

```

### W17-A15 — 仅__GFP_ZERO清零
[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	// if (init)
	// kernel_init_pages(page, 1 << order);
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);
```

### W17-A16 — 默认分配GFP不带清零位
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_HIGHUSER_MOVABLE]
```c
	#define GFP_HIGHUSER			(GFP_USER | __GFP_HIGHMEM)
	#define GFP_HIGHUSER_MOVABLE	(GFP_HIGHUSER | __GFP_MOVABLE | \
										__GFP_SKIP_KASAN_POISON)
```

### W17-A17 — 只有非zero PFN调用复制
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	if (!pfn_is_zero) {
		int err = __wp_page_copy_user(&new_folio->page, vmf->page, vmf);
		if (err) {
```

### W17-A18 — 非空源复制入口
[VERIFIED mykernel/mm/fault/fault.c::__wp_page_copy_user]
```c
	if (likely(src)) {
		copy_user_highpage(dst, src, addr, vma);
		return -ENOERR;
	}
```

### W17-A19 — 空源分支ret归零
[VERIFIED mykernel/mm/fault/fault.c::__wp_page_copy_user]
```c

	ret = 0;

pte_unlock:
```

### W17-A20 — copy_user_highpage直映
[VERIFIED mykernel/mm/highmem/highmem.h::copy_user_highpage]
```c
			copy_user_page((void *)page_to_virt(to),
					(void *)page_to_virt(from), vaddr, to);
		}
```

### W17-A21 — arch copy helper
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::copy_user_page]
```c
		void
		copy_user_page(void *to, void *from,
				ulong vaddr, page_s *topage) {
			copy_page(to, from);
```

### W17-A22 — PAGE_SIZE memcpy宏
[VERIFIED mykernel/include/linux/kernel/asm-generic/page.h::copy_page]
```c
	#	define clear_page(page)		memset((page), 0, PAGE_SIZE)
	#	define copy_page(to,from)	memcpy((to), (from), PAGE_SIZE)
```

### W17-A23 — PTE复核条件
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	//  */
	// vmf->pte = pte_offset_map_lock(mm, vmf->pmd, vmf->address, &vmf->ptl);
	if (likely(vmf->pte_ptr != NULL &&
			pte_same(ptep_get(vmf->pte_ptr), vmf->orig_pte))) {
```

### W17-A24 — PTE val相等
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_same]
```c
		int
		pte_same(pte_t a, pte_t b) {
			return a.val == b.val;
		}
```

### W17-A25 — 新页PTE构造
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		entry = mk_pte(&new_folio->page, vma->vm_page_prot);
		// entry = pte_sw_mkyoung(entry);
		if (unlikely(unshare)) {
```

### W17-A26 — 普通私有写进入dirty/mkwrite
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		} else {
			entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		}
```

### W17-A27 — VM_WRITE才设RW
[VERIFIED mykernel/mm/fault/pgtable.h::maybe_mkwrite]
```c
		maybe_mkwrite(pte_t pte, vma_s *vma) {
			if (likely(vma->vm_flags & VM_WRITE))
				pte = pte_mkwrite(pte);
			return pte;
		}
```

### W17-A28 — BUG_ON与set_pte_at
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		// ptep_clear_flush(vma, vmf->address, vmf->pte);
		// folio_add_new_anon_rmap(new_folio, vma, vmf->address, RMAP_EXCLUSIVE);
		// folio_add_lru_vma(new_folio, vma);
		BUG_ON(unshare && pte_write(entry));
		set_pte_at(mm, vmf->address, vmf->pte_ptr, entry);
```

### W17-A29 — PTE设置入口
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_at]
```c
		set_pte_at(mm_s *mm, ulong addr, pte_t *ptep, pte_t pte) {
			// page_table_check_pte_set(mm, addr, ptep, pte);
			set_pte(ptep, pte);
		}
```

### W17-A30 — PTE WRITE_ONCE
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte]
```c
		void
		set_pte(pte_t *pte_ptr, pte_t pte) {
			WRITE_ONCE(*(pte_ptr), (pte));
		}
```

### W17-A31 — 成功安装后new_folio别名old_folio
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		/* Free the old page.. */
		new_folio = old_folio;
		page_copied = 1;
		// pte_unmap_unlock(vmf->pte_ptr, vmf->ptl);
	} else {
```

### W17-A32 — 第一处folio_put
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	if (new_folio)
		folio_put(new_folio);
	if (old_folio) {
		// if (page_copied)
		// 	free_swap_cache(old_folio);
```

### W17-A33 — 第二处folio_put与0返回
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		// 	free_swap_cache(old_folio);
		folio_put(old_folio);
	}

	return 0;
```

### W17-A34 — OOM及old ref清理
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
oom:
	ret = VM_FAULT_OOM;
out:
	if (old_folio)
		folio_put(old_folio);
```

### W17-A35 — get原子增加
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_get]
```c
		void
		folio_get(folio_s *folio) {
			// VM_BUG_ON_FOLIO(folio_ref_zero_or_close_to_overflow(folio), folio);
			folio_ref_inc(folio);
		}
```

### W17-A36 — put原子减少
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_put]
```c
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
		}
```

### W17-A37 — 回收接口没有活动归还调用
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
	// else if (unlikely(folio_test_large(folio)))
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
}
```

### W17-A38 — 返回锁指针非加锁
[VERIFIED mykernel/mm/fault/pgtable-generic.c::pte_offset_map_nolock]
```c
	ptep = __pte_offset_map(pmdp, addr, &pmdval);
	if (likely(ptep))
		*ptlp = pte_lockptr(mm, &pmdval);
	return ptep;
```

### W17-A39 — 复制助手出错的folio归还
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
			folio_put(new_folio);
			if (old_folio)
				folio_put(old_folio);

			return err == -EHWPOISON ? VM_FAULT_HWPOISON : 0;
```

### W17-A40 — PTE复核失败后的动作
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	} else {
		update_mmu_tlb(vma, vmf->address, vmf->pte_ptr);
		// pte_unmap_unlock(vmf->pte_ptr, vmf->ptl);
	}
```

### W17-A41 — PTE空指针前置返回0
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	vmf.pte_ptr = pte_offset_map_nolock(vmf.vma->vm_mm,
					vmf.pmd_entp, vmf.address, &(vmf.ptl));
	if (unlikely(vmf.pte_ptr == NULL))
		return 0;
```

### W17-A42 — orig_pte读取后分派
[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	vmf.orig_pte = ptep_get_lockless(vmf.pte_ptr);
	vmf.flags |= FAULT_FLAG_ORIG_PTE_VALID;


	ret = handle_pte_fault(&vmf);
```

### W17-A43 — zero特殊处理注释
[VERIFIED mykernel/mm/fault/fault.c::vm_normal_page]
```c
	// 		if (!is_cow_mapping(vma->vm_flags))
	// 			return NULL;
	// 	}
	// }

```

### W17-A44 — PRESENT或PROTNONE
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_is_present]
```c
		int
		pte_is_present(pte_t pte) {
			return arch_pte_flags(pte) &
						(_PAGE_PRESENT | _PAGE_PROTNONE);
		}
```

### W17-A45 — none掩码语义
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_is_none]
```c
		PREFIX_STATIC_INLINE
		int
		pte_is_none(pte_t pte) {
			return !(pte.val & ~(_PAGE_KNL_ERRATUM_MASK));
		}
```

### W17-A46 — flush与清旧PTE在注释
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		// /*
		//  * Clear the pte entry and flush it first, before updating the
		//  * pte with the new entry, to keep TLBs on different CPUs in
		//  * sync. This code used to set the new PTE then flush TLBs, but
		//  * that left a window where the new PTE could be loaded into
```

### W17-A47 — rmap语句在注释
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		// 	 */
		// 	folio_remove_rmap_pte(old_folio, vmf->page, vma);
		// }

		/* Free the old page.. */
```

### W17-A48 — 旧页复用在注释
[VERIFIED mykernel/mm/fault/fault.c::do_wp_page]
```c
	// if (folio && folio_test_anon(folio) &&
	//     (PageAnonExclusive(vmf->page) || wp_can_reuse_anon_folio(folio, vma))) {
	// 	if (!PageAnonExclusive(vmf->page))
	// 		SetPageAnonExclusive(vmf->page);
	// 	if (unlikely(unshare)) {
```

### W17-A49 — OOM路径返回
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	if (old_folio)
		folio_put(old_folio);

	return VM_FAULT_OOM;
```

### W17-A50 — 回退返回ret
[VERIFIED mykernel/mm/fault/fault.c::__wp_page_copy_user]
```c
	// flush_dcache_page(dst);

	return ret;
```

### W17-A51 — zero PFN谓词
[VERIFIED mykernel/mm/fault/pgtable.h::is_zero_pfn]
```c
		int
		is_zero_pfn(ulong pfn) {
			extern ulong zero_pfn;
			return pfn == zero_pfn;
```

### W17-A52 — 物理分配失败NULL
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
		// }
		}
	// }
	return NULL;
```

### W17-A53 — old_folio身份来自vmf.page
[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
	if (vmf->page)
		old_folio = page_folio(vmf->page);
	// ret = vmf_anon_prepare(vmf);
```

## 反向证据与限制

已查看 `fault.c` 函数完整控制流。`pte_offset_map_nolock` 不加锁；`vm_normal_page` 的zero/PFNMAP特殊过滤、旧页复用、清旧PTE/flush、rmap更新是注释，不可计为活动。`folio_alloc_noprof` 无分配NULL门，`page_folio` 会调用读page字段的 `_compound_head`；不能据外层OOM标签推断安全失败。`__folio_put` 活动主体没有归还页。CPU重执行、硬件TLB、并发与物理内存内容均未验证。
