接收模型：次一档（默认），GPT-6＋Extra。首轮终点为第130批作者正式交回。

---
package_id: MYOS2-A-C08b-TO-C08c-130-20261123
track_id: MYOS2-LEAD-002
record_type: receiver_startup_prompt
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_then_execute_owner_activated_contract_181
sender_display_name: "MYOS2-A-C08b 代码分析执行（由C08依据远端记录代备）"
receiver_display_name: "MYOS2-A-C08c 代码分析执行"
prepared_by: "MYOS2-A-C08 内核分析主线续建（GPT-6 Astra Pro自报，后台未认证）"
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
read_channel: connector
receiving_model_tier: next_tier_default
receiving_model_tier_reason: "边界已由主线裁定，接收后执行普通有界合同"
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations: [receive_authorized_handoff_package, emit_receive_report, independently_load_nine_MyOS2_constraints_and_110_124, separately_load_applicable_Mnemosyne_guidance, fresh_G00_and_execute_181]
status: PREPARED_AWAITING_OWNER_START_IN_NEW_CHAT
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
execution_head_at_preparation_short12: 824ce6b5607a
lead_parent_short12: 0b52728d0314
owner_decisions_requested_now: []
---

# 给新对话「MYOS2-A-C08c 代码分析执行」

请以MYOS2-LEAD-002-CHAT-EXEC-20接收 MYOS2-A-C08b-TO-C08c-130-20261123。Owner提供本01固定链接与PR17发布回执，授权你在同一轮完成第130批。先读同提交[00交接包](00-C08b-to-C08c-handoff-package.md)、181合同、两份裁定及检查点。[129独立验收](../reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-review.md) · [两项历史裁定](../reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-D00-127-128-review.md) · [181号第130批合同](../181-user-shell-error-diagnostics-contract.md) · [主线检查点](../checkpoints/2026-11-23-datetime129-reviewed-diagnostics130-dual-handoff.md)

1. 依据00的Mnemosyne直接头先作接收记录，独立读完00列明的九项MyOS2原约束，另读 [110号全文](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) 与 [124号全文](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)；单独刷新任务适用Mnemosyne指导。MANIFEST记录两种操作及真实读件；不继承C08b已读状态。
2. 按181做fresh G00，逐一验证四直接ref、原两PR、原129单父六件、其作者回执#7081367607、本轮固定发布及唯一标记、合同目录/既有部分写入。遇到合法的182签认或本合同已完成步骤按00所列关联合同恢复。[129固定交付回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7081367607)
3. 同轮完成R127-02与R129-01的可回放差量及A/B/C五命令的有界诊断分析。仅原分支六个新增；核compare和固定/活动全文、UTF-8、Git Blob。按63读远端效果后只补缺项，PR17唯一评论 `MYOS2-LEAD-002-CHAT-EXEC-20 / CORE-USER-SHELL-ERROR-DIAGNOSTICS-130 / FORMAL_DELIVERY`，ID双读；全页、四ref、两PR和六件终检后READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING。
4. 把固定提交、六件目录、正式评论、真实核验与未运行边界交Owner。主线接收另由C09办理，你的作者交付不依赖其签认。
