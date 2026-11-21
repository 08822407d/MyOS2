---
task_id: "CORE-USER-EXEC-STACK-23"
track_id: "MYOS2-LEAD-002"
record_type: "delivery_manifest"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra (Owner selected, backend not independently attested)"
date: "2026-11-02"
source_branch: "time"
source_pin_short12: "a039d9803ade"
instruction_short12: "c9be5ac03550"
result_base_short12: "b32f3ebc9095"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
phase: 2
conventions_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
self_check: {"source_quotes":116,"quotes_reconfirmed":116,"downgraded_to_inferred":0,"nodes":72,"edges":74,"scenario_rows":35,"effects":20,"bounded_math":4,"new_conditional_findings":5,"prior_evidence_appends":3,"files":6}
---

# CORE-USER-EXEC-STACK-23 — S00–S04交付清单

**READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；runtime_evidence=NOT_RUN。** 第23批仅是阶段2有界静态分析，不是C03审查已通过或内核/CPU栈访问正确性认证。

唯一授权新增路径：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-23/`。现有执行身份`MYOS2-LEAD-002-CHAT-EXEC-02`、原PR17/`claude/dazzling-cori-q0dnyt`；首次写前期待HEAD`b32f3ebc9095`，旧件全部冻结。仅六件：

| 文件 | 说明 |
|---|---|
| `MANIFEST.md` | 本清单、S00–S04完成程度与禁止事项 |
| `exec-stack.md` | exec新mm/用户栈VMA/参数页/地址权限/expand_stack/缺页衔接主说明 |
| `facts-and-boundaries.yaml` | 72节点、74类关系、35条件场景、20effects及4组纸面演算 |
| `exec-stack-paths.dot` | 与facts一致的ID/方向/类型有界关系图 |
| `deferred-findings.yaml` | 5新增条件待核、3条旧MF12证据追加，旧状态保留 |
| `evidence.md` | 116段冻结time源码path::symbol与1–5行逐字锚点 |

## S00–S04

**S00 completed：** 全文读取固定42号任务书、22主线回执R22-S01/S02/S03；定点读取12主线回执及MF12-F01/F02/F03延期、21初始NULL迭代边界，复用15 mm替换和19/20局部权限/匿名判定的已有证据，18延期与24/25/28/36分工延续。`options_flags.cmake`已从固定time读取；首读源码与任务/结果提交严格分开。

**S01 completed_bounded_static：** alloc_bprm→bprm_mm_init→__bprm_mm_init及mm_alloc/vma_init/匿名设置、初始PAGE_SIZE VMA、vm_flags/page_prot/pgoff、insert_vm_struct与计数/清理；get_arg_page参数页消费另按必要依赖定点补证。不是参数页实际完整有效认证。

**S02 completed_bounded_static：** load_elf_binary的PT_GNU_STACK来源、begin_new_exec→exec_mmap切换mm时序、setup_new_exec→setup_arg_pages、stack_top/arch_align_stack/stack_shift及指针字段、EXSTACK局部vm_flags与VMA真正清位、注释的mprotect_fixup/shift_arg_pages、mmap锁/错误早退。

**S03 completed_bounded_static; runtime_fault_reachability_partial：** expand_stack的PAGE_MASK/min/grow/pgoff门、字段/自旋锁/计数及限额注释、默认31页扩张算例；exec主动调用与arch do_user_addr_fault调用的前提、稳定点包含查询造成的旧MF12-F01可达界限，进入myos_handle_mm_fault的有限接口。完整fault/PTE/硬件未验证。

**S04 content_QA_completed; GitHub_postcommit_receipt_external：** 六件结构内容已生成，116条1–5行源码引文逐字与冻结time中的17个源文件核对；facts和deferred使用JSON作为YAML1.2子集，图端点/ID/分类及引文引用集合、有限无符号算术由允许文本工具检查。正式提交、固定commit与执行分支各读六件、compare、交付PR17评论只能根据真实后置结果宣称，不在本MANIFEST提前宣告。

## 正确消费与未做

源文校验和编号一致不等于内核性质成立。新5条为有生产者/前提的条件问题，不代表五项已复现漏洞；MF12-F01/F02/F03三组历史追加不是新独立根因。R22-S02先行副作用在后续不返回前已可发生；R21-S02初始NULL迭代仍是定义良好前提。bprm.mm、current.mm、VMA、参数页、PTE和用户寄存器不混为同一证据层。

不执行仓库旧脚本、预处理/编译/运行C或ASM、内核/QEMU/实机、并发或实际argv/RSP验证。不得修改内核、用户程序、任务书或以前任一结果；不得写master/time/lead、重建PR、改PR标题正文/Draft、合并关闭、删分支/强推、进入阶段3。普通问题仅入库，Owner当前不用决定修复。正式六件交付及自己的PR17评论后立即停写，交「MYOS2-A-C03 内核分析主线续建」独立验收，不自行开始下一批。
