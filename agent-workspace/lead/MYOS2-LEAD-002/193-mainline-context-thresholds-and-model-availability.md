---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: owner_mainline_context_threshold_and_model_availability_supplement
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6.2（模型标识；实际后台与effort未独立认证）"
model_per_owner: "本轮未另行告知选中档位；可用版本按下文Owner报告登记"
date: 2026-11-23
date_timezone: Asia/Shanghai
read_channel: Owner_current_message_and_GitHub_connector
base_snapshot: agent/MYOS2-LEAD-002
parent_short12: 28f8646df32f
status: ACTIVE_OWNER_SUPPLEMENT
scope: MYOS2-LEAD-002当前及后继主线验收与设计对话；模型分工适用于本轨道后续执行发令
preservation_level: OWNER_MESSAGE_AND_NORMALIZED_IMPLEMENTATION
source_of_authority: Owner本轮明确指令
future_mainline_handoff_must_reference_this_file: true
mainline_design_count: {conversation: C09, first_batch: 131, last_batch: 140, distinct_batches: 10, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
model_availability_per_owner: {chat_non_pro: "GPT-6.2 Sol", chat_pro: "GPT-6 Astra", reported_on: 2026-11-23, account_scope: OWNER_REPORTED, backend_attestation: UNATTESTED}
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
open_questions: []
---

# 主线对话上下文两级提醒与可用模型版本更新

本续记补充 [28号同轮验收与派发](28-review-and-next-batch-same-turn.md)、[79号合包](79-work-package-sizing-and-pro-review.md)、[110号Owner沟通](110-owner-communication-and-delivery-continuity.md)、[124号模型分工](124-model-routing-and-pro-message-budget.md)。Owner 本轮新增要求自本轮生效；旧任务书、旧验收、既有作者归属及技术结论不改写。

## 1. Owner 原话

> @GitHub 那么你增加一条本系列主线工作的规则：承担批次工作验收和设计的主线对话系列每当设计工作数量分别到12批次和15批次时在回复开头明显提示已达到xx批次，以提醒我当前上下文可能快要耗尽。此外更新一下可用模型版本：chat中非pro模型目前已经可以使用gpt-6.2 sol，但pro模型仍然只可用gpt-6 astra

## 2. 主线每对话独立设计计数及两级提醒（持续规则）

适用于当前 C09 和以后 C10、C11 等承担独立验收与新任务设计的主线对话；不把 CHAT-EXEC 执行对话的交付次数混入。

- 每个新主线对话以自己首次正式设计的批次为起点，从 0 独立累计不同批次的设计数。以固定合同、正式发布回执和主线设计归属为证。前任设计、接收或验收的继承批次不计；同批补正、换发、重新启动、复核、恢复发布、单纯规则续记不重复计。尚未正式发布的候选方案不作已完成设计；如果发布中断，另记实际完成状态，不虚增。
- 本对话累计正式设计达到 **12 批次** 时，在该轮面向 Owner 的最终回复开头、先于普通验收结果或启动词，以独立醒目加粗首段提示：**【上下文提醒】本主线对话已累计设计 12 批次任务，当前上下文可能接近耗尽，请留意后续交接。**
- 累计达到 **15 批次** 时，同样在最终回复开头醒目提示：**【上下文重点提醒】本主线对话已累计设计 15 批次任务，上下文耗尽风险可能较高，建议评估新老对话交接。**
- 两个门分别触发、分别登记。验收与后继设计合在一轮时，以本轮发布后实际计数为准；如准备在回复较早部分给出结果，先说明预计达到门槛，正式结果仍须区分预计与实际。中断导致未向 Owner 送达或无法确认送达时，在恢复后的首个可用回复开头补发未确认提醒；不假称已提示。不因提醒另开一次 Pro 回合。
- 12、15 是 Owner 指定的工作量风险提示，不是产品 token 上限、剩余上下文测量、自动停工、自动交接或写权移交门。更早发现实际上下文风险时仍须及时报告、保全进度；Owner 指示继续、暂停或交接按既有授权执行。

每轮新批合同发布后的主线检查点记录本会话身份、首次/最新设计批次、累计数、两门状态；下轮根据固定合同和回执核实。后续主线交接包、接收启动词及必读清单必须显式引用本193号的固定提交，独立全文读取，与110/124号一起继承。交接记录旧对话计数与提醒状态，新对话另从0开始，不继承旧计数；既有写权确认门保持。

## 3. C09 当前计量与触发点

根据 [C08→C09正式交接](182-continuation/03-C08-to-C09-handoff-CONFIRMED.md)、183～192号原合同与 [第139批验收/140发布检查点](checkpoints/2026-11-23-argv139-reviewed-entry-view140.md)，C09 首次设计131，已发布至140，共10个不同批次。130归C08；140已正式设计发布，执行尚未启动（仅指本轮直接核对时点）。本规则发布不增加计数。

| 本会话设计累计数 | 全局批号（若连续由C09设计） | 提醒状态 |
|---|---:|---|
| 10 | 140 | 当前 |
| 12 | 142 | 首次触发12批提醒 |
| 15 | 145 | 首次触发15批重点提醒 |

历史口径依据：C03 09–29（21）、C04 30–56（27）、C05 57–76（20）、C06 77–100（24）、C07 101–114（14）、C08 115–130（16）；原始归属见50/85/109/143/163/182号交接与相应合同。此统计仅反映历代已设计工作量，不推断实际上下文容量。

## 4. ChatGPT 模型可用版本（Owner 2026-11-23 报告）

| 角色／场景 | 当前可用版本与后续建议 |
|---|---|
| 次一档、普通非Pro执行、只读接收、状态说明、已裁定发布恢复 | GPT-6.2 Sol；普通分析执行沿用既有高推理强度偏好，在Owner界面提供相应选项时选择Extra或Owner指定的可用强度 |
| 主线正式独立验收及后继设计，或重要实质裁定 | GPT-6 Astra 的 Pro 档；Owner报告目前Pro仅此版本可用 |

本表更新124号中旧GPT-6 Sol／GPT-6＋Extra的当前默认版本表述；角色分离、按整轮消息选择、非Pro适用场景和醒目提醒不变。版本可用性仅为Owner本日所报告的Chat界面情况，不是全球产品发布、账户配额或实际后端认证；不得据此杜撰GPT-6.2 Pro、自动切换或在回复内部换模型。未来以Owner最新可用版本和明确选择为准，保留日期与来源，历史记录不回填新模型。已发布的192号第140批合同技术范围、作者席位、交付和运行禁令全部不变；未来发令的模型提示按当前可用情况更新，已经启动的工作不因此重新开始。只有Owner能在发新消息时选择模型。

## 5. 不变的权限与远端边界

仍为阶段2，原lead/PR16、原执行分支/Draft PR17。无内核、旧分析成果、旧验收、旧正式评论修改，无新执行批次、合并、强推或阶段3授权。完整本地P9、预处理、编译链接、仓库程序/脚本、Graphviz、QEMU、实机及真实操作均NOT_RUN。规则写入不等于第140批执行启动。
