接收模型：次一档（默认GPT-6＋Extra）。执行位置：新普通ChatGPT对话“MYOS2-A-C06b 代码分析执行”；新身份MYOS2-LEAD-002-CHAT-EXEC-11。

---
package_id: MYOS2-C06a-EXEC10-TO-C06b-EXEC11-HANDOFF-20261113
package_status: non_execution_source_transfer_artifact
intended_receiver_action: receive_Mnemosyne_handoff_then_execute_authorized_MyOS2_contract84
prepared_by: MYOS2-A-C06主线（主线介导执行者轮换，不是旧执行者自签）
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 主线已冻结有界取证范围和权限，次一档执行；裁定另交Pro
date: 2026-11-13
read_channel: connector
receiver_display_name: MYOS2-A-C06b 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-11
old_display_name: MYOS2-A-C06a 代码分析执行
old_identity: MYOS2-LEAD-002-CHAT-EXEC-10
old_writer_state: STOP_WRITING_OWNER_RETIRED_CONTEXT
receiver_state_at_publication: NOT_CREATED_OR_STARTED_BY_MAINLINE
project_execution_source: Owner当前指令、MyOS2的00工作令及所引约束、120合同条件授权
mnemosyne_execution_source: 08822407d/Mnemosyne/current/human-approved-spec.md（仅Mnemosyne自身范围）
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - receive_explicit_package_and_form_real_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_owner_supplement_in_full
    - separately_load_current_applicable_Mnemosyne_guidance
    - pass_contract84_G00_then_continue_G01_G02_G03_G04
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
mainline_before_publication_short12: b129d706e350
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_start_short12: 18047d54b0a2
execution83_comment: 6847954250
master_direct_short12: e8e60cd19540
source_time_short12: a039d9803ade
mnemosyne_observed_master_short12: 8b58dd7b07c1
next_contract: ../120-user-early-root-handoff-contract.md
source_preservation_level: EXCERPT_OR_SUMMARY_WITH_FIXED_REPOSITORY_ORIGINAL_POINTERS
unknowns: [旧C06a未发布的私有上下文, 后台真实模型档次及上下文余量, 本机或硬件运行状态]
owner_decisions_requested_now: []
---

# 执行者10 → 11：本C06主线介导的新对话交接

本包传递MyOS2的工作状态和证据入口，不是Mnemosyne维护任务，也不是独立执行源。权利来源是Owner本轮明确要求83验收、后续安排并在新对话开展，以及主线按00/18/24/28/63/79/110签发的120合同；不是因为读到本包或Mnemosyne就自动获得任意写权。

## 1. 轮换依据与当前状态

Owner明确83已交付18047d54b0a2、评论6847954250，同时表示C06a上下文过大，下一批在新开对话中开展。83正式交付声明在完成回读后STOP_WRITING；主线已独立核交付和六件净新增，并签发83-review。

83为PASS_PENDING_LOCAL，必须合读R83-S01/S02/S03，没有新增RETURN或D00。主线补充互斥分支和真实写包装、present/反转前提、同层索引以及端点/计数的消费纠正，原83六件和原评论冻结。81、82既有补充继续，79/80及更早文档关闭或历史撤回不重开；任何静态接收不代表运行通过。

84是启动页表来源、启动修正/临时恒等映射、C入口清零/根复制/后续交接三个专题；尚未有84执行交付或验收。范围不是重做83或整个系统启动验证。

旧C06a/10在本次轮换后不再获得84写权，旧C05c/09继续停用。新11不得沿用10的身份、临时文件或已读资格。无需让Owner回旧C06a发RECEIVED_READY/CONFIRMED或索取新确认：停写依据是交付和Owner续接要求，本包没有冒称旧方另签确认。

本C06主线没有移交，仍独占lead/PR16并独立验收。11仅在接收、加载及G00全部通过后取得84指定结果目录和本人评论的有限写权；不并写其他目录，不修改PR元数据。新对话名称为项目既有C系派生名，canonical身份为CHAT-EXEC-11；不借DR号，不冒称已替Owner创建或改名网页侧栏。

## 2. 固定可追溯材料

L=agent-workspace/lead/MYOS2-LEAD-002/，工作材料全部取Owner转发本119启动URL中的同一实际发布提交，而不是活动分支或父b129d706e350。

必须全文读取：本00、同目录01、L/120-user-early-root-handoff-contract.md、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-review.md、L/checkpoints/2026-11-13-table-install83-reviewed-early-root84-exec11-ready.md，以及81和82的正式review全文。

83原件固定于18047d54b0a2，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/。六件分别为MANIFEST.md、direct-map-table-install.md、facts-and-boundaries.yaml、direct-map-table-install-paths.dot、deferred-findings.yaml、evidence.md；必须独立完整读取，并读PR17评论6847954250。报告原件与主线消费纠正分开，119摘要不能替代原证据。

主线观察的83净差异为3f41eb242040→18047d54b0a2，严格六件added5437行、旧件零净改删。读到这个事实仍须新接收者核现场，不继承前任已经核对的资格。其他历史原件按120实际依赖定点读取；未知内容标未读，不把历史目录全部加载以填满上下文。

## 3. 先接收并形成真实报告

接收前不写GitHub。先读本包/01/120/83回执及检查点等接收材料，核实际PR和分支；为理解接收程序，读取08822407d/Mnemosyne固定观察版本8b58dd7b07c1的README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md。按执行源入口读取notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md的适用导航，不导入其中未授权路线。记录Mnemosyne实际master；若已改变，记录差异并在稍后的指导刷新选择实际适用版本。

形成真实mnemosyne_handoff_receive报告，至少记录package_present/id/status、receiver_guidance_load、两个项目各自execution_source、declared/actual模型档及来源、re_tier_on_arrival、实际已核路径与缺失路径、当前84任务、禁止动作、safe_next_action和未知。此时九规则或指导尚未读完，必须写pending，不预先写完成。后台档次不能证明时明确unknown/Owner-reported；不能偷偷升级、调用额外代理或付费API。

本步报告先于后面的规则加载。它证明接收状态，不是规则已刷新，也不是已经获得84写权。临时文件确实可创建才引用其路径；无法落盘则如实保留实际报告文字及限制。不得用猜测的sandbox链接代替记录。

## 4. 再独立全文加载九份MyOS2约束和110

全部取本119启动URL同一固定MyOS2提交，逐件读到末尾，截断分段补齐：
1. L/00-work-order.md，含§12全部续记。
2. agent-workspace/README.md。
3. agent-workspace/conventions.md。
4. agent-workspace/tasks/00-gpt-task-protocol-v2.md，P1–P13全部。
5. L/18-deferred-findings-and-resume.md。
6. L/24-chatgpt-specialist-conversation-separation.md。
7. L/28-review-and-next-batch-same-turn.md。
8. L/63-github-write-retry-and-readback.md。
9. L/79-work-package-sizing-and-pro-review.md。

然后单独全文L/110-owner-communication-and-delivery-continuity.md。它不替代九规则；以后每次主线交接的交接包、接收启动词和必读清单仍须显式引用它的真实固定版本。新11不能继承C06a、C06或其它对话任何已读声明。

当前Owner无需技术审查、编译或逐项决定源码修复。普通源码问题按18分类留库；AI报告错误追加纠正，证据不足及时收窄。只在真实访问/授权/未知并行冲突阻断必要动作时提出最小人工操作，不把普通代码缺口当作整体停工理由。

## 5. 随后单独加载适用Mnemosyne指导

按commands/load-mnemosyne-guidance.md执行独立的行为刷新，不把接收报告等同刷新。先核实际master并记录短提交；完整读核心README、human-approved-spec、user-operation-next-step-capability-and-intent-guard和load命令；读取guard-registry.yaml导航头及实际触发项。

本任务涉及接收模型、仓库写入谱系、重要发布来源、跨对话交付、文件与设计来源、普通对话命名及下一步写入，须完整读取相应指导：model-tier-and-subtask-cost-routing-guard、github-single-active-pr-lineage-guard、run-context-and-pr-provenance-guard、artifact-delivery-and-direct-generation-guard、cross-conversation-execution-intent-and-operator-flow-guard、source-artifact-preservation-and-design-rationale-guard、external-research-display-name-guard、next-step-repository-write-visibility-guard，均为current/下.md文件；命名另读notes/registries/project-research-display-name-registry-v0.1.md的§7适用范围。当前loader触发其他指导时按实际范围补齐；该清单不是永久免读例外。

真实mnemosyne_guidance_refresh记录应列实际读取范围、current_conversation_task_preserved=true、maintenance_live_route_imported=false。只读接收阶段存临时记录；G00获84写权后，将接收报告、九规则/110加载账及刷新记录原有内容并入84的MANIFEST/evidence或facts，不额外交第七件，不倒造旧C06a历史日志。

通用Ready-PR、模型路由、维护整编或当前任务路线提示不能覆盖MyOS2的PR17 Draft保留、禁止额外模型/API、仅新84写区及不运行内核等具体边界。Mnemosyne的task-local yes不是通用目标项目政策，也没有任何Mnemosyne/Alaya/其它仓库写入授权。

## 6. G00后连续执行及终态

完整加载后，按120核direct lead/exec/master/time、PR16/17、开放PR、84目标目录/11 marker、公约原句和分支金丝雀。exec预期18047d54b0a2；master预期e8e60cd19540，源码time a039d9803ade；lead为本119/120真实发布提交。未知变化先查归属，不覆盖、不自动变基、不重复交付。

Owner将固定01启动词发到新C06b，即要求接收并在全部门通过后执行84，不另设一次人工批准回合。G00后11仅可新增user-early-root-handoff-84/严格六件及本人一次PR17正式交付评论，连续G01–G04；无D00。主线不代写执行成果，旧10/09不恢复写入。

按63号先核远端实际效果，再间隔安全重试；完整expected-head、force=false、单父提交，复用成功对象。新固定/活动六件全文和blob、compare、四refs、PR与唯一评论全部回读后STOP_WRITING，经PR17交回现MYOS2-A-C06 Pro独立验收。不自签84，不启动85，不向旧C06a/C05c交回。

禁止内核/loader/initramfs/脚本修改、补丁、预处理/汇编/编译/链接、仓库程序/旧夹具/Graphviz/QEMU/实机或硬件实验、外部libc镜像、本机凭据、额外模型/子代理/API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。仅按既有许可新建临时纯文本检查/有限整数模型，并标明非内核运行。

本包仅保留授权GitHub原件指针、当前Owner要求的规范化摘要和主线裁定，不声称完整旧对话或隐藏上下文无损迁移。缺失旧scratch不要求Owner整理导出；84按固定材料独立取证。真正缺少必读文件时不得伪造接收成功，先保全独立可完成部分并明确最小阻断。
