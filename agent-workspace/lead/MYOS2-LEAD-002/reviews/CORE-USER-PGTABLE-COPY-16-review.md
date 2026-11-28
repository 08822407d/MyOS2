---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PGTABLE-COPY-16-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-01
read_channel: connector
evidence_class: 固定交付全文阅读、选定源码语义核查及远端增量比较；不是运行认证
authorization_ref: Owner报告CORE-USER-PGTABLE-COPY-16完成；28号验收后同轮安排下一工作包的持续授权
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 38a23958a355
reviewed_base_short12: 6ea1003c2ae1
contract_short12: 20b981103e72
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 978303ec19fb
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: 已核fork复制门、选定页表遍历/PTE变换、分配与错误出口；必须合读本回执
returned_scope: []
consumption_corrections: [R16-S01, R16-S02]
wp_cow_coverage: ENTRY_ONLY_BACKEND_PENDING_17
kernel_reference_or_isolation_problems_closed: false
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
human_technical_review_attested: false
reviewer_execution: false
reviewer_reproduced_162_quote_checks: false
reviewer_reproduced_84_narrative_checks: false
reviewer_parsed_yaml_or_dot: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-WP-COW-17
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/35-user-wp-cow-contract.md
inputs_read:
  - PR17交付评论issuecomment-6553944996、固定38a23958a355六件全文；facts 1–1721分段读取，截断的状态/锚点和DOT尾部用重叠范围补齐。
  - 固定time：fault.c 210–422及1210–1310；pgalloc.c、pgalloc.h、pgalloc_macro.h、pgtable-generic.c、pgtable_macro.h全文；pgtable.h 145–465；memfault_api.h 20–42。
  - 固定time：pgtable_arch.h 1–22、200–475、588–622；pgtable_macro_arch.h 1–42；pgtable_const_arch.h 1–105；bitops_arch.h 1–225及260–385。
  - 固定time fork.c的dup_mmap、copy_mm/dup_mm、mmput/__mmput及此前PGD端点，沿用本对话先前已读取的同一固定对象；不声称本轮重新取得全部fork源码。
  - 为17设计，补读固定time fault.c 480–770的wp_page_copy/do_wp_page及同源先前已读vm_normal_page、folio_prealloc和__wp_page_copy_user入口；仅范围预检，未完成17分析。
  - 34号任务、15回执R15-S01/S02及18/24/25/28约束沿用本对话已加载版本；未重做接收或整套Mnemosyne刷新。
  - 默认分支clear_bit、_PAGE_BIT_RW、arch_clear_bit搜索仅作导航，相关技术判断已回固定time；两PR、直接refs、开放PR第三项为空、PR16完整变更路径和本批compare。
review_limits:
  - 全文阅读与定点语义复核，不是162条源码和84段正文机械检查的独立复现；未逐条认证全部69条图边或全局调用。
  - 本轮未执行命令、JS/Python、解析器、哈希、仓库脚本、预处理、C/ASM、构建、内核/QEMU或硬件。
  - 原子/锁和内联汇编只按所读文本及配置前提消费；同厂模型分角色复核不是异族或人类技术验收。
  - 实际页引用、分配器、COW隔离、TLB/PCID及多CPU协议未验证；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项不关闭。
---

# 第16批限定接收；软件谓词、目标初态和真实副作用分别解释

## 1. 裁定与交付核查

第16批以PASS_PENDING_LOCAL接受，须合读R16-S01/S02。主体静态证据可复用；本回执直接给出必要替代读法，不另发小型文档返工，不覆盖旧结果。接收分析成果不等于内核引用、父子隔离或失败恢复已经正确。

被审头38a23958a355，与最终交付评论一致。compare 6ea1003c2ae1→38a23958a355为9个后继提交，仅user-pgtable-copy-16/六件A新增，共3035行，无旧件净修改/删除。六件全文已读：MANIFEST 83、正文781、facts 1721、DOT 133、延期146、evidence 171行；PR17仍open/Draft。

执行方162/162、84/84、55节点/69边及6/6双重回读保留为其自查记录。主线独立核了固定内容、净差异及关键源路径，没有声称再次运行全部检查。七项普通发现和四组旧项证据追加保持原ID、前提及延期/needs_evidence状态。

交付评论称MANIFEST成功后会话中断，恢复时先确认远端六件及尚无交付评论，再补回读/评论。本轮确认最终文件与差异，不持有客户端日志，不诊断卡顿原因或将其自动判为成果损坏。

## 2. 可保留的源码结论

非共享fork经dup_mm/dup_mmap到copy_page_range；CLONE_VM共享与VM_DONTCOPY、VM_WIPEONFORK的门必须分开。VMA复制、根PGD、下级表页分配、叶子PTE变换与程序数据页复制不是同一动作。P4D在所读alloc/offset定义中直接转换PGD指针，不据此认证所有构建配置。

已完整核copy_page_range至copy_present_pte的所选主体。目的下级表页可在检查源下层none之前分配；源bad助手可清源表项。do/while中的continue仍执行尾部地址和指针推进；无效或空范围不能直接套正常复制结果。

[VERIFIED mykernel/mm/fault/fault.c::copy_present_pte]
```c
	if (is_cow_mapping(vm_flags) && pte_write(pte)) {
		arch_ptep_set_wrprotect(src_pte_ptr);
		pte = pte_wrprotect(pte);
```

已核is_cow_mapping使用VM_MAYWRITE且非VM_SHARED，而不是单看VM_WRITE。父PTE指针传给原位清RW接口；子局部pte再按共享条件清DIRTY，并清ACCESSED，最后set_pte_at→set_pte写目标。补读RW位定义及clear_bit对应的arch_clear_bit文本，仍不声称编译或机器指令已经执行。所读叶子链没有数据页内容复制或get_page/folio_get，安装同一数据PFN并不自动取得另一份数据页引用。

__pud_alloc/__pmd_alloc/__pgtbl_alloc在成功取得相应锁且new分配失败的分支，return -ENOMEM确实位于本函数spin_unlock之前。目标PTE分配失败时，copy_pte_range还使用未初始化dst_ptl；源映射NULL则未检查就解引用。此类前提不能仅凭末尾errno签成安全返回，也不预言固定死锁/崩溃。

[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc_map_lock]
```c
			return (pgtble_alloc(mm, pmd_entp) ?
						NULL : pte_offset_map_lock(mm, pmd_entp, vaddr, ptlp));
```

pgtble_alloc自身使用逻辑&&，非零结果是失败指示；内层-ENOMEM、外层NULL和复制调用者的-ENOMEM分别记录，不能认为每层原样透传同一个值。copy_present_pte当前活动返回为0；EAGAIN注释不证明当前存在重试生产者。正常完成与非法指针/未返回路径继续分列。

先前父RW变更、子表建立和VMA/file副作用不因上层dup_mm调用mmput就获得全事务回滚；__mmput清理注释和dup_mmap父TLB刷新注释沿用已核端点。此处不关闭普通资源问题，也不证明硬件转换仍保持某个状态。

## 3. R16-S01：真实谓词，而非名称蕴含的硬件检查

### 上级目录的实际门

PC16-E06写“source PGD present and passes none/bad”，但该调用点只检查none/bad助手，未另行检查present。

[VERIFIED mykernel/mm/fault/fault.c::copy_page_range]
```c
		if (pgd_none_or_clear_bad(src_pgd_entp))
			continue;
		if (copy_p4d_range(dst_vma, src_vma,
```

pgd_none_or_clear_bad是p4d_ent_none_or_clear_bad别名。已核助手及p4d_ent_is_bad：后者检测不允许的flags，并不额外验证PRESENT为1。因此E06统一改读“none/bad助手返回0且控制流抵达copy_p4d_range”，不能添加源码没有的present门。有效目录指针及映射前提仍另列，不能反向断言某非法输入必然发生。

PC16-M06是copy_p4d_range局部条件案例。对同一稳定、折叠后的表项，顶层已经用同一助手过滤；不能将其当成普通顶层调用必然能独立命中的第二组状态，也不能凭它宣称有并发修改。

### 叶子的软件present和none

原PC16-A152在按位与符号处截断，未给出真正的present掩码，必须补读：

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_is_present]
```c
		pte_is_present(pte_t pte) {
			return arch_pte_flags(pte) &
						(_PAGE_PRESENT | _PAGE_PROTNONE);
		}
```

所以E31/E32、M10及各处“present/nonpresent”统一指本树软件谓词，不等同只检查硬件PRESENT位。PRESENT为0但PROTNONE使谓词非零的项不能直接归入while(!pte_present)忙等类。PC16-S07关于不返回的结论只在该软件谓词稳定为假、有效指针及相应语言执行前提成立时消费。

pte_is_none也不是字面值必须为0；它忽略_PAGE_KNL_ERRATUM_MASK，所读定义为DIRTY|ACCESSED。所有“空项”均按该真实判定理解，不由函数名补出硬件语义。

### 后续写缺页入口

E60/M23可用来描述本批选择的写保护PTE场景，但handle_pte_fault的活动门不是完整的present验证：!pte_present和protnone专门分支为注释。进入do_wp_page还要求orig_pte不满足none、有效pte_ptr、当前PTE与orig_pte相同、FAULT_FLAG_WRITE及当前PTE不可写；PTE不相同走另一出口。不得省略这些门后把所有只读项称作正常COW输入。

这里修正的是报告消费边界，不给特殊映射或权限检查签发通过。DIRTY/ACCESSED的局部清位也不表示全部软件元数据都被重置。

## 4. R16-S02：跳过复制不等于目标已空，输出动作不等于取引用

PC16-M09.limits的“child PTE left empty”过强。源码只跳过目标写入：

[VERIFIED mykernel/mm/fault/fault.c::copy_pte_range]
```c
		if (pte_none(*src_pte_ptr)) {
			continue;
		}
```

替代为“本次迭代不安装或清空目标PTE，其原值保持；只有证明目标槽来自新分配且已清零的表页、或此前已为空且未被其他写者改变时，才能进一步称其为空”。pgtble_alloc可复用已有目标页表；不能将目标申请接口每次调用都理解为新建表页。PC16-O04和相应正文的‘newly allocated table’也受此初态限定，正确非共享fork仍可与源拥有不同表结构。

PC16-M03的CLONE_VM门补用已有PC16-A162实际if/mmget，不能只凭A001中的else支路证明共享引用增加。M01中DONTCOPY跳过需合读dup_mmap的实际continue，而非仅门及注释；本对话先前同一固定fork主体已读到该语句。

E42、E47–E53及E67等目标对象关系只能按‘使用目标指针/结构或输出参数’消费；安装、写入、发布与输出赋值以X04/X07等effects为准，不能把图箭头读作从目的对象取得了源PTE值或数据页引用。特别是E53的__pte_offset_map_lock既可用于src也可用于dst，实际对象由mm/pmd和ptlp参数决定；成功才更新*ptlp，不能固定叫作源锁。图ID存在与源码引文相等不替代这些语义前提，本轮不认证全部69边。

以上限定同时覆盖facts、DOT、正文和同义摘要，旧原件保留，不要求执行方重做整批。

## 5. 下一批与权限

第16批核到fork后PTE的共享数据身份、写保护及错误副作用，真实写保护缺页后的数据分配/复制、PTE替换和引用转移仍是入口级边界。故同轮发布CORE-USER-WP-COW-17，专门检查已有PTE的do_wp_page/wp_page_copy路径，不重做15/16，不扩大为完整内存管理、缺PTE文件填页或硬件COW认证。

主线只在原lead分支新增本回执、35号任务及检查点，随后同步两PR导航和验收评论；不改执行成果。保持阶段2、18号延期、两PR/分支及PR17 Draft，无内核/用户程序修改、补丁、编译运行、合并关闭、删除强推或阶段3授权。当前无需Owner决定修复。
