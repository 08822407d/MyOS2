---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: owner_direction_same_turn_acceptance_and_next_task
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
base_snapshot: lead=agent/MYOS2-LEAD-002；发布前短提交9af19669b2e8
evidence_class: Owner工作方式补充；不是技术验收或执行完成
status: ACTIVE_OWNER_DIRECTION
scope: 本主线及其后继的验收回合与下一工作包发布
authorization_ref: Owner本轮明确要求；既有工作令阶段2、18号及24至27号分工
inputs_read: [Owner本轮指令, 既有阶段与执行验收分离约定]
owner_decisions_requested_now: []
open_questions: []
---

# 验收与下一工作包在同一轮完成

## Owner原话

> 我对你的工作方式增加一个要求，从这次开始每次你验收完成之后自动给出下一步工作内容，以免我需要用两次pro对话额度。

## 自本次第10批验收开始适用

每次接收执行交付后，主线在同一轮完成：实际读取与有界验收、审查回执和检查点写回、下一项工作的选择与任务书发布、必要PR导航同步，以及可直接交给现有执行对话的短启动块。不得在没有真实阻断时仅回复“验收完成，等待你再要求发布下一批”。

这是主线规划与派发的持续授权，不要求Owner对AI内部步骤重复批准。执行对话仍按明确工作包自主推进，完成后交回本主线独立验收；不自行选择后继工作或给自己签发主线通过。

验收出现局部报告问题时，必须如实保留RETURN和消费限制。可把定点文档处置作为下一工作包首项，与不依赖该问题的安全取证同轮安排，不强迫为一个小修正再增加一次派发回合。涉及错误证据的后续结论不得在修正前作为真值使用；执行完成也不自动关闭主线退回项。

下一工作包须说明目标、固定输入、唯一执行者、既有分支/PR、新写区、交付、验收条件和停止边界。模型沿用Owner指定的执行对话GPT-6＋Extra；主线模型仅按Owner告知记录，不认证后台。允许正常工作包内连续推进，不自动启用额外付费服务。

若所有可继续方向都受真实访问、授权、输入冲突或数据安全风险阻断，仍须在同一轮给出最小安全下一步及准确阻断，不虚构可执行任务、不假称已经派发。已完成的可用部分及时保全。

## 不变的门与边界

保持阶段2；18号普通问题延期和原件保全有效。没有新增内核修改、修复补丁、预处理、构建、C/ASM/QEMU运行、仓库外libc/镜像读取、阶段3、合并/关闭PR、删除分支或强推权限。任何此类动作仍须原有独立授权。

发布GitHub任务不等于自动创建、唤醒或向另一聊天投递成功。最终回复优先给验收结论和下一任务短启动块，Owner只需转发入口；不要求下载、上传或搬运长报告。不得把同轮连续完成写成后台异步工作承诺。

本记录补充工作令与后继检查点的执行方式，不修改Mnemosyne本身、不改变C03主线与C03a执行职责，也不重开已经结束的旧任务。
