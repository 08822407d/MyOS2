---
task_id: CORE-USER-ARG-PAGES-24
track_id: MYOS2-LEAD-002
record_type: frozen_time_source_evidence
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected, backend not independently attested)
date: 2026-11-02
read_channel: connector
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: 5b81217fc614
result_base_short12: 299a960562a9
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
self_check:
  numbered_anchor_segments: 82
  exact_fixed_source_quotations: 82
  max_quote_lines: 5
---

# AG24 固定源码逐字证据账

AG24-A###的标签是所选固定time源码path::symbol与1–5行原文。标签只证明源码字面和有界控制流，注释不等于活动执行，调用未必完成，指针或返回值未定义时不得继续推断确定机器结果。

### AG24-A001 — get_arg_page中局部page未初始化而ret是int且mm来自bprm
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
*get_arg_page(linux_bprm_s *bprm, ulong pos, int write) {
	page_s *page;
	vma_s *vma = bprm->vma;
	mm_s *mm = bprm->mm;
	int ret;
```

### AG24-A002 — 调用者命名mmap_read_lock
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	mmap_read_lock(mm);
```

### AG24-A003 — long GUP结果写入int ret并在非正时return NULL
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	ret = myos_get_user_pages(mm, pos, 1, &page);
	mmap_read_unlock(mm);
	if (ret <= 0)
		return NULL;
```

### AG24-A004 — 返回page前未见对page有效性的另外验证
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	// if (write)
	// 	acct_arg_size(bprm, vma_pages(vma));

	return page;
```

### AG24-A005 — put_arg_page直接调用put_page
[VERIFIED mykernel/sched/forkexec/exec.c::put_arg_page]
```c
static void
put_arg_page(page_s *page) {
	put_page(page);
}
```

### AG24-A006 — free_arg_pages的选定函数体没有活动释放语句
[VERIFIED mykernel/sched/forkexec/exec.c::free_arg_pages]
```c
static void
free_arg_pages(linux_bprm_s *bprm) {
}
```

### AG24-A007 — copy_strings对argc逐字符串逆向迭代
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	while (argc-- > 0) {
		const char __user *str;
		int len;
		ulong pos;
```

### AG24-A008 — copy_strings取得用户源指针并判错误
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
			goto out;
```

### AG24-A009 — 字符串长度和有效长度门
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			goto out;

		ret = -E2BIG;
		if (!valid_arg_len(bprm, len))
			goto out;
```

### AG24-A010 — 先保存旧pos再减整个len并检查argmin
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		pos = bprm->p;
		str += len;
		bprm->p -= len;
		if (bprm->p < bprm->argmin)
			goto out;
```

### AG24-A011 — 每块从pos页内余量选择最大字节数
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				offset = PAGE_SIZE;

			bytes_to_copy = offset;
			if (bytes_to_copy > len)
				bytes_to_copy = len;
```

### AG24-A012 — 块处理先推进pos str和len
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			offset -= bytes_to_copy;
			pos -= bytes_to_copy;
			str -= bytes_to_copy;
			len -= bytes_to_copy;
```

### AG24-A013 — 块取得page并在NULL时转E2BIG
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c

				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
```

### AG24-A014 — 每块先memcpy_to_page后put_arg_page
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);
```

### AG24-A015 — 旧kmap缓存/flush路径被注释
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				// kmapped_page = page;
				// kaddr = kmap(kmapped_page);
				// kpos = pos & PAGE_MASK;
				// flush_arg_page(bprm, kpos, kmapped_page);
			// }
```

### AG24-A016 — copy_strings正常0和错误out返回逻辑
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	// 	flush_dcache_page(kmapped_page);
	// 	kunmap(kmapped_page);
	// 	put_arg_page(kmapped_page);
	// }
	return ret;
```

### AG24-A017 — 内核字符串长度和pos设置及len错误门
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	ulong pos = bprm->p;

	if (len == 0)
		return -EFAULT;
```

### AG24-A018 — 内核字符串先减bprm p再判argmin
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	/* We're going to work our way backwards. */
	arg += len;
	bprm->p -= len;
	if (bprm->p < bprm->argmin)
		return -E2BIG;
```

### AG24-A019 — 内核块大小用min_not_zero及PAGE_SIZE
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	while (len > 0) {
		uint bytes_to_copy = min_t(uint, len,
				min_not_zero(offset_in_page(pos), PAGE_SIZE));
		page_s *page;
```

### AG24-A020 — 内核块推进pos/arg/len
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		pos -= bytes_to_copy;
		arg -= bytes_to_copy;
		len -= bytes_to_copy;
```

### AG24-A021 — 内核块获取页后复制和归还
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		if (!page)
			return -E2BIG;
		// flush_arg_page(bprm, pos & PAGE_MASK, page);
		memcpy_to_page(page, offset_in_page(pos), arg, bytes_to_copy);
		put_arg_page(page);
```

### AG24-A022 — 内核字符串正常返回0
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	}

	return 0;
}
EXPORT_SYMBOL(copy_string_kernel);
```

### AG24-A023 — 内核多字符串包装调用copy_string_kernel
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c

	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
```

### AG24-A024 — free_bprm按bprm mm条件mmput并调用空free_arg_pages
[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
	free_arg_pages(bprm);
```

### AG24-A025 — exec文件名内核字符串复制先于env
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
```

### AG24-A026 — exec环境字符串先于argv用户字符串
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

### AG24-A027 — exec最后调用bprm_execve和free_bprm
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);

```

### AG24-A028 — GUP返回long且参数start为virt_addr_t数量ulong输出pages二级指针
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
long
myos_get_user_pages(mm_s *mm, virt_addr_t start,
	ulong nr_pages, page_s **pages)
{
```

### AG24-A029 — GUP循环每页偏移计算且VMA未命中直接continue
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
	for (int i = 0; i < nr_pages; i++) {
		virt_addr_t addr = start + i * PAGE_SIZE;
		vma_s *vma = simple_find_vma(mm, addr);
		if (vma == NULL)
			continue;
```

### AG24-A030 — 有覆盖VMA时先查询现有映射再决定分配
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
		page_s *pg = myos_get_one_page_from_mapping(mm, addr);
		if (pg == NULL) {
			pg = alloc_page(GFP_USER);
```

### AG24-A031 — 新page分配成功先创建单页映射后get_page
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
			pg = alloc_page(GFP_USER);
			if (pg != NULL) {
				myos_creat_one_page_mapping(mm, addr, pg);
				get_page(pg);
			}
```

### AG24-A032 — 映射帮助返回值在调用处没有记录和判错
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
				myos_creat_one_page_mapping(mm, addr, pg);
				get_page(pg);
```

### AG24-A033 — GUP无论new或old分支同一输出赋值且函数末尾无return
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
		}

		pages[i] = pg;
	}
}
```

### AG24-A034 — memcpy_to_page类型与形参
[VERIFIED mykernel/mm/highmem/highmem.h::memcpy_to_page]
```c
		PREFIX_STATIC_INLINE
		void
		memcpy_to_page(page_s *page, size_t offset,
				const char *from, size_t len) {
```

### AG24-A035 — memcpy_to_page中页内越界VM_BUG_ON被注释
[VERIFIED mykernel/mm/highmem/highmem.h::memcpy_to_page]
```c

			// VM_BUG_ON(offset + len > PAGE_SIZE);
			// memcpy(to + offset, from, len);
			// flush_dcache_page(page);
			// kunmap_local(to);
```

### AG24-A036 — memcpy_to_page直接按page_to_virt和offset调用memcpy
[VERIFIED mykernel/mm/highmem/highmem.h::memcpy_to_page]
```c
			virt_addr_t page_vaddr = (virt_addr_t)page_to_virt(page);
			memcpy((void *)(page_vaddr + offset), from, len);
		}
```

### AG24-A037 — offset_in_page从虚拟字节地址截取偏移
[VERIFIED mykernel/mm/highmem/highmem_macro.h::offset_in_page]
```c
	#define offset_in_page(p)	((unsigned long)(p) & ~PAGE_MASK)

	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)
```

### AG24-A038 — alloc_page使用alloc_pages order0
[VERIFIED mykernel/mm/highmem/highmem_macro.h::alloc_page]
```c
	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)
```

### AG24-A039 — page_to_virt从PFN转直接映射虚拟地址
[VERIFIED mykernel/mm/highmem/highmem_macro.h::page_to_virt]
```c
	#ifndef page_to_virt
	#  define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
	#endif
```

### AG24-A040 — simple_find_vma只返回包含查询地址的VMA或NULL
[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma]
```c
			for_each_vma(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
```

### AG24-A041 — mmap_read_lock实际不执行down_read
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_read_lock]
```c
	{
		// __mmap_lock_trace_start_locking(mm, false);
		// down_read(&mm->mmap_lock);
		// __mmap_lock_trace_acquire_returned(mm, false, true);
	}
```

### AG24-A042 — mmap_read_unlock实际不执行up_read
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_read_unlock]
```c
	static inline void mmap_read_unlock(mm_s *mm)
	{
		// __mmap_lock_trace_released(mm, false);
		// up_read(&mm->mmap_lock);
	}
```

### AG24-A043 — min_not_zero宏处理零余量的真实表达式
[VERIFIED mykernel/include/linux/kernel/minmax.h::min_not_zero]
```c
	#define min_not_zero(x, y) ({				\
				typeof(x) __x = (x);			\
				typeof(y) __y = (y);			\
				__x == 0 ? __y : ((__y == 0) ?	\
				__x : min(__x, __y));			\
```

### AG24-A044 — GUP公开声明返回long
[VERIFIED mykernel/mm/misc/mm_misc_api.h::myos_get_user_pages]
```c
	// gup.c
	long
	myos_get_user_pages(mm_s *mm, virt_addr_t start,
		ulong nr_pages, page_s **pages);

```

### AG24-A045 — 单页创建函数和各级PTE指针局部声明
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
```

### AG24-A046 — 新建路径pgd来源及p4d检查
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	pgd = pgd_offset(mm, addr);
	p4d = p4d_alloc(mm, pgd, addr);
	if (!p4d) {
		goto fail;
	}
```

### AG24-A047 — pud_alloc后检查pud
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	pud = pud_alloc(mm, p4d, addr);
	if (!pud) {
		goto fail;
	}
```

### AG24-A048 — pmd_alloc后检查pmd
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	pmd = pmd_alloc(mm, pud, addr);
	if (!pmd) {
		goto fail;
	}
```

### AG24-A049 — pte_alloc_map后误检查pmd而非pte
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	pte = pte_alloc_map(mm, pmd, addr);
	if (!pmd) {
		goto fail;
	}
```

### AG24-A050 — 非NULLpage时先mapcount增再PTE写
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	if (page != NULL)
	{
		atomic_inc(&(page->_mapcount));
		ulong page_addr = page_to_phys(page);
		*pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);
```

### AG24-A051 — 从page到物理页后使用固定PAGE_SHARED_EXEC构建PTE
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
		atomic_inc(&(page->_mapcount));
		ulong page_addr = page_to_phys(page);
		*pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);
```

### AG24-A052 — 映射帮助函数返回pte或ERR_PTR ENOMEM
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	return pte;

fail:
	return ERR_PTR(-ENOMEM);
```

### AG24-A053 — 已有映射查询函数retval初始化NULL
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
page_s *myos_get_one_page_from_mapping(mm_s *mm, virt_addr_t addr)
{
	page_s *retval = NULL;
	pgd_t *pgd;
	p4d_t *p4d;
```

### AG24-A054 — 查询先取得pgd再p4d, 只测试p4d_none
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
	pgd = pgd_offset(mm, addr);
	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d)) {
		goto fail;
	}
```

### AG24-A055 — 已有映射查询pud级none门
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
	pud = pud_offset(p4d, addr);
	if (pud_none(*pud)) {
		goto fail;
	}
```

### AG24-A056 — 已有映射查询pmd级none门
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd)) {
		goto fail;
	}
```

### AG24-A057 — 已有映射查询pte_none门
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
	pte = pte_offset(pmd, addr);
	if (pte_none(*pte)) {
		goto fail;
	}
```

### AG24-A058 — 已有映射PTE物理地址转page后返回,无持有调用
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
	phys_addr_t page_paddr = arch_pte_addr(*pte);
	retval = phys_to_page(page_paddr);

fail:
	return retval;
```

### AG24-A059 — p4d_alloc返回原pgd槽强制转换
[VERIFIED mykernel/mm/fault/pgalloc.h::p4d_alloc]
```c
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		}
```

### AG24-A060 — pud_alloc按p4d none和__pud_alloc决定NULL或PUD槽
[VERIFIED mykernel/mm/fault/pgalloc.h::pud_alloc]
```c
		pud_t
		*pud_alloc(mm_s *mm, p4d_t *p4d_entp, ulong vaddr) {
			return (p4d_ent_is_none(*p4d_entp)) && __pud_alloc(mm, p4d_entp) ?
						NULL : pud_entp_from_vaddr_and_p4d_entp(p4d_entp, vaddr);
		}
```

### AG24-A061 — pmd_alloc按pud none和__pmd_alloc决定NULL或PMD槽
[VERIFIED mykernel/mm/fault/pgalloc.h::pmd_alloc]
```c
		pmd_t
		*pmd_alloc(mm_s *mm, pud_t *pud_entp, ulong vaddr) {
			return (pud_ent_is_none(*pud_entp)) && __pmd_alloc(mm, pud_entp)?
						NULL: pmd_entp_from_vaddr_and_pud_entp(pud_entp, vaddr);
		}
```

### AG24-A062 — pgtble_alloc仅当pmd_none调用__pte_alloc
[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc]
```c
		PREFIX_STATIC_INLINE
		int
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp) {
			return (unlikely(pmd_none(*(pmd_entp))) && __pte_alloc(mm, pmd_entp));
		}
```

### AG24-A063 — pgtble_alloc_map可按分配失败返回NULL
[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc_map]
```c
		pte_t
		*pgtble_alloc_map(mm_s *mm, pmd_t *pmd_entp, ulong vaddr) {
			return (pgtble_alloc(mm, pmd_entp) ?
						NULL : pgtbl_entp_from_vaddr_and_pmd_entp(pmd_entp, vaddr));
		}
```

### AG24-A064 — pte_alloc_map宏绑定pgtble_alloc_map
[VERIFIED mykernel/mm/fault/pgalloc_macro.h::pte_alloc_map]
```c
	#define pte_alloc				pgtble_alloc
	#define pte_alloc_map			pgtble_alloc_map
	#define pte_alloc_map_lock		pgtble_alloc_map_lock
```

### AG24-A065 — folio_get对page_folio引用加一
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_get]
```c
		void
		folio_get(folio_s *folio) {
			// VM_BUG_ON_FOLIO(folio_ref_zero_or_close_to_overflow(folio), folio);
			folio_ref_inc(folio);
		}
```

### AG24-A066 — folio_put对folio引用减一测零再调__folio_put
[VERIFIED mykernel/mm/page_alloc/buddy.h::folio_put]
```c
		folio_put(folio_s *folio) {
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
		}
```

### AG24-A067 — get_page直接调用folio_get(page_folio)
[VERIFIED mykernel/mm/page_alloc/buddy.h::get_page]
```c
		PREFIX_STATIC_INLINE
		void
		get_page(page_s *page) {
			folio_get(page_folio(page));
		}
```

### AG24-A068 — put_page直接调用folio_put(page_folio)
[VERIFIED mykernel/mm/page_alloc/buddy.h::put_page]
```c
			//  */
			// if (put_devmap_managed_page(&folio->page))
			// 	return;
			folio_put(page_folio(page));
		}
```

### AG24-A069 — page_ref_inc作用于_refcount
[VERIFIED mykernel/mm/page_alloc/buddy.h::page_ref_inc]
```c
		page_ref_inc(page_s *page) {
			atomic_inc(&page->_refcount);
			// if (page_ref_tracepoint_active(page_ref_mod))
			// 	__page_ref_mod(page, 1);
		}
```

### AG24-A070 — order0出队页追加初始_refcount的增量
[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));

		return page;
```

### AG24-A071 — alloc_pages成功经prep_new_page后返回page
[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
			page_s *page = __rmqueue_smallest(zone, order);
			if (likely(page)) {
				prep_new_page(page, order, gfp);
				return page;
			}
```

### AG24-A072 — __folio_put活动函数体空无free
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
void __folio_put(folio_s *folio)
{
	// if (unlikely(folio_is_zone_device(folio)))
	// 	free_zone_device_page(&folio->page);
	// else if (unlikely(folio_test_large(folio)))
```

### AG24-A073 — _mapcount与_refcount是独立字段
[VERIFIED mykernel/mm/page_alloc/buddy_types.h::struct page]
```c
			atomic_t			_mapcount;
		};

		/* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
		atomic_t				_refcount;
```

### AG24-A074 — phys_to_page按物理地址移位求PFN
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::phys_to_page]
```c
	// #define __boot_pa(x)	__pa(x)

	#define phys_to_page(kaddr) pfn_to_page(kaddr >> PAGE_SHIFT)
	/*
```

### AG24-A075 — arch_pte_addr只剥掉PTE_FLAG_MASK
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::arch_pte_addr]
```c
		PREFIX_STATIC_INLINE
		phys_addr_t
		arch_pte_addr(pte_t pte) {
			return arch_pte_val(pte) & ~PTE_FLAG_MASK(pte);
		}
```

### AG24-A076 — PAGE_SHARED_EXEC页属性RW且未写NX
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_SHARED_EXEC]
```c
	#define PAGE_SHARED					__pg(__PP|__RW|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_SHARED_EXEC			__pg(__PP|__RW|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY_NOEXEC			__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_COPY_EXEC				__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY					__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
```

### AG24-A077 — GFP_USER宏不包含__GFP_ZERO
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::GFP_USER]
```c
	#define GFP_NOFS				(__GFP_RECLAIM | __GFP_IO)
	#define GFP_USER				(__GFP_RECLAIM | __GFP_IO | __GFP_FS | __GFP_HARDWALL)
	#define GFP_DMA					__GFP_DMA
	#define GFP_DMA32				__GFP_DMA32
	#define GFP_HIGHUSER			(GFP_USER | __GFP_HIGHMEM)
```

### AG24-A078 — __folio_put的其余主体均为注释、没有活动回收调用
[VERIFIED mykernel/mm/page_alloc/buddy.c::__folio_put]
```c
	// 	__folio_put_large(folio);
	// else
	// 	__folio_put_small(folio);
}
```


### AG24-A079 — alloc_pages定义的参数
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
page_s *alloc_pages(gfp_t gfp, uint order)
{
	// struct mempolicy *pol = &default_policy;
	page_s *page;
```

### AG24-A080 — alloc_pages实际调用__myos_alloc_pages
[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
	page = __myos_alloc_pages(gfp, order);

	return page;
```

### AG24-A081 — virt_addr_t是u64
[VERIFIED mykernel/include/linux/kernel/types.h::virt_addr_t]
```c

		typedef u64		phys_addr_t;
		typedef u64		virt_addr_t;

```

### AG24-A082 — __GFP_ZERO条件才清空数据页
[VERIFIED mykernel/mm/page_alloc/buddy.c::post_alloc_hook]
```c
	// kernel_init_pages(page, 1 << order);
	if (gfp_flags & __GFP_ZERO)
		for (int i = 0; i < (1 << order); i++)
			clear_page((void *)page_to_virt(page) + i);

```

## 证据消费与未做事项

全文读取43号任务与23主线验收回执R23-S01/S02/S03，定点读取18主线R18-S01/S02/S03和18延期记录；23已有ES23-F02/F03、12缺页及21初始NULL迭代限制沿用。主要已读固定time源码：exec.c、gup.c、fault.c、pgalloc.h及宏、buddy.h/c、highmem.h及宏、VMA点查询、PTE/页宏、页类型与mmap锁包装。
get_arg_page将long GUP结果存入int ret再读取；GUP缺return，输出槽未写/NULL/有效指针是不同事实。已有映射直接物理地址转page指针，没有在该帮助体增加引用。新页先分配、调用PTE创建、然后get_page；_refcount和_mapcount分开。此前源码页面成功与引用归属并未通过机器测试。
所有纸面页/引用示例均需对象有效、调用抵达、无溢出及无并发写者；不能从片段推导全局回收/真实数据/硬件权限。没有安装新工具、运行仓库脚本、预处理/编译/运行C/ASM、内核/QEMU或硬件实验。

