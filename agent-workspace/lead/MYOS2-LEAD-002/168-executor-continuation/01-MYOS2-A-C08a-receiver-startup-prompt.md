---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-INPUT-DISPATCH-119"
record_type: "receiver_startup_prompt"
status: "READY_TO_FORWARD_NEW_CHAT_NOT_STARTED"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md"]
---

# 168/01 · 给新开对话的第119批启动词

你是 **MYOS2-A-C08a 代码分析执行**，唯一执行编号 `MYOS2-LEAD-002-CHAT-EXEC-18`。Owner选择GPT-6＋Extra，已将本固定入口和C08正式PR17回执转给当前新对话。请现在接收并执行第119批 `CORE-USER-SHELL-INPUT-DISPATCH-119`；前任C07c/EXEC-17已停写。

1. 读同一提交的 [168/00 完整接续包](00-C07c-to-C08a-handoff-package.md)、[169号唯一执行合同](../169-user-shell-input-dispatch-contract.md)、[118独立验收](../reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md)、[五项历史裁定](../reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md) 和 [主线检查点](../checkpoints/2026-11-21-shell-cwd118-reviewed-input119.md)，逐项比对Owner转发的正式评论中118验收、119发布标记及接收身份。118作者材料为 [118固定六件](https://github.com/08822407d/MyOS2/tree/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118) 与 [118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701)。
2. 独立完成168/00中的九份项目约束、110、124和Mnemosyne交接及局部指引加载。说明真实所读来源、显示名、模型及不确定项；在对话给短接收回执，继续 fresh G00，在119 MANIFEST存接收和刷新两项记录。
3. 按169核四条直接ref、原PR16/17、固定/活动118六件与Git Blob、全量顶层评论唯一性、119目录空闲。119只用 `claude/dazzling-cori-q0dnyt` / Draft PR17、父 `cb96288abb8b`。一旦发现已有写入，核实来源与内容，按63只补缺项。
4. 依次完成D00三项及读行、分词、派发。只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/` 新增合同的六件。做可行的P9文本检查，按固定/活动全文与Blob验提交，一条PR17正式作者交付评论，按ID双读、分页查重到空并复核四ref。按实际完成进度回报；完成即 `STOP_WRITING`，交回C08 Pro独立验收。

首次行动由此开始，无需先回旧对话补交材料。[168/00 完整接续包](00-C07c-to-C08a-handoff-package.md) · [169号唯一执行合同](../169-user-shell-input-dispatch-contract.md)
