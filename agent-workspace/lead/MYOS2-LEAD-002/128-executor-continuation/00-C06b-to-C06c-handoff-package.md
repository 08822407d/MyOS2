接收与执行模型：次一档，沿用GPT-6＋Extra。执行位置：MyOS2项目中新开的普通ChatGPT对话“MYOS2-A-C06c 代码分析执行”；新身份MYOS2-LEAD-002-CHAT-EXEC-12。任务：接收后通过G00，连续完成第90批D00、G01–G04。

---
package_id: MYOS2-C06b-EXEC11-TO-C06c-EXEC12-BATCH90-HANDOFF-20261115
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_Mnemosyne_handoff_then_execute_authorized_MyOS2_batch90
prepared_by: MYOS2-A-C06主线（Owner要求下的执行者换接，不是旧执行者自签）
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: 本轮未重新申报；事务性换接无需Pro，不重新作技术验收
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 第90批技术范围及两项补正已经冻结；新对话独立加载和有界执行，技术验收另交原C06 Pro
date: 2026-11-15
read_channel: connector
receiver_display_name: MYOS2-A-C06c 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-12
old_display_name: MYOS2-A-C06b 代码分析执行
old_identity: MYOS2-LEAD-002-CHAT-EXEC-11
old_writer_state: STOP_WRITING_OWNER_RETIRED_CONTEXT
receiver_state_at_preparation: NOT_CREATED_OR_STARTED_BY_MAINLINE
project_execution_source: Owner本轮换接指令、MyOS2的00工作令及其约束、127合同与本目录02补充令
mnemosyne_execution_source: 08822407d/Mnemosyne/current/human-approved-spec.md（仅其声明范围，不授予MyOS2写权）
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
mainline_parent_short12: 07ed0fed054c
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_start_short12: 42897a06feb1
source_time_short12: a039d9803ade
master_direct_short12: e8e60cd19540
mnemosyne_observed_master_short12: ee65de8a255d
original_contract: 127-user-id-allocation-lifecycle-contract.md@07ed0fed054c
source_preservation_level: NORMALIZED_OWNER_REQUEST_AND_FIXED_REPOSITORY_ORIGINAL_POINTERS
private_C06b_unpublished_context: UNKNOWN_NOT_TRANSFERRED
owner_decisions_requested_now: []
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - receive_explicit_package_and_form_real_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_and_124_in_full
    - separately_load_current_applicable_Mnemosyne_guidance
    - complete_fresh_G00_then_continue_D00_G01_G02_G03_G04
---

# C06b → C06c：第90批新对话接收包

## 1. 换接依据及状态

Owner本轮说明C06b上下文已满、无法继续，要求修改第90批启动提示词和相关材料，在新对话开展。这里保存任务相关规范化摘要，不声称获得旧对话完整导出、隐藏上下文或未提交草稿。

本C06主线已核：lead为07ed0fed054c，exec仍42897a06feb1，master为e8e60cd19540，time为a039d9803ade；PR16/17均Open且未合并，PR17仍Draft。第90批目录在固定exec下返回预期404；89主线评论6894330789之后的评论查询为空。因此没有远端第90批成果需要覆盖或迁移；不能据此断言旧C06b从未私下阅读或起草。

旧C06b/11自Owner本次换接指令起退出第90批写者位置。无需Owner返回已满对话索取STOP_WRITING、RECEIVED_READY或CONFIRMED；不冒称旧方另行签了确认。旧C06a/10、C05c/09仍停用。12不得冒用11身份或继承其已读资格。

本C06主线未交接，仍负责lead/PR16和后续独立验收。新显示名沿用既有MYOS2-A-C06派生C系，canonical身份为CHAT-EXEC-12；名称登记在本项目本包，不修改Mnemosyne或Alaya，不占DR号，不声称已创建或改名网页侧栏。

## 2. 已完成与未完成

第89批执行交付：42897a06feb1，PR17评论6893704757。第89批主线审查、88文档闭合及90原合同：07ed0fed054c，主线评论6894330789。

- R88-01已关闭，88组合PASS_PENDING_LOCAL；R86-01已关闭，87及其限定维持原状。
- 89仍为RETURN，仅R89-01/02；其余主体按R89-S01/S02/S03保留。换接不改变裁定，不重做89整批。
- 90没有远端执行成果或验收；新12尚未由主线自动创建、启动或认证G00。旧C06b私有进度未知，不能倒造已完成阶段。
- 90仍先补正两个89问题，再做IDA范围/占用回收、IDR范围/NULL槽/游标、映射接口及有界消费者三个专题。不是新增第91批，也不是恢复84。

本包传递任务状态，不是独立执行源。具体授权来自Owner当前指令、MyOS2项目规则及127合同与02补充令；通用指导不能扩权。

## 3. 固定材料及阅读边界

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。本128目录三件取Owner转发启动URL中的真实固定发布提交；下述已有L文件固定取07ed0fed054c，原件版本不随活动分支漂移。

### 接收与执行必读

完整读取本00、同目录01和02，以及：

1. L/127-user-id-allocation-lifecycle-contract.md，技术条款保留，人员及接收条款按02替代。
2. L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md。
3. L/reviews/CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-review.md。
4. L/reviews/CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-review.md及同目录CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-DOC-CLOSURE-review.md。
5. L/checkpoints/2026-11-15-workqueue89-reviewed-id90-ready.md；其中“继续11、未轮换”是旧时点，已由本02替代。
6. L/reviews/CORE-USER-PERCPU-ALLOCATION-87-review.md、CORE-USER-PERCPU-BOOTSTRAP-86-review.md及CORE-USER-PERCPU-BOOTSTRAP-86-DOC-CLOSURE-review.md，完整读取其现有资格和消费限定，不重开旧闭合项。
7. PR17评论6893704757（作者交付）、6894330789（主线审核/原90发布），以及本次128发布评论（真实编号由发布后的Owner消息/PR导航提供）。

89原六件全部独立完整读取，固定42897a06feb1的R/user-workqueue-pool-bootstrap-89/：MANIFEST.md、workqueue-pool-bootstrap.md、facts-and-boundaries.yaml、workqueue-pool-bootstrap-paths.dot、deferred-findings.yaml、evidence.md。它们是D00纠正的对象，不因旧作者声称读过而免读。

历史去重按127：完整读取L/reviews/CORE-USER-PID-LIFETIME-11-review.md；固定2c410132e42d的R/user-pid-lifetime-11/MANIFEST.md和deferred-findings.yaml全文，以及facts-and-boundaries.yaml中PID11-A13/A66/A67/A70/A72/A73/A74及必要上下文。PID11-F01/F02保留原身份，旧回执的历史RETURN文字不等于允许重开其后来处置。

其它历史原件仅在具体断言或消费者实际依赖时定点读取并登记范围；未读不作源证据。无需整包重读旧83/84、119启动词、全部历史批次或完整旧聊天。119只为本主线准备本包时的历史换接模板，不再是12的发令入口。127指定源码仍须按固定time独立读取，主线摘要不能替代原函数。

## 4. 先形成接收报告

接收前只读，不写GitHub。先读取本包、01/02、127、89回执及检查点等接收材料，核现场；为执行接收程序，读取Mnemosyne@ee65de8a255d的README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md，以及notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md适用导航。核其实际master；后续指导刷新若版本变化，如实登记所选固定版本，不将活动版本变化自动变成MyOS2合同变更。

按接收命令形成真实mnemosyne_handoff_receive报告，至少记package身份、receiver_guidance_load、MyOS2与Mnemosyne各自执行源、接收模型声明及实际可见/Owner来源、re_tier_on_arrival、实际已核与缺失路径、90当前状态、禁止动作、safe_next_action和未知。未读取的九规则及指导此时应为pending，不提前宣布完成。模型后端不能认证则记unknown/Owner-reported，不索取重复模型确认、不调用额外模型。

接收报告先于后续规则加载；不能用最终汇总倒填一个从未形成的早期报告。可以真实保存临时文件；不能落盘则如实保存报告文本及限制，不虚构sandbox路径。接收报告不是G00通过或写权凭证。

## 5. 新对话独立全文读取九规则，再读110和124

以下九件固定07ed0fed054c，逐件到EOF，截断分段补齐：

1. L/00-work-order.md，含§12全部Owner续记。
2. agent-workspace/README.md。
3. agent-workspace/conventions.md。
4. agent-workspace/tasks/00-gpt-task-protocol-v2.md，全部P1–P13。
5. L/18-deferred-findings-and-resume.md。
6. L/24-chatgpt-specialist-conversation-separation.md。
7. L/28-review-and-next-batch-same-turn.md。
8. L/63-github-write-retry-and-readback.md。
9. L/79-work-package-sizing-and-pro-review.md。

之后独立全文读取L/110-owner-communication-and-delivery-continuity.md和L/124-model-routing-and-pro-message-budget.md；它们追加于九规则，不替代任一原文件。不能继承C06b、C06主线或其它对话的已读声明。新12在自身有效会话内确已读完且对象一致的材料可以复用，但必须明确记为12自己的读取记录。

## 6. 再单独加载适用Mnemosyne指导

这一步是行为约束刷新，不是重新选择任务。按load命令核实际master、固定所用版本，完整读取核心README、human-approved-spec、user-operation-next-step-capability-and-intent-guard和load命令；读取guard-registry.yaml导航头及实际触发项。

按本次触发完整读取current/下的model-tier-and-subtask-cost-routing-guard.md、github-single-active-pr-lineage-guard.md、run-context-and-pr-provenance-guard.md、artifact-delivery-and-direct-generation-guard.md、cross-conversation-execution-intent-and-operator-flow-guard.md、source-artifact-preservation-and-design-rationale-guard.md、external-research-display-name-guard.md、next-step-repository-write-visibility-guard.md；命名只读notes/registries/project-research-display-name-registry-v0.1.md中§7适用范围。当前loader有额外适用触发时补齐，不假称这张清单永远完整。

真实mnemosyne_guidance_refresh记录注明current_conversation_task_preserved=true、maintenance_live_route_imported=false、实际加载版本及范围。不把current/active-context.md、handoff/handoff-current.md、todo/open-questions等维护路线导入90。task-local mnemosyne_guidance=yes不是全项目通用政策。

新12的接收报告、九规则/110/124读取账与刷新记录在G00前只作临时记录，门通过后并入90六件的MANIFEST、facts或evidence，不新增第七件、不另写Mnemosyne仓库。通用Ready-PR、自动模型路由等不得覆盖PR17保留Draft、禁止额外模型/API、禁止内核运行的MyOS2具体规则。

## 7. G00、限定写权及连续执行

按02和127重新完成G00：四direct refs、PR16/17、开放PR、90目标目录、公约原句、time/master金丝雀；配置判断前完整读取options_flags.cmake。exec预期42897a06feb1，master e8e60cd19540，time a039d9803ade，lead为本128实际发布提交。查询90的所有正式交付标记，必须兼查旧11和新12，不能只因换身份就漏掉旧交付。

发现90目录、任何90交付或未知HEAD变化，先核归属和内容，不覆盖、不强推、不重复交付。若为本12已经成功的动作则只补回读；若是旧11或未知写者的推进，停止相关写动作并交C06裁定，继续安全独立阅读，不自行接管未知草稿。

Owner转发本固定启动词即要求接收并在门通过后执行90；不另等一次人工批准。通过后12成为唯一执行写者，只新增90约定六件及本人一次PR17正式交付评论，连续D00、G01–G04。不在接收报告或G00后正常停工，不以换接为由重做89或启动91。

## 8. 终态与禁止事项

全部固定/活动六件、内容或blob身份、compare、四refs、PR状态及正式评论内容/唯一性回读后STOP_WRITING；经PR17交回原“MYOS2-A-C06 内核分析主线续建”，提醒Owner发正式验收消息前选择Pro。12不自签R89-01/02闭合或90通过。完整本地P9未完成不能冒称机械PASS，静态验收上限仍PASS_PENDING_LOCAL。

禁止改旧成果/评论、PR元数据、主线、master/time、内核/loader/Owner脚本；禁止补丁、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。既有临时纯文本检查和有限整数模型许可保持，不能运行仓库实现冒充静态模型。

GitHub暂态异常先核是否已写成，再按63间隔安全重试，通常首次加三次；完整expected-head、force=false、复用成功对象。未知并行写入不覆盖，实际重试与工具发现分账。未来主线交接继续显式引用110与124。本包仅准备执行者换接，不把发布当作新对话已启动。
