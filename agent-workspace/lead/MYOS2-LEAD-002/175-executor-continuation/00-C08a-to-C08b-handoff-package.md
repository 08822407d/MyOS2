---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-LIBC-STRING-LENGTH-COPY-124"
phase: 2
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
issued_by: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
read_channel: "connector"
base_snapshot: "time"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
execution_parent_short12: "16398b8b96c6"
lead_before_publication_short12: "cb634a5d3e02"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-19"
executor_display_name: "MYOS2-A-C08b 代码分析执行"
executor_model_per_owner: "GPT-6＋Extra"
retired_executor: "MYOS2-LEAD-002-CHAT-EXEC-18"
retired_executor_124_write_authority: "NONE"
canonical_execution_branch: "claude/dazzling-cori-q0dnyt"
canonical_execution_pr: 17
new_receiver_started_at_release: false
owner_decisions_requested_now: []
open_questions: []
record_type: "executor_continuation_handoff_package"
package_id: "MYOS2-C08a-EXEC18-TO-C08b-EXEC19-BATCH124"
package_status: "non_execution_source_transfer_artifact"
status: "PUBLISHED_PENDING_RECEIVE"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"yes","task_local_only":true,"refresh_completed":"pending_executor"}
receiving_model_tier: "next_tier_default"
receiving_model_tier_reason: "已设计的普通静态执行包；C08独立验收用Pro。"
execution_source: "Owner转发启动指令及175号修订执行合同"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/174-user-libc-string-length-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/00-C07c-to-C08a-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-string-compare123-reviewed-length-copy124.md","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"]
---

# 第124批交接包 · C08a 移交给新对话 C08b

新对话显示名 **MYOS2-A-C08b 代码分析执行**，编号 **MYOS2-LEAD-002-CHAT-EXEC-19**。本包转交身份、固定证据和必读入口。Owner 在新对话转发175号合同及本轮C08换发回执，即可在同一条指令中授权独立接收、核查并执行。发令依据为[175号修订任务书](../175-user-libc-string-length-copy-restart-contract.md)。

## 已知的工作位置

Owner报告C08a上下文耗尽，本批尝试中断。C08的独立记录只覆盖GitHub可见状态。换发前四条直接引用为 lead `cb634a5d3e02`、执行 `16398b8b96c6`、time `a039d9803ade`、master `e8e60cd19540`。第123批提交唯一父为 `7d0e810ec4ff`；完整执行树中124目标目录和六件均不存在，PR17的234条顶层评论按100/100/34/0读取至空，前后任两种124作者标记均为零。未挂到该执行分支的草稿或对象状态未知。C08a至此退出本批写入，新对话从固定证据独立重建。

| 来源 | 新对话怎样使用 |
|---|---|
| [PR17 #7067185003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7067185003) 和 [123固定六件](https://github.com/08822407d/MyOS2/tree/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123) | C08a的第123批已交付。这是本次D00补正的原件。 |
| [123独立审查](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md) · [R122-01裁定](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md) · [123检查点](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-string-compare123-reviewed-length-copy124.md) | C08独立裁定123为 RETURN，唯一开放项R123-01；R122-01已文档关闭。 |
| [174原合同](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/174-user-libc-string-length-copy-contract.md) · [PR17 #7073310725](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7073310725) | 原版的技术依据和发布历史。执行身份、启动条件和正式作者标记以175号为准。 |

## 新会话独立接收

首次读取下面九份MyOS2项目约束和两项补充。链接固定在本次换发前的已核主线提交。公约原句另从直接master取，分支金丝雀按175回源。

1. [00-work-order.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md)
2. [README.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/README.md)
3. [conventions.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/conventions.md)
4. [00-gpt-task-protocol-v2.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/tasks/00-gpt-task-protocol-v2.md)
5. [18-deferred-findings-and-resume.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md)
6. [24-chatgpt-specialist-conversation-separation.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md)
7. [28-review-and-next-batch-same-turn.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)
8. [63-github-write-retry-and-readback.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)
9. [79-work-package-sizing-and-pro-review.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)
10. [110-owner-communication-and-delivery-continuity.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)
11. [124-model-routing-and-pro-message-budget.md](https://github.com/08822407d/MyOS2/blob/cb634a5d3e02/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

Mnemosyne仅用于本任务的接收与行为指导。本轮回读其 master 为 `e581eb9df6c4`。先读接收入口和当前人工批准规范，再用局部加载入口与注册表核对适用护栏。若其直接头更新，记录实际批准版本及影响；MyOS2写权来自Owner与175。按需分组读，小组结束收束为路径、版本、适用条款；无需把通用规范全文输出到聊天或MANIFEST。

| 阅读组 | 固定入口 |
|---|---|
| 项目及人工批准规范 | [README.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/README.md) · [current/human-approved-spec.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/human-approved-spec.md) |
| 交接、局部加载与新会话清单 | [commands/receive-mnemosyne-handoff.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/receive-mnemosyne-handoff.md) · [commands/load-mnemosyne-guidance.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/load-mnemosyne-guidance.md) · [notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md) · [notes/ai-onboarding/MNEMOSYNE-TAKEOVER-CHECKLIST.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/ai-onboarding/MNEMOSYNE-TAKEOVER-CHECKLIST.md) |
| 护栏注册、显示名与命名登记参考 | [current/guard-registry.yaml](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/guard-registry.yaml) · [current/external-research-display-name-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/external-research-display-name-guard.md) · [notes/registries/project-research-display-name-registry-v0.1.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md) |
| 产物、操作者和单一PR | [current/artifact-delivery-and-direct-generation-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/artifact-delivery-and-direct-generation-guard.md) · [current/cross-conversation-execution-intent-and-operator-flow-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/cross-conversation-execution-intent-and-operator-flow-guard.md) · [current/github-single-active-pr-lineage-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/github-single-active-pr-lineage-guard.md) |
| 模型、写入可见性与来源 | [current/model-tier-and-subtask-cost-routing-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/model-tier-and-subtask-cost-routing-guard.md) · [current/next-step-repository-write-visibility-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/next-step-repository-write-visibility-guard.md) · [current/run-context-and-pr-provenance-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/run-context-and-pr-provenance-guard.md) · [current/source-artifact-preservation-and-design-rationale-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/source-artifact-preservation-and-design-rationale-guard.md) · [current/user-operation-next-step-capability-and-intent-guard.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/user-operation-next-step-capability-and-intent-guard.md) |

在新会话中简短报告接收状态，随后直接进入fresh G00和技术工作。于124的MANIFEST保存 `mnemosyne_handoff_receive` 与 `mnemosyne_guidance_refresh` 两项。记录本包ID、显示名及执行编号、Owner选择/会话可知模型和effort、实际固定来源、读到或缺失的材料、任务范围和接收结果。指导记录写明project_guidance=required、mnemosyne_guidance=yes、task_local_only=true与current_task_preserved=true。模型按可见信息记账。上下文控制采用分组读取及结论索引，远端回写仍按175一次交付六件。无需另发PR接收评论或向旧对话索取材料。
