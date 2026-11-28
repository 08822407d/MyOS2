---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MM-CONTEXT-15
record_type: bounded_mm_context_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话，Owner指定GPT-6/Extra；后台身份无法独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "dd8e81ba6fc7"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/33-user-mm-context-contract.md @ 9a423f5b45b1"
instruction_base: "18ee22673d83"
required_review_record: CORE-USER-VMA-LIFETIME-14-REVIEW-001
prior_status: {vma14: PASS_PENDING_LOCAL_WITH_R14_S01_S02}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
old_results_changed: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mm-context-15/"
deliverables: [MANIFEST.md, mm-context.md, facts-and-boundaries.yaml, mm-context-paths.dot, deferred-findings.yaml, evidence.md]
open_questions:
  - "没有动态验证mm用户/结构引用、调度借用和旧对象清理是否平衡"
  - "真实CR3/CPU TLB、硬件及跨CPU状态、完整页表释放和异常返回均未知"
  - "静态源文件通过结构与引文检查不等于编译/运行正确性"
---

# CORE-USER-MM-CONTEXT-15 · 六件交付总入口

**A00–A04已完成有界静态分析和交付取证，状态仅为READY_FOR_MAINLINE_REVIEW。** 不签真实内核通过。第14批及更早全部原件冻结，C03主线负责独立审查、后续派工与最终验收。

## 1. 六件及消费顺序

| 文件 | 主要内容 |
|---|---|
| `MANIFEST.md` | 本包身份、固定输入、完成范围与限制、六件交接入口 |
| `mm-context.md` | **先读**：mm_users/mm_refcount及init_mm特殊值，fork共享/复制和失败清理，exec新旧mm安装与转移，调度active_mm借用/CR3接口，最终释放边界 |
| `facts-and-boundaries.yaml` | 固定time的**110段短源码锚点/15个来源文件**、46个图节点、60条关系、19种代表输入/结果、9状态、10所有权、10条effects |
| `mm-context-paths.dot` | 与facts的46节点、60边ID相同；活动、条件、注释边分离，保留mm_free_pgd、switch_mm_irqs_off和load_new_mm_cr3中间层 |
| `deferred-findings.yaml` | 新6项有前提发现及4项对旧VL14-F03/F04、FC13-F05、UV-15的证据追加；无修复，不变更旧编号状态 |
| `evidence.md` | 完整的源文核验、结构一致性、主线更正消费、主动反证、未运行事项与远端验收界限 |

六件仅新增在`allowed_write_prefix`目录；无第七份脚本、工具、日志或勘误文件。

## 2. A00–A04完成范围

| 阶段 | 本次完成 | 保留限制 |
|---|---|---|
| A00 | 全读33号固定任务书、14主线回执R14-S01/S02，读取旧14 mmput/exec/exit端点与13的文件缓存引用限界；首次写前核两个开放PR、执行头/master/time/lead、目录冲突；先读options_flags.cmake | 未重跑旧14的107/73；R14-S01关于正常错误出口恢复brk字段的替代结论生效；不预处理 |
| A01 | 直接回源task.mm/active_mm及mm_s.mm_users/mm_refcount；mmget/get_task_mm/mmput/__mmput/mmdrop；allocate_mm/mm_init/pgd_alloc/pgd_free及失败清理；静态init_mm特殊计数 | mm_refcount不是可直接套用的Linux mm_count；mmdrop未按它判零，mmput不代表资源已释放 |
| A02 | copy_mm current.mm为空、CLONE_VM共享和非共享dup_mm；memcpy+mm_init和dup_mmap、复制PGD/VMA/页表关键门；copy_process及bad_fork_cleanup_mm；bprm_mm_init失败与free_bprm等准备入口 | 后期复制失败mmput到0仍无活动VMA/pgd清理；不能假称事务式回滚 |
| A03 | begin_new_exec→exec_mmap安装mm/active_mm、exec_mm_release、activate_mm与旧mm的mmput/mmdrop；scheduler_core.c经scheduler.h内联context_switch/finish_task_switch；user→user、user→kernel、kernel→kernel、kernel→user条件；switch_mm_irqs_off→load_new_mm_cr3→write_cr3源码链 | active_mm借用mmget操作的是mm_users，延迟drop注释；同mm早退不是硬件验证，CR3文本不等于运行或全局TLB正确 |
| A04 | 六件新目录交付、19类输入/状态矩阵、引用与图/ID自查、普通问题增量、固定time短引文及全文/引文比对 | 最终提交后的六件远端固定SHA与分支回读、差异和PR17评论，须在本MANIFEST之后执行并仅写到评论 |

## 3. 本批已执行的自查

- **源码引文：** 110段1–3行强制引文从15个`time @ a039d9803ade`文件再次全文取回，两批64+46段，**110/110逐字及行范围匹配**（首行合法2行锚点由验证器区别处理）；这不是110项独立运行证明。
- **正文：** `mm-context.md`内88段`[VERIFIED]`的路径、符号、范围、逐字引用与facts全部一致，**88/88匹配**。
- **图/引用：** facts/DOT的46节点、60边ID集合一致，全部60条边from/to及锚点均可解析；19类输入、9种状态、10项所有权与10个effects的证据均可定位。
- **问题：** 6个有前提新问题、4条旧条目证据追加，未修内核；对同指针不必重新装CR3、mm_users借用计数未配对、mmput零计数无法释放、mmdrop直接free但不检mm_refcount等都保留来源与未知。
- **主动反证：** mm_users=0不等于mm已释放；task.mm==active_mm不等于双份引用；mm结构memcpy不等于独立页；赋值task.mm不等于CR3切换；write_cr3源码存在不代表跨CPU TLB正确；后继失败不自动回滚；do_exit/释放接口存在不等于实际调用。

## 4. 写入历史与最终回读分界

执行开始时PR17 open/Draft、head=`dd8e81ba6fc7`，master=`e8e60cd19540`、time=`a039d9803ade`；开放PR只有16/17，本批目录不存在、与PR16无路径冲突。先提交facts，后提交DOT、正文、deferred和evidence；所有旧件保留。准备MANIFEST前远端compare从旧头到`57ed83a9e9f5`为**5个后继提交、仅本目录五个A新增**、没有旧件净修改/删除，PR17仍open/Draft。

**本MANIFEST形成后方可得出最终head和完整六件diff。** 应按最终固定提交完整读取六件，再按当前执行分支回读全部文本与blob身份，并检查PR17仍open/Draft及master/time不变。真正完成后在自己的PR17交付评论中记录结果；本文不虚构未发生的GitHub校验。

## 5. 停写和权限边界

仅使用已授权GitHub连接器与有界文本/ID比较；不运行任何仓库专属检查器、正式YAML/Graphviz解析渲染、预处理、构建、C/ASM、内核/QEMU、硬件、外部libc/镜像或多核CPU实验。没有修改内核/脚本/既有结果，没有启动额外付费模型或API，没有新增分支、合并/关闭PR、改PR正文/标题/Draft或进入阶段3。

GitHub Contents写入接口不提供全局原子head锁，保留读写之间的竞态限制。完成后停写、交MYOS2-A-C03独立验收；最高只可申请`PASS_PENDING_LOCAL`有限静态接收，不能作为内核运行通过证书。C03a不自行选择后续批次或恢复CC02权限。
