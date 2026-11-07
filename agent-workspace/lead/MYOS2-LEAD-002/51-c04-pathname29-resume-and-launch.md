执行模型：次一档，沿用现有C03c的GPT-6＋Extra。执行位置：原「MYOS2-A-C03c 代码分析执行续建」，不新建对话。

---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATHNAME-COPY-29
record_type: existing_contract_resume_and_mainline_return_route_update
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: github_connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
evidence_class: 已保存验收与原任务核对；不是重新验收28或执行29
authorization_ref: 本文第1节Owner本轮指令、已生效的C03至C04确认、28号同轮后继安排约定
mainline_owner: MYOS2-A-C04
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
lead_before_publication_short12: a4b594028b5a
handoff_confirmation_short12: a4b594028b5a
executor: MYOS2-A-C03c / MYOS2-LEAD-002-CHAT-EXEC-03
executor_model_per_owner: GPT-6
executor_effort_per_owner: Extra
executor_rotation: false
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_expected_head_short12: 427604c818a5
source_pin_short12: a039d9803ade
master_observed_short12: e8e60cd19540
original_contract_ref: agent-workspace/lead/MYOS2-LEAD-002/49-user-pathname-copy-contract.md
original_contract_short12: 324927712a98
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-INTERPRETER-28-review.md
required_review_short12: 0a94f32ad00e
review28_status: PASS_PENDING_LOCAL
review28_supplements: [R28-S01, R28-S02, R28-S03]
review28_returned_scope: []
review28_repeated_this_turn: false
prior_launch_hold: retained_until_Owner_explicitly_forwards_this_batch29_launch
activation_gate: Owner向现有C03c发送本51号的执行启动词，且固定输入与现场预检通过
external_execution_started: false
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pathname-copy-29/
allowed_files: [MANIFEST.md, pathname-copy.md, facts-and-boundaries.yaml, pathname-copy-paths.dot, deferred-findings.yaml, evidence.md]
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 51 · 恢复原第29批入口；成果交回C04

## 1. 本轮指令、现状与暂停的处理

Owner本轮原话（不含连接器入口标记）：

> 现在我把当前对话模型切换到了gpt pro。旧对话最后一次设计的任务是“CORE-USER-ELF-INTERPRETER-28”并且C03c对话已经完成了，你看看旧对话有没有对它的结果进行验收，没有的话你就验收。然后按照老规矩每次验收完成后同时设计下一步工作内容并给出提示词。

实际核查：C03已在2026-11-02完成28限定验收，回执为CORE-USER-ELF-INTERPRETER-28-REVIEW-001，PASS_PENDING_LOCAL、returned_scope=[]，三组补正必须合读。本轮完整回读该回执，未重新全文审28六件、重跑源码引文或签发第二份验收。原49号第29批也已经存在，只是交接时暂停；因此恢复原任务，不重复设计同名29、不跳到30。

C04已按固定a4b594028b5a中的03-CONFIRMED回读并接任原lead唯一主线写者；本轮再次核PR16/17及直接分支集合，lead仍a4b594028b5a、执行仍427604c818a5，源码time及master未变。PR16仍open，PR17仍open/Draft，均未合并；29结果目录在执行固定头返回404。远端观察不证明其他会话没有未推送草稿。

本轮Owner要求恢复后继工作安排，故发布可直接转发的29启动入口；不把Owner原话改录成“继续29”。**具体第29批启动许可绑定Owner把本51号执行启动词发送给原C03c这一动作，并须通过执行者现场预检。未转发前，交接暂停及C03c的STOP_WRITING继续有效；文件发布不等于消息已投递或分析已启动。** 这一安排不需要另开一次主线对话索取任务。

## 2. 固定输入和有界替代关系

C03c完整读取本51号、固定324927712a98中的49号全文，以及固定0a94f32ad00e中的28回执全文。原49引用的0a94f32ad00e与本轮主线a4b594028b5a所含28回执，连接器返回相同blob；本轮在后者完整阅读，前者仅核身份，不冒称两次全文阅读。

- [49号原技术合同](https://github.com/08822407d/MyOS2/blob/324927712a98/agent-workspace/lead/MYOS2-LEAD-002/49-user-pathname-copy-contract.md)
- [28主线回执：原49指定固定对象](https://github.com/08822407d/MyOS2/blob/0a94f32ad00e/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-INTERPRETER-28-review.md)
- [C03至C04正式确认](https://github.com/08822407d/MyOS2/blob/a4b594028b5a/agent-workspace/lead/MYOS2-LEAD-002/50-continuation/03-C03-to-C04-handoff-CONFIRMED.md)

本51号只补充当前启动门和主线返回对象：原49号中的“交C03验收”“C03独占主线”在后续操作中改为**C04**。不改原49文件，不改变其P00–P04技术范围、六件清单、固定源码、结果基线、验收上限或禁令。不重放45号首次接收，也不重新启动25–28。

沿用已经加载的MyOS2工作令、公约、反编造协议v2、18号延期、24号主线/执行分工、28号同轮安排和45号执行身份约定；若实际上下文或工具发生缺口，应按精确入口补读，不声称继承不可见工具状态。

## 3. 第29批工作内容与验收关注

按原49连续完成P00–P04：绑定固定输入及28补正；核getname/getname_kernel/__myos_getname/putname的对象与顺序；核用户字符串测长、复制、NUL及实际帮助函数；跟踪所选open/exec调用者的filename消费和释放；完成六件、自查、提交与远端回读。

本批重点是用户/内核路径字符串如何形成filename对象：长度是否含NUL、容量与复制数、负返回转无符号值、错误指针与正常对象、对象本体与字符区、借用指针与所有权、失败出口与释放尝试。没有生产者或全链证据时保留条件模型；不把Linux同名函数契约套到MyOS2，不跨过缺return或无效对象推出确定后果，不把释放调用当物理资源已回收。

原28的EI28-F01等已记录问题使用原qualified_id追加证据，不换调用者就重复计为独立新漏洞。普通发现登记延期；不要求Owner现在处理修复，不重做ELF、参数栈、完整路径遍历、权限或整个uaccess审计。

关键结论必须由所读固定源码的真实path::symbol和1–5行原样片段支持；标签、邻接行号和文本匹配计数不替代语义。执行自查不代替C04验收；只提交READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING。

## 4. 执行启动、写区和停止线

Owner向原C03c转发本启动词时，本批从交接暂停进入“待预检执行”。C03c先核实际master/time/执行头、开放PR及路径交集、29目录和交付评论；执行首次写前预期427604c818a5。未知头变化、同名正式交付或竞争写入先核归属，不覆盖、不强推、不重复做已完成批次。主线合法前进不构成执行冲突；源码漂移不允许静默换固定基线。

唯一执行者仍CHAT-EXEC-03，唯一文件写区仍原PR17的user-pathname-copy-29/六件，另允许本人PR17进度/交付评论。全部旧件冻结，不写lead，不改PR标题/正文/Draft或他人评论，不建分支或PR。阶段2静态范围内自主连续推进，普通内部步骤不逐项等Owner确认。

最终从固定结果提交完整回读六件，并按活动分支核正文/blob/头；compare起点427604c818a5至交付头必须恰六件新增、无旧件净修改/删除或不明后继。正文不预造最终提交号，不把工具超时当失败后盲目重写。实际提交与回读结果写本人交付评论。

正式交付后STOP_WRITING，交「MYOS2-A-C04 内核分析主线续建」验收；Owner仅回报“29已完成”。不得自行开始30。C04后续按28号约定同轮完成该批实际验收及安全后继安排。

禁止改内核、用户程序、脚本、旧成果，生成补丁，预处理/编译/运行C/ASM、QEMU/硬件、真实用户指针样本；禁止安装、挂盘、读凭据、操作Owner本机、读取仓库外libc/镜像或调用额外付费模型；无合并、关闭、强推、删改分支或阶段3权限。

## 5. 人类操作与模型

在现有C03c普通聊天中，沿用次一档GPT-6＋Extra，发送“继续执行CORE-USER-PATHNAME-COPY-29”及本51号的最终固定提交入口。无需新对话、重新交接或搬运长报告。C03c完成后把“29已完成”和实际短提交通知C04即可。

本轮主线模型仅按Owner报告记录为gpt pro。第29批沿用冻结任务的有界执行分档；后续复杂语义裁定由C04主线承担，建议Owner保持其当前Pro选择。无需Deep Research、额外跨厂评审或API调用。发布/转发/实际执行三种状态分别记录，不承诺后台自动启动。
