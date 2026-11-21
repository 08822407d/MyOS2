---
task_id: "CORE-USER-MMAP-PLACEMENT-21"
track_id: "MYOS2-LEAD-002"
record_type: "delivery_manifest"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra (Owner selected, backend not attested)"
date: "2026-11-02"
source_branch: "time"
source_pin_short12: "a039d9803ade"
task_contract_short12: "84ca207f5cc2"
result_base_short12: "0e139c6ca86d"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
phase: 2
conventions_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
source_canaries: ["time: -mcmodel=kernel -fno-pie -fno-pic","time panic.c: this_cpu = smp_processor_id()"]
self_check: {"verified_claims":92,"quotes_reconfirmed":92,"downgraded_to_inferred":0,"nodes":55,"edges":63,"scenarios":30,"effects":17,"math_examples":5,"new_conditional_findings":7,"prior_evidence_appends":1,"files":6}
---

# 第21批 A00–A04 交付清单

**READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；runtime_evidence=NOT_RUN。** 静态资料不是C03验收或实机权限/分配正确性证明。

本批唯一可写目录：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mmap-placement-21/`。沿用PR17、执行分支`claude/dazzling-cori-q0dnyt`，写前预期基线`0e139c6ca86d`；全部历史内容冻结，不能重复写入同名成果。

六件且仅六件：`MANIFEST.md`（本清单）、`mmap-placement.md`（选址及校验主报告）、`facts-and-boundaries.yaml`（55节点、63类型边、30条件场景、17effects、5组纸面算例）、`mmap-placement-paths.dot`（同ID图）、`deferred-findings.yaml`（7新条件记录＋1旧问题追加）、`evidence.md`（92个固定time path::symbol逐字引文）。

## A00–A04执行状态

**A00 completed：** 全文读取40号固定任务书、20主线验收回执R20-S01–S04；依14与12回执的解除旧映射/区间谓词限定消费，不重做19/20。已读冻结time的`options_flags.cmake`及`panic.c`分支金丝雀；继续25/36接收、18延期、24/28单写者与验收分工。

**A01 completed_bounded_static：** do_mmap输入、hint和MAP_FIXED_NOREPLACE、__get_unmapped_area回调选择、arch_pick_mmap_layout与setup_new_exec、x86地址/页数/界限符号及条件数字演算。

**A02 completed_bounded_static：** simple_get_unmapped_area快速提示返回、Top-down与仅定义的Bottom-up搜索、for_each_vma宏/相邻VMA帮助函数、NULL/anchor与候选覆盖、guard/alignment字段。

**A03 completed_bounded_static：** fixed早返回与非fixed后验校验，NOREPLACE的半开区间交集/严格前驱门、overflow、错误码、0地址/不返回、simple_mmap_region进入旧映射处理的边界。

**A04 content_check_completed; remote_postcommit_evidence_separate：** 六件文本已暂存。92引文逐字来源每条1–5行，事实与延期文件采用可解析JSON⊂YAML1.2，图边ID/端点和证据锚点联动检查、有限64位算术检查。只有真实提交后的GitHub双重文件回读和净diff通过才报告远端交付成功；本文件不预称完成那些动作。

## 严格限制

永久不把主线`PASS_PENDING_LOCAL`解释成内核运行正确；`VERIFIED`代表固定源码文字证据，不是所有场景都实际可达。迭代NULL、unsigned回绕、VMA身份与列表状态要保留语言及对象前提。未预处理、编译、运行C/ASM、QEMU、内核或硬件，未执行仓库旧脚本，不安装工具，未验证全部SMP锁与CPU页表权限。

只许可本批六件正常快进及自己的PR17交付评论；不修改kernel/旧报告/主线lead，不更改PR标题、正文、Draft或其它评论，不合并/关闭/删分支/强推，不进入阶段3。正式交付后停止写入交C03验收，不自行执行下一批。
