接收模型：次一档（默认），GPT-6＋Extra。首轮只读接收，终点为真实接收回执。

---
package_id: MYOS2-A-C08-TO-C09-20261123
track_id: MYOS2-LEAD-002
record_type: mainline_handoff_package
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_authorized_handoff_read_only
sender_display_name: "MYOS2-A-C08 内核分析主线续建"
receiver_display_name: "MYOS2-A-C09 内核分析主线续建"
prepared_by: "MYOS2-A-C08 内核分析主线续建（GPT-6 Astra Pro自报，后台未认证）"
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
read_channel: connector
receiving_model_tier: next_tier_default
receiving_model_tier_reason: "首轮核对固定材料和现场，签认另行办理"
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations: [receive_authorized_handoff_package, emit_receive_report, independently_load_nine_MyOS2_constraints_and_110_124, separately_load_applicable_Mnemosyne_guidance, emit_readonly_receipt_and_await_C08_confirmation]
status: PREPARED_NOT_RECEIVED_NOT_CONFIRMED
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
execution_head_at_preparation_short12: 824ce6b5607a
lead_parent_short12: 0b52728d0314
owner_decisions_requested_now: []
---

# C08 → C09 · 主线交接准备包

第129批执行者耗尽上下文。原六件、父链、正式作者和当前远端结果已由C08重建，不需要从旧执行对话取草稿、日志或补发评论；其内部最终操作顺序仍未知。第129作者交付已独立证实。R128-01文档关闭；128本批专题与指定补正组合PASS_PENDING_LOCAL。127仍RETURN（仅R127-02），129为RETURN（仅R129-01）。两个OPEN项只在130追加补正，由接管后的主线Pro独立裁定。 完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。

准备状态为PREPARED，当前主线唯一写者仍为C08，限完成本次交接的核实、必要修订和正式签认。新C09接收轮只读。核130状态时允许exec仍为129，或已出现从824ce6b5607a快进的一次181约定六新增、旧件不变；后者按实际核已提交/待作者评论/已作者交付。读回固定和活动129六件，区分130提交与唯一EXEC-20评论；正在接收或G00、尚无远端写痕的对话状态只能记未知。无需等130作者交回来签认，也无需延后130启动。未预期的头或身份要定位，不擅自复位。 [129独立验收](../reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-review.md) · [两项历史裁定](../reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-D00-127-128-review.md) · [181号第130批合同](../181-user-shell-error-diagnostics-contract.md) · [主线检查点](../checkpoints/2026-11-23-datetime129-reviewed-diagnostics130-dual-handoff.md) · [C09接收启动词](../182-continuation/01-MYOS2-A-C09-receiver-startup-prompt.md) · [已发布的执行启动词](../181-executor-continuation/01-MYOS2-A-C08c-receiver-startup-prompt.md) · [上一代主线签认](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/163-continuation/03-C07-to-C08-handoff-CONFIRMED.md) · [显示名与命名](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/04-guidance-and-conversation-name.md)

直接读原仓库的四个refs/heads：agent/MYOS2-LEAD-002、claude/dazzling-cori-q0dnyt、time、master。time=a039d9803ade、直接master=e8e60cd19540。仅原PR16 Open/Ready和原题PR17 Open/Draft，head分别为lead、exec，base均为master且未合并。开放列表和PR17无since的全部顶层评论分别分页读至空，正式标记按全文、身份和评论ID查重。 固定129作者提交824ce6b5607a、唯一父4a8952c6c956；完整比对六件固定/活动、父子diff及6,612行、283,929字节，读取 #7081367607 并在所有顶层评论内核作者唯一。按Owner启动链接确定本轮发布提交的完整对象，其唯一父0b52728d0314。在该提交下核同一批8个lead新增文件，检查发布/独审/准备三枚标记位于同一条唯一C08正式回执；此前180发布为 #7080232408。固定来源用于复核原状态；另行记录真实活动头及已发生的授权进展。[129正式作者](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7081367607) · [180发布](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7080232408) · [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/MANIFEST.md) · [180号](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/180-user-shell-datetime-format-contract.md)

对照181及第129作者固定六件，恢复两项OPEN的准确范围：R127-02只余三个旧C簇的来源边和B主体边界；R129-01为共享表实参绑定、最终字节映射及真实存储顺序。R128-01已文档关闭，原表原图继续冻结。历史100–126依当期回执及PR16/17归档导航。第130批五条诊断的错误文本、输出请求和链接证据按合同区分，执行者C08c/EXEC-20。仓内原件、time/master、执行分支和旧作者评论均不属于主线写区。

### 新对话必须自行读完的项目约束

依次全文读取九份原约束，再读第10、11项。固定父0b52728d0314，本轮只追加文件。按小组读，截断则补齐到末尾；只摘要路径、版本、适用条款。

1. [00-work-order.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md)（全文至§12第9条）
2. [README.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/README.md)
3. [conventions.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/conventions.md)
4. [00-gpt-task-protocol-v2.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/tasks/00-gpt-task-protocol-v2.md)（P1–P13全文）
5. [18-deferred-findings-and-resume.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md)
6. [24-chatgpt-specialist-conversation-separation.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md)
7. [28-review-and-next-batch-same-turn.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)
8. [63-github-write-retry-and-readback.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)
9. [79-work-package-sizing-and-pro-review.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)
10. [110-owner-communication-and-delivery-continuity.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)
11. [124-model-routing-and-pro-message-budget.md](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

### 接收与指导刷新分别留证

直接取 `08822407d/Mnemosyne` 的master。发送方读取时为e581eb9df6c4。先读 [README.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/README.md)、[current/human-approved-spec.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/human-approved-spec.md)、[commands/receive-mnemosyne-handoff.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/receive-mnemosyne-handoff.md)，形成真实 `mnemosyne_handoff_receive`：包号/性质、档位声明/实际可见、任务来源、核过/待核路径、写权、下一步与未知。然后独立完成上述项目约束；再按 [commands/load-mnemosyne-guidance.md](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/load-mnemosyne-guidance.md) 和 [current/guard-registry.yaml](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/guard-registry.yaml) 单独刷新本任务适用指导。按登记读取显示名、模型与成本、跨对话意图、产物交付、原PR谱系、写入可见性、运行来源、原件保存及Owner下一步。记录 `mnemosyne_guidance_refresh`、真实版本、适用护栏、project_guidance=required、mnemosyne_guidance=yes、task_local_only=true、current_task_preserved=true。本轮选yes仅用于本项目这次接收。包传递项目状态；MyOS2业务授权来自Owner、工作令与现行合同，Mnemosyne行为指导的执行源为其人工批准规范。保持各项目写区独立。

### 真实收件之后的权属手续

如果接收重试时远端已经存在正式03，先核签认提交及评论，按下面第3步处理；若只有部分写入，向Owner报告已见步骤，交旧C08补齐其已授权的发布。

1. C09本轮向Owner交回真实的 `RECEIVED_READY`（缺件则BLOCKED，列出对象）。回执注明包号、显示名、收件/指导刷新结果、已实读九份＋110/124、四个完整直接头、两个PR、两项OPEN及R128-01关闭、129和130实际状态、待旧C08签认及未触及写区。可用一份可转交的Markdown回执或完整短报；无需向仓库发接收评论。
2. Owner将该回执带回原C08。原C08用次一档核真实回执与现场，只在原lead新增 `182-continuation/03-C08-to-C09-handoff-CONFIRMED.md`，向原PR17发一次独立正式确认标记 `MYOS2-LEAD-002 / C08_TO_C09 / HANDOFF_CONFIRMED`，更新原PR16/17导航并终检。若出现签认的部分写入，按63先查已成步骤，避免重复。准备包、准备评论和早代03都不能代替这次真实回执。原C08完成正式签认及回读后STOP_WRITING。
3. Owner把新03的固定链接和正式确认评论带回C09。C09全文回读，核签认父链、唯一标记、四ref及原PR，报告 `TAKEOVER_CONFIRMED`，接任原lead与主线PR导航/验收的唯一写者。至此Owner可归档旧C08，无需再请旧C08二次签认。Owner将130作者回执交C09、选择Pro后，才作该批实质独审并同轮设计后继。131未设计、未启动。
4. 若旧C08确实不可用，只在Owner明确指名原C08不可用、指定C09接替的覆盖授权后，记录授权原文与来源，先重核现场、唯一写者及未完成签认，按原轨另记例外再接管。上下文告急、超时或已准备都不能自动移权。
