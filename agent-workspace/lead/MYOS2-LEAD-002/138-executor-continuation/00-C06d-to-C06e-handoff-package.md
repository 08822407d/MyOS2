**范围恢复声明（执行优先级最高）**：原主线36f7c342dc88已发布的136/00、136/01与137-user-boot-info-transfer-contract.md属于一次非保真恢复，错误地将第97批改为MBI复制与BSS交接。该版97执行范围已由本138三件和139原定合同追加取代；原第95、96批审核裁定、证据与执行提交不变。本文件仅恢复上一轮Pro已准备的第97批技术范围，未重新进行Pro审核。实际发布提交由Owner提供的固定GitHub链接决定，不用旧头拼接。**

接收与执行模型：次一档（默认），沿用GPT-6＋Extra。新对话MYOS2-A-C06e 代码分析执行；新身份MYOS2-LEAD-002-CHAT-EXEC-14。正式GitHub发布和G00通过后连续执行97，不只接收后等待。

---
package_id: MYOS2-C06d-EXEC13-TO-C06e-EXEC14-BATCH97-HANDOFF-20261117
package_status: non_execution_source_transfer_artifact
publication_status_at_composition: PREPARED_NOT_PUBLISHED
intended_receiver_action: receive_explicit_handoff_then_execute_authorized_batch97_after_publication_and_G00
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 有界静态工作包已由主线设计，正式独立验收另交C06 Pro
prepared_by: MYOS2-A-C06主线
produced_by: GPT-6 Astra Pro（模型自报；后台UNATTESTED）
date: 2026-11-17
read_channel: connector
source_preservation_level: NORMALIZED_OWNER_REQUEST_WITH_FIXED_REPOSITORY_POINTERS
project_execution_source: Owner本轮执行者轮换要求、MyOS2既有约束、正式发布139和本目录02
mnemosyne_execution_source: 08822407d/Mnemosyne/current/human-approved-spec.md（仅适用行为，不授MyOS2写权）
old_executor: MYOS2-A-C06d / MYOS2-LEAD-002-CHAT-EXEC-13
new_executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
old_writer_state: STOP_WRITING_OWNER_RETIRED_FOR_NEXT_BATCH
mainline_owner: MYOS2-A-C06
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_start_short12: 6a1fa78d0356
lead_parent_short12: fda0f3d1cb60
source_time_short12: a039d9803ade
master_direct_short12: e8e60cd19540
mnemosyne_observed_master_short12: ee65de8a255d
new_receiver_started: false
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - verify_actual_GitHub_release_and_read_handoff_then_form_receive_report
    - independently_read_nine_MyOS2_rules_in_full
    - independently_read_110_and_124_in_full
    - separately_load_applicable_Mnemosyne_guidance
    - complete_fresh_G00_then_execute_D00_G01_G02_G03_G04_continuously
owner_decisions_requested_now: []
---

# 138：新C06e接收第97批

## 1. Owner意图、门与角色

Owner已交回96并明确下一批在新开对话执行，原因是C06d性能下降。旧13依据Owner本轮安排退出后续执行写者位置，无需它再签确认；这不是主线观察其后台退出，也不假称旧私有草稿已经迁移。96作者交付READY、STOP_WRITING，固定提交6a1fa78d0356、评论6941872312。

本C06独立验收已作出95两项关闭及96两项文档RETURN决定，正式结论须读本轮新回执，不只读本包摘要。96未受影响主体按R96-S01/S02/S03保留；97先补R96-01/02再做三个新专题。旧95原件不改，原96原110/110/0不改，未证源码问题不因文档闭合而修复。

本包只是非执行源转移材料，主线C06未换届，lead/PR16唯一写权不转给14。旧13、12、11、10、09不得并写97。新名沿MyOS2既有C06派生序列，不占DR编号，不更新外项目注册表，也不声称更改界面侧栏。

**发布门**：当前准备包没有真实发布提交或新正式评论时DO_NOT_RUN；不以附件或此处父提交伪装正式启动。正式发布后，Owner向新C06e转发实际固定138/01链接即要求RUN_AFTER_GATE_G00，不需第二次批准。

## 2. 固定必读材料

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。

以下七件和110/124、九规则，统一取Owner138/01固定URL的真实发布提交：
- 本目录00-C06d-to-C06e-handoff-package.md、01-MYOS2-A-C06e-receiver-startup-prompt.md、02-batch97-executor-authority-and-checkpoint.md。
- L/139-user-bootinfo-tag-build-contract.md。
- L/reviews/CORE-USER-FRAMEBUFFER-MAPPING-96-review.md。
- L/reviews/CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95-DOC-CLOSURE-review.md。
- L/checkpoints/2026-11-17-framebuffer96-reviewed-bootinfo97-new-executor.md。
- L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md。

独立全文读取96原六件，固定6a1fa78d0356的R/user-framebuffer-mapping-96/：MANIFEST.md、framebuffer-mapping.md、facts-and-boundaries.yaml、framebuffer-mapping-paths.dot、deferred-findings.yaml、evidence.md。实际body读到EOF，评论不是报告替代品；长响应分段补齐。

读取96正式评论6941872312、原96发布评论6938385614、本次97及新执行者正式发布评论（实际编号由C06正式发布后给出）。旧评论的旧人员/状态只作历史，不恢复13写权。

全文读取80-review（同发布提交的L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md）；80原MANIFEST和deferred固定0e9e9e89c4be，facts/evidence按139实际依赖定点读取。其它历史限界按139触发读取；不把91–96全套与每次交接全部历史无差别灌入新上下文。旧134、135、128、119不作新入口，也不要求14执行旧95/96/90/84。

## 3. 接收报告先真实形成

核发布门和已提供的00/01/02、139、96回执、95闭合及检查点。接收程序读取Mnemosyne@ee65de8a255d的README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md和notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md适用导航；核实际master有无变化。固定程序与实际新鲜度分别记录，变化不自动改MyOS2任务授权。

形成真实mnemosyne_handoff_receive，包含package_id、声明/可见模型档位与re_tier_on_arrival、项目与Mnemosyne各自执行源、已核路径、未读/缺失/冲突、97任务、安全下一步、禁止事项及未知。九规则、续记或指导未读完时必须pending，不提前写已加载，不到最终阶段倒造“早期报告”。后台未知写UNATTESTED，不反复问Owner确认模型，不启用子代理。

报告只建立接收状态，不等于指导刷新、G00或写权。暂时记录存实际临时文件，不能落盘如实说明。随后同轮继续，不以RECEIVED_READY作为正常终点。

## 4. 独立完整读取九规则与续记

九份都取138/139实际正式发布提交，逐件至EOF：
1. L/00-work-order.md，包含§12全部Owner续记。
2. agent-workspace/README.md。
3. agent-workspace/conventions.md。
4. agent-workspace/tasks/00-gpt-task-protocol-v2.md，全部P1–P13。
5. L/18-deferred-findings-and-resume.md。
6. L/24-chatgpt-specialist-conversation-separation.md。
7. L/28-review-and-next-batch-same-turn.md。
8. L/63-github-write-retry-and-readback.md。
9. L/79-work-package-sizing-and-pro-review.md。

之后独立全文读取110和124。不能继承C06d、C06主线或任何旧会话的已读声明；本新14自己已实际完整读过且Git对象一致的可以明确复用，记录新读/本人复用/未读。旧00建分支/等待开工仪式不重演，角色/写区由本次已发布139具体化，不创建分支PR。

## 5. 随后单独加载适用Mnemosyne指导

按load命令核实际master、固定采用对象；准备时观察ee65de8a255d。完整核心README、人类spec、user-operation-next-step-capability-and-intent-guard、load命令；读取guard-registry导航及匹配项。

按触发完整加载current/下model-tier-and-subtask-cost-routing-guard.md、github-single-active-pr-lineage-guard.md、run-context-and-pr-provenance-guard.md、artifact-delivery-and-direct-generation-guard.md、cross-conversation-execution-intent-and-operator-flow-guard.md、source-artifact-preservation-and-design-rationale-guard.md、external-research-display-name-guard.md、next-step-repository-write-visibility-guard.md；命名只参notes/registries/project-research-display-name-registry-v0.1.md适用§7。loader有其它触发则据实补齐，清单不是永久穷尽声明。

独立记录mnemosyne_guidance_refresh，明确current_conversation_task_preserved=true、maintenance_live_route_imported=false、真实版本/已读guard。按命令保存实际临时或任务记录，聊天简短确认；不能把Mne active-context/handoff-current/todo变成本97计划，不写Mne，不改变PR17 Draft或调用额外模型。此yes仅MyOS2本次局部适用，不推广其它项目。

接收报告、规则读账、刷新记录在G00前仅临时保存，门后并入97六件，不增加第七件，不伪造过往日志。

## 6. G00、执行与交回

按139和本02查四direct refs、PR16/17、开放PR、目标六件和所有97 marker、配置和canary；主线预期新发布头、exec6a1fa78d0356、mastere8e60cd19540、timea039d9803ade。未知推进/旧13晚到写入先核归属，停冲突写、不覆盖、不自动变基。相同14已成功对象只补缺项回读。

全部门通过后连续D00、G01–G04，不再向Owner或旧C06d索取重复批准。D00补R96-01/02；三个新专题按139完整执行。仅原执行分支97目录严格六件及14本人一次PR17顶层正式评论，marker为MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOTINFO-TAG-BUILD-97 / FORMAL_DELIVERY。

63号先核远端实际效果再安全重试；完整expected_sha、force=false、单父提交。发布compare仅六件added，固定全文/活动身份、PR、四refs、评论实际编号/全文/唯一性全部回读。完成后READY_FOR_MAINLINE_REVIEW、R96补正待C06复核、STOP_WRITING；交原C06，Owner正式验收消息前选Pro。不自签96闭合/97通过、不启动98。

不改旧成果/评论/PR元数据/主线/master/time/内核/loader/Owner脚本；不出补丁、不构建或运行仓库程序/脚本/Graphviz/QEMU/实机/真实并发、不读取本机凭据、不启用额外模型/子代理/API、不新建分支PR、不合并强推删除、不导入跨项目维护。普通问题先记录，不要求Owner逐项选择。110与124继续显式进入未来主线交接。
