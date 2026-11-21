---
task_id: "CORE-USER-MMAP-PROTECTION-20"
track_id: "MYOS2-LEAD-002"
record_type: "delivery_manifest"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra（Owner选择；实际后端不可独立认证）"
date: "2026-11-01"
source_branch: "time"
source_pin_short12: "a039d9803ade"
contract_short12: "038217219faa"
initial_result_head_short12: "6166b3e2f5b9"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
source_canary: ["time: -mcmodel=kernel -fno-pie -fno-pic","time panic.c: this_cpu = smp_processor_id();","master: -mcmodel=large -fPIE; raw_smp_processor_id"]
self_check: {"verified_claims":155,"quotes_reconfirmed":155,"downgraded_to_inferred":0,"nodes":53,"edges":60,"scenario_cases":32,"effects":21,"new_findings":6,"evidence_appends":2,"files":6}
---

# CORE-USER-MMAP-PROTECTION-20 · 交付清单

**READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；runtime_evidence=NOT_RUN。** 继续由C03b（`MYOS2-LEAD-002-CHAT-EXEC-02`）按Owner选择GPT-6＋Extra执行阶段2静态取证；此状态不是C03验收或CPU运行证明。

**唯一新写区**：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mmap-protection-20/`，保持PR17及`claude/dazzling-cori-q0dnyt`既有执行分支。写前结果基线`6166b3e2f5b9`。严格六件且只新增：

| 文件 | 内容 |
|---|---|
| `MANIFEST.md` | 本清单、P00–P04与写入限制 |
| `mmap-protection.md` | mmap入参、权限、文件模式、回调及VMA模板主体 |
| `facts-and-boundaries.yaml` | 53节点、60边、32场景、21实际副作用及分层权限账 |
| `mmap-protection-paths.dot` | 结构化facts同ID关系图 |
| `deferred-findings.yaml` | 6条新条件问题、2条旧问题追加证据 |
| `evidence.md` | 155条固定time源文件1–5行逐字锚点与未做事项 |

**P00 completed**：完整读取39号任务及19主线验收R19-S01/S02/S03，定点读取14回执R14-S01/S02、08回执的输入/返回限定；沿用C03b已完成25/36首次接收，继续18号延期、24/28分工。源码`time @ a039d9803ade`，指令`038217219faa`，结果基线`6166b3e2f5b9`互不混淆。先读`mykernel/scripts/options_flags.cmake`，并以`mykernel/debug/panic.c`对照time/master。PR17 open/Draft，PR16由主线独占，本批写区严格单写者。

**P01 completed_bounded_static**：x64 syscall表/包装宏六寄存器实参、off/pgoff、anon/file、fget/fput/get_file、vm_mmap_pgoff与vm_mmap差异、锁包装活动状态。

**P02 completed_bounded_static**：PROT_READ的EXEC增位、calc_vm_prot_bits/calc_vm_flag_bits、当前VM与MAY位、OPEN_FMODE、文件MAP_PRIVATE/SHARED/VALIDATE、noexec及append/回调门、返回条件。

**P03 completed_bounded_static**：旧VMA边界/合并、新建VMA字段、文件get_file与FAT32→generic_file_mmap回调、变更后保护不刷新、vm_get_page_prot查表与掩码、16类模板中的代表位和对19批的上游交接。

**P04 content_check_completed，remote_readback_to_be_reported_in_comment**：结构化事实及DOT引用检查、证据155项逐字来源，JSON子集用作YAML1.2格式、对位值与对象条件做有界算术/词法检查。固定提交与活动分支双重回读及compare/交付评论只有在真实提交后核验，不能在本文件事先声称通过。

**核验与未做**：P2源码引用以`path::symbol`及原始1–5行核对，分段锚点数量不是155项独立技术性质认证。P6 `verified_claims=155,quotes_reconfirmed=155,downgraded_to_inferred=0`只指文字逐字来源核验。没有预处理/编译C/ASM、内核、QEMU、硬件、全局SMP锁、TLB、用户态实际权限或完整文件系统运行验证。旧18/19/14/08文件完全冻结，问题延期不修复，不能改PR标题正文/Draft、不合并关闭/删分支/强推或进入阶段3。

C03主线独立审阅前`mainline_acceptance=PENDING`；本执行者仅在第20批六件交付评论完成后停止写入，不自行下一批。
