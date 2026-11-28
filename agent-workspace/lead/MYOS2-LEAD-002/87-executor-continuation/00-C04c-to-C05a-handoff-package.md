接收模型：次一档（默认），建议GPT-6＋Extra。
新对话名称：「MYOS2-A-C05a 代码分析执行」。仅接执行身份，不接MYOS2主线写权。

---
package_id: MYOS2-C04C-TO-C05A-HO-20261108-01
package_status: non_execution_source_transfer_artifact
record_type: mainline_prepared_new_executor_handoff
prepared_by: MYOS2-A-C05
produced_by: unknown
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-08
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 输入和三个专题范围冻结、有明确停止门及Pro主线独立验收；接收不作开放式最终裁决
source_executor: MYOS2-A-C04c / MYOS2-LEAD-002-CHAT-EXEC-06
receiver_display_name: MYOS2-A-C05a 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-07
identity_scope: MyOS2任务局部身份与显示名登记，非全局NREG注册
mainline_owner: MYOS2-A-C05 内核分析主线续建
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
source_pin_short12: a039d9803ade
result_base_short12: 9b7239b29b0d
lead_before_publication_short12: 89319f0e3ac1
current_task_from_package: CORE-USER-FAT-DIRECTORY-MUTATION-57
current_task_ref: ../86-user-fat-directory-mutation-contract.md
project_execution_source: MyOS2 Owner当前及转发指令、00-work-order.md和适用补充、本次固定86号合同
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations: [receive_authorized_package, emit_receive_report, load_MyOS2_rules, separately_load_applicable_Mnemosyne_guidance, verify_live_GitHub, execute_bound_batch57]
receiver_created: NOT_ATTESTED
receiver_received: NOT_ATTESTED
external_execution_started: false
source_executor_writing: STOP_WRITING
source_executor_future_delegation: NO_BATCH57
source_executor_physical_state: unknown
status: PREPARED_FOR_OWNER_FORWARDING
phase: 2
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
base_snapshot: agent/MYOS2-LEAD-002
read_channel: GitHub_connector
evidence_class: Owner授权与固定GitHub证据支持的任务规划；不是执行结果或验收
open_questions: []
inputs_read: [Owner本轮指令, 85号03-CONFIRMED, 56主线回执, 63号, 79号, 固定time目录修改相关原文]
---

# 旧C04c停止；全新C05a接第57批

## 1. 两次轮换分开，当前权威不混用

85号完成的是主线C04→C05轮换。Owner已提供89319f0e3ac1的03-CONFIRMED，C05回读并核现场后接任原lead唯一写者。本87号另行准备分析执行者C04c→C05a，不把两种角色合为一个授权。
Owner指出C04c上下文过长，因此本主线直接从固定GitHub交付制包，不要求旧C04c再工作，不测量或声称其真实token余量。76号属于旧执行者历史，不得重新执行其中49启动词。
本包本身非执行源。Mnemosyne human-approved-spec只在其协议域有效；MyOS2的业务权威始终是Owner及本项目工作令／合同，不能套模板替换成Mnemosyne维护任务。
沿用原PR16/17。执行分支claude/dazzling-cori-q0dnyt只是历史名称，不表示新执行者必须使用Claude。C05a的C05后缀a按本项目派生命名沿用，CHAT-EXEC-07从既有06接续；本次在本包登记，不写Mnemosyne、Alaya或全局命名库。

## 2. 已保存事实和最小恢复材料

固定源码time@a039d9803ade；当前执行PR17为9b7239b29b0d，正式56交付评论6709291114。第56批A/B/C已由旧C04限定接收，PASS_PENDING_LOCAL、returned_scope=[]，须合读R56-S01/S02。不是要求新执行者重验56或重做全部历史。
旧C04c在正式交付后声明STOP_WRITING；85号03再次确认不派57给它。旧C04在主线确认后停止全部业务／仓库写；C05只写lead与既有PR导航，C05a只写新57成果。无法技术认证旧会话进程已终止，所以仍须实时核远端与冲突。

令L=agent-workspace/lead/MYOS2-LEAD-002/。以Owner启动URL中的同一固定发布提交完整读：
1. 本87/00和87-executor-continuation/01-C05a-receiver-startup-prompt.md；
2. L/86-user-fat-directory-mutation-contract.md；
3. L/85-continuation/03-C04-to-C05-handoff-CONFIRMED.md；
4. L/reviews/CORE-USER-FAT-ALLOCATION-GEOMETRY-56-review.md；
5. L/checkpoints/2026-11-08-c05-active-directory57-c05a-ready.md。
随后按86号补读35/36闭合/38/55消费限定及63/79，不以文件列表代替实际正文。56原件只按任务实际依赖定点读，固定9b7239b29b0d；源码仍只从a039d9803ade读。

## 3. 必须按次序完成的接收与规则加载

先读上述恢复材料，并核Mnemosyne当前master身份，读README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md。发送方定向指导参考针为14a71c3b059a；接收者核自己当时版本，不能盲称最新。
形成mnemosyne_handoff_receive报告：包ID、非执行源属性、project_execution_source、声明次一档、Owner报告或可见模型选择（未知则unknown）、后台不可认证、re_tier_on_arrival、已核/未核证据、禁令和安全下一步。此时未加载的规则如实pending。

A. 独立完整加载MyOS2九份文件，均取本固定发布提交：
- L/00-work-order.md（必须读到§12第9条末尾）；
- agent-workspace/README.md、agent-workspace/conventions.md、agent-workspace/tasks/00-gpt-task-protocol-v2.md（P1–P13）；
- L/18-deferred-findings-and-resume.md、L/24-chatgpt-specialist-conversation-separation.md、L/28-review-and-next-batch-same-turn.md、L/63-github-write-retry-and-readback.md、L/79-work-package-sizing-and-pro-review.md。
截断须分段补齐，不能借用C04c或C05的“已读”声明。旧00工作令的最初建分支/PR仪式已经完成，本次不重放；主线权限不自动下放给执行者。

B. 项目规则完成后，单独按Mnemosyne commands/load-mnemosyne-guidance.md加载核心及本任务触发的适用规则，记录当前任务仍是MyOS2第57批。只借用沟通、模型分档、交接、证据、单PR、发布和安全边界；其todo/active-context/handoff-current、维护/测试/归档任务不得成为行动计划。历史厂商示例不要求恢复Claude或调用API。
指导记录按实际载入范围填写；无写权阶段可用真实存在的临时记录，最后并入57 MANIFEST，不增第七件。接收报告、项目加载、指导刷新三项分别留痕，不虚构临时文件。

C. 完成86号G00，Owner已在此新对话明确“接收并执行”且预检无阻断时，同轮连续完成G01–G04，不只回复RECEIVED_READY便索要第二次开工。Owner只要求接收时则只读停止，不自行执行。出现高影响新冲突只交主线裁决，不自升模型或暗用额外子代理。

## 4. 写区与启动门

唯一执行写区：agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-directory-mutation-57/，只新增86号列明六件及本人唯一正式交付评论。所有旧原件、旧评论、lead/PR16、master/time、PR标题/body/Draft均冻结。
首次执行写前PR17/直接执行ref必须仍为9b7239b29b0d，新目录未存在；如发现自己的既有完成提交，先回读恢复而不重复提交，未知变化停止相关写入。主线发布本86/87造成lead正常前进不等于执行冲突，但不能因此把新HEAD其它内容自动纳入固定合同。
不新建分支/PR、不合并关闭/强推删分支，不修内核、不出补丁、不编译预处理运行仓库程序或旧夹具/QEMU/硬件，不操作Owner本机/凭据、不安装工具、不跨仓维护或进入阶段3。纯文本／结构检查和自建有限模型与真实内核证据分层。

## 5. 交付及返回

63号完整适用：暂态GitHub故障先核远端，再通常首次加三次有间隔安全重试，遵守限流；发现次数与实际写请求分开。expected-head/force=false；复用对象，评论查重，不覆盖未知写者，不绕过真实认证/策略禁止。
六件固定提交及活动分支全文/blob/HEAD回读，compare仅允许新六件、旧件零净改删。实际检查范围与NOT_RUN如实留MANIFEST；不得称完整P9或实机通过。
正式交付READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING；结果回MYOS2-A-C05，不回旧C04/C04c，不自行设计58。Owner只需带回实际短提交、正式评论入口和简短完成状态，不下载上传长文件。
本包不证明新对话已经创建、加载、执行或完成；未来真实状态由新接收者的固定交付证明。
