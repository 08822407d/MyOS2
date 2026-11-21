接收与执行模型：次一档（默认），沿用GPT-6＋Extra。新对话MYOS2-A-C06e 代码分析执行；新身份MYOS2-LEAD-002-CHAT-EXEC-14。接收和G00通过后直接执行97，不只接收后等待。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-INFO-TRANSFER-97
package_id: MYOS2-C06d-EXEC13-TO-C06e-EXEC14-BATCH97-HANDOFF-20261117
record_type: fresh_executor_handoff_package
package_status: non_execution_source_transfer_artifact
evidence_class: Owner明确换接指令及固定仓库材料；不是旧聊天导出
intended_receiver_action: receive_then_execute_authorized_batch97_after_gates
prepared_by: MYOS2-A-C06主线
produced_by: GPT-6 Astra Pro（模型自报；后台未独立认证）
model_per_owner: Pro（本轮独立验收与设计）
receiving_model_tier: next_tier_default
receiving_model_tier_reason: 冻结范围的静态取证与追加文档纠正，另由C06 Pro独立验收
date: 2026-11-17
base_snapshot: time
read_channel: connector
inputs_read: [96固定六件与评论, 本轮审查与95闭合, 137合同, Owner换接请求, 适用Mnemosyne交接程序]
source_preservation_level: NORMALIZED_TASK_STATE_WITH_FIXED_REPOSITORY_ORIGINAL_POINTERS
project_execution_source: Owner本轮继续与换接指令、MyOS2既有约束和137合同
mnemosyne_execution_source: 08822407d/Mnemosyne/current/human-approved-spec.md（仅适用行为范围）
receiver_display_name: MYOS2-A-C06e 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-14
old_executor: MYOS2-A-C06d / MYOS2-LEAD-002-CHAT-EXEC-13
old_writer_state: STOP_WRITING_AND_OWNER_RETIRED_FOR_NEXT_BATCH
mainline_owner: MYOS2-A-C06
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
mainline_parent_short12: fda0f3d1cb60
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
execution_start_short12: 6a1fa78d0356
source_time_short12: a039d9803ade
master_direct_short12: e8e60cd19540
mnemosyne_observed_master_short12: ee65de8a255d
batch95_composite: PASS_PENDING_LOCAL
batch96: RETURN_ONLY_R96_01_R96_02
batch97: NOT_STARTED
status: READY_FOR_OWNER_FORWARDING
new_receiver_started_by_mainline: false
D00: REQUIRED_APPEND_R96_01_R96_02
phase: 2
owner_decisions_requested_now: []
open_questions: []
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - read_handoff_and_form_genuine_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_and_124_in_full
    - separately_load_applicable_Mnemosyne_guidance
    - pass_fresh_G00_then_continuously_execute_D00_G01_G02_G03_G04
---

# C06d → C06e：第97批新执行对话交接包

## 1. 权限、状态和未转移内容

Owner在96交回消息中明确要求：“你注意下一批任务我将要在新开对话中执行，因为当前执行任务的C06d对话出现了性能下降的现象，所以在设计任务时你得考虑这种情况”。这是任务相关原话摘录，不冒充整个聊天的逐字节导出。

96正式交付固定6a1fa78d0356，评论6941872312，严格六件新增；13已按Owner和交付声明STOP_WRITING。C06独立关闭95的R95-01/02；96仅R96-01/02定点文档退回，其余按R96-S01/S02/S03保留。97包含D00和三个引导信息专题，不重做95/96整包，不重开旧已闭合文档。

本次仅轮换执行者。C06继续保有lead/PR16唯一写权与验收职责；13不取得97写权，旧12/11/10/09继续停用。无需旧C06d新确认。没有迁移私有草稿、导出旧聊天或监控另一聊天后台；转移的是固定仓库成果和显式授权。

本00不是执行源；137承接Owner既有任务发布授权。134是旧13接收95的历史入口，135是已交付96的历史合同，不作为97入口。新14不得继承任何前任或主线的已读资格；原主线身份不转给14。

## 2. 固定材料和版本分流

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。本136两件、137、本轮96-review、95-DOC-CLOSURE与检查点，统一取Owner转发136/01链接中的真实发布提交，不能去父fda0f3d1cb60查新文件。本次正式发布marker为MYOS2-LEAD-002 / CORE-USER-FRAMEBUFFER-MAPPING-96 / REVIEW_AND_97_PUBLISHED，评论实际编号由Owner启动词或PR导航给出。

完整读取本00、同目录01-MYOS2-A-C06e-receiver-startup-prompt.md及下列文件：
- L/137-user-boot-info-transfer-contract.md。
- L/reviews/CORE-USER-FRAMEBUFFER-MAPPING-96-review.md。
- L/reviews/CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95-DOC-CLOSURE-review.md。
- L/checkpoints/2026-11-17-framebuffer96-reviewed-bootinfo97-new-executor-ready.md。
- L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md，按下述加载顺序全文读取。
- 96作者评论6941872312及本次97正式发布评论；6938385614是旧96发布，不误作97发令。

96原六件取6a1fa78d0356:R/user-framebuffer-mapping-96/：MANIFEST.md、framebuffer-mapping.md、facts-and-boundaries.yaml、framebuffer-mapping-paths.dot、deferred-findings.yaml、evidence.md。新14独立全文读至EOF。95-review固定fda0f3d1cb60，与本轮闭合件合读；95原件只在确需追溯时定点取be83c6095eb1，不默认全量重读。

80、84及84闭合回执和必要原件严格按137§2清单读取。其余历史按具体ID、函数或争议定点补证，不灌入全部91–95原报告、所有旧交接或其它项目任务；不能把未读的具体历史断言当新事实。

## 3. 先形成真实接收报告

准备时Mnemosyne直接master仍ee65de8a255d。先按此固定对象读README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md及notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md适用导航；结合本包和137核现场。阅读load命令本身不等于完成指导刷新。

形成真实mnemosyne_handoff_receive报告：package_id、非执行源性质、MyOS2/Mnemosyne各自执行源、声明模型及可见/Owner报告档位、后台UNATTESTED、re_tier_on_arrival、已核路径、缺失/未读项、97任务、禁令、安全下一步和未知。九规则、110/124或指导尚未读完时写pending，不预先宣布加载或G00完成，也不在最终汇总倒造早期记录。

模型沿Owner指定次一档，不反复索取后台确认，不自动切模型或调用子代理/API。接收报告先保全在真实临时空间；不能落盘就如实说明，不虚构路径。形成后同轮继续，不以RECEIVED_READY作为正常终点。

## 4. 再独立全文九规则和110/124

九件固定MyOS2@fda0f3d1cb60，逐件至EOF，截断补齐：
1. L/00-work-order.md，含§12全部Owner续记。
2. agent-workspace/README.md。
3. agent-workspace/conventions.md。
4. agent-workspace/tasks/00-gpt-task-protocol-v2.md，全部P1–P13。
5. L/18-deferred-findings-and-resume.md。
6. L/24-chatgpt-specialist-conversation-separation.md。
7. L/28-review-and-next-batch-same-turn.md。
8. L/63-github-write-retry-and-readback.md。
9. L/79-work-package-sizing-and-pro-review.md。

随后独立全文读取同一固定提交下110和124。旧00最初建分支、等开工的仪式不重做；本14角色和门由当前Owner及136/137具体控制。只承认14在本新会话自己真正完整读取过的相同对象，逐项区分新读/本人复用/未读。

## 5. 随后单独刷新适用Mnemosyne指导

刷新时再核实际master，固定实际采用版本并记录；版本变化不自动变成MyOS2合同变更。按load命令全文读核心README、human-approved-spec、current/user-operation-next-step-capability-and-intent-guard.md及load命令，读取current/guard-registry.yaml导航头和实际匹配项。

本次涉及模型分档、同PR谱系、写入来源、跨对话操作、保全及下一步写入提示，按触发完整加载current/下：model-tier-and-subtask-cost-routing-guard.md、github-single-active-pr-lineage-guard.md、run-context-and-pr-provenance-guard.md、artifact-delivery-and-direct-generation-guard.md、cross-conversation-execution-intent-and-operator-flow-guard.md、source-artifact-preservation-and-design-rationale-guard.md、external-research-display-name-guard.md、next-step-repository-write-visibility-guard.md；命名只消费既有MyOS2 C06派生序列适用规则，不创建DR登记。实际loader另有触发则补齐，不把清单当永久穷尽。

独立形成mnemosyne_guidance_refresh，保留97任务、maintenance_live_route_imported=false，记录真实版本和实际读取集合。按load要求保存实际临时/任务记录，聊天只简短确认。不得把active-context、handoff-current、TODO变成97计划；通用Ready-PR或维护/模型规则不能扩大MyOS2写区、改PR17 Draft、运行项目或调用额外模型。

接收、规则、指导记录在G00前仅临时保全；门后并入97六件的MANIFEST/facts/evidence，不写Mnemosyne、不另交第七件。此task-local yes不是其它项目的普遍政策。

## 6. 新G00、限定执行与查重

按137核PR16/17及开放PR、四direct refs、97目录/所有作者正式marker、公约、双canary与固定time配置。lead预期为本次新发布头；exec=6a1fa78d0356、master=e8e60cd19540、time=a039d9803ade。PR缓存base_sha不是direct master。

已有同批文件、旧执行者晚到动作或未知HEAD推进，先核归属，停止冲突写，不覆盖、不自动变基、不因换身份重交。仅14本人已成功动作才补缺项/回读。接收、九规则、110/124、指导和G00全部通过前只读；门后连续D00、G01–G04，不再次等Owner或旧C06d。

14仅新增R/user-boot-info-transfer-97/严格六件及本人一次PR17顶层正式评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOT-INFO-TRANSFER-97 / FORMAL_DELIVERY。原执行分支与PR17不变；C06不并写results，退休执行者停用。

137全部质量门不变。单父提交、完整expected_sha、force=false；暂态按63先核实际效果再间隔重试，复用成功对象。compare六件added、旧件零净改删；固定全文、活动内容/Blob身份、四refs、PR和唯一评论全部回读后才报正式交付。

## 7. 终态、禁止事项及Owner操作

完成后READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R96纠正待C06复核，随后STOP_WRITING，经PR17交原C06；Owner正式验收消息前选Pro。不得自签96闭合/97通过，不启动98，不交旧C06d。

不改旧成果/回执/评论、PR元数据、lead/PR16/master/time、内核/loader/Owner脚本；不出补丁，不执行配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发，不用外部libc镜像、本机凭据、额外模型/子代理/付费API，不新建分支PR、合并关闭强推删除、启动阶段3或跨项目维护。独立纯文本/数学不等于运行仓库实现；未做项如实NOT_RUN。

Owner只需在本MyOS2项目新建普通对话MYOS2-A-C06e 代码分析执行，选次一档GPT-6＋Extra并转发完整启动词，不必返回旧C06d、导出聊天、搬运报告、审代码或编译。下一步仓库写入：是，仅14通过全部门后的97限定范围；包发布不等于新会话已创建或启动。110/124继续进入以后主线交接。
