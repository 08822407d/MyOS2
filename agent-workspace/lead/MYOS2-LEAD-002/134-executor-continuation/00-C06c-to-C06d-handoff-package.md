接收与执行模型：次一档（默认），沿用GPT-6＋Extra。新执行对话：MYOS2-A-C06d 代码分析执行；新身份：MYOS2-LEAD-002-CHAT-EXEC-13。接收和G00通过后直接执行第95批，不只接收后等待。

---
package_id: MYOS2-C06c-EXEC12-TO-C06d-EXEC13-BATCH95-HANDOFF-20261117
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_explicit_handoff_then_execute_authorized_MyOS2_batch95
prepared_by: MYOS2-A-C06主线（Owner指定的执行者换接）
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: 本轮未重新申报；事务性换接无需Pro
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 第95批技术范围已冻结；新会话独立接收和有界执行，正式独立验收另交C06 Pro
date: 2026-11-17
read_channel: connector
source_preservation_level: NORMALIZED_OWNER_REQUEST_WITH_FIXED_REPOSITORY_POINTERS
project_execution_source: Owner本次换接指令、MyOS2既有约束及133合同合读本目录02替换令
mnemosyne_execution_source: 08822407d/Mnemosyne/current/human-approved-spec.md（仅其适用范围，不授予MyOS2写权）
receiver_display_name: MYOS2-A-C06d 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-13
old_executor: MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12
old_writer_state: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
mainline_parent_short12: 48656d77e260
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_start_short12: 665aea0f7afd
source_time_short12: a039d9803ade
master_direct_short12: e8e60cd19540
mnemosyne_observed_master_short12: ee65de8a255d
original_contract: 133-user-console-registration-output-contract.md@48656d77e260
batch94: PASS_PENDING_LOCAL_WITH_R94_S01_S02_S03
batch95_remote_delivery_at_preflight: ABSENT
batch95_private_C06c_work: UNKNOWN_NOT_TRANSFERRED
new_receiver_state: NOT_STARTED_BY_MAINLINE
D00: NOT_REQUIRED
phase: 2
owner_decisions_requested_now: []
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - read_explicit_package_and_form_real_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_and_124_in_full
    - separately_load_applicable_Mnemosyne_guidance
    - complete_fresh_G00_then_continue_G01_G02_G03_G04
---

# C06c → C06d：第95批新执行对话接收包

## 1. 换接范围与真实状态

Owner明确报告C06c上下文耗尽，要求将95改为新对话执行。此包保存任务相关规范化信息和固定原件指针，不冒充旧聊天全文导出、私有草稿迁移或旧执行者另行签发的确认。

准备时直接核得lead=48656d77e260、exec=665aea0f7afd、master=e8e60cd19540、time=a039d9803ade；PR16/17均Open/unmerged，PR17仍Draft。95结果目录在该exec下404；95原发布评论6936558016之后的PR17评论查询为空。故未见远端95成果需要覆盖；旧C06c是否已阅读或起草未知，不写成其从未启动。新13尚未被本主线创建或启动。

旧C06c/12自Owner本次换接指令起不再具有95执行写权；旧C06b/11、C06a/10、C05c/09继续停用。无需返回已满聊天索取确认。C06主线没有换届，lead/PR16唯一写权和独立验收职责不移交给13。显示名沿用MyOS2既有C06派生序列，不占DR号、不修改外项目命名登记，也不声称已修改网页侧栏。

94交付固定665aea0f7afd、作者评论6935238551；94验收、133合同与检查点固定48656d77e260、主线评论6936558016。94为PASS_PENDING_LOCAL且须合读R94-S01/S02/S03，无新增RETURN；95无D00。90文档闭合、91–93限定及53文档误判撤回保持。本次不重验94、不修源码、不设计96。

本00是非执行源转移材料；写权来自Owner指令及MyOS2任务授权。Mnemosyne接收/指导流程不得把本任务替换成其自身维护、Alaya或其它路线。

## 2. 固定必读材料

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。134三件取Owner转发启动URL中的真实发布提交。以下既有MyOS2主线文件全部固定取48656d77e260，不随活动分支漂移：

- 本目录00、01-MYOS2-A-C06d-receiver-startup-prompt.md、02-batch95-executor-amendment-and-checkpoint.md全文；133-user-console-registration-output-contract.md全文。
- L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md全文。
- L/reviews/CORE-USER-PRINTK-FORMAT-BUFFER-94-review.md、L/checkpoints/2026-11-17-printk94-reviewed-console95-ready.md全文。检查点中的“继续12”是旧时点，已由本02替代。
- L/reviews/CORE-USER-PRINTF-STRING-POINTER-93-review.md、L/reviews/CORE-USER-PRINTF-INTEGER-FORMAT-92-review.md、L/reviews/CORE-USER-KTHREAD-NAME-STORAGE-91-review.md全文，不能仅读PASS摘要。
- L/reviews/CORE-USER-ID-ALLOCATION-LIFECYCLE-90-DOC-CLOSURE-review.md、L/reviews/CORE-USER-COMPLETION-OWNERSHIP-53-DOC-CLOSURE-review.md全文；保留R90C-S01和53撤回的旧误判，不把文档关闭当源码候选关闭。
- PR17评论6935238551（94作者交付）、6936558016（94验收与原95发布）、本次134正式换接评论（真实编号由发布后Owner消息和PR导航给出）。

94原六件在665aea0f7afd的R/user-printk-format-buffer-94/独立完整读取：MANIFEST.md、printk-format-buffer.md、facts-and-boundaries.yaml、printk-format-buffer-paths.dot、deferred-findings.yaml、evidence.md。结合94回执恢复边界，不重复执行94，也不把其作者申报当新13亲历。

91–93原件仅对实际依赖的qualified_id、函数与证据定点读取：91为474a2c0363f0的R/user-kthread-name-storage-91/；92为dbd0ee1684d4的R/user-printf-integer-format-92/；93为2fe1ce176f4d的R/user-printf-string-pointer-93/。其它历史原件仅在真实依赖触发时补读，不全量重读90及更早批次。128和119是历史换接模板，不是本次接收入口，不要求13执行旧90/84任务。

## 3. 接收报告先形成，不提前声称加载完成

G00前只读GitHub。先读本包、01/02、133、94回执和检查点，核现场；按Mnemosyne@ee65de8a255d的README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md及notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md适用导航执行接收程序。接收用固定对象，并核实际master是否变化；变化如实登记，不自动变成MyOS2合同变更。

形成真实mnemosyne_handoff_receive报告，记录package_id、receiver_guidance_load、MyOS2与Mnemosyne各自执行源、声明/实际可见模型档位、re_tier_on_arrival、已核路径、缺失或未读材料、当前95任务、禁止动作、安全下一步和未知。后台未知如实写unknown或Owner-reported，不为此重复索取模型确认，不自动换模型或调用子代理。

九规则、110/124或指导若尚未读完，此时必须写pending。接收报告不等于指导刷新、G00通过或取得写权，不可在最终汇总倒造早期报告。可以先保存真实临时记录；不能落盘则如实说明，不虚构路径。形成报告后在同一响应继续下一步，不以RECEIVED_READY作为本次正常终点。

## 4. 独立完整加载九份MyOS2规则，再读两份续记

九份均固定MyOS2@48656d77e260；逐件读至EOF，长响应分段补齐：

1. L/00-work-order.md，包含§12全部Owner续记。
2. agent-workspace/README.md。
3. agent-workspace/conventions.md。
4. agent-workspace/tasks/00-gpt-task-protocol-v2.md，全部P1–P13。
5. L/18-deferred-findings-and-resume.md。
6. L/24-chatgpt-specialist-conversation-separation.md。
7. L/28-review-and-next-batch-same-turn.md。
8. L/63-github-write-retry-and-readback.md。
9. L/79-work-package-sizing-and-pro-review.md。

然后独立全文读取110、124。不得继承C06c、C06主线或任何前任的已读资格；13在本新会话自己实际完整读取过且固定对象一致的材料可如实复用，必须区分新读/本人复用/未读。历史00最初建立主线分支和停等开工的启动仪式不重新执行；本13的角色、启动和写区由本次Owner换接、133及02具体规定，不新建分支PR、不接管主线。

## 5. 随后单独刷新本任务适用的Mnemosyne指导

按load命令核实际master，固定实际采用的版本并记录；准备时观察值为ee65de8a255d。完整读取核心README、human-approved-spec、current/user-operation-next-step-capability-and-intent-guard.md和load命令；读取current/guard-registry.yaml导航头及匹配项。

按本次触发完整读取current/下的model-tier-and-subtask-cost-routing-guard.md、github-single-active-pr-lineage-guard.md、run-context-and-pr-provenance-guard.md、artifact-delivery-and-direct-generation-guard.md、cross-conversation-execution-intent-and-operator-flow-guard.md、source-artifact-preservation-and-design-rationale-guard.md、external-research-display-name-guard.md、next-step-repository-write-visibility-guard.md；命名参阅notes/registries/project-research-display-name-registry-v0.1.md中§7适用范围。实际loader另有适用触发则补齐并记录，不假称此清单永久穷尽全部规则。

独立形成mnemosyne_guidance_refresh记录，明确current_conversation_task_preserved=true、maintenance_live_route_imported=false及实际固定版本、真实加载范围。指导记录按load要求保存到实际临时文件或任务记录，聊天只给简短确认，不抄大段读取日志。不得把current/active-context.md、handoff/handoff-current.md、todo/open-questions等变成本95行动计划，不借通用Ready-PR或模型路由修改PR17 Draft、启动额外模型或扩大运行权。

接收报告、九规则与110/124读账、指导刷新记录在G00前仅临时保存；门通过后并入95六件的MANIFEST/facts/evidence，不另交第七件、不写Mnemosyne仓库。此task-local yes不构成其它项目的普遍政策。

## 6. G00后接续原技术任务

Owner转发本固定启动词即要求RUN_AFTER_GATE_G00。按02与133重新核PR16/17、开放PR、四direct refs、95目录与全部95正式marker（兼查旧12、新13及任何作者）；核公约原句和time/master金丝雀，配置判断前全文读固定time的options_flags.cmake。lead预期为134真实发布头，exec仍665aea0f7afd、master e8e60cd19540、time a039d9803ade。缓存base_sha不能代替direct master。

遇已有同批文件、正式评论或未知HEAD变化，先核归属和净差异，停止冲突写入、不覆盖、不自动变基。确为13自己已成功的操作才转补缺项/回读；旧12晚到动作不自动归为13成果，不自动重做或重复发布。

全部门通过后，13成为95唯一执行写者，连续G01–G04，不逐专题等批准，不在G00后再等待Owner或旧C06c。无D00；按133完成控制台注册/节点发布、内置write实际字符消费、启动视频准备与绘字对象范围三个专题。保留R94及R91–93限定、SP93-F01、KN91-U01、PF92-U06和PK94相关未证门；新旧根因沿完整qualified_id去重。

## 7. 交付与停止

沿用claude/dazzling-cori-q0dnyt及PR17。仅新增R/user-console-registration-output-95/的MANIFEST.md、console-registration-output.md、facts-and-boundaries.yaml、console-registration-output-paths.dot、deferred-findings.yaml、evidence.md；另加13本人一次PR17顶层正式交付评论。

新marker：MYOS2-LEAD-002-CHAT-EXEC-13 / CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95 / FORMAL_DELIVERY。

133全部技术质量门不变：十四字段、真实self_check三整数、全六件原始标签计数、1–5行真实单定义短锚、实际符号与语义支持、事实/模型/图索引、对象/类型/调用关系前提均核齐。单宏单锚，已知错误发布前纠正；完整P9和仓库构建运行未做就写NOT_RUN，不把数学模型当执行测试。

写前再查四头、目标及marker；单父提交、完整expected_sha、force=false。暂态按63先核远端效果，再间隔安全重试，通常首次加三次并遵Retry-After；复用成功blob/tree/commit/评论，未知并行推进不覆盖。发布后compare严格六件added、旧件零净改删，固定全文、活动内容或blob身份、PR状态、评论编号/全文/唯一性和四direct refs全部回读。

全部完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，经PR17交原MYOS2-A-C06；Owner发送正式验收消息前选Pro，由主线同轮验收95及安排后继。13不自签95通过、不启动96、不交旧C06c。

禁止修改旧成果/旧评论/PR元数据/lead/master/time/内核/loader/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具/Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。允许的独立纯文本与有限数学检查不执行仓库实现。以后主线交接继续显式引用110和124。
