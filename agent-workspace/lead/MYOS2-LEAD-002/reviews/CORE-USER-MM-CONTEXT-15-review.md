---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-MM-CONTEXT-15-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-10-31
read_channel: connector
evidence_class: 固定交付全文阅读、关键源码控制流核查及远端增量比较；非运行认证
authorization_ref: Owner报告CORE-USER-MM-CONTEXT-15完成；28号同轮验收和后继派发持续授权
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 6ea1003c2ae1
reviewed_base_short12: dd8e81ba6fc7
contract_short12: 9a423f5b45b1
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: a2b86d12ac39
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: 已核mm对象/引用、fork共享与复制入口、exec替换、选定调度借用及页表根装载接口；须合读本回执
returned_scope: []
consumption_corrections: [R15-S01, R15-S02]
page_table_copy_coverage: CALLER_AND_ROOT_ENDPOINTS_ONLY_NEXT_16
kernel_lifetime_problems_closed: false
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
human_technical_review_attested: false
reviewer_execution: false
reviewer_reproduced_110_quote_checks: false
reviewer_reproduced_88_narrative_checks: false
reviewer_parsed_yaml_or_dot: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-PGTABLE-COPY-16
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/34-user-pgtable-copy-contract.md
inputs_read:
  - PR17交付评论issuecomment-6545504446；6ea1003c2ae1六件全文。facts分段读取，长边表截断后的内容以110行起重叠段补齐。
  - 固定time：fork.c 245–465、595–704、1115–1175、1360–1435；mm_misc.h 1–65、mm_misc_types.h 1–70、init-mm.c 20–60。
  - 固定time：scheduler.h 140–225、scheduler_core.c 760–900；pgtable_arch.c 1–100；mmu_arch.c 1–230、mmu_macro_arch.h全文、special_insns_arch.h 116–136。
  - 固定time：exec.c 95–185、880–925、1067–1093；同一固定对象的exec_mmap完整主体、get_task_mm/dup_mmap、do_exit等沿用本对话前轮已读正文，不冒称本轮再次全文取回。
  - 33号任务全文、14回执R14-S01/S02、18/24/25/28及源码options_flags.cmake沿用本对话既有固定版本；未重做接收或整套Mnemosyne刷新。
  - 为16设计：copy_page_range默认分支检索仅作导航；固定time fault.c 1–485作范围预检，未展开所有页表helper或签发16结论。
  - 两PR状态、直接heads、开放PR第三项为空、PR16全部变更路径、本批六件compare。
review_limits:
  - 全文阅读和定点语义核验，不是110条源码及88段正文检查的独立机械复现；全部边的形式认证及本地闸门仍partial/pending_local。
  - 本轮未执行命令、JS/Python、解析器、哈希、旧脚本、预处理、C/ASM、构建、内核/QEMU或硬件。
  - 同厂分角色复核不是异族模型或人类技术验收；不证明原子/锁原语、全局引用平衡、页表复制、并发或TLB协议正确。
  - RS-01、GAP-U04硬件/运行部分、GAP-U10及既有普通延期问题不关闭。
---

# 第15批限定接收；对象身份与实际完成动作继续分开

## 1. 裁定与交付范围

按PASS_PENDING_LOCAL接收第15批的有界静态资料，须合读R15-S01/S02。无须执行方单独返工；本回执直接给出替代记录，旧件全部冻结。不把接受分析报告变成内核代码正确或资源问题已修。

被审头6ea1003c2ae1。相对dd8e81ba6fc7，compare为6个连续后继提交，仅user-mm-context-15/六件A新增、合计2460行，无旧件净修改/删除。六件全文已读：MANIFEST 82、正文808、facts 1249、DOT 114、延期131、evidence 76行。PR17仍open/Draft，头与交付评论一致。

执行方110/110、88/88及46节点/60边一致性记录只作为其自查证据，主线未重新运行这些检查；引文真实也不能自动证明数据边、完整可达性或硬件效果。六项普通发现、四组旧项证据追加保留原ID与延期/needs_evidence状态。

## 2. 关键源码核对

### 两类计数和两种退出接口

已核真实字段为mm_users/mm_refcount，mm_init均置1；init_mm静态值则为users=2、refcount=1。mmget使用mm_users，mmput减少该计数，归零调用__mmput；__mmput的exit_mmap和mmdrop为注释。不能据函数注释中的Linux历史文案补出活动实现。

[VERIFIED mykernel/mm/misc/mm_misc.h::mmget]
```c
		mmget(mm_s *mm) {
			atomic_inc(&mm->mm_users);
		}
```

另一个真实接口mmdrop没有读取/递减mm_refcount的活动判零门；在有效非init对象、检查及下层函数正常完成的前提下，经mm_free_pgd→pgd_free→_pgd_free调用free_pages，之后通过free_mm调用kmem_cache_free。已核pgd_free包含pgd链摘除，不能跳过这些前置步骤称直接free。对init_mm有BUG/while门；WARN语句不是最后引用判定。

这里的“释放”仅指源码到达释放接口及其参数，未审计整个分配器，更不证明根以下所有PTE页、数据页、VMA或cache槽被归还。MC15-S08/O05/X03等短摘要也受此限制。

### fork和exec的边界

已核copy_mm三分支：current.mm为空时新task两个字段保持NULL；CLONE_VM对旧mm一次mmget后让两个字段同指；非共享dup_mm成功后才赋值。两个指针不表示两份引用。mm_init的PGD失败与dup_mmap已有副作用后的失败是不同出口；copy_process后期错误标签条件mmput(p->mm)，不能概括为全部fork失败都没有清理或全部都有事务回滚。

bprm_mm_init的早期错误可用mmdrop；free_bprm在仍持bprm.mm时用mmput。begin_new_exec在exec_mmap返回成功后清bprm.mm；不能把字段清空当另一份引用递减。exec_mmap旧mm非空用mmput，旧mm为空的分支另用mmdrop(旧active_mm)，且更早的mm_release还有clear_child_tid/mm对象前提。14的brk错误出口修正已被正确消费，没有恢复旧错误说法。

### 调度借用和CR3接口

已回读context_switch与finish_task_switch主体。next.mm为空且prev.mm非空时借用active_mm并mmget；kernel→kernel只转移；kernel→user把旧借用放rq.prev_mm。finish_task_switch若被实际调用，会取值并清空该字段，但延迟mmdrop操作注释。局部借用路径缺配对证据可保留，不推定全树没有其他归还者或已测得泄漏增长率。

[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::switch_mm_irqs_off]
```c
	if (prev == next)
		return;
```

[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::switch_mm_irqs_off]
```c
	load_new_mm_cr3(next->pgd, new_asid, need_flush);
```

已核activate_mm宏→switch_mm→switch_mm_irqs_off→load_new_mm_cr3→write_cr3及写CR3内联汇编文本。same-mm早退本身不是故障证明；不同指针也不免除next/pgd、地址转换及上游条件。写寄存器的源码不是CPU观测、PCID或跨CPU TLB认证。

## 3. R15-S01：E35对象来源及字段/契约措辞

MC15-E35从mm_release连向N01 task.mm，但所引A081读取的是传入mm参数的mm_users，并以tsk.clear_child_tid为条件，函数并没有在这里重新从tsk.mm取mm。尤其exec替换前后的task.mm身份不能凭这条边混同。

[VERIFIED mykernel/sched/forkexec/fork.c::mm_release]
```c
	if (tsk->clear_child_tid) {
		if (atomic_read(&mm->mm_users) > 1) {
```

替代消费记录同时覆盖facts及DOT中的E35，原边不再作为另一条活动依赖：

```yaml
target: CORE-USER-MM-CONTEXT-15::MC15-E35
from: MC15-N30
to: MC15-N04
kind: data
status: conditional
condition: tsk.clear_child_tid非NULL时读取传入mm对象的mm_users；须有有效mm，非读取task.mm字段
anchors: [MC15-A081]
```

clear_child_tid的读取、put_user及清空属于其自身条件/副作用，不以N01代替该字段。MC15-E54文字中的next.pgdir按真实源码改读next->pgd，原节点N06正确。MC15-O09.limits的“wrong vs standard mm_refcount contract”收窄为“所读借用操作增加mm_users，所选收尾未证明配对；本树mm_refcount协议未闭合”，不把其他内核的契约直接作为本项目已批准规格或修复选择。

## 4. R15-S02：切换收尾必须有实际抵达前提

E50的C调用确实存在，但只能说明switch_to之后实际恢复到该C路径时会调用finish_task_switch；不能把一次switch_to画成同一栈上的普通函数调用并保证每次新任务首次运行也会经过它。

[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch]
```c
			switch_to(prev, next, prev);
			barrier();
```

[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch]
```c
			return finish_task_switch(prev);
```

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule_tail]
```c
	// finish_task_switch(prev);
	preempt_enable();
```

该schedule_tail注释已在原A099记录。替代消费为：E50保留活动源调用属性但明确控制流须抵达；MC15-M14/S06/X08及正文中“读取并清rq.prev_mm”都限定为实际进入finish_task_switch的路径。新任务首次上下文/汇编返回链未由本批重新审计，不据此断言必然清字段或一定未清。F03的已读路径归还断点保留，不能以注释自动补成活动调用。

同样，F06仍是证据缺口，不把same-mm不重载CR3独立认定为漏洞。所有data边需合读effects，指针赋值与引用变化、实际执行指令与源码存在继续分离。

## 5. 下一批与权限

本批对mm持有、fork/exec/调度切换接口取得限定证据。copy_page_range仍只是接合端点，父子下级页表、共享物理页、写保护与复制失败副作用没有被15验收。故同轮发布CORE-USER-PGTABLE-COPY-16，专门分析fork页表复制及写保护边界，不重做14区间或15对象链，不扩展全部COW缺页/回收与运行测试。

本轮只在既有lead分支新增回执、34号任务与检查点，随后更新PR导航/评论；不写执行结果。保持阶段2和18号普通问题延期，两PR/分支及PR17 Draft保留。未运行、未修内核、未生成内核补丁、未合并关闭或进入阶段3。当前无需Owner作技术修复决定。
