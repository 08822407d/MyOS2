接收模型：次一档（默认），沿用 GPT-6＋Extra。新对话先只读核验；取得正式确认后，等待 Owner 在新对话另行发令并选 Pro 才设计后继。

---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: receiver_startup_prompt
package_id: MYOS2-A-C07-TO-C08-HO-20261120-01
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_authorized_handoff_read_only
sender_display_name: MYOS2-A-C07 内核分析主线续建
receiver_display_name: MYOS2-A-C08 内核分析主线续建
produced_by: GPT-6 Astra Pro
model_per_owner: Pro
date: 2026-11-20
phase: 2
read_channel: connector
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 恢复已裁定状态并核对现场
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations: [receive_authorized_handoff_package, emit_receive_report, independently_load_nine_MyOS2_constraints_and_110_124, separately_load_applicable_Mnemosyne_guidance, report_RECEIVED_READY_or_BLOCKED, await_C07_confirmation, read_confirmation_and_recheck_refs]
status: READ_ONLY_RECEIVE_AWAITING_C07_CONFIRMATION
repository_write_authorized_to_receiver_now: false
lead_before_publication_short12: 41b2c2f955c6
execution_head_short12: a2dd370fcf15
source_pin_short12: a039d9803ade
master_direct_short12: e8e60cd19540
batch114: RETURN_R114_01_02_OPEN
batch115: NOT_DESIGNED_NOT_PUBLISHED_NOT_STARTED_ON_HOLD
future_design_gate: TAKEOVER_CONFIRMED_AND_SEPARATE_OWNER_INSTRUCTION_WITH_PRO
executor_rotation_now: false
owner_decisions_requested_now: []
---

# 给新对话「MYOS2-A-C08 内核分析主线续建」

请接收 MYOS2-A-C07-TO-C08-HO-20261120-01。Owner 给你的本启动词固定链接与 PR17 的 C07 准备回执共同标识本次准备提交。先全文读本启动词和同提交 [00交接包](00-MYOS2-A-C07-to-C08-handoff-package.md)，依次执行以下操作。此回合仅负责原主线的只读接收。

1. 全文读同提交 [D00历史核收](../reviews/CORE-USER-FILE-SEEK-POSITION-114-D00-112-113-review.md)、[114主线验收](../reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md)、[主线检查点](../checkpoints/2026-11-20-seek114-reviewed-c07-to-c08-handoff-prepared.md)，再读交接包列出的历史162合同、113回执及04命名依据。读取准备评论原文和114作者评论，核两条标记的身份与唯一性。[114正式作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7034736478)
2. 原仓库 08822407d/MyOS2。直接读 `refs/heads/agent/MYOS2-LEAD-002`、`refs/heads/claude/dazzling-cori-q0dnyt`、`refs/heads/time`、`refs/heads/master`。准备发布后的 lead 应等于 Owner 提供的163固定链接所指提交，41b2c2f955c6 是它的父。其余预期依次为 a2dd370fcf15、a039d9803ade、e8e60cd19540。开放列表只含 PR16/17，base 均为 master、未合并，PR16 对应 lead 且 Open/Ready，PR17 对应 exec 且保留原题 Open/Draft。master 取 direct ref。 取固定与活动114六件全文和blob，核 41697d76ae5e → a2dd370fcf15 及六新增2,581行、旧改删0/0；核五件准备材料、确认文件缺席、第115批未启动。发现已存在后来的签认时，先核其身份与谱系并如实报告当前阶段，勿重复发布。
3. 按00包先读 Mnemosyne 接收基础，形成 `mnemosyne_handoff_receive` 报告；记录真实版、接收档位与未核事项。再依序读 MyOS2 的00工作令、workspace README、公约、协议v2的P1–P13、18、24、28、63、79九份全文，另读110、124。最后单独按 `commands/load-mnemosyne-guidance.md` 加载本任务适用指导，保留原接收任务。报告和刷新记录留本对话临时记录。后台实际模型无法认证时如实记。
4. 确认历史 R112-02、R113-01 已文档闭合，112/113 组合 PASS_PENDING_LOCAL。114为 RETURN，R114-01、R114-02 OPEN。C07c/EXEC-17已申报STOP_WRITING。第115批未设计、未发布、未启动。完整本地P9及编译运行 NOT_RUN。
5. 实际完成后给Owner一条可转发短报，写明 `RECEIVED_READY`、包号、实核四头和PR、九份约束＋110/124及适用指导的真实完成状态、114裁定、写权待旧C07确认、115暂停。缺项则给 `BLOCKED` 及具体对象。此回合不提交文件，不发PR评论，不试写、设计或启动批次。

Owner 把 C08 的真实 `RECEIVED_READY` 短报带回旧 C07。C07 用次一档核实包号、读件、分支、PR和双方状态，核对无并发冲突后，才在原 lead 新增 `163-continuation/03-C07-to-C08-handoff-CONFIRMED.md`、发布独立确认评论、更新 PR16/17 导航并回读。Owner 把真实确认件的固定链接与评论转给 C08。C08 全文读确认件，重核四条直接引用、两PR和正式确认评论，报告 `TAKEOVER_CONFIRMED`，成为原 lead 唯一主线写者；至此无需返回旧对话作第二次确认。

收到正式确认后仍等待 Owner 在本对话另行发令并选择 Pro 才设计第115批。旧 C07 不可用时仅按00包的 Owner 明确覆盖路径处理。
