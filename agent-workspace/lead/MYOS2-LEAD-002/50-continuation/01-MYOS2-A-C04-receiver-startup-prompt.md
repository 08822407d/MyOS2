接收模型：次一档（默认）。执行位置：MyOS2辅助分析Project内新建的ChatGPT普通聊天；不是Deep Research、Codex或新的代码执行对话。

---
package_id: MYOS2-A-C03-TO-C04-HO-20261102-01
record_type: paired_mainline_receiver_startup_prompt
package_status: non_execution_source_transfer_artifact
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（Owner先前说明；后端未独立认证）
date: 2026-11-02
read_channel: github_connector
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 首轮只做交接接收与固定状态核对，后续复杂验收再按实际需求选档。
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations: [receive_package, emit_receive_report, load_MyOS2_constraints, separately_load_applicable_Mnemosyne_guidance, report_RECEIVED_READY_or_BLOCKED, wait_for_confirmation]
package_ref: agent-workspace/lead/MYOS2-LEAD-002/50-continuation/00-MYOS2-A-C03-to-C04-handoff-package.md
package_initial_commit_short12: 8ec1860b16ee
intended_scope: 主线C03至C04接续；不接替C03c执行者
execution_intent: RUN_NOW_RECEIVE_ONLY
repository_write_authorized_in_receive: false
batch29_activation: ON_HOLD
---

# 可直接发送给新主线的启动内容

@GitHub 接收模型：次一档（默认）。请将本对话用于「MYOS2-A-C04 内核分析主线续建」，接手旧对话「MYOS2-A-C03 内核分析主线续建」的主线职责，继续任务轨道MYOS2-LEAD-002。

我要求按照Mnemosyne协议接收显式交接包，不是直接开始新的源码研究，也不是创建下一个执行者。完整读取：

https://github.com/08822407d/MyOS2/blob/8ec1860b16ee/agent-workspace/lead/MYOS2-LEAD-002/50-continuation/00-MYOS2-A-C03-to-C04-handoff-package.md

同时读取Owner启动消息所指定交接提交中的本01启动词及配套检查点；最新活动PR头仅用于核时效，不以新目录名自动授权行动。

按包内顺序先执行Mnemosyne接收并形成mnemosyne_handoff_receive报告，再分别完整加载MyOS2项目约束及本次适用的Mnemosyne指导。两类操作分开，指导刷新保持已接收的MyOS2任务；不导入Mnemosyne维护路线，不向该仓写入。不要声称继承了旧聊天、旧沙箱或旧工具检查。

特别注意真实恢复状态：第28批427604c818a5的验收回执已保存在原PR16的8c36303cdd0c，结论PASS_PENDING_LOCAL且必须合读R28-S01/S02/S03。本次是接收已有验收记录，不重派28，也不把执行者旧PENDING当作最新主线状态。中断回合已经保存的49号第29批任务书保留原件，但其启动已被本次交接暂停；本轮不派发、不执行、不重写29，也不新建30。

现有「MYOS2-A-C03c 代码分析执行续建」仍为执行者MYOS2-LEAD-002-CHAT-EXEC-03，已完成25–28四批。本次只转移主线，不轮换或恢复其他执行对话。

接收阶段仅只读核GitHub、28回执与必要交付边界；不得新建分支/PR、写接收回执到仓库、修改PR元数据/评论、内核或报告，不合并、不编译运行，不接管PR17结果写区。

完成后明确回复RECEIVED_READY或BLOCKED，报告所核交接package_id、lead与执行短头、规则加载状态和能否安全接手，然后停下等待旧C03确认。不要开始主线业务写入或下一批规划。Owner会把接收状态和两头短号带回C03；C03保存CONFIRMED后，Owner再将确认短提交带回本对话。

若旧对话已无法响应，只有Owner明确宣布旧C03不再写入并授权C04接管，才走包内替代确认流程；不能凭超时自认接管。即使完成写权移交，29也须Owner明确要求继续并重新核现场后才恢复。

行为规则均由包内材料读取，不让我重复解释；关键材料读不到或出现不明并行写入就如实BLOCKED，不伪造完整接收。后续复杂独立验收/规划建议沿用Owner选择的GPT Pro，但本次接收不自动切换主模型或调用额外收费API。
