---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MM-FAULT-12
record_type: user_address_fault_readable_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话，Owner选用GPT-6/Extra，backend身份未独立认证"
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
execution_base: "2c410132e42d"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions:
  - "C入口到IDT/汇编/ISA真实异常返回未核实"
  - "并发VMA/PTE、文件页缓存与用户访问成功条件未全面证明"
---

# 第12批：用户地址缺页如何分派，返回状态如何消费

## 0. 结论与停止线

**本批确定的是固定`time @ a039d9803ade`的C控制流，不是运行验证。** 在架构层，`exc_page_fault`从CR2读故障地址，`handle_page_fault`按**故障地址是否达到TASK_SIZE_MAX**划分内核/用户地址；用户地址分支再从`current->mm`找VMA，并将访问类型传给`myos_handle_mm_fault`。没有检查真实IDT/汇编入口或硬件回退重试，故不能据C函数存在证明所有页面异常都实际调用它。

**三项重要限定：** 第一，`simple_find_vma`仅返回*包含*目标地址的VMA，因而在稳定VMA数据条件下，调用者试图对“返回后继VMA、扩展栈”作处理的代码没有正常可达输入。第二，`myos_handle_mm_fault`对部分PTE指针失败分支直接返回0，不保证PTE已建立；已有PTE的dirty/young分支也可能未写回。第三，架构层非零`vm_fault_t`的处理是`while(fault != ENOERR)`**仅重复比较同一个变量**，不是重跑处理函数、不是发送SIGSEGV，也不是完整OOM/retry机制。

M00的`pid11-disposition.md`仅修正文档消费，旧11的SR11-01仍由C03裁定；旧04/05/09/10/11材料保留不改。

## 1. M01：入口、来源、VMA与扩栈

在`mykernel/arch/x86_64/mm/fault.c`，`exc_page_fault`读CR2并传`regs/error_code/address`。这里的`regs`与硬件入口绑定、是否为异常帧、最终IRET/指令重试均属边界，未以Linux实现替代证据。

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault · MF12-A20，415–419行]**

```c
__visible noinstr void
exc_page_fault(pt_regs_s *regs, ulong error_code)
{
	ulong address = read_cr2();
	// irqentry_state_t state;
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault · MF12-A21，457–461行]**

```c
	// state = irqentry_enter(regs);

	// instrumentation_begin();
	handle_page_fault(regs, error_code, address);
	// instrumentation_end();
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::fault_in_kernel_space · MF12-A01，112–116行]**

```c
	// if (IS_ENABLED(CONFIG_X86_64) && is_vsyscall_vaddr(address))
	// 	return false;

	return address >= TASK_SIZE_MAX;
}
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault · MF12-A18c，400–402行]**

```c
		do_kern_addr_fault(regs, err_code, address);
	} else {
		do_user_addr_fault(regs, err_code, address);
```


`fault_in_kernel_space(address)`的实际规则仅是`address >= TASK_SIZE_MAX`；上限之上调用`do_kern_addr_fault`，所读活动正文有无限循环。低于此上限时，调用`do_user_addr_fault`，**即使故障发生于supervisor访问用户地址**也进入该分支，而不直接视为用户态异常。此处的`user_mode(regs)`只影响`FAULT_FLAG_USER`与开中断，`X86_PF_SHSTK`或`X86_PF_WRITE`使WRITE位被设置，`X86_PF_INSTR`使INSTRUCTION位被设置；`FAULT_FLAG_DEFAULT`原含允许重试等flags，但当前函数之后未实施完整重试。

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A06，172–173行]**

```c
	tsk = current;
	mm = tsk->mm;
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A08，235–239行]**

```c
	if (user_mode(regs)) {
		local_irq_enable();
		flags |= FAULT_FLAG_USER;
	} else {
		if (regs->flags & X86_EFLAGS_IF)
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A09，251–255行]**

```c
	if (err_code & X86_PF_SHSTK)
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_WRITE)
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_INSTR)
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A09b，256–256行]**

```c
		flags |= FAULT_FLAG_INSTRUCTION;
```


缺少有效mm或正在不可缺页的上下文时，本应检查的`faulthandler_disabled() || !mm`分支在源码中是注释；SMAP、保留位和`access_error`等检查也为注释。故在已读此路径，**有VMA不等于有VM_READ/WRITE/EXEC许可**；也不能假设所有非法地址都返回特定errno。

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A07，219–223行]**

```c
	// /*
	//  * If we're in an interrupt, have no user context or are running
	//  * in a region with pagefaults disabled then we must not take the fault
	//  */
	// if (unlikely(faulthandler_disabled() || !mm)) {
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A13，303–307行]**

```c
	//  */
	// if (unlikely(access_error(error_code, vma))) {
	// 	bad_area_access_error(regs, error_code, address, vma);
	// 	return;
	// }
```


`simple_find_vma(mm,address)`按`addr >= vm_start && addr < vm_end`逐项扫描，有命中立即返回，否则NULL。它**不返回地址后方第一个VMA**。当前查不到VMA时，`myos_bad_area`打印后在活动`while(1)`自旋，不会发出已证的SIGSEGV。查到稳定VMA后，`vm_start<=address`恒成立，直接跨到`lock_mmap`，不会按VM_GROWSDOWN尝试`expand_stack`；后者只有非常规不一致/并发改变等前提才可能被考虑，本包没有证实那种可达性。

**[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma · MF12-A22b，258–261行]**

```c
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A10，275–279行]**

```c
	// vm_fault_s vmf = myos_dump_pagetable(address);
	vma = simple_find_vma(mm, address);
	if (unlikely(!vma)) {
		myos_bad_area(regs, err_code, address);
		return;
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A11，281–285行]**

```c
	if (likely(vma->vm_start <= address))
		goto lock_mmap;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		myos_bad_area(regs, err_code, address);
		return;
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A12，287–290行]**

```c
	if (unlikely(expand_stack(vma, address))) {
		myos_bad_area(regs, err_code, address);
		return;
	}
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::myos_bad_area · MF12-A04，97–101行]**

```c
	// if (!(err_code & (ARCH_PF_EC_WR & ~ARCH_PF_EC_P)))
	// {
		color_printk(RED, BLACK, buf);
		while (1);
	// }	
```


`expand_stack`函数自身在其它调用条件下确能按页对齐地址、检查`mmap_min_addr`，在低于`vm_start`时更新`vm_start/vm_pgoff`；但该函数体存在并不证明上述架构缺页入口能够用它正常扩栈。未对VMA并发更改和同步正确性做结论。

**[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack · MF12-A24，992–996行]**

```c
expand_stack(vma_s *vma, ulong address)
{
	mm_s *mm = vma->vm_mm;
	vma_s *prev;
	int error = -ENOERR;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack · MF12-A25，1023–1027行]**

```c
	if (address < vma->vm_start) {
		ulong size, grow;

		size = vma->vm_end - address;
		grow = (vma->vm_start - address) >> PAGE_SHIFT;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack · MF12-A26b，1049–1053行]**

```c
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
			// 	anon_vma_interval_tree_post_update_vma(vma);
			// 	vma_gap_update(vma);
				spin_unlock(&mm->page_table_lock);
```


## 2. M02：页表准备、PTE分支与代表副作用

`myos_handle_mm_fault`的真实签名返回`vm_fault_t`。函数把地址按页对齐写入`vmf.address`，保存vma、flags、pgoff、gfp、current；然后从`vma->vm_mm`沿pgd→p4d→pud→pmd→pte分配/查找。其`p4d_alloc`在本树折叠为pgd指针；`pte_alloc`宏映射到`pgtble_alloc`，若页表缺失可进一步调`__pgtbl_alloc`。默认`ret=VM_FAULT_OOM`，多个分配失败goto fail而返回这个非零位集合；若PMD仍为空会原地自旋。源码调用存在不等于对应页表一定有效。

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A30，1309–1313行]**

```c
vm_fault_t myos_handle_mm_fault(vma_s *vma,
		ulong address, uint flags)
{
	vm_fault_t ret = VM_FAULT_OOM;
	__set_current_state(TASK_RUNNING);
```

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A31，1315–1319行]**

```c
	vm_fault_s vmf = {
		.vma		= vma,
		.address	= address & PAGE_MASK,
		.flags		= flags,
		.pgoff		= linear_page_index(vma, address),
```

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A32，1323–1327行]**

```c
	uint dirty = flags & FAULT_FLAG_WRITE;
	mm_s *mm = vma->vm_mm;
	pgd_t *pgd = pgd_offset(mm, address);

	vmf.p4d_entp = p4d_alloc(mm, pgd, address);
```

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A33b，1336–1340行]**

```c
	if (!vmf.pmd_entp) {
		goto fail;
	}
	if (pte_alloc(mm, vmf.pmd_entp)) {
		goto fail;
```

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A34，1349–1352行]**

```c
	if (pmd_none(*(vmf.pmd_entp))) {
		pr_err("Current PMD is empty.");
		while (1);
	}
```

**[VERIFIED mykernel/mm/fault/pgalloc.h::p4d_alloc · MF12-A68，75–79行]**

```c
		PREFIX_STATIC_INLINE
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		}
```


**关键反例：** `pte_offset_map_nolock`返回NULL时，`myos_handle_mm_fault`直接返回0，而非`VM_FAULT_OOM`或继续执行`handle_pte_fault`。拿到PTE指针后才会取得orig_pte并派发；故`ret==0`不意味着建立了有效PTE。

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A35，1353–1357行]**

```c
	vmf.pte_ptr = pte_offset_map_nolock(vmf.vma->vm_mm,
					vmf.pmd_entp, vmf.address, &(vmf.ptl));
	if (unlikely(vmf.pte_ptr == NULL))
		return 0;
	vmf.orig_pte = ptep_get_lockless(vmf.pte_ptr);
```

**[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault · MF12-A36，1361–1364行]**

```c
	ret = handle_pte_fault(&vmf);
fail:
	return ret;
}
```


**缺PTE的路由。** `handle_pte_fault`若`pte_none(orig_pte)`调用`do_pte_missing`；后者按`vma_is_anonymous(vma)`判断，源码定义是`!vma->vm_ops`：匿名走`do_anonymous_page`，文件/其它vm_ops走`do_fault`。交换页与NUMA本应区分的检查全在注释中，不能当作活动路径。

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · MF12-A37，1252–1256行]**

```c
static vm_fault_t
handle_pte_fault(vm_fault_s *vmf) {
	pte_t entry;

	if (pte_none(vmf->orig_pte))
```

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · MF12-A37b，1257–1257行]**

```c
		return do_pte_missing(vmf);
```

**[VERIFIED mykernel/mm/fault/fault.c::do_pte_missing · MF12-A41b，1234–1235行]**

```c
		return do_fault(vmf);
}
```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_is_anonymous · MF12-A23，166–170行]**

```c
		PREFIX_STATIC_INLINE
		bool
		vma_is_anonymous(vma_s *vma) {
			return !vma->vm_ops;
		}
```


| 代表情况（与facts的MF12-M09～M23对照） | 源码处理 | 不得升级的断言 |
|---|---|---|
| 匿名、缺PTE、读/取指 | 构造零页PTE并`set_pte_at`，正常返回0 | PTE指针无效时可能返回0未写入；未核硬件可读许可 |
| 匿名、缺PTE、写 | `alloc_anon_folio`→`folio_prealloc`，再有条件`mk_pte/set_pte_at` | `IS_ERR(folio)`直接返回0未必成功；NULL→OOM |
| VM_SHARED但无vm_ops的缺PTE | `do_anonymous_page`返回`VM_FAULT_SIGBUS` | 尚未真正发送SIGBUS，架构对非0忙等 |
| 文件、缺PTE、读/取指、缓存命中 | `do_read_fault→do_fault_around→vm_ops.map_single_page`，generic绑定写PTE并返回0 | 不是所有vm_ops均已绑定/核实；文件页有效性未知 |
| 文件、缺PTE、读/取指、缓存未命中 | fast-path为NOPAGE，fallback走`__do_fault→vm_ops.fault→finish_fault` | 文件read结果未检查，cache索引及分配/IO结果未核 |
| 文件、缺PTE、私有写 | `do_cow_fault`先准备cow页，经文件fault后复制，调用`finish_fault` | 多项错误、并发与COW正确性未证 |
| 文件、缺PTE、共享写 | `do_shared_fault`开头活动无限循环 | 函数体后面的finish_fault不是当前可达成功路径 |
| 已有PTE、写保护、共享或可共享 | `do_wp_page`活动while(1) | 没有成功写保护处理返回 |
| 已有PTE、写保护、私有 | `do_wp_page→wp_page_copy`尝试复制并在PTE匹配时写回 | 复制失败或PTE变化路径仍可返回0而未更换页 |
| 已有PTE、原可写或读/取指 | 只局部计算dirty/young entry，访问标志写回代码注释，返回0 | 无法据此证明PTE真的修改或本次fault解决 |

**[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page · MF12-A43，793–797行]**

```c
	if (pte_alloc(vma->vm_mm, vmf->pmd_entp))
		return VM_FAULT_OOM;

	/* Use the zero-page for reads */
	if (!(vmf->flags & FAULT_FLAG_WRITE)) {
```

**[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page · MF12-A45，819–823行]**

```c
	/* Returns NULL on OOM or ERR_PTR(-EAGAIN) if we must retry the fault */
	folio = alloc_anon_folio(vmf);
	if (IS_ERR(folio))
		return 0;
	if (!folio)
```

**[VERIFIED mykernel/mm/fault/fault.c::do_anonymous_page · MF12-A47，854–858行]**

```c
	// folio_add_new_anon_rmap(folio, vma, vmf->address);
	// folio_add_lru_vma(folio, vma);
setpte:
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte_ptr, entry);

```

**[VERIFIED mykernel/mm/fault/fault.c::do_shared_fault · MF12-A58，1137–1141行]**

```c
static vm_fault_t
do_shared_fault(vm_fault_s *vmf) {
	pr_alert("API not implemented - 'do_shared_fault'\n");
	while (1);
	
```

**[VERIFIED mykernel/mm/fault/fault.c::do_wp_page · MF12-A59b，718–722行]**

```c
	vmf->page = vm_normal_page(vma, vmf->address, *vmf->pte_ptr);
	if (vmf->page)
		folio = page_folio(vmf->page);

	/*
```

**[VERIFIED mykernel/mm/fault/fault.c::do_wp_page · MF12-A60，763–767行]**

```c
	if (folio)
		folio_get(folio);

	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return wp_page_copy(vmf);
```

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · MF12-A39b，1271–1273行]**

```c
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
```

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · MF12-A83，1295–1299行]**

```c
	// }
unlock:
	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return 0;
}
```


**文件指针分派的实际绑定。** 当文件映射沿`simple_mmap_region→call_mmap`，且所用`generic_file_mmap`将`vma->vm_ops=&generic_file_vm_ops`时，静态`generic_file_vm_ops`中的`.map_single_page=simple_filemap_map_page`、`.fault=simple_filemap_fault`为这两条间接边提供了真实目标绑定。没有证明所有文件或其它自定义vm_ops都走这一绑定；图显式保留dispatch节点，而不是凭Linux惯例把目标画成无条件call。

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · MF12-A28，689–693行]**

```c
	vma->vm_pgoff = pgoff;

	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
```

**[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap · MF12-A67b，416–420行]**

```c
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
}
```

**[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_vm_ops · MF12-A66，184–188行]**

```c
const vm_ops_s generic_file_vm_ops = {
	.fault				= simple_filemap_fault,
	.map_single_page	= simple_filemap_map_page,
	.page_mkwrite		= filemap_page_mkwrite,
};
```

**[VERIFIED mykernel/mm/fault/fault.c::do_fault_around · MF12-A53b，1069–1073行]**

```c
	// 		vmf->pgoff + to_pte - pte_off);
	ret = vmf->vma->vm_ops->map_single_page(vmf,
			vmf->pgoff + from_pte - pte_off);
	// rcu_read_unlock();

```

**[VERIFIED mykernel/mm/fault/fault.c::__do_fault · MF12-A54，898–902行]**

```c
	while (pmd_none(*vmf->pmd_entp));


	ret = vma->vm_ops->fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
```


对generic文件：`simple_filemap_map_page`从`mapping->page_array[pgoff]`取缓存页，若有则直接写PTE并返回0，否则返回`VM_FAULT_NOPAGE`；`do_read_fault`据此非零值回退到`__do_fault`。回退的`simple_filemap_fault`会索引cache、分配页并调用`file->f_op->read`，随后设置`vmf->page`返回0，**没有判读read的返回值**；再经`finish_fault`可将页写入PTE。对应文件缓存上界、分配器与FAT/ATA后端只读到必要接口，不能推定文件数据一定正确。

**[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_map_page · MF12-A84，147–151行]**

```c
	page = mapping->page_array[pgoff];
	if (page != NULL) {
		*vmf->pte_ptr = mk_pte(page,
			__pg(_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT));
		return 0;
```

**[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_fault · MF12-A85b，130–134行]**

```c
	virt_addr_t vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);

	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
	vmf->page = *pgcache_ptr;

```

**[VERIFIED mykernel/mm/fault/fault.c::finish_fault · MF12-A56d，1011–1015行]**

```c

	// folio_ref_add(folio, nr_pages - 1);
	// set_pte_range(vmf, folio, page, nr_pages, addr);
	entry = mk_pte(page, vma->vm_page_prot);
	set_pte(vmf->pte_ptr, entry);
```


写保护私有路径中`wp_page_copy`若旧PTE未变更可执行`set_pte_at`；但复制失败时可能返回0，或者PTE已变更而不替换也可返回0。此处记录语言层控制分支，不是复现数据损坏，完整COW、TLB同步和folio生命周期并未穷尽。

**[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy · MF12-A61b，574–578行]**

```c
	if (!pfn_is_zero) {
		int err = __wp_page_copy_user(&new_folio->page, vmf->page, vmf);
		if (err) {
			/*
			 * COW failed, if the fault was solved by other,
```

**[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy · MF12-A62g，632–636行]**

```c
		// ptep_clear_flush(vma, vmf->address, vmf->pte);
		// folio_add_new_anon_rmap(new_folio, vma, vmf->address, RMAP_EXCLUSIVE);
		// folio_add_lru_vma(new_folio, vma);
		BUG_ON(unshare && pte_write(entry));
		set_pte_at(mm, vmf->address, vmf->pte_ptr, entry);
```

**[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy · MF12-A63b，680–684行]**

```c
		folio_put(old_folio);
	}

	return 0;
oom:
```


## 3. M03：返回状态向架构侧的传递

`vm_fault_t`的定义是位集合型uint；`VM_FAULT_OOM=1`、`VM_FAULT_SIGBUS=2`、`VM_FAULT_NOPAGE=0x100`、`VM_FAULT_RETRY=0x400`等都属于位集合，不是`-errno`。另外`ENOERR=0`。两者在当前架构代码中仅以数值比较`fault!=ENOERR`，没有应用`VM_FAULT_ERROR`掩码决定错误处理。

**[VERIFIED mykernel/mm/fault/fault_const.h::VM_FAULT_values · MF12-A76，30–34行]**

```c
	enum vm_fault_reason {
		VM_FAULT_OOM			= 0x000001,
		VM_FAULT_SIGBUS			= 0x000002,
		VM_FAULT_MAJOR			= 0x000004,
		VM_FAULT_HWPOISON		= 0x000010,
```

**[VERIFIED mykernel/mm/fault/fault_const.h::VM_FAULT_values · MF12-A76b，35–39行]**

```c
		VM_FAULT_HWPOISON_LARGE	= 0x000020,
		VM_FAULT_SIGSEGV		= 0x000040,
		VM_FAULT_NOPAGE			= 0x000100,
		VM_FAULT_LOCKED			= 0x000200,
		VM_FAULT_RETRY			= 0x000400,
```

**[VERIFIED mykernel/mm/mm_type_declaration.h::vm_fault_t · MF12-A79b，37–38行]**

```c
	 */
	typedef __bitwise	uint vm_fault_t;
```

**[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR · MF12-A80，5–5行]**

```c
	#define	ENOERR		0	/*  */
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A14，322–324行]**

```c
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);
```


**逐层消费：** `myos_handle_mm_fault`的0值使`do_user_addr_fault`跳过`while`并作为void函数返回，随后`handle_page_fault`对用户地址分支执行`local_irq_disable()`，最后`exc_page_fault`返回到其C调用者。没有由本批确认的硬件指令重试/页表权限正确性。非零`vm_fault_t`则被同一局部`fault`值卡在不变的`while`检查中，源码的`VM_FAULT_RETRY/COMPLETED/OOM/SIGBUS/SIGSEGV`处理都注释了；这不是重新调用handler、不是动态SIGSEGV/OOM送达。

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A15，338–342行]**

```c
	// /* The fault is fully completed (including releasing mmap lock) */
	// if (fault & VM_FAULT_COMPLETED)
	// 	return;

	// /*
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A16，363–367行]**

```c
	// if (fault & VM_FAULT_OOM) {
	// 	/* Kernel mode? Handle exceptions or die: */
	// 	if (!user_mode(regs)) {
	// 		kernelmode_fixup_or_oops(regs, error_code, address,
	// 					 SIGSEGV, SEGV_MAPERR,
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault · MF12-A17b，382–385行]**

```c
	// 	else if (fault & VM_FAULT_SIGSEGV)
	// 		bad_area_nosemaphore(regs, error_code, address);
	// 	else
	// 		BUG();
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault · MF12-A19，409–412行]**

```c
		 */
		local_irq_disable();
	}
}
```

**[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault · MF12-A21，457–461行]**

```c
	// state = irqentry_enter(regs);

	// instrumentation_begin();
	handle_page_fault(regs, error_code, address);
	// instrumentation_end();
```


**地址未命中/VMA失败**走`myos_bad_area`的不返回路径；内核地址故障另走`do_kern_addr_fault`不返回。`get_signal`及系统调用返回尾是09已核的另一接口，不能把“异常C函数返回”自动当作09的`syscall_exit_to_user_mode`入口。文件页访问、信号帧用户地址写入等只能作潜在消费者，实际fault结果/硬件回退从未运行。

## 4. 反证与未覆盖

- “有VMA就有权限”：**错误**，`access_error`注释，页表许可也未完整验证。
- “调用分配器就有PTE”：**错误**，分配可能失败、PMD可能自旋、PTE指针NULL时仍返回0。
- “返回0必然修复异常”：**错误**，存在PTE指针NULL、匿名folio错误和COW未重建等返回0路径。
- “非0就产生信号或触发恢复重试”：**错误**，这里只检查不变`fault`并自旋；原signal/OOM代码被注释。
- “有函数名和源码命中就证明活动边”：**错误**，`do_shared_fault`开头死循环，扩栈分支在稳定VMA输入下不可达，vm_ops需要独立绑定。
- “用户地址异常都是用户态读写”：**错误**，主管态访问用户地址亦走`do_user_addr_fault`，但`FAULT_FLAG_USER`仅按user_mode设置。
- 本批未覆盖完整IDT/汇编/ISA、copy_user、全MM映射与并发、实际page-cache/slab正确性、FAT/ATA后端及运行。普通问题与既有04/05/09接口按`deferred-findings.yaml`有条件登记；SR11-01由主线独立核收。

**交付状态仅READY_FOR_MAINLINE_REVIEW；runtime_evidence: NOT_RUN。**
