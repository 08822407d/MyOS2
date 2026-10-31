---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PGTABLE-COPY-16
record_type: fixed_fork_pagetable_copy_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-11-01"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "6ea1003c2ae1"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/34-user-pgtable-copy-contract.md @ 20b981103e72"
required_review: "CORE-USER-MM-CONTEXT-15-REVIEW-001 @ f791212e5bf9"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
measured_quotes: "facts 162/162 exact source slices (112+50); article 84/84 exact matching"
graph_check: "55 nodes / 69 edges identical IDs, endpoints and anchor references present"
final_remote_readback: "Perform after MANIFEST commit and record in PR17 comment"
---

# P04：证据范围、反证、自查及后置远端核验规则

## 一、实际读取的对象

本轮完整读取34号固定任务书及其指定`CORE-USER-MM-CONTEXT-15-REVIEW-001`回执。R15-S01纠正了15旧图中`mm_release`真正消费的是形参mm的`mm_users`，不是新从task.mm取得对象；R15-S02限定`finish_task_switch`的实际控制流抵达前提。本次消费两条修正，不改旧第15批文件、不重跑110/88引文。还保留R14-S01关于`sys_brk`字段恢复和R13-S03有关源缓存页与私有COW folio的区别，不重新审计这些旧链。

源码固定使用`time @ a039d9803ade`；`master`、执行分支与主线回执**不能替代该固定源码**。先全文读`mykernel/scripts/options_flags.cmake`，发现x86-64、CONFIG_64BIT和其它构建C flags的文本设定，但外部变量与DEBUG等条件没有真实预处理；本包对P4D折叠、inline函数及各层页表的判断以本树直接源码与宏定义为依据，不认证实际所有编译配置。

实际用于facts源码锚点的12个固定文件：

- `mykernel/sched/forkexec/fork.c`：`copy_mm→dup_mm→dup_mmap→copy_page_range`真实门、VM_DONTCOPY/VM_WIPEONFORK、VMA file引用、页表失败、mmput与__mmput及父TLB flush注释；
- `mykernel/mm/fault/fault.c`：`copy_page_range/copy_p4d_range/copy_pud_range/copy_pmd_range/copy_pte_range/copy_present_pte`全部活动函数体、返回、none/nonpresent及PTE写保护、下游`handle_pte_fault/do_wp_page`接口；
- `mykernel/mm/fault/pgalloc.h`、`pgalloc.c`、`pgalloc_macro.h`、`memfault_api.h`：折叠P4D、PUD/PMD/PTE页表申请、__pte_alloc与pte_alloc_map_lock宏别名、分配失败/锁端点；
- `mykernel/mm/fault/pgtable.h`、`pgtable_macro.h`、`pgtable-generic.c`：`is_cow_mapping`、源none/bad清除、地址边界、pgd_offset/下级offset别名、pte映射/锁指针、pte_unmap_unlock；
- `mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h`、`pgtable_macro_arch.h`：RW/Dirty/Accessed位和PTE写入的真实inline及别名；
- `mykernel/mm/vm_map/vm_map_const.h`：VM_SHARED、VM_MAYWRITE、VM_DONTCOPY、VM_WIPEONFORK实际位定义。

默认分支GitHub代码搜索只做路径导航：`copy_page_range`、`pte_unmap_unlock`、`is_cow_mapping`、`arch_ptep_set_wrprotect`、`pte_offset_map_lock`、各级bound/alloc/offset等。所有报告正式引文都来自固定`time`文件，负向结论仅限选定函数及直接帮助函数，不声明全仓不存在其它引用/释放/TLB动作。

## 二、五组不可混同的源码证据

**1. 复制调用门。** 源VMA未被跳过且子VMA已建立/挂链后，只有非WIPEONFORK才调用页表复制；复制返回值由dup_mmap消费。

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A009, 216–218行]**

```c
		mm->map_count++;
		if (!(tmp->vm_flags & VM_WIPEONFORK))
			retval = copy_page_range(tmp, mpnt);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · PC16-A010, 217–219行]**

```c
		if (!(tmp->vm_flags & VM_WIPEONFORK))
			retval = copy_page_range(tmp, mpnt);

```


**2. 私有源PTE真实清RW与子PTE局部变换。** 不能把源内存`clear_bit`和子`pte_wrprotect`合成一处写。

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A019, 224–226行]**

```c
	 */
	if (is_cow_mapping(vm_flags) && pte_write(pte)) {
		arch_ptep_set_wrprotect(src_pte_ptr);
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::arch_ptep_set_wrprotect · PC16-A147, 218–220行]**

```c
		arch_ptep_set_wrprotect(pte_t *ptep) {
			clear_bit(_PAGE_BIT_RW, (ulong *)&ptep->val);
		}
```

**[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte · PC16-A025, 237–239行]**

```c

	set_pte_at(dst_vma->vm_mm, addr, dst_pte_ptr, pte);
	return 0;
```


**3. 非present不是swap处理。** 对非none但非present输入，当前函数停在值不变的while，不调用注释里的copy_nonpresent_pte。

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A036, 269–271行]**

```c
		// swap currently not supported.
		while (unlikely(!pte_present(*src_pte_ptr)));
		// if (unlikely(!pte_present(*src_pte_ptr))) {
```


**4. 分配失败与锁。** 目标PTE映射失败后传入未赋值的锁指针；下级alloc持锁失败早返。

**[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range · PC16-A030, 255–257行]**

```c
	if (!dst_pte_ptr) {
		pte_unmap_unlock(dst_pte_ptr, dst_ptl);
		/* ret == 0 */
```

**[VERIFIED mykernel/mm/fault/pgalloc.c::__pgtbl_alloc · PC16-A107, 57–59行]**

```c
		if (!new)
			return -ENOMEM;

```


**5. 部分失败的上层释放。** 页表复制报错后的dup_mm主要用mmput尝试回收；__mmput内的VMA清理与mmdrop为注释。

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · PC16-A003, 671–673行]**

```c
free_pt:
	mmput(mm);
fail_nomem:
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · PC16-A015, 439–441行]**

```c
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · PC16-A016, 449–451行]**

```c
	// 	module_put(mm->binfmt->module);
	// mmdrop(mm);
}
```


## 三、真正已完成的机械自查

| 机制 | 结果 | 明确保留的局限 |
|---|---|---|
| facts锚点 | **162/162**段源码短引文，均附路径、局部符号标签、1–3行固定time行号与全文；分两批固定文件回读**112＋50**全部逐字匹配 | 段数包含相邻调用/条件片段，不是162项独立技术正确性证明 |
| 正文引文 | `pgtable-copy.md`中**84/84**个`[VERIFIED]`块的路径、符号、行号及整个引文都与facts完全对应 | 文本正确不证明运行中每个有条件分支可达 |
| 图 | facts和DOT的**55节点、69边**ID集合相同；所有边from/to、来源锚点存在；call/data与条件/注释按schema区分 | 未执行Graphviz，也未用正式解析器认证复杂宏C类型 |
| 结构 | 23条代表输入/返回矩阵、9状态、10类所有权、14条effects引用全部可解析 | 只对应选定fork复制/失败范围，不能证明分页系统全正确 |
| 延期 | 新7项有条件风险、向旧MM/VMA/cache/fault条目追加4组源证；旧问题status和历史文件原样保留 | 不签发修复授权或需求决策 |
| 运行 | **NOT_RUN** | 无正式YAML/Graphviz解析、源码预处理或编译、C/ASM/内核/QEMU/CPU、物理页引用/TLB/跨核/COW实验 |

本次用授权GitHub连接器固定源`fetch_file`、PR/refs/compare和有界JavaScript字符串、正则、集合及路径/行号核对；不安装第三方工具、不执行仓库旧脚本。开始撰写正文时发现缺`CLONE_VM`具体分支门锚点，已仅在**本批facts新件**追加`PC16-A162`，正文随后才写入；另外将PTE地址步进由“宏调用”改为data依赖、并做一次文字微调，均仅限本批自己的新文件。没有借任何勘误理由修改15/14旧报告。

## 四、主动反证

第一，新mm/新PGD/新下级页表页不等于新程序数据页。第二，复制PTE物理页号不等于给源数据页增加引用。第三，父PTE内存写保护不等于实际TLB已经失效。第四，顶层返回0不等于每个虚拟页都新装了子PTE——源none与WIPEONFORK有跳过路径。第五，名义`-ENOMEM`不能替代分配失败之后的锁、空指针和部分子页表/父权限回滚证据。第六，注释中的swap/huge/EAGAIN/flush不可凭名称算作活动实现。

所有源码断言按上下文条件/异常前提限定：有合法源与目的VMA/mm、有效PGD及各级指针、锁接口定义良好、合法`[start,end)`与地址推进。硬件页表/物理页/TLB内容、实际fork、COW写缺页和多CPU并发一概未经执行或验证。

## 五、交付远端边界

首次写入前，PR17 open/Draft，头`6ea1003c2ae1`，master=`e8e60cd19540`、time=`a039d9803ade`；只PR16/17开放，PR16无本批目录交集，目标目录原先不存在。当前在本证据文件发布前已GitHub compare `6ea1003c2ae1…67f2f6de0d47`：**7次连续提交，净只有本批目录四个A新增文件，旧件零净修改/删除**。执行者对facts和正文只使用本批自身blob SHA作了定点更新，没有触碰旧件。

本文件之后还需提交第六件`MANIFEST.md`。**最终固定提交与分支的六件完整回读、SHA身份、diff及PR17交付评论必须在MANIFEST之后实际执行，仅在自己的PR17评论记录结果**；本文件不会提前假称已验证未来状态。GitHub Contents单文件写接口没有全局原子head锁，保留读写之间的并行竞态限制。完成后停写等C03独立验收，不自行开启下一批。
