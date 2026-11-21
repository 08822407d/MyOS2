---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-INPUT-DISPATCH-119"
record_type: "executor_continuation_handoff_package"
status: "PUBLISHED_PENDING_RECEIVE"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "2b170b8072ed"
execution_pin_short12: "cb96288abb8b"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118: "RETURN"
R115_03: "CLOSED_DOCUMENTATION_ONLY"
R116_01: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "CLOSED_DOCUMENTATION_ONLY"
R117_02: "OPEN"
R117_03: "CLOSED_DOCUMENTATION_ONLY"
R118_01: "OPEN"
R118_02: "OPEN"
package_id: "MYOS2-C07c-EXEC17-TO-C08a-EXEC18-BATCH119"
package_status: "non_execution_source_transfer_artifact"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"yes","task_local_only":true,"refresh_completed":"pending_executor"}
retired_executor: "MYOS2-LEAD-002-CHAT-EXEC-17"
old_executor_119_write_authority: "NONE"
executor_display_name: "MYOS2-A-C08a 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-18"
receiving_model_tier: "next_tier_default"
receiving_model_tier_reason: "普通有界执行包，Owner向新对话发令时选择 GPT-6＋Extra，C08用Pro独立验收。"
new_receiver_started_at_release: false
execution_status: "NOT_STARTED"
activation_gate: "OWNER_FORWARDS_FIXED_168_01_AND_C08_RECEIPT_TO_NEW_CHAT_THEN_RECEIVE_REFRESH_FRESH_G00"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/00-C07b-to-C07c-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md"]
---

# 168/00 · C07c 交接给新对话 C08a

接收人 **MYOS2-A-C08a 代码分析执行**，执行身份 `MYOS2-LEAD-002-CHAT-EXEC-18`。旧 C07c / `MYOS2-LEAD-002-CHAT-EXEC-17` 已停写；Owner报告第118批交付后上下文耗尽。该交接包由 **MYOS2-A-C08 内核分析主线续建** 依远端实物签发，无需原接收人再次确认。C08a是独立执行对话；C08继续主线裁定。[168/01 接收入口](01-MYOS2-A-C08a-receiver-startup-prompt.md) · [169号唯一执行合同](../169-user-shell-input-dispatch-contract.md) · [主线检查点](../checkpoints/2026-11-21-shell-cwd118-reviewed-input119.md)

## 交接依据和当前状态

Owner在本项目中将下一批交由新开对话；GitHub的168包承担知识移交，169合同承担119的任务范围。发布本身不代表新会话已接收、后台已排队或已经执行。项目和行为依据各有来源，勿从旧对话“已读”继承新身份。旧C07c的移交前身见 [上一执行身份记录](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/00-C07b-to-C07c-handoff-package.md)。

| 权限或成果 | 本轮固定事实 |
|---|---|
| 主线写者 | C08，agent/MYOS2-LEAD-002，PR16 Open/Ready；发包前 `2b170b8072ed`，实际新头取Owner转发的PR17正式回执。 |
| 唯一新执行者 | MYOS2-A-C08a 代码分析执行，MYOS2-LEAD-002-CHAT-EXEC-18，既有 claude/dazzling-cori-q0dnyt / PR17，预期父 `cb96288abb8b`。新接收前 NOT_STARTED。 |
| 源码 | time `a039d9803ade`；direct master `e8e60cd19540` 仅用于对照和公约自检。 |
| 原118 | `52c51a3752d3` → `cb96288abb8b`，6新增、3762行。作者已交，独立RETURN。 |
| 历史 | 115、116组合PASS_PENDING_LOCAL；117余R117-02；118新增R118-01、R118-02。119承担三个文档差量。 |
| 费用和运行 | Owner选择GPT-6＋Extra执行、Pro独立验收；后端身份不可独立认证。完整本地P9、构建、运行未做。 |

[118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701) · [118固定六件](https://github.com/08822407d/MyOS2/tree/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118) · [167发布 #7057197079](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057197079) · [118独立验收](../reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md) · [五项历史裁定](../reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md)

## 新对话独立接收

1. 核Owner发来的固定168/01和PR17正式回执，读同提交168/00、169、两回执和检查点。用当前会话自报显示名、执行编号、Owner指定模型、实际可知模型／effort，未知写unknown。未经Owner指定的额外服务和并行执行者均不启用。
2. 逐份完整阅读下列 MyOS2 九份项目约束，再独立读110与124。记录路径及固定基线。按169门在direct master重取公约原句和金丝雀，不能只从旧MANIFEST复制。
3. 按下列Mnemosyne交接及加载入口独立接收与刷新，在本对话先给一条简短接收进度并继续，不等第二次派令。记录本任务相关的行为依据，执行授权始终来自Owner及169。发布者检查时Mnemosyne master为 `e581eb9df6c4`；接收者复核其直接头，如有更新对照已批规范并记录实际读取的固定版本。
4. 完成169的fresh G00后才写唯一119目录。未完成的读项逐一标明。如遇真实冲突，先读远端已有内容并依63处理。

### MyOS2 独立必读

1. [00-work-order.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md)
2. [README.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/README.md)
3. [conventions.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/conventions.md)
4. [00-gpt-task-protocol-v2.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/tasks/00-gpt-task-protocol-v2.md)
5. [18-deferred-findings-and-resume.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md)
6. [24-chatgpt-specialist-conversation-separation.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md)
7. [28-review-and-next-batch-same-turn.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)
8. [63-github-write-retry-and-readback.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)
9. [79-work-package-sizing-and-pro-review.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)
10. [110-owner-communication-and-delivery-continuity.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)
11. [124-model-routing-and-pro-message-budget.md](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

### Mnemosyne 独立必读

- [README.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/README.md)
- [current/human-approved-spec.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/human-approved-spec.md)
- [commands/receive-mnemosyne-handoff.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/receive-mnemosyne-handoff.md)
- [commands/load-mnemosyne-guidance.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/load-mnemosyne-guidance.md)
- [notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md)
- [notes/ai-onboarding/MNEMOSYNE-TAKEOVER-CHECKLIST.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/ai-onboarding/MNEMOSYNE-TAKEOVER-CHECKLIST.md)
- [current/guard-registry.yaml](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/guard-registry.yaml)
- [current/external-research-display-name-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/external-research-display-name-guard.md)
- [notes/registries/project-research-display-name-registry-v0.1.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md)

按 current/guard-registry.yaml 选用与该执行任务有关的护栏，至少说明以下九项如何约束本轮的命名、模型、操作者、唯一PR、来源、交付及写入可见性。仅刷新任务局部指导，跨项目维护队列及任何后台执行状态均不导入。

- [current/artifact-delivery-and-direct-generation-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/artifact-delivery-and-direct-generation-guard.md)
- [current/cross-conversation-execution-intent-and-operator-flow-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/cross-conversation-execution-intent-and-operator-flow-guard.md)
- [current/external-research-display-name-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/external-research-display-name-guard.md)
- [current/github-single-active-pr-lineage-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/github-single-active-pr-lineage-guard.md)
- [current/model-tier-and-subtask-cost-routing-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/model-tier-and-subtask-cost-routing-guard.md)
- [current/next-step-repository-write-visibility-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/next-step-repository-write-visibility-guard.md)
- [current/run-context-and-pr-provenance-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/run-context-and-pr-provenance-guard.md)
- [current/source-artifact-preservation-and-design-rationale-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/source-artifact-preservation-and-design-rationale-guard.md)
- [current/user-operation-next-step-capability-and-intent-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/user-operation-next-step-capability-and-intent-guard.md)

### 首次接收留下的两个记录

在119 MANIFEST中保存 `mnemosyne_handoff_receive`：package_id、受理/缺项状态、当前项目及Owner意图、现行批准规范、固定交接证据、显示名/身份、声明和实际可证模型及effort、是否调档与理由、能力/缺项、下一步。另存 `mnemosyne_guidance_refresh`：实际固定来源和所读护栏、project_guidance=required、mnemosyne_guidance=yes、task_local_only=true、current_task_preserved=true、实际结果及未知项。确认接收与刷新并完成独立G00后沿169连续执行；无需在PR预发接收评论。

## G00 与中断恢复

实读四ref、两个open PR及原题、固定/活动118六件与 blob；无 since 逐页到空，核既有关键作者与C08回执。正式评论中的lead短头在GitHub解析全值，首次启动应与直接lead相等。执行父 `cb96288abb8b`、119新目录及六件初始均未出现；查119本人正式标记为0。只写169列明的六件；单父、expected-head与force=false。若回到同一任务且提交/评论已经存在，先对正文和唯一标记，再补缺失的终检。旧C07c的标记不赋予119权限。

三项D00纠正提交后仍OPEN，交回阶段只写 `READY_FOR_MAINLINE_REVIEW` / `mainline_acceptance=PENDING`；正式评论和终检后 `STOP_WRITING`，由C08独立验收。作用域始终是阶段2，有界文本分析。[169号唯一执行合同](../169-user-shell-input-dispatch-contract.md)
