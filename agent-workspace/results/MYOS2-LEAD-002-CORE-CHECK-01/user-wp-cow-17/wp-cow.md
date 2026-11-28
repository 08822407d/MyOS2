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
record_type: "bounded_static_analysis"
---

# CORE-USER-WP-COW-17 已存在PTE写保护缺页

## 核心结论

在固定的MyOS2 `time @ a039d9803ade`，已有 **写保护PTE分派→新页分配→条件性页面复制→PTE构造和WRITE_ONCE替换** 的活动源码链。但**它不能证明COW完整正确**：零页PFN分支缺少清零保证、引用/错误回滚未闭合、PTE比较与写入间缺少本链实际PTL与flush/rmap同步。均为有条件的源代码结论，不等于复现任何泄漏、双重释放或CPU故障。

## 入口及与第16批接口 W01

`myos_handle_mm_fault`取得PTE指针，NULL即返回0；非NULL记录orig_pte进入`handle_pte_fault`（A41–A42）。原PTE为none时走缺席页分支，不属于本批；已有PTE只有首次快照相同、write fault且当前PTE不可写才进入`do_wp_page`（A01–A03）。需合读主线第16批R16-S01：`pte_is_present`包括PROTNONE，`pte_is_none`使用掩码，不把“非none”当成硬件已present（A44–A45）。

`do_wp_page`从PTE经`vm_normal_page`取得page描述符（超过PFN上界返回NULL），zero PFN特判在本函数中为注释，**`vmf.page==NULL`不是`pfn_is_zero`的同义词**（A04/A07/A08/A43）。`VM_SHARED|VM_MAYSHARE`命中活动`while(1)`，私有路径有old_folio才执行`folio_get`、随后`wp_page_copy`；不能把注释掉的旧页就地复用当作现实优化（A05/A06/A48）。

## 新页、复制和内容 W02

`wp_page_copy`用orig_pte的PFN判断zero，并调用`folio_prealloc(mm,vma,addr,need_zero)`（A09/A51）。该分配器实际忽略need_zero而固定`folio_alloc(GFP_HIGHUSER_MOVABLE,0)`，封装`folio_alloc_noprof`再将`alloc_page`直接传进`page_folio`（A10–A13）。底层可返回NULL（A52）；页出队`_refcount`增一（A14），分配时只有__GFP_ZERO才清零，但此处GFP没有该标志（A15/A16）。这只是相对计数增加，起始值未知。

非zero PFN且源页非NULL时，`__wp_page_copy_user`→`copy_user_highpage`→`copy_user_page`→`copy_page`（`memcpy`一个PAGE_SIZE），前提是两页直映地址可访问（A17/A18/A20–A22）；源页NULL时实际回退分支将ret设0，不执行完整备用拷贝（A19/A50）。zero PFN时调用者完全跳过复制，且未保证清零，**可能暴露非零的旧内存字节**；这是条件风险而非硬件信息泄漏认定。

特别注意分配失败：`folio_alloc_noprof`对`alloc_page`返回NULL无显式检查，而`page_folio`展开进入读取page字段的`_compound_head`。因此外层的`if(!new_folio)goto oom`不保证捕获这一失败；禁止宣布可靠`VM_FAULT_OOM`恢复（A11–A13/A49/A52）。

## PTE替换、锁/TLB W02–W03

分配与复制后，只在`vmf.pte_ptr != NULL && pte_same(current,orig_pte)`时构造`mk_pte`（A23–A25）。普通写复制标dirty和softdirty、`maybe_mkwrite`只在VMA含VM_WRITE时设RW；UNSHARE跳过此标志阶段且可能命中BUG_ON（A26–A28）。`set_pte_at`→`set_pte`→`WRITE_ONCE(*pte_ptr,pte)`是实际替换新PTE的源码链（A29/A30）。

**锁与可见性不能按Linux注释补齐**：选择的`pte_offset_map_nolock`仅提供ptl指针没有加锁，初次快照比较、copy后的pte_same以及最终WRITE_ONCE间有本链未证明的并发保护；`ptep_clear_flush`、rmap和重加锁为注释，recheck不等时调用的`update_mmu_tlb`在所读默认实现为空（A02/A23/A38/A40/A46/A47）。这不是全系统锁已不存在或CPU一定错乱的断言。

## 引用与失败的对象账 W03

| 情景 | 旧folio | 实际新folio | 返回与边界 |
|---|---|---|---|
| 私有页进入 | 有则`folio_get`一次(+1) | 尚无 | 临时持有而非映射新引用 |
| 成功set_pte_at | `new_folio=old_folio`，末尾对**旧folio**执行两次folio_put(-2) | 新页已进入PTE，但末尾变量不再指向它 | 0；不等于自动“双重释放” |
| copy helper错误 | old_folio非NULL则put一次 | 新folio put一次 | 只有假设helper返回非零；当前实际分支返回0 |
| PTE复核不等 | old_folio非NULL则put一次 | 新folio put一次 | 0，但本次未安装新PTE |
| OOM标签可达时 | old_folio非NULL则put一次 | 未成功取得 | VM_FAULT_OOM标签不保证真能抵达 |
| folio_put归零 | `__folio_put`活动体没有free | 同左 | 不推出物理页已回收 |

最容易误读的是成功路径`new_folio=old_folio`，紧随其后的两个put都针对旧物理folio（A31–A36）。一处可能意在归还原映射引用，另一处意在平衡临时get；但第16批R16-S02 **没有证明fork子PTE复制时增加了该物理page映射引用**，因此不能签发配平/下溢/双重释放的确定结论。也不能据空`__folio_put`宣称已经发生泄漏增长（A37）。

`__wp_page_copy_user`目前活动分支返回0，所以复制错误分支的非零返回并无本批已证明的现实生产者；PTE重检失败的清理返回0不能被误当COW成功。上层源码`do_user_addr_fault`里`while(fault != ENOERR);`没有重新调用缺页处理器，不能作为真实retry证据（上层固定time已单独阅读）。情景矩阵及边图只记录实质调用、数据与副作用，不把文件缺席PTE的`do_cow_fault`混入。

## 结案边界

全部普通问题置`deferred_owner_not_ready`待主线消费，旧12/13/16问题注明增量来源，不作为新修复授权。无需本批编译、运行或修改内核；第17批提交后停止写入，等待「MYOS2-A-C03 内核分析主线续建」独立验收。
