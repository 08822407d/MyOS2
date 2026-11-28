---
task_id: "CORE-USER-VMA-MERGE-22"
track_id: "MYOS2-LEAD-002"
record_type: "delivery_manifest"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra (Owner selected; backend not independently attested)"
date: "2026-11-02"
source_branch: "time"
source_pin_short12: "a039d9803ade"
instruction_short12: "cffb519d089b"
result_base_short12: "16baace7bd3c"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
phase: 2
conventions_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
source_canaries: ["time options_flags.cmake: -mcmodel=kernel -fno-pie -fno-pic","time panic.c: this_cpu = smp_processor_id()"]
self_check: {"source_quotes":94,"quotes_reconfirmed":94,"downgraded_to_inferred":0,"nodes":58,"edges":59,"scenario_rows":36,"effects":17,"identity_records":9,"math_examples":4,"new_conditional_findings":3,"previous_finding_evidence_appends":4,"deliverable_files":6}
---

# CORE-USER-VMA-MERGE-22 — G00–G04交付清单

**READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；runtime_evidence=NOT_RUN。** 继续使用C03b执行身份`MYOS2-LEAD-002-CHAT-EXEC-02`，Owner选定GPT-6＋Extra；平台后端配置不作独立认证。这只是静态材料的交付请求，非C03验收与内核运行证明。

唯一授权新增目录：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vma-merge-22/`；沿用既有PR17及`claude/dazzling-cori-q0dnyt`分支，首次写前期望HEAD`16baace7bd3c`。只允许六件，全部旧文件（含19–21结果）冻结。

| 文件 | 交付内容 |
|---|---|
| `MANIFEST.md` | 本清单、G00–G04与交付/未做范围 |
| `vma-merge.md` | 调用、候选、目标身份、边界/pgoff、链表与资源的连贯分析 |
| `facts-and-boundaries.yaml` | 58节点、59条类型关系、36条件场景、17effects、9身份账及4组纸面演算 |
| `vma-merge-paths.dot` | 与facts使用同一节点/边ID的静态图 |
| `deferred-findings.yaml` | 3项条件性新增问题；VL14-F01/F02/F03/F05四项历史证据追加 |
| `evidence.md` | 固定time 94条path::symbol、1–5行逐字源码锚点与未做事项 |

## G00–G04记录

**G00 completed：** 读取41号固定任务、21主线验收回执R21-S01/S02/S03，定点合读20主线R20-S02/S04、14主线R14-S01/S02及其VL14-F01/F02/F03/F05延期记录。继续18/24/25/28/36权限与身份规则，指令`cffb519d089b`、结果基线`16baace7bd3c`、冻结源码`a039d9803ade`分开。已读time options_flags.cmake及panic.c金丝雀。

**G01 completed_bounded_static：** simple_mmap_region两次merge调用及prev的实际生产、curr/next的查询来源、VM_SPECIAL/VM_SOFTDIRTY、file/close和before/after的页号条件与邻接缺口。

**G02 completed_bounded_static：** merge_prev/merge_next/prev/curr/next/adjust_target的赋值和读前定值矩阵；直接假设输入与正常首次调用分离；在未初始化指针路径停止确定性推演。

**G03 completed_bounded_static：** __simple_vma_adjust字节/页单位与先后更新、全摘/部分裁切、__vma_unlink_from_list的真实计数与节点操作、VMA与文件引用区分；文件回调后二次合并成功仅释放临时新建VMA、再采用旧目标。

**G04 content_and_consistency_checked; postcommit_remote_receipt_separate：** 六件文本形成，94条固定time原文quote逐字1–5行；facts/deferred采用JSON作为YAML1.2子集，DOT节点/边与证据锚点一致，4组算术为纸面算例。写后固定提交与活动分支各六件回读、比较新增文件及交付PR17评论，必须以真实后置结果为准；本文件不得提前报告已成功。

## 已知边界与强制消费

第21批主线已纠正源引文头计数、初始NULL迭代和严格前驱条件；本批不重写21。第14批已有严格前驱、页单位混合、摘链未释放和非事务回滚根因；本批只追加merge路径证据，不把这些重复计成新增缺陷。第20批已限定文件回调后二次合并会替换临时对象，不能混淆其fput与旧VMA摘链。普通问题只入库，Owner暂不需要修复决定。

没有预处理、编译/运行C或ASM、仓库旧脚本、内核/QEMU/硬件、全局并发/引用追踪；未证明具体内存泄漏、CPU权限或PTE完整正确。若当前PR17 HEAD/目录与基线不符，停止分支写入并只读核查归属。只能使用expected-head普通快进；不新建PR、修改PR正文标题/Draft或旧文件，不修源码、不合并/关闭、删分支或强推、不进入阶段3。

正式交付六件及本人PR17交付评论后，C03b停止写入，等待「MYOS2-A-C03 内核分析主线续建」独立验收；不得自行开始下一批。
