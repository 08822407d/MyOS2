---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C02 内核分析主线"
record_type: integration_review_and_nonblocking_baseline_checkpoint
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "master（工作区）；time（内核）；agent/MYOS2-LEAD-002（主线写分支）"
read_channel: connector
evidence_class: "可读回收审查与续做安排；主线无命令执行"
inputs_read:
  - "reviews/CORE-SCHED-INTEGRATION-03-review.md所列本轮实际范围。"
  - "17-memory-baseline-contract.md本轮生成；写后读回另由本轮收尾核对。"
supersedes: agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-10-02-scheduler-witness-reviewed-integration-ready.md
supersedes_scope: "INTEGRATION-03已回收；不把实施A或决定B设为所有后续分析的前置。"
status: PHASE2_INTEGRATION_PARTIAL_ACCEPTED_MM_BASELINE_READY
current_phase: 2
phase3_entered: false
lead_pr: 16
execution_pr: 17
execution_branch: claude/dazzling-cori-q0dnyt
reviewed_execution_short12: ee9e6a738224
results_frozen_short12: b7fa83583e35
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-INTEGRATION-03-review.md
acceptance_verdict: RETURN
returned_scope: "仅IR-01的未来计费预期与IR-02的B保持项；不是内核修复任务。"
accepted_scope: "已读静态分析可按限界保存；原W/V验收不撤销。"
kernel_acceptance_verdict: NOT_ISSUED
kernel_change_authorized: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_policy_decision_required_now: false
next_followup: CORE-MM-BASELINE-04
next_taskbook: agent-workspace/lead/MYOS2-LEAD-002/17-memory-baseline-contract.md
next_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/mm-baseline-04/
requires_merge_before_work: false
requires_owner_file_transfer: false
external_execution_started: false
reviewer_execution: false
branch_release: false
local_validation: "主线未执行命令/解析/哈希/测试。新任务仅云端静态工作，真实ELF/IRQ/SMP/上下文切换仍未验证。"
open_questions:
  - "IR-01/02并入下一次M00处理，之后同轮继续四个内存子系统，不另占一轮单独勘误。"
  - "A/B、键下限、内核改动都未授权；未来真实运行须按路径重新评估依赖风险。"
---

# 当前建议操作：一次转发17号任务，不需要现在改内核或选择A/B

**这些问题不阻碍下一步静态盘点，但限制可依赖它们的运行实验。** 调度修复先保留为未实施方案，转向四个内存子系统的实现与依赖基线；不是把问题宣布无害、永久搁置或已修好。

本轮实际接收了INTEGRATION-03正文及索引；尚有两处AI规格矛盾需处置：NR-6在idle键0、P5选中idle时不能再给P6的N2计费10；B不能既声明有序唤醒又继承A的普通唤醒头插不变。主线已给出条件性修正，M00独立核对并追加一份小处置后，直接继续内存盘点。**连续状态必须由上一步返回者成为下一步current，不能为维持旧总数而在P6强行切回N2。**

## Owner原话登记（工作令§12续记）

<details>
<summary>本轮原话；解释与授权范围见随后段落</summary>

> [$github](app://connector_76869538009648d5b282a4bb21c3d157) CORE-SCHED-INTEGRATION-03 的 D01–D05 已全部完成，结果已推送到 `claude/dazzling-cori-q0dnyt`.claude code cloud在这次工作中提出了以下几个建议和需要我决定的事项.似乎都是我现有代码中的问题,你大致讲一下是什么问题,如果我现在搁置不处理,会不会影响下一步工作,并且我回顾它的前几次任务执行结果回复里面似乎也提到了一些我的代码实现上的问题.:"**建议：**
>
> 1. **先做候选 A（兼容的局部修复）。** 只改 `pick_next_task_myos` 的重新入队和计费顺序，共六条规则：
>    - 先计费再定位插入点；
>    - 比较项跟随游标前进；
>    - 先查锚点再比较 vruntime；
>    - 按身份判断 idle；
>    - 链入和计数作为一个操作完成；
>    - 插到第一个键值不小于自身的节点之前。
>    这能修正 W01/W02 两处问题，并消除 DV-11：单任务加 idle、时间片到期把队列取空后，现有代码会读到一个由锚点算出的伪任务指针。
> 2. **候选 B（统一排序）作为后续目标。** 在 `set_task_cpu` 里改成有序插入，`wake_up_new_task` 只加防护，选取改为排除当前任务后的最小键值。它只影响高键值任务被唤醒的情形。
> 3. **有三项风险本轮只是推导，没有证实，也不在 A 的修复范围内：**
>    - DV-13：TASK_NEW 任务在 `wake_up_new_task` 之前就被唤醒，可能导致空转、环形链表损坏，或把正在运行的任务链入环中；
>    - DV-23：任务写入非运行状态后、检查条件之前被切换出去，存在中断窗口；
>    - DV-19：`schedule_timeout` 的有限超时分支不调度，却会删除定时器，经宏别名走到忙等的 `timer_delete_sync`，`down_timeout` 是已知受影响的调用方。
>    GAP-01..11 等未解项和反证都保留在 `facts-and-regressions.yaml`。新增回归 NR-1..NR-6 全部是 NOT_RUN。
>
> **需要你决定的事项：**
>
> - **OD-1：队列契约。** A 作为最终方案，还是先 A 后 B？我建议先 A 后 B。不答复则默认只做 A。
> - **OD-2：如果走 B，唤醒时的键值下限是否取 max(自身键值, 队首非 idle 任务的键值)？** 不答复则默认不准备 B。
> - **内核修改授权。** 实施 A 需要你另行授权；本轮按要求没有改内核，也没有出补丁。".如果现在可以搁置,那你做完自己这步后就继续给出他的下一步工作内容

</details>

执行理解（主线判断，不是新增Owner原话）：本轮要求解释、审查和可行时继续工作；不构成A/B采用或内核修改授权。主线判断静态盘点能够继续，暂不安排实施；不能把引用的执行者建议或“不答时A”登记为Owner决定。

## 五件事

**规则：** 保留当前主线、分支与GitHub双向交接；只追加分析文件，不合并、不删分支、不改旧件或内核。其他项目误发不纳入。

**进展：** 两份主报告与三个小脚本已读，长YAML/索引/证据按审查范围定点读取，核心timer/pick/semaphore再次回源。区分已见证缺陷、源码条件风险与政策提案。没有重跑测试、没有逐字节检查全部14件。IR-01/02属于报告规格，不是再发现两条用户代码bug。

**不阻塞：** 分配器/堆/虚拟映射/缺页的源码盘点、依赖关系及学习素材准备不要求先修调度。已知原子操作、锁、等待、超时问题作为依赖风险保留；不能据静态连通就说这些机制可运行。

**待办：** 明确运行实验或修复前再处理相关风险、NR回归和授权；没有把A设成沉默默认，没有替Owner选择B或键下限。002R/003R/007R全量仍未完成，局部内存基线不得冒充整项交付。

**接续：** 先读本检查点、INTEGRATION-03-review、17-memory-baseline-contract。Owner在原云会话转发启动块；同一轮先M00后M01–M04，结束只回PR链接。PR17保持Draft；若已由Owner合并则按17号任务的原件核对与唯一PR规则续做。不把合并插成开始条件。

## 写入与保全

本轮仅新增本审查回执、17号任务与本检查点，写前主线为40bc4faa4202。远端f36到ee9e为14件新增，b7fa到ee9e为6件文档/回读新增；核心结果批未改。新文件的读回、收尾差异及PR状态以本轮后续连接器返回为准，不预填未做的检查。没有新建执行任务会话或自动启动云端。
