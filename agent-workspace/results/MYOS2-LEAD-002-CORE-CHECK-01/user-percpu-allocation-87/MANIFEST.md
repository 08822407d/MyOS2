---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-PERCPU-ALLOCATION-87"
record_type: "six_file_static_analysis_manifest"
produced_by: "GPT-6（当前对话显示的模型家族；后台不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定执行档）"
date: "2026-11-14"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
lead_release_short12: "fc14432f2a56"
execution_start_short12: "596fb3d7e1a5"
phase: 2
status: "FINAL_STATIC_DELIVERABLE_PENDING_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
R86_01: "CORRECTION_SUBMITTED_PENDING_C06_REVIEW"
prior86: "RETURN_DOCUMENTATION_ONLY_RETAIN_TECHNICAL_CORE_R86_S01_S02_S03"
D00: "APPEND_ONLY_SELF_CHECK_DOCUMENTATION"
runtime_evidence: "NOT_RUN"
open_questions: ["PA87-U01","PA87-U02","PA87-U03","PA87-U04","PA87-U05","PA87-U06","PA87-U07","PA87-U08","PA87-U09"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/123-user-percpu-allocation-contract.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-14-percpu86-reviewed-allocation87-ready.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","mykernel/mm/misc/percpu.c","mykernel/include/linux/smp/percpu_chunk.h","mykernel/include/linux/kernel/align.h","mykernel/include/uapi/linux/const.h","mykernel/include/linux/kernel/math.h","mykernel/include/uapi/asm-generic/errno-base.h","mykernel/lib/list/double_list_macro.h","mykernel/lib/list/double_list.h","mykernel/mm/kmalloc/slub.h","mykernel/mm/kmalloc/slub.c","mykernel/mm/page_alloc/buddy_const.h","mykernel/kactive/workqueue/workqueue.c","mykernel/kactive/workqueue/workqueue_types.h","mykernel/kactive/workqueue/workqueue_const.h","mykernel/include/linux/smp/percpu.h","mykernel/arch/x86_64/include/asm/percpu.h","mykernel/include/linux/compiler/compiler.h","mykernel/arch/x86_64/smp/setup_percpu.c","mykernel/include/linux/kernel/cpumask.h","mykernel/kactive/workqueue/workqueue.h","mykernel/mm/kmalloc/kmalloc_api.h","mykernel/kactive/workqueue/workqueue_macro.h","mykernel/kactive/kactive_type_declaration.h","mykernel/mm/misc/mm_misc.h","mykernel/mm/misc/mm_misc_types.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/percpu-bootstrap.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/percpu-bootstrap-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/evidence.md"]
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options_cflags":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options_cflags":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
formal_delivery_marker: "MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-PERCPU-ALLOCATION-87 / FORMAL_DELIVERY"
self_check: {"verified_claims":102,"quotes_reconfirmed":102,"downgraded_to_inferred":0,"new87_definition_anchors":102,"new87_raw_prefixes_expected":102,"D00_old86_definition_anchors":77,"D00_old86_explanatory_literal":1,"D00_old86_raw_prefixes":78,"D00_original_author_selfcheck_frozen":[77,77,0],"matrix_groups":10,"matrix_rows":72,"finite_model_cases":28,"conditional_new_findings":9,"new_evidence_gaps":9,"source_files_anchored":19,"source_files_bounded_context":6,"total_source_paths_87":25,"soft_source_limit_overage":1,"P9_full_local":"NOT_RUN","CMake_build_compile_link_repo_scripts_Graphviz_QEMU_hardware":"NOT_RUN","additional_models_subagents_paid_API":"NOT_USED","old_files_changed":false,"forty_hex_occurrences_expected":0}
write_retry_log: {"logical_operation":"single_bounded_87_commit_plus_one_PR17_comment","actual_write_attempts_before_publication":0,"retry_status":"not_needed_before_publication","expected_head_short12":"596fb3d7e1a5","force":false,"after_publication_receipt":"formal PR17 top-level comment with actual real commit and GitHub readbacks"}
reading_account: "facts-and-boundaries.yaml::reading_account split into new_read_full_current_turn, new_read_source_files_bounded, same_active_C06b_conversation_read_reused, not_read"
allowed_new_files: ["MANIFEST.md","percpu-allocation.md","facts-and-boundaries.yaml","percpu-allocation-paths.dot","deferred-findings.yaml","evidence.md"]
---

# 第87批六件清单与消费边界

身份继续MYOS2-LEAD-002-CHAT-EXEC-11，原C06b执行对话。主线合同123固定fc14432f2a56，源码仅time@a039d9803ade，执行父596fb3d7e1a5，PR17原分支不变。G00已核四direct refs、PR16/17、目标目录预期404及本人正式marker零出现；未在G00前写入。公开GitHub连接器为唯一源码读取及交付通道。

| 唯一新增六件 | 说明 | 认证边界 |
|---|---|---|
| MANIFEST.md | 任务、权限/分支、自检、公约逐字句和time/master金丝雀 | 后置正式提交号及评论号不预造 |
| percpu-allocation.md | D00原86自检计数纠正和三个新专题的源码分析；10组72行矩阵、28个条件模型 | 不是实际程序/构建运行 |
| facts-and-boundaries.yaml | 新读/复用/未读账、源码ledger、D00逐件数字、新技术矩阵/模型、事实键和证据链接 | 19个有短锚的源码文件＋6份最近上下文，超过24软限一份 |
| percpu-allocation-paths.dot | 显式区分真实C调用、宏数据关系、同函数源顺序和假设的图 | 未运行Graphviz，不是完整CFG |
| deferred-findings.yaml | PA87-F01..F09条件性风险、PA87-U01..U09证据缺口、历史ID关联 | 未将其计作18个实测bug |
| evidence.md | 102个固定time单定义内部连续1–5行短引文，作者发布前回源二读 | 全六件原始标签仅在此文件出现 |

D00独立从原86固定提交读取六件，当前回查为：MANIFEST0、正文0、facts1（说明字面量）、DOT0、deferred0、evidence77；实际原始前缀总78而真正定义锚77。原86作者77/77/0保留历史，不能改成78个引文；更不能据此自签R86-01闭合。原86 RETURN仅文档范围，技术受R86-S01/S02/S03限定；R84-01已闭合、85限定PASS_PENDING_LOCAL均不重开。

本87的新静态计数独立于原86：102个新的定义锚对应102次新全包原始前缀，quotes_reconfirmed=102、downgraded_to_inferred=0；正文、D00说明、JSON、DOT与本清单不包含额外字面标签。可读源码源路径有25份，软限24多1份：为核实际工作队列接口与C/宏最近依赖，另读了直接相关的上下文头文件，不把没有独立定义锚的6份说明当强证据。没有全仓漫游。

按122/123及P9字段说明：未执行CMake、gcc、预处理、汇编、链接、内核或仓库Python/脚本、Graphviz、QEMU、实机、真实并发实验；未修改旧结果/评论、PR正文/元数据、lead/master/time、内核或脚本，没有补丁、新分支PR、合并、额外模型/子代理/付费API。完整本地P9为NOT_RUN；静态原文核对和有限整数推导不等于P9本地套件通过或真实运行。

本文件在正式提交前生成：后置Git commit与唯一正式PR17评论的真实编号在GitHub成功响应后记录于正式交付评论，不能预填不存在的SHA或评论号。提交发布及两轮固定/活动分支回读完成后，执行者报READY_FOR_MAINLINE_REVIEW与mainline_acceptance=PENDING、R86-01补正待复核，随后STOP_WRITING并交原C06 Pro独立审查87，不签86闭合/87通过、不启动88。Owner无需亲自审查源码或编译。
