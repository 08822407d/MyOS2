接收模型：次一档（默认）
执行位置：新建 ChatGPT 普通对话（不是 Work、Codex、Deep Research 或 Claude Code）
建议对话名：MYOS2-A-C03 内核分析主线续建

---
package_id: MYOS2-A-C02-TO-C03-HO-20261028-01
startup_prompt_role: receiver_entry
execute_in: new_chatgpt_ordinary_conversation
receiving_model_tier: next_tier_default
receiving_model_tier_reason: "先做证据回链与08受限审查，首轮无需不可逆设计裁决或最尖端模型独占能力。"
intended_receiver_action: receive_mnemosyne_handoff
package_status: non_execution_source_transfer_artifact
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_basis: "MyOS2 00-work-order.md §12 第9条，借用适用的行为约束，不接入Mnemosyne维护任务。"
  ordered_operations:
    - receive_authorized_handoff_package
    - emit_receive_report
    - load_MyOS2_confirmed_owner_rules
    - separately_Load_Mnemosyne_guidance
    - continue_received_task_after_single_writer_confirmation
package_path: agent-workspace/lead/MYOS2-LEAD-002/09-continuation/00-MYOS2-A-C02-to-C03-handoff-package.md
repository: 08822407d/MyOS2
receiver_task_scope: MYOS2-LEAD-002_mainline_successor
---
 
# 下面整段是Owner发给新对话的启动消息

请将本新对话命名为「MYOS2-A-C03 内核分析主线续建」，作为旧对话「MYOS2-A-C02 内核分析主线」的后继。保持 MyOS2 主线分析目的，不接管 Claude Code 本地执行任务。

**接收模型：次一档（默认）。** 不要求最尖端/Pro额度做普通交接，除非接收后发现首阶段存在不能推迟的高影响冲突并明确报告理由。

请使用 GitHub 连接器访问公共仓库08822407d/MyOS2。此次请求是显式的**接收交接包**，不是仅加载指导，也不是启动新研究。按固定分支 agent/MYOS2-LEAD-002 先完整读取：

agent-workspace/lead/MYOS2-LEAD-002/09-continuation/00-MYOS2-A-C02-to-C03-handoff-package.md

并按其中的证据导航读取当前 MyOS2 主线工作令、约束和08产物。旧主线任务工作令00-work-order.md必须从头读到§12最后；读不到时不得猜测内容。先只读，核PR16/PR17实时头与分支、08是否已有主线审查，以及是否有更新的Owner指令；不能把交接包里的短SHA当永久最新状态。

请另在Mnemosyne仓库08822407d/Mnemosyne@master读取 README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md 与必要的接收协议。此处Mnemosyne的spec是**Mnemosyne自己的执行源**，不是MyOS2的执行源；MyOS2业务以Owner工作令和项目约束为准。

必须遵守交接包显式的指导加载操作：

```yaml
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations:
    - receive_authorized_handoff_package
    - emit_receive_report
    - load_MyOS2_confirmed_owner_rules
    - separately_execute_Load_Mnemosyne_guidance_for_applicable_behavior_only
    - continue_received_task_after_single_writer_confirmation
```

请**先输出 receive-mnemosyne-handoff.md 要求的完整 mnemosyne_handoff_receive 报告**，区分包存在/权限/已核证据/未核证据/模型档、当前任务、禁止动作、唯一安全下一项以及不足。对MyOS2项目约束做独立确认；在接收报告之后单独执行 commands/load-mnemosyne-guidance.md，仅加载与本目标项目相关的通用行为约束，不导入Mnemosyne维护任务、Owner未授权的阶段或仓库写权限。

**接收的当前业务：** MyOS2阶段2，PR16是ChatGPT主线，PR17是Claude Code本地执行；07已有限审查，08已提交但当前没有主线审查。第一项待做业务是对CORE-USER-SYSCALL-STDIO-08进行GitHub证据审查，不是重复派08、修bug、直接计划09或进入阶段3。

**交接阶段纪律：** 第一步只读核验并报告 RECEIVED_READY / BLOCKED；旧对话仍在等待确认。准备让旧对话停写之前，不要在同一lead分支上并行写业务结果、不另建竞争PR或改变PR16/17状态。Owner把你的接收状态带回旧对话后，旧对话确认接收并停止写入；此后由你成为唯一主线写入者，在已经授权的lead范围内继续评审08。不要自动合并、删分支、改内核、启动构建或QEMU。

普通代码问题全部只记录和延期，不向Owner提出修复选择题；新的权限/证据冲突才报告最小阻断。涉及旧对话的技术背景只从GitHub原件取证，不要求用户转贴全量旧聊天。

接收报告尾部请单独给出：拟用显示名、实际可核任务身份、接收结果、需要旧对话确认的一句话；不要在交接第一轮直接派 Claude 下一任务。

**本启动消息本身不授权任何超出旧工作令的写入、模型付费升级、外部自动唤醒或阶段3开发。**
