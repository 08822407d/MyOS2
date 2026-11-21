---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PGTABLE-COPY-16
record_type: bounded_fork_pagetable_copy_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话；Owner指定GPT-6/Extra；实际backend不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-11-01"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "6ea1003c2ae1"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/34-user-pgtable-copy-contract.md @ 20b981103e72"
instruction_base: "f791212e5bf9"
required_review: CORE-USER-MM-CONTEXT-15-REVIEW-001
prior_status: {mm_context15: PASS_PENDING_LOCAL_WITH_R15_S01_S02}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
kernel_change_made: false
old_results_changed: false
branch_or_pr_created: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pgtable-copy-16/"
deliverables: [MANIFEST.md, pgtable-copy.md, facts-and-boundaries.yaml, pgtable-copy-paths.dot, deferred-findings.yaml, evidence.md]
open_questions:
  - "页表复制父子物理数据页的引用计数、故障后COW隔离及内存回收无法由本批静态文本证明"
  - "分配失败有锁/未初始化指针前提，不能把名义-ENOMEM视为运行中可靠错误处理"
  - "父PTE写保护后TLB/多核同步、非法VMA区间的终止及硬件/编译结果均NOT_RUN"
---

# CORE-USER-PGTABLE-COPY-16 · 六件交付总入口

本批**P00–P04的授权静态取证、结构核验及问题增量已完成**，仅申请`READY_FOR_MAINLINE_REVIEW`；没有内核编译、运行正确性或实际CPU父子内存隔离结论。此前15及全部历史结果保留，必须合读其主线回执R15-S01/S02，不自行重写历史。

## 1. 六件文件和消费顺序

| 文件 | 实质成果 |
|---|---|
| `MANIFEST.md` | 当前身份、固定源、阶段覆盖、交付计数、主线验收与停止边界 |
| `pgtable-copy.md` | **先读：** fork/dup_mmap门、PGD到PTE复制、父子RW/Dirty/Accessed与PFN身份、PUD/PMD/PTE申请和锁、非present、失败副作用、TLB与已present写保护缺页边界 |
| `facts-and-boundaries.yaml` | 固定time的162段1–3行精准源码锚点（12文件），55节点、69边、23例状态/结果输入、9种阶段状态、10项所有权、14条effects与前提 |
| `pgtable-copy-paths.dot` | 和facts的55节点/69边ID对应；活动、条件、注释及宏展开/直接调用区分，写父PTE、装子PTE、分配页表页在effects独立列示 |
| `deferred-findings.yaml` | 本批新增7个有条件风险、向已有15/14/13/12问题追加4组证据，不改变旧ID/status |
| `evidence.md` | 固定源范围、实际源文回读、语义反证与机械检查区分、禁止动作和最终远端回读要求 |

全部写入仅位于`allowed_write_prefix`，没有生成第七份仓库工具/日志/更正材料或编辑旧批次文件。

## 2. P00–P04完成情况

| 阶段 | 已完成 | 不允许的推断 |
|---|---|---|
| P00 | 完整读取34固定任务书、15主线回执R15-S01/S02、18/28持续规则；先读options_flags.cmake；核PR16/17、两页开放PR、master/time/lead/执行ref、目录不存在及写区不重叠 | 不能把15的110/88报告当本轮重新验证，也不套用未预处理的全部编译定义 |
| P01 | 实际`copy_mm→dup_mm→dup_mmap`及VM_DONTCOPY/VM_WIPEONFORK门；`copy_page_range→copy_p4d_range→copy_pud_range→copy_pmd_range→copy_pte_range`；PGD和折叠P4D、目标下级页表申请、源none/bad副作用及跨级边界 | 新mm/PGD、目标下级页表页不等于复制了程序数据页；未认证所有空/溢出范围的终止 |
| P02 | `copy_present_pte`对源父RW位的实际`clear_bit`，子局部`pte_wrprotect/pte_mkclean/pte_mkold`，`set_pte_at`安装；现存PFN身份与未见数据页get/page引用动作；与12已present写保护入口有限连接 | 不证明共享数据页引用正确、COW或父子写隔离成功；不能把缺PTE文件写和已有PTE WP合并 |
| P03 | 逐层目标分配-锁-失败出口；PTE目标NULL/uninit锁传参、源PTE映射指针NULL前提；nonpresent忙等；EAGAIN注释与真实0返回；上层-ENOMEM转换和部分复制后mmput未全面回滚；TLB flush注释 | 不能假定每条名义-ENOMEM都安全返回、所有失败无副作用或父CPU TLB已同步 |
| P04 | 六件有界静态报告、图与结构ID、代表输入矩阵、问题增量、源码与正文逐字检查；仅授权的GitHub文本/ID工具 | 最终完整六件GitHub compare/固定提交与分支双重回读和交付评论在本文件之后执行，必须据实记录 |

## 3. 已完成的本批自查

- **来源：** facts记录162条路径/符号/固定行号的1–3行逐字短证据，涉及12个固定`time @ a039d9803ade`文件；按6＋6文件分两批重取原文，112＋50=**162/162逐字及行范围相同**。这些相邻证据片段不是162项互相独立的技术正确性证明。
- **正文：** `pgtable-copy.md`中**84处[VERIFIED]**完整对照facts的路径、符号标签、行范围与文本，**84/84完全相等**。
- **结构图：** facts与DOT精确对应55节点/69边；全部边的from/to及引用锚点均可解析，23例代表输入、9状态、10所有权、14 effects的引用也能解析。
- **问题：** 7项本批有前提的发现、4条向已有MM/VMA/缓存/缺页条目的证据追加；无主线问题关闭、内核修复或Owner技术决定。
- **反证：** 新mm不等于新物理数据页；安装相同PFN的子PTE不等于增加page引用；父PTE写保护不等于TLB同步；返回0不等于逐页安装；失败不等于父/子页表事务回滚；注释的swap、EAGAIN、flush及TLB功能不等于实际活动实现。

## 4. 提交历史与最终回读安排

首次写入前，PR17为open/Draft，执行head=`6ea1003c2ae1`，`master=e8e60cd19540`、`time=a039d9803ade`；开放PR只有16/17，PR16不含本批写区，目标目录原先不存在。先在本批目录创建facts，然后仅对其自身做宏/算术方向和新增精确CLONE_VM引文两次授权更新，再提交DOT、正文（自身文字微调一次）、deferred与evidence。历史所有结果文件一直冻结。

**本MANIFEST发布前实际GitHub compare**从`6ea1003c2ae1`到`f53beaf67734`显示8个连续后继提交、仅本目录五件A新增、旧文件零净修改/删除，执行头与PR17一致。第六件本MANIFEST的提交和随后的六件最终回读必须真正执行，最终提交、全部blob与全文对照、最终diff和PR17回执仅在实际完成后记录于**自己的PR17评论**，不在本文件预测未来Git对象。

## 5. 验收与停止边界

阶段2、仅固定time源码静态读取与有限JavaScript字符串/集合比对，未运行仓库脚本、正式YAML/Graphviz解析/渲染、C/ASM预处理/编译、内核/QEMU、硬件、文件镜像、外部libc或多CPU/TLB/COW实验。GitHub Contents API没有可证的全局原子head锁，写前检查与写入之间保留并发竞态限制；不修改master/time/lead、PR标题正文/Draft状态，不新建PR/分支、不force-push或合并，也不进入阶段3。

请C03先核PTE复制父/子RW/Dirty/Accessed及相同PFN、下级表页分配，然后核nonpresent和各级失败/锁状态及失败前副作用，最后审六件diff。当前`mainline_acceptance=PENDING`，静态上限`PASS_PENDING_LOCAL`，**不是内核运行PASS**。交付评论发布并核验后C03a停写，不自行开始下一批。
