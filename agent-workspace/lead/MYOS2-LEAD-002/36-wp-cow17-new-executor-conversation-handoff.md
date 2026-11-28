---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: owner_requested_executor_conversation_rotation_and_batch17_binding
handoff_id: MYOS2-C03A-TO-C03B-20261101
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；Owner曾报告主线选择GPT Pro，后台身份不可独立认证
date: 2026-11-01
status: READY_FOR_OWNER_TO_OPEN_NEW_CHAT
read_channel: github_connector
authorization_ref: Owner报告C03a多次无法发送CORE-USER-WP-COW-17启动消息，要求改在新聊天续任务并检查启动提示词；原24/25/28和35授权边界
role_prior: MYOS2-A-C03a 代码分析执行
role_new: MYOS2-A-C03b 代码分析执行续建
executor_prior_id: MYOS2-LEAD-002-CHAT-EXEC-01
executor_new_id: MYOS2-LEAD-002-CHAT-EXEC-02
executor_model_per_owner: GPT-6
executor_effort_per_owner: Extra
new_chat_exists: false
new_chat_receive_attested: false
new_chat_execution_started: false
old_chat_send_failure: owner_reported_not_independently_diagnosed
old_executor_local_partial_state: unknown
old_executor_future_batch17_write_delegation: REVOKED_ON_MAINLINE
new_executor_batch17_write_delegation: CONDITIONAL_ON_OWNER_LAUNCH_AND_REMOTE_PREFLIGHT
mainline_review_and_planning_owner: MYOS2-A-C03
lead_branch: agent/MYOS2-LEAD-002
lead_pr: 16
executor_branch: claude/dazzling-cori-q0dnyt
executor_pr: 17
executor_head_observed_short12: 38a23958a355
lead_head_before_handoff_short12: 0914e23961ee
source_branch: time
source_pin_short12: a039d9803ade
master_head_observed_short12: e8e60cd19540
task: CORE-USER-WP-COW-17
contract: agent-workspace/lead/MYOS2-LEAD-002/35-user-wp-cow-contract.md
contract_commit_short12: c1493507a818
required_previous_review: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PGTABLE-COPY-16-review.md
required_previous_review_commit_short12: 8fcefd53ef79
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-wp-cow-17/
allowed_files: [MANIFEST.md, wp-cow.md, facts-and-boundaries.yaml, wp-cow-paths.dot, deferred-findings.yaml, evidence.md]
write_pr_comments: executor_own_progress_and_delivery_only
all_prior_results_frozen_through: 38a23958a355
old_taskbook_modified: false
scope_or_deliverables_changed: false
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_c_asm_execution_authorized: false
qemu_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 36 · C03a→C03b 新聊天续接第17批：接收与执行一条启动消息完成

## 1. 本协议为什么存在及适用优先级

Owner报告已向旧执行对话C03a多次发送第17批启动消息，但发送失败，疑似长上下文限制。**这只是Owner报告，不是已证实的软件根因、接收成功或旧对话后台任务状态**。最新远端核查：PR17仍open/Draft，执行头38a23958a355（第16批终态）；读取user-wp-cow-17目录返回404、从最近主线评审以后没有新的PR17交付评论。目录404是有限时点的观察，不保证写入前不会变化。

为避免再由Owner解释旧规则，本文件是新执行者「MYOS2-A-C03b 代码分析执行续建」的**唯一新入口**。本批原35号任务书的目标、W00–W04、六件交付、技术证据标准和禁令全部保持；不新建新技术批次，不复核16，不改内核。

本协议仅对以下较早条款作**有界替代**：
- 25号旧启动包的「对话名C03a、执行身份CHAT-EXEC-01、首次只读然后等待任务、接收后停工」是原对话的初始安排。C03b须首次读取相关通用规则、完成自己的接收检查，但**无需先结束一轮等待新任务**；本36号即是Owner所要求的新接收入口，35号是已发布的明确任务。
- 35号中的「发给已有C03a」「沿用已完成接收」「C03a为本批唯一执行者」及对应activation_gate、writer_assignment，只在本批改为**新建C03b、首次接收、C03b为唯一执行者**。其余内容原样生效；不得暗改35号旧冻结文本或执行成果。
- 旧C03a自此**不再获得第17批的后续写入授权**；不要继续向C03a发送17启动指令。旧C03a在16交付后已经按协议停写；旧聊天有没有本地草稿、排队消息或无法验证的内部工作一律unknown，不能声称已物理停止或已保全。这次改派不得形成两个并行执行写者。
- 主线C03继续唯一负责lead分支、规划、审查、PASS/RETURN和下一批任务；C03b只是本批执行角色。C03b不是C04主线，不占用C03验收职责。

出现文档冲突时：本36号只覆盖上述角色/启动/单写者字段；35号决定本批技术与交付；25/24/18/28及项目工作令决定通用规则；更具体的Owner最新明确指令优先，但不能以聊天自动产生写权。

## 2. 新聊天的接收过程：首次加载必要规则，但不重跑整个历史

Owner在MyOS2辅助分析Project中新建普通ChatGPT聊天，建议命名**「MYOS2-A-C03b 代码分析执行续建」**，选择GPT-6＋Extra。新聊天不能假装保有旧C03a的上下文、25号接收答复、缓存源码或其沙箱；所有实际读取要按新对话逐项记录。也不要求Owner搬运长报告、完整旧聊天、ZIP或文件。

**启动序列：**
1. GitHub连接器核仓库08822407d/MyOS2、PR16/17及各分支实际头；完整读取本36号、原25号启动包、原35号技术任务与16主线回执。原25号的09历史状态只作背景，遇过时内容以最新主线回执为准。固定提交取证不能靠master默认搜索替代。
2. 按25号要求，读取MyOS2的00-work-order.md（包括§12最后一条）、agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md、18-deferred-findings-and-resume.md、24-chatgpt-specialist-conversation-separation.md和28-review-and-next-batch-same-turn.md。查看最新恢复检查点；只读必要段落，不全树重读08–16的大型事实库。
3. 依25号内规定的Mnemosyne接收/指导入口，在08822407d/Mnemosyne的master核README、human-approved-spec、commands/receive-mnemosyne-handoff.md、notes/object-templates-and-id-rules.md §8、commands/load-mnemosyne-guidance.md；按分层规则加载通用证据、模型、GitHub单写者、交付与回读guard。不借此取得Mnemosyne写权或跨项目维护任务。任何不能实际读取的条款准确标记未核，不伪称接收完整。
4. 做**自己**的工具能力检查：GitHub读工具和拟用写工具是否可见，沙箱文本工具是否可用；工具存在≠调用成功≠获得授权。接收摘要保留到本批evidence/MANIFEST或最终回复；**不用单独交接报告、开新PR或等待Owner再次确认。**
5. 若必须材料已实际读取、写前条件无阻断，就将接收状态在内部切换为RECEIVED_AND_TASK_BOUND，直接进入35号W00–W04。若关键规则不能读取、无法核写区、出现不明并行写入或身份冲突，则**在写入前**停止，报告BLOCKED与已安全完成的范围；不能假设上一对话的接收可补足缺口。

**不要求同一回复同时塞满全部原始源码与交付文本。** 尽量分批调用GitHub，长文件按行范围完整读，不把截断当全文成功。任务内部连续推进，不逐步骤问Owner；允许先做好有界文本核验再提交，禁止声称后台会继续运行。

## 3. 第17批完整原样接续

工作ID严格为CORE-USER-WP-COW-17，而不是重新开启第16批、19批或第一个启动测试。遵循35号的W00–W04，主要链为：

**已有PTE写保护写缺页**：handle_pte_fault → do_wp_page → wp_page_copy及其直接页/folio、锁、PTE和返回依赖。区分old page/old folio/new folio、局部变量重绑定、page字节真正复制与仅分配、空页与zero PFN、页表项被安装与返回0、folio_get/put各对象来源、copy失败以及共享映射陷入非返回分支。不得将文件缺PTE的do_cow_fault混入。16回执R16-S01/S02是强制限定，源固定为time @ a039d9803ade。

结果仅新增**六件**，确切位置按front matter的allowed_write_prefix/allowed_files；不修改任何旧批文件、35号原任务书或既有脚本。成果署名C03b / CHAT-EXEC-02，不能冒称C03a做过的检查；可借用先前已经由主线接受的16结论，但必须合读修正和明确原始检验者。完成后只申请READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING，不能自行宣布PASS。

## 4. 写入授权及现场查验

这次Mainline替代35号原有写者：**C03b在Owner将本36号入口发至新对话、完成自身必需接收、并且GitHub写前前提都成立后**，获得本批唯一写入授权。继续使用历史分支claude/dazzling-cori-q0dnyt及原PR17；该名字只是旧历史，不代表当前模型。禁止新建竞争分支或PR、写lead/master/time或更改PR正文/状态。

写前核：PR17仍open/Draft，原执行头应为38a23958a355，任务目录不存在（或已存在时逐件核真实任务/当前归属）、源time固定提交实际可读、PR16/17路径没有冲突。主线合法前进不等于执行源漂移。**如执行头非预期、存在同名未确证内容/不明新提交或旧C03a活动写入证据，停写并报告；不得盲覆盖、force或重置。** 未知旧对话本地草稿不等于远端冲突，也不能凭想象认领为新聊天的执行结果。

GitHub Contents写接口无可证明的全局原子分支锁。仅在指定前缀顺序新增/必要时以当前blob SHA修订**本批自己尚未正式交付的文件**；旧件冻结。超时/中断先核远端头、路径与内容、已有评论，已成功的文件不重做/不覆盖。可以对新聊天自己的临时文本沙箱做任务书允许的静态字符串/集合核对，不安装工具、不开额外付费API。

写后：固定最终提交与当前执行分支双重回读六件，核内容/blob/头一致；compare 38a23958a355至交付头只允许user-wp-cow-17六件A新增，旧件无净修改/删除且无未知提交。于PR17**追加自己的**交付评论，记录真实短提交、差异、限制、NOT_RUN和结果入口；不代写主线验收评论。若PR评论写入被拒而文件已提交，准确报短提交，由C03直接回读，不因此建额外PR。

## 5. 一直生效的纪律和停止线

中文、少打扰Owner、技术细节留GitHub；只做阶段2静态证据和严格有前提的推导。源码事实标路径/符号/1–5行引文；评论/历史审查/其他Linux实现不能替代固定time活动调用，否定结论限实际检索/读取的范围。检查器通过、引文数量、同模型自查、任务书签名均不构成语义正确性或内核运行通过。已做/未做、VALID/UNKNOWN/NOT_RUN分层。普通风险只登记deferred-findings，按18号延期，不把修复问题抛回Owner。

禁止修改内核/用户程序/旧成果，生成修复补丁，预处理/编译/运行C/ASM，构建/QEMU/硬件测试，安装/挂盘/读凭据，读取仓库外libc/镜像，额外API/收费模型/Deep Research，改PR状态、merge/close/delete/force-push、阶段3。真实修复/实验需要新Owner授权。

**完成第17批后停写，把成果交给C03。** 下一批依28号由主线在验收同轮选择并发放，不由C03b自行开始。任务发布或新聊天建立不会自动通知主线；Owner只需在交付后回复C03“17已完成”，主线从GitHub读结果。旧C03a不要再重试本批。

本36号协议是启动/写者迁移材料；发布它并不证明新聊天已经建立、收到了消息或完成了35号技术任务。
