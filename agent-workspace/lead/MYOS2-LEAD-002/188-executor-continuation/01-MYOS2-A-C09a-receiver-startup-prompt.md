---
receiving_model_tier: "next_tier_default"
receiving_model_per_owner: "GPT-6＋Extra"
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "receiver_startup_prompt"
followup_id: "CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（主线独审档）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "676e4169f14c"
execution_pin_short12: "d01efe19ccec"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/181-executor-continuation/00-C08b-to-C08c-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/181-executor-continuation/01-MYOS2-A-C08c-receiver-startup-prompt.md","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
status: "PREPARED_AWAITING_OWNER_START_IN_NEW_CHAT"
package_id: "MYOS2-A-C08c-TO-C09a-136-20261123"
package_status: "non_execution_source_transfer_artifact"
intended_receiver_action: "receive_refresh_then_execute_owner_activated_contract_188"
sender_display_name: "MYOS2-A-C08c 代码分析执行（C09 根据远端实物代备）"
old_executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-20"
receiver_display_name: "MYOS2-A-C09a 代码分析执行"
receiver_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"yes","task_local_only":true,"ordered_operations":["receive_authorized_handoff_package","emit_receive_report","independently_read_nine_MyOS2_constraints_and_110_124","separately_refresh_applicable_Mnemosyne_guidance","fresh_G00_then_execute_188"]}
mnemosyne_sender_read_short12: "e581eb9df6c4"
actual_receiver_state: "NOT_STARTED"
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 请在新对话「MYOS2-A-C09a 代码分析执行」接收并执行

**接收及执行模型为次一档 GPT-6＋Extra。首轮终点为第 136 批正式作者交付与 G04 终检。**

请在 MyOS2辅助分析 项目中，以新身份 MYOS2-LEAD-002-CHAT-EXEC-21 接收 MYOS2-A-C08c-TO-C09a-136-20261123。Owner 同时提供本启动词与 PR17 的 C09 正式回执，授权你在同一轮完成以下工作。[00 交接包](00-C08c-to-C09a-handoff-package.md) · [135 独审](../reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md) · [133/134 补正裁定](../reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md) · [检查点](../checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md) · [188 号合同](../188-user-new-task-runqueue-entry-contract.md)

1. 先完整读 00、01、同提交 188 合同、两份裁定和检查点。按 00 的 Mnemosyne 直接 ref 读 README、人工批准规范和 receive 命令，形成自己的 mnemosyne_handoff_receive。随后全文读 00 明列的 MyOS2 九份约束与 [110 号](https://github.com/08822407d/MyOS2/blob/676e4169f14c/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)、[124 号](https://github.com/08822407d/MyOS2/blob/676e4169f14c/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)；再用 load 命令和登记文件作独立 mnemosyne_guidance_refresh，记录 current_task_preserved=true。接收与刷新分别留证，不复用前一席位的“已读”。
2. 完成 fresh G00。核原四 refs、PR16/17 和开放分页、顶层评论完整分页、135 固定执行父子及六件重算、作者 #7095153852、187 发布 #7094597734、本次主线六件与三标记唯一发布评论，再核 136 新写区。C09 已重建远端可见终态，135 原作者发评后的内部只读顺序未知。[PR17 #7095153852](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7095153852) · [PR17 #7094597734](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7094597734)
3. 按 188 完成 R135-01/02/03，追清 sched_fork 的初始化、wake_up_new_task 的 CPU 0 队首插入，并比较一次给定 picker 调用在两个 tick 输入下是否选 K。仅在原 claude/dazzling-cori-q0dnyt 的 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/ 追加六件。保持阶段 2 和全部运行禁令。
4. 以单父、expected_sha、force=false 完成 G04。对照 compare、固定/活动六件，PR17 仅一条 `MYOS2-LEAD-002-CHAT-EXEC-21 / CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136 / FORMAL_DELIVERY`；按 ID 双读并全分页查唯一，再核四 refs、原两 PR 和六件，以聊天终检注明实际读回。成功后 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，交回原 C09 使用 Pro 独审并同轮设计下一批。出现可恢复的 GitHub 异常依 63 查远端、只补缺步。
