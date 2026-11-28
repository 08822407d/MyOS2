---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: owner_model_routing_and_next_message_reminder_supplement
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner对正式验收的选择）
date: 2026-11-15
base_snapshot: agent/MYOS2-LEAD-002
read_channel: Owner_current_messages_and_GitHub_connector
parent_short12: fc14432f2a56
status: ACTIVE_OWNER_SUPPLEMENT
preservation_level: NORMALIZED_TASK_RELEVANT_SUMMARY
new_kernel_or_execution_authority: false
future_mainline_handoff_must_reference_this_file: true
owner_decisions_requested_now: []
open_questions: []
---

# 按整轮消息选择模型，并醒目标明无需Pro的场景

本续记补充[79号工作包粒度](79-work-package-sizing-and-pro-review.md)、[110号Owner沟通续记](110-owner-communication-and-delivery-continuity.md)、[24号职责分离](24-chatgpt-specialist-conversation-separation.md)及[28号同轮闭环](28-review-and-next-batch-same-turn.md)，不覆盖旧原件，不改变独立验收标准和写区。

## 1. Owner明确给出的约束与本轮确认

Owner说明：模型只能在其向主线或执行对话发送新消息时切换；Pro还受到消息次数额度约束，不能仅按工作量优化。因此不设计同一响应内自动换模型，也不为了拆出简单步骤而增加Pro往返。本记录将其作为Owner提供的工作约束，不核定产品周限额、扣费算法、重置时点或后台路由。

Owner本轮原句（去除消息转义与排版空白）：
> 好，那就继续主线工作，以后当前对话不需要pro模型时你就醒目地提醒我。

结合前两轮已确认分工，采用下表；不是强制所有未来问题都使用同一模型。

## 2. 默认分工

| 发消息所要完成的工作 | 发送前建议模型 | 一轮终点 |
|---|---|---|
| C06b执行已经设计好的普通有界分析包 | 次一档，沿用Owner的GPT-6 Sol／GPT-6＋Extra选择 | G00后连续完成包内分析、自检、发布、回读和停写 |
| C06主线正式独立验收成果并决定后继 | Pro | 当前包验收、局部纠正、回执、下一包设计发布、PR导航、正式评论及远端回读同轮完成 |
| 重要新研究方向、任务边界或跨批次证据冲突的实质裁定 | Pro | 尽量并入已有主线验收回合；独立专题确有必要才另起一轮 |
| 状态查询、解释已有结论、重给启动词、流程澄清、已裁定内容的整理或发布恢复 | 次一档即可 | 不重新作实质裁定，不冒充Pro独立验收 |
| 普通执行确需重建核心分析框架的例外 | 下次发令明确指定Pro执行 | 仍为作者工作，之后仍须作者对话之外的主线独立验收 |

普通工作包目标是一轮次一档执行加一轮Pro主线闭环；下一包设计包含在上一包验收内，不拆成两次Pro。不是保证所有任务一定一轮完成，也不允许为省次数伪签通过。

## 3. 醒目提醒与人工切换

主线判断本次不需要Pro时，回复开头醒目标注：**本轮无需Pro，次一档即可。** 涉及下一次发令时，预先明确发送对话、建议模型及终点，避免Owner另发一条询问模型。

正式交回时标明：**交回本主线独立验收：使用Pro，并同轮安排下一包。** 普通执行启动词标明次一档；曾临时用Pro执行后，下一普通包需要显式提示切回次一档。

提醒不能追溯改变已经发出的消息或退还额度，故优先在上一轮终点预告下一条的选择。AI不能自动切换模型；Owner已明确选择时不得仅因后台无法认证反复索取确认。实际模型只按可见信息及Owner报告记账，未知后端不猜测。

## 4. 避免为模型调度制造额外往返

局部难点随固定证据、冲突和未证前提一起交回，在既定Pro验收回合中做有界复核；只有大量新调查才另派执行。Pro对原件的必要交叉检查仍独立，不把主线新增推导伪装成作者已完成且又被独立认证的结论。

机械计数和结构检查尽量在次一档交付前完成；已经进入Pro验收回合后，授权范围内的文档和发布操作继续做完，不分派成“Pro裁定—次一档整理—Pro再确认”的固定链。标签漏计、字段遗漏或GitHub暂态异常本身不构成升级执行模型的理由；63号安全恢复继续适用。

约三个关联专题合包，按真实依赖决定边界，不靠增加文档长度凑工作量。不允许执行者自行连续启动若干未发布批次再集中验收；扩大或合并范围应在上一次主线设计时明确。局部文档补正可并入后继包，关闭前保留原RETURN，不另造必需的Pro小回合。

## 5. 权限和交接

模型切换不改变作者/审查者身份、单写者、固定源、原件冻结或运行禁令。主线用次一档处理事务不等于完成Pro正式验收；执行对话用Pro仍不能自审签收。完整本地P9未做时最高仍为PASS_PENDING_LOCAL。

以后主线交接包、接收启动词及固定必读清单在显式引用110号的同时，也须引用本124号，并给出真实固定提交；新主线独立完整读取，不继承旧方已读声明。本轮87验收与88设计仍是一轮Pro主线工作；88默认继续现C06b/CHAT-EXEC-11用次一档。无额外模型、子代理、付费API或跨项目维护授权。
