---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-PGTABLE-COPY-16
record_type: bounded_fork_pagetable_copy_readable_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-11-01"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "6ea1003c2ae1"
contract_ref: "34-user-pgtable-copy-contract.md @ 20b981103e72"
prior_review: "CORE-USER-MM-CONTEXT-15-REVIEW-001 @ f791212e5bf9"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第16批 · fork页表复制、父子PTE权限、数据页身份和失败

## 一、主要结论和复核前提

`copy_page_range`按源VMA的`[vm_start,vm_end)`遍历PGD、折叠P4D、PUD、PMD和PTE，必要时为**子mm分配下级页表页**。对有效present叶子，它以源PTE为基础变换权限/标志并安装至目标表。**它没有在这条叶子复制链中重新分配或复制实际程序数据页，也没有可核的get_page/folio_get或数据页mapcount/rmap增量。** 子PTE可指向与父PTE相同的物理页号；这不能代替数据页引用安全和完整COW正确性的证明。

私有COW且原PTE可写时，本树活动源码真的对**父表中原PTE内存**执行RW清位，并对要安装的**子局部PTE**清RW；共享映射则不按此COW分支清父RW，只清子项的Dirty/Accessed。两种都没有证明父CPU原先缓存的TLB权限已经失效。**文件名里出现copy、COW或fork成功，不是父子内存隔离或硬件写时复制已验证的证书。**

下级页表分配失败的锁边界尤其需要保留：`__pud_alloc`、`__pmd_alloc`、`__pgtbl_alloc`均有持锁后遇空分配指针直接`return -ENOMEM`、绕过后续解锁的活动路径；`copy_pte_range`的目的PTE表申请失败后，会将NULL目标指针和未赋值`dst_ptl`交给`pte_unmap_unlock`。故不能因错误出口末尾写了`-ENOMEM`就宣称该返回安全抵达调用者，更不能假定父表写保护和子表部分分配已被回滚。

固定源是`time @ a039d9803ade`；第15批及之前保留在`6ea1003c2ae1`。本轮未预处理、编译、运行C/ASM/QEMU、验证CPU或真实页引用。15主线的R15-S01关于mm_release读取形参mm_users和R15-S02关于finish_task_switch须真实抵达的补充约束继续有效；不重做14/15。

## 二、P01：fork真实调用门和多级复制

### 1. 共享mm、跳过VMA、复制VMA和复制页表是不同操作

`copy_mm`的非共享分支才调用`dup_mm`；`CLONE_VM`本来共享同一mm，不会沿该分支复制新页表。非共享`dup_mm`先准备新mm/PGD，随后调用`dup_mmap`。在`dup_mmap`中，`VM_DONTCOPY`跳过整条VMA；其它VMA经`vm_area_creat_dup`创建子VMA、重绑`vm_mm`，并按文件映射情况增file引用、挂入子VMA链；若`VM_WIPEONFORK`成立，保留子VMA却**跳过`copy_page_range`**。不能把复制VMA等同复制所有PTE。

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · PC16-A162，689–691行]**

```c

	if (clone_flags & CLONE_VM) {
		mmget(oldmm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · PC16-A002，665–667行]**

```c

	if ((err = dup_mmap(mm, oldmm)) != 0)
		goto free_pt;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A004，172–174行]**

```c

		if (mpnt->vm_flags & VM_DONTCOPY) {
			// vm_stat_account(mm, mpnt->vm_flags, -vma_pages(mpnt));
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A006，189–191行]**

```c
			goto fail_nomem;
		tmp->vm_mm = mm;
		// retval = dup_userfaultfd(tmp, &uf);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A007，198–200行]**

```c

			get_file(file);
			i_mmap_lock_write(mapping);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A008，213–215行]**

```c
		 */
		list_header_add_to_tail(&mm->mm_mt, &tmp->list);

```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A009，216–218行]**

```c
		mm->map_count++;
		if (!(tmp->vm_flags & VM_WIPEONFORK))
			retval = copy_page_range(tmp, mpnt);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A010，217–219行]**

```c
		if (!(tmp->vm_flags & VM_WIPEONFORK))
			retval = copy_page_range(tmp, mpnt);

```


### 2. 根PGD、折叠P4D及下级表页

`copy_page_range`用`src_vma->vm_start/vm_end`确定复制范围，分别从源和目的mm经`pgd_offset`取PGD项，计算`pgd_addr_end`并以`do...while`推进。源上级项为none时跳过；坏项检查实际使用`pgd_none_or_clear_bad`宏别名到`p4d_ent_none_or_clear_bad`，其坏项分支可能**清掉源父页表项**。此清除与“源为空就什么都没做”必须分开。函数没有自身前置`addr<end`、页对齐或范围溢出门，所有正常迭代推理都受有效VMA/源目的mm及地址边界前提约束。

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A068，393–395行]**

```c
int
copy_page_range(vma_s *dst_vma, vma_s *src_vma)
{
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A069，396–398行]**

```c
	ulong next;
	ulong addr = src_vma->vm_start;
	ulong end = src_vma->vm_end;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A072，403–405行]**

```c

	dst_pgd_entp = pgd_offset(dst_mm, addr);
	src_pgd_entp = pgd_offset(src_mm, addr);
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A074，407–409行]**

```c
	do {
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(src_pgd_entp))
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A075，408–410行]**

```c
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(src_pgd_entp))
			continue;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A076，410–412行]**

```c
			continue;
		if (copy_p4d_range(dst_vma, src_vma,
				dst_pgd_entp, src_pgd_entp, addr, next)) {
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A078，415–417行]**

```c
		}
	} while (dst_pgd_entp++, src_pgd_entp++, addr = next, addr != end);

```

**[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_none_or_clear_bad alias · PC16-A120，95–97行]**

```c
	#define pgdp_get				p4d_entp_get_ent
	#define pgd_none_or_clear_bad	p4d_ent_none_or_clear_bad

```


本树`p4d_alloc(mm,pgd_entp,addr)`直接将PGD指针强转为P4D指针，`p4d_offset`实现同样折叠；本次**没有观测独立P4D物理页创建**。之后`copy_pud_range`先对目的mm调用`pud_alloc`，再用源pud_offset取表项并检查源PUD none/bad；`copy_pmd_range`也先为目的申请PMD表，再检查源PMD none/bad。因此子目录表页的分配**可以发生在随后发现源下层为空之前**，不能把“源空条目”概括成“子侧绝对没有分配”。源的bad目录项清除本身也是独立副作用。

**[VERIFIED mykernel/mm/fault/pgalloc.h::p4d_alloc · PC16-A085，76–78行]**

```c
		p4d_t
		*p4d_alloc(mm_s *mm, pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
```

**[VERIFIED mykernel/mm/fault/pgtable.h::p4d_entp_from_vaddr_and_pgd_entp · PC16-A124，199–201行]**

```c
				pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		};
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pud_range · PC16-A052，344–346行]**

```c

	dst_pud_entp = pud_alloc(dst_mm, dst_p4d_entp, addr);
	if (!dst_pud_entp)
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pud_range · PC16-A056，353–355行]**

```c
		// So here skip corresponding check and handling
		if (pud_ent_none_or_clear_bad(src_pud_entp))
			continue;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pmd_range · PC16-A044，316–318行]**

```c

	dst_pmd_entp = pmd_alloc(dst_mm, dst_pud_entp, addr);
	if (!dst_pmd_entp)
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pmd_range · PC16-A048，325–327行]**

```c
		// So here skip corresponding check and handling
		if (pmd_ent_none_or_clear_bad(src_pmd_entp))
			continue;
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pud_alloc · PC16-A098，21–23行]**

```c
		smp_wmb();	/* See comment in pmd_install() */
		p4d_populate(mm, p4d_entp, new);
	}
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pmd_alloc · PC16-A103，43–45行]**

```c
		smp_wmb();	/* See comment in pmd_install() */
		pud_populate(mm, pud_entp, new);
	}
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pgtbl_alloc · PC16-A108，61–63行]**

```c
		smp_wmb();	/* See comment in pmd_install() */
		pmd_populate_kernel(mm, pmd_entp, new);
	}
```


各级`pgd_addr_end/p4d_addr_end/pud_addr_end/pmd_addr_end`是不同层级边界宏；外层`do...while`里的`continue`会先走末尾的指针和`addr=next`更新，再判断是否到end，而不是立即结束整个复制。因此源none不自动中止后续下一个表项的处理。最终进展还取决于有效对齐区间、指针/表页范围、边界算术以及没有未定义输入。

**[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_ent_bound_end · PC16-A114，23–25行]**

```c
	 */
	#define pgd_ent_bound_end(addr, end) ({							\
				unsigned long __boundary = 							\
```

**[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_ent_bound_end · PC16-A115，26–28行]**

```c
					((addr) + PGDIR_SIZE) & PGDIR_MASK;				\
				(__boundary - 1 < (end) - 1)? __boundary: (end);	\
			})
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_p4d_range · PC16-A067，387–389行]**

```c
			return -ENOMEM;
	} while (dst_p4d_entp++, src_p4d_entp++, addr = next, addr != end);

```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pud_range · PC16-A059，358–360行]**

```c
			return -ENOMEM;
	} while (dst_pud_entp++, src_pud_entp++, addr = next, addr != end);

```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pmd_range · PC16-A051，330–332行]**

```c
			return -ENOMEM;
	} while (dst_pmd_entp++, src_pmd_entp++, addr = next, addr != end);

```


## 三、P02：叶子PTE——源内存、子局部变量、子表安装、数据页

`copy_pte_range`先对目标mm执行`pte_alloc_map_lock`，然后对源mm执行`pte_offset_map_lock`。源`pte_none`直接跳过此子PTE的安装，`do...while`末尾仍更新两个PTE指针与地址；源项非none却非present，则活动代码为`while(!pte_present(...))`，不会进入被注释的swap/nonpresent复制处理。目的PTE表页先前可能已分配，即使源全部为空。

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A029，253–255行]**

```c

	dst_pte_ptr = pte_alloc_map_lock(dst_mm, dst_pmd_entp, addr, &dst_ptl);
	if (!dst_pte_ptr) {
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A032，260–262行]**

```c
	// src_pte_ptr = pte_offset_map_nolock(src_mm, src_pmd_entp, addr, &src_ptl);;
	src_pte_ptr = pte_offset_map_lock(src_mm, src_pmd_entp, addr, &src_ptl);;
	orig_src_pte_ptr = src_pte_ptr;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A035，265–267行]**

```c
	do {
		if (pte_none(*src_pte_ptr)) {
			continue;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A036，269–271行]**

```c
		// swap currently not supported.
		while (unlikely(!pte_present(*src_pte_ptr)));
		// if (unlikely(!pte_present(*src_pte_ptr))) {
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A037，271–273行]**

```c
		// if (unlikely(!pte_present(*src_pte_ptr))) {
		// 	ret = copy_nonpresent_pte(dst_mm, src_mm,
		// 				  dst_pte_ptr, src_pte_ptr,
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A040，298–300行]**

```c
			break;
	} while (dst_pte_ptr++, src_pte_ptr++, addr += PAGE_SIZE, addr != end);

```


只有当前present时才进入`copy_present_pte`。函数把`*src_pte_ptr`复制到本地`pte`，并以源VMA的`vm_flags`分类。本树`is_cow_mapping`真正判断`(VM_SHARED|VM_MAYWRITE)`中只有`VM_MAYWRITE`成立；它并非单独测试当前`VM_WRITE`。

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A018，218–220行]**

```c
	ulong vm_flags = src_vma->vm_flags;
	pte_t pte = *src_pte_ptr;

```

**[VERIFIED mykernel/mm/fault/pgtable.h::is_cow_mapping · PC16-A122，163–165行]**

```c
			// must VM_MAYWRITE and not VM_SHARED
			return (flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;
		}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A019，224–226行]**

```c
	 */
	if (is_cow_mapping(vm_flags) && pte_write(pte)) {
		arch_ptep_set_wrprotect(src_pte_ptr);
```


| 已有源PTE条件 | 父表内存中的源PTE | 子表项及状态 | 物理数据页和引用 |
|---|---|---|---|
| 私有可COW且源RW=1 | `arch_ptep_set_wrprotect`直接清源PTE RW | 先在本地`pte_wrprotect`清RW，后清Accessed，`set_pte_at`写入子表 | PFN随源PTE派生；本链无数据页copy/get_page |
| 私有可COW但源RW=0 | 不走父表清RW调用 | 源本来不可写；子继续清Accessed后安装 | PFN未变，未证明额外数据页持有 |
| 共享映射且源RW=1 | 此COW分支不改父RW/Dirty/Accessed | 子RW继承源值，子局部清Dirty、Accessed再安装 | 与源共享PFN，引用增加未见 |
| 非共享、非COW映射 | 不改父RW | 子只清Accessed后安装 | 仍非新数据页 |
| `pte_none` | 跳过叶子复制 | 子项没有通过本函数安装 | 可能已有空目标PTE表页 |
| 非none、非present | 在活动while中不返回 | 本次不完成子项安装 | 不是可靠swap复制或错误返回 |

源父表修改和子变量变换有**不同实现**：`arch_ptep_set_wrprotect(src_pte_ptr)`用`clear_bit(_PAGE_BIT_RW,...)`直接操作指针指向的源项；`pte_wrprotect`、`pte_mkclean`和`pte_mkold`只是返回经过位清除的局部`pte_t`；最后`set_pte_at(dst,...)`经`set_pte`写子项。这里没有给父PTE清Dirty/Accessed，也没有在此处新分配或复制数据页内容。

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A020，225–227行]**

```c
	if (is_cow_mapping(vm_flags) && pte_write(pte)) {
		arch_ptep_set_wrprotect(src_pte_ptr);
		pte = pte_wrprotect(pte);
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::arch_ptep_set_wrprotect · PC16-A147，218–220行]**

```c
		arch_ptep_set_wrprotect(pte_t *ptep) {
			clear_bit(_PAGE_BIT_RW, (ulong *)&ptep->val);
		}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A021，226–228行]**

```c
		arch_ptep_set_wrprotect(src_pte_ptr);
		pte = pte_wrprotect(pte);
	}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A023，234–236行]**

```c
	if (vm_flags & VM_SHARED)
		pte = pte_mkclean(pte);
	pte = pte_mkold(pte);
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A024，235–237行]**

```c
		pte = pte_mkclean(pte);
	pte = pte_mkold(pte);

```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A025，237–239行]**

```c

	set_pte_at(dst_vma->vm_mm, addr, dst_pte_ptr, pte);
	return 0;
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_at · PC16-A145，212–214行]**

```c
			// page_table_check_pte_set(mm, addr, ptep, pte);
			set_pte(ptep, pte);
		}
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte · PC16-A154，605–607行]**

```c
		void
		set_pte(pte_t *pte_ptr, pte_t pte) {
			WRITE_ONCE(*(pte_ptr), (pte));
```


**子PGD/PUD/PMD/PTE页**是页表结构对象，通过页表分配器`alloc_page`和安装接口得到；**父源数据页**只是从`pte_t`值中保留同一物理页号。`copy_present_pte`没有活动的`get_page/folio_get/rmap`或数据复制操作，不能用“安装第二个PTE”推断页引用自动增加；这仍是本条固定复制链的局部负向证据，不是全仓引用策略认证。

## 四、P03：错误出口、锁和前置副作用

### 1. PUD/PMD/PTE分配失败不等于已安全解锁

`__pud_alloc`、`__pmd_alloc`、`__pgtbl_alloc`分别取得上级页表锁，必要时分配新表页。三处`if (!new) return -ENOMEM`均发生在**后续`spin_unlock(ptl)`之前**。在命中这些分支且未由其它机制接管锁的条件下，源码没有执行本函数的解锁；任何上层`return -ENOMEM`的表述都须承认已有持锁状态的前提风险，不能认定为可靠的失败恢复。

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pud_alloc · PC16-A095，11–13行]**

```c
{
	spinlock_t *ptl = p4d_lock(mm, p4d_entp);
	if (p4d_ent_none_or_clear_bad(p4d_entp) ||
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pud_alloc · PC16-A097，17–19行]**

```c
		if (!new)
			return -ENOMEM;

```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pud_alloc · PC16-A099，23–25行]**

```c
	}
	spin_unlock(ptl);
	return 0;
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pmd_alloc · PC16-A100，33–35行]**

```c
{
	spinlock_t *ptl = pud_lock(mm, pud_entp);
	if (pud_ent_none_or_clear_bad(pud_entp) ||
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pmd_alloc · PC16-A102，39–41行]**

```c
		if (!new)
			return -ENOMEM;

```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pmd_alloc · PC16-A104，45–47行]**

```c
	}
	spin_unlock(ptl);
	return 0;
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pgtbl_alloc · PC16-A105，51–53行]**

```c
{
	spinlock_t *ptl = pmd_lock(mm, pmd_entp);
	if (pmd_ent_none_or_clear_bad(pmd_entp) ||
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pgtbl_alloc · PC16-A107，57–59行]**

```c
		if (!new)
			return -ENOMEM;

```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pgtbl_alloc · PC16-A109，63–65行]**

```c
	}
	spin_unlock(ptl);
	return 0;
```


`pte_alloc_map_lock`实际是宏别名`pgtble_alloc_map_lock`；后者先尝试分配目标PTE表再取得PTE锁。当分配失败、目标指针返回NULL时，`copy_pte_range`立即用这个NULL指针及**尚未初始化的`dst_ptl`**调用`pte_unmap_unlock`；其宏展开首先`spin_unlock(ptl)`，然后才`pte_unmap(pte)`。所以不能把紧接的`return -ENOMEM`看成已验证可安全执行；具体运行崩溃/死锁形式未知。

**[VERIFIED mykernel/mm/fault/pgalloc_macro.h::pte_alloc_map_lock alias · PC16-A110，6–8行]**

```c
	#define pte_alloc_map			pgtble_alloc_map
	#define pte_alloc_map_lock		pgtble_alloc_map_lock

```

**[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc_map_lock · PC16-A092，109–111行]**

```c
				ulong vaddr, spinlock_t **ptlp) {
			return (pgtble_alloc(mm, pmd_entp) ?
						NULL : pte_offset_map_lock(mm, pmd_entp, vaddr, ptlp));
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A029，253–255行]**

```c

	dst_pte_ptr = pte_alloc_map_lock(dst_mm, dst_pmd_entp, addr, &dst_ptl);
	if (!dst_pte_ptr) {
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A030，255–257行]**

```c
	if (!dst_pte_ptr) {
		pte_unmap_unlock(dst_pte_ptr, dst_ptl);
		/* ret == 0 */
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A031，257–259行]**

```c
		/* ret == 0 */
		return -ENOMEM;
	}
```

**[VERIFIED mykernel/mm/fault/pgtable_macro.h::pte_unmap_unlock macro · PC16-A112，9–11行]**

```c
	#define pte_unmap_unlock(pte, ptl)	do {	\
				spin_unlock(ptl);				\
				pte_unmap(pte);					\
```


源侧`pte_offset_map_lock`亦可能返回NULL，并且只有成功映射/锁定时才会给`*ptlp`赋值；`copy_pte_range`没有在`pte_none(*src_pte_ptr)`之前做NULL检查。源指针无效时不能据后续正常unlock/return作出确定结论。

**[VERIFIED mykernel/mm/fault/pgtable-generic.c::__pte_offset_map · PC16-A135，70–72行]**

```c
	// if (unlikely(pmd_none(pmdval) || is_pmd_migration_entry(pmdval)))
	if (unlikely(pmd_none(pmdval)))
		goto nomap;
```

**[VERIFIED mykernel/mm/fault/pgtable-generic.c::__pte_offset_map_lock · PC16-A139，150–152行]**

```c
	ptep = __pte_offset_map(pmdp, addr, &pmdval);
	if (unlikely(!ptep))
		return ptep;
```

**[VERIFIED mykernel/mm/fault/pgtable-generic.c::__pte_offset_map_lock · PC16-A141，155–157行]**

```c
	if (likely(pmd_same(pmdval, pmdp_get_lockless(pmdp)))) {
		*ptlp = ptl;
		return ptep;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A032，260–262行]**

```c
	// src_pte_ptr = pte_offset_map_nolock(src_mm, src_pmd_entp, addr, &src_ptl);;
	src_pte_ptr = pte_offset_map_lock(src_mm, src_pmd_entp, addr, &src_ptl);;
	orig_src_pte_ptr = src_pte_ptr;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A035，265–267行]**

```c
	do {
		if (pte_none(*src_pte_ptr)) {
			continue;
```


### 2. 非present与返回值的实际消费

`copy_present_pte`当前唯一活动返回是0；`copy_pte_range`虽然检查`ret==-EAGAIN`，其已读取callee没有该返回，且它在清理后**无条件返回0**。不能用注释中的“prealloc EAGAIN”推导已实现重试。各级copy_pmd/copy_pud/copy_p4d的下级非零会转换为`-ENOMEM`，顶层copy_page_range接收非零也将ret设`-ENOMEM`并break；`dup_mmap`见非0则goto out。这是有条件正常返回的源码传播，不为上述未初始化锁等路径签发正常回传保证。

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A026，238–240行]**

```c
	set_pte_at(dst_vma->vm_mm, addr, dst_pte_ptr, pte);
	return 0;
}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A039，296–298行]**

```c
		 */
		if (unlikely(ret == -EAGAIN))
			break;
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A043，304–306行]**

```c

	return 0;
}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pmd_range · PC16-A050，329–331行]**

```c
				dst_pmd_entp, src_pmd_entp, addr, next))
			return -ENOMEM;
	} while (dst_pmd_entp++, src_pmd_entp++, addr = next, addr != end);
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_pud_range · PC16-A058，357–359行]**

```c
				dst_pud_entp, src_pud_entp, addr, next))
			return -ENOMEM;
	} while (dst_pud_entp++, src_pud_entp++, addr = next, addr != end);
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_p4d_range · PC16-A066，386–388行]**

```c
				dst_p4d_entp, src_p4d_entp, addr, next))
			return -ENOMEM;
	} while (dst_p4d_entp++, src_p4d_entp++, addr = next, addr != end);
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_page_range · PC16-A077，412–414行]**

```c
				dst_pgd_entp, src_pgd_entp, addr, next)) {
			ret = -ENOMEM;
			break;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A011，222–224行]**

```c

		if (retval)
			goto out;
```


### 3. 部分复制失败后，没有对应的全事务逆操作

若先复制若干private可写PTE，父表RW已被直接清掉、子PTE已安装且子下级表页已分配；后来某级分配失败，`dup_mm`的`free_pt`只调用`mmput(mm)`。第15批已核`__mmput`中`exit_mmap/mmdrop`均为注释，不能据此推定父表RW已经恢复、子表PTE/PGD/VMA全部撤销或物理数据页引用得到了配对清理。

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · PC16-A003，671–673行]**

```c
free_pt:
	mmput(mm);
fail_nomem:
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · PC16-A015，439–441行]**

```c
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · PC16-A016，449–451行]**

```c
	// 	module_put(mm->binfmt->module);
	// mmdrop(mm);
}
```


另`dup_mmap`的`flush_tlb_mm(oldmm)`在源码中为注释。**父PTE内存已清RW，不等于父CPU旧TLB中的可写翻译已失效。** 本轮不验证PCID、多CPU shootdown、实际CR3/TLB和指令重试。

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A012，229–231行]**

```c
	// mmap_write_unlock(mm);
	// flush_tlb_mm(oldmm);
	// mmap_write_unlock(oldmm);
```


## 五、连接已present写保护缺页（不重审COW后端）

针对本次复制后已有present但RW被清的PTE，后续写缺页的既有`handle_pte_fault`在`FAULT_FLAG_WRITE`且`!pte_write`时进入`do_wp_page`；不同于第13批文件**缺PTE**的`do_cow_fault`。在本树`do_wp_page`里，VM_SHARED/VM_MAYSHARE分支原地循环，非共享才有条件沿`wp_page_copy`继续；本批仅标明入口及断点，不重新审计其folio、TLB、数据复制、源缓存put或故障返回。

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · PC16-A080，1255–1257行]**

```c

	if (pte_none(vmf->orig_pte))
		return do_pte_missing(vmf);
```

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · PC16-A081，1271–1273行]**

```c
	if (vmf->flags & FAULT_FLAG_WRITE) {
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
```

**[VERIFIED mykernel/mm/fault/fault.c::handle_pte_fault · PC16-A082，1272–1274行]**

```c
		if (!pte_write(*vmf->pte_ptr))
			return do_wp_page(vmf);
		else if (likely(vmf->flags & FAULT_FLAG_WRITE))
```

**[VERIFIED mykernel/mm/fault/fault.c::do_wp_page · PC16-A083，725–727行]**

```c
	 */
	if (vma->vm_flags & (VM_SHARED | VM_MAYSHARE)) {
		// catch this case, handler temporarily not implemented
```

**[VERIFIED mykernel/mm/fault/fault.c::do_wp_page · PC16-A084，766–768行]**

```c
	// pte_unmap_unlock(vmf->pte, vmf->ptl);
	return wp_page_copy(vmf);
}
```


## 六、反证、状态矩阵及严格边界

`facts-and-boundaries.yaml::case_matrix`提供23种代表情形，区分VMA跳过、源空/坏表、已存在PTE权限、共享/非共享、非present、三级分配失败、源/目的锁前提、先复制后失败、跨边界和无效范围；还分别列出页表页、PFN/数据页引用和真正返回/忙等的前提。它们是源文控制流矩阵，**不是23组真实fork实验**。

主动反证：新mm和新PGD不意味着独立数据页；拷贝源PTE位并不自动增加数据页引用；父PTE内存写保护不保证父TLB已同步；顶层返回0不代表源none/VM_WIPEONFORK的页都被安装；错误返回不保证父PTE/子表/文件引用回滚；注释里的swap、EAGAIN和flush均不是活动实现。

新增问题与对`MC15-F05`、`VL14-F04`及相关旧MM/文件缓存所有权记录的证据追加见`deferred-findings.yaml`。不修改旧文件、不修内核、不生成补丁；未运行正式YAML/Graphviz解析、预处理/编译、C/ASM/内核/QEMU或硬件。COW隔离、数据页计数、TLB和多CPU并发**全部NOT_RUN**。本批仅申请`READY_FOR_MAINLINE_REVIEW`，最终`mainline_acceptance=PENDING`、静态上限`PASS_PENDING_LOCAL`。
