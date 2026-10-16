---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
record_type: mm_baseline_manifest
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
execution_model_selection: unknown_or_not_attestable
execution_surface: "claude.ai/code 托管云端会话容器；只读 git 对象查询与 Python 文本处理（容器既有 git 与 Python）；与 pilot/core/recheck/scheduler-order-02/integration-03 同一会话、同一执行分支"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 agent/MYOS2-LEAD-002 c2176ad4da02（开工时固定）；执行基 ee9e6a738224；integration-03 结果 b7fa83583e35。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: "eb75b3100601"
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_mm_baseline_04
kernel_correctness_verdict: NOT_ISSUED
kernel_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
candidate_A_adopted: false
candidate_B_adopted: false
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "只在 mm-baseline-04/ 新增，依据 Owner 本轮指令与 17 号任务书 allowed_write_prefix（scripts/mm_scope.py 机械核对）；ee9e6a738224 全部文件不变；未写 agent/MYOS2-LEAD-002、master、time；PR17 保持 Draft 复用"
inputs_read: "见 facts-and-dependencies.yaml 的 inputs_read"
open_questions: "见 memory-baseline.md §0 与 YAML gaps（9 open + 1 narrowed）"
m00_m04:
  M00: "完成：IR-01 三张值表（原函数 idle 键 0：50/40/10；原函数最大键：60/40/0；A/B 规格预期：60/40/0），机械算术核对通过；IR-02 显式保持项 B-K1…B-K10 与键下限两分支；R1–R7 消费限界登记"
  M01: "完成：四子系统入口索引、21 个代表能力（5/5/6/5），旧 ID 映射 OLD-01…OLD-16，未覆盖项单列"
  M02: "完成：每个能力五轴证据（implementation/correctness/runtime=NOT_RUN/concurrency/source_condition）；20 条静态疑点含前提、路径、反证、unknown"
  M03: "完成：39 条能力级边、9 个链路断点、四问回答；include 不计为调用"
  M04: "完成：21 行可用性表（可读讲解/宿主可验证/需真实内核分列）；6 个下一步验证候选，全部 NOT_RUN"
data_summary: {"m00_original_idle0": "N2=50,N1=40,idle=10", "m00_original_idlemax": "N2=60,N1=40,idle=0", "m00_spec_A_or_B": "N2=60,N1=40,idle=0", "capabilities": 21, "capabilities_per_subsystem": {"mm.page_alloc": 5, "mm.kmalloc": 5, "mm.vm_map": 6, "mm.fault": 5}, "implementation_evidence": {"connected_body": 16, "declaration_only": 0, "not_assessed": 0, "not_located_in_scope": 0, "partial_body": 5}, "edges": 39, "link_breaks": 9, "concerns": 20, "refuted_or_narrowed": 5, "dependency_risks": 6, "anchors_verified": "80/80", "markdown_tags_verified": {"integration-03-disposition.md": "4/4", "memory-baseline.md": "8/8"}, "queries": 43, "next_validation_candidates_not_run": 6, "gaps_open": 9, "gaps_total": 10, "checker_probes": "control pass + 10/10 mutations rejected"}
delivered_files: ["MANIFEST.md", "evidence.md", "facts-and-dependencies.yaml", "integration-03-disposition.md", "memory-baseline.md", "records/after_results/readback_facts-and-dependencies.branch.json", "records/after_results/readback_facts-and-dependencies.object.json", "records/after_results/readback_facts_check.branch.json", "records/after_results/readback_facts_check.object.json", "records/after_results/readback_integration-03-disposition.branch.json", "records/after_results/readback_integration-03-disposition.object.json", "records/after_results/readback_memory-baseline.branch.json", "records/after_results/readback_memory-baseline.object.json", "records/checker_probes.json", "records/facts_check.json", "records/queries.json", "records/scope_stage.json", "records/scope_start.json", "scripts/mm_facts_check.py", "scripts/mm_scope.py"]
---

# CORE-MM-BASELINE-04 交付清单

## 当前效果

- **M00（规格处置）：** IR-01 的值表与主线推导一致，没有找到反证：原函数在 idle 键为 0 时是 N2=50、N1=40、idle 区间 10；idle 取最大键时是 60/40/0；A 或 B 按规格应为 60/40/0。IR-02 已把 B 的保持项逐条列出，排除了普通唤醒头插，键下限按设与不设两个分支分别写明，没有替 Owner 选择。A、B、键下限都没有采用，Owner 沉默也不构成授权。
- **M01–M04（四个内存子系统）：** 主路径都能讲清楚，并且确实接在一起：缺页补页表，取页靠 buddy，堆从 buddy 取页，VMA 用链表管理。但下面几处不能当作可靠基础：
  - 用户内存只增不减：put_page 不归还，munmap 与退出不拆页表，buddy 不合并（MC-05、MC-16、MC-01）。
  - 未处理的缺页在内核内自旋，数据页取页失败时先读空指针，都不发信号（MC-23、MC-24）。
  - 匿名页不清零（MC-21）。
  - mmap 选址固定、相交查询漏判、合并时读未初始化指针、拆分后 pgoff 算错（MC-11、MC-13、MC-14、MC-15）。
  - 无锁分配，mmap 锁为空函数（MC-02、MC-18）。
  - fork 后不刷 TLB（MC-25）。
- **与调度、锁、等待的关系：** 内存子系统不直接使用 integration-03 已登记的问题原语（负检索 Q28）。缺页自旋能否被换下依赖中断出口调度（DR-01）；文件缺页下面的块 I/O 等待没有追踪（DR-02）。
- **保留的反证：** 独立审查驳回了初稿的 2 个后果推断（sighand 锁自旋、munmap 自旋），把 1 个收窄为潜伏（重复分配），更正了 1 条路径，驳回了 1 条“唯一”式断言，见 RF-01…RF-05。

## 逐文件说明

| 文件 | 内容 | 覆盖状态 | 怎么消费 |
|---|---|---|---|
| `MANIFEST.md` | 本清单 | 完成 | 先读 |
| `evidence.md` | 执行命令、核对结果、独立审查过程、提交与回读、边界检查 | 完成 | 核实过程时读 |
| `facts-and-dependencies.yaml` | 机器可读的全部数据：子系统、能力、边、断点、疑点、驳回项、风险、M04、候选、旧 ID 映射、引文、检索、M00 值表、缺口 | 完成 | 按 ID 消费；mm_facts_check.py 可复核 |
| `integration-03-disposition.md` | M00：IR-01 值表、IR-02 的 B 保持项、R1–R7 消费限界 | 完成；规格处置，非内核修复 | 替换 integration-03 中 supersedes_scope 所列文字 |
| `memory-baseline.md` | M01–M04 正文：结论、能力表、依赖、可用性、疑点、风险 | 完成；静态，NOT_RUN | 主阅读入口；ID 查 YAML |
| `records/after_results/readback_facts-and-dependencies.branch.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_facts-and-dependencies.object.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_facts_check.branch.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_facts_check.object.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_integration-03-disposition.branch.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_integration-03-disposition.object.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_memory-baseline.branch.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/after_results/readback_memory-baseline.object.json` | 结果提交后的远端回读记录 | 完成 | 核对远端正文身份 |
| `records/checker_probes.json` | 检查器对照与 10 个负面探针的结果 | 完成 | — |
| `records/facts_check.json` | mm_facts_check.py 的结果 | 完成 | — |
| `records/queries.json` | 43 个检索的命令、返回码、命中（含 active/comment 与所在函数） | 完成 | 查边与负检索依据 |
| `records/scope_stage.json` | 结果提交前的 stage 检查输出 | 完成 | — |
| `records/scope_start.json` | 开工检查输出 | 完成 | — |
| `scripts/mm_facts_check.py` | 引文/标签/结构/检索/M00 核对，写 records/queries.json | 完成 | 复核事实 |
| `scripts/mm_scope.py` | 写区与绑定检查（start/stage/docs） | 完成 | 复核边界 |

## 明确没有做的事

- 没有编译或运行 C/ASM，没有运行 cmake、内核、QEMU 或仓库原有脚本，没有重跑 W/V/M/N。
- 没有修改内核，没有出补丁，A、B、键下限都没有采用；没有修改 ee9e 原件、主线分支、master 或 time。
- 没有合并，没有删除分支，没有 force-push，没有安装工具，没有提权，没有输出凭据。
- 内存子系统的“缺失项”（回收、OOM、per-CPU、大页、swap、mprotect 等）没有逐项检索，见 YAML 的 `old_inputs_index` 与 `uncovered_old_capabilities`。

## 后续怎么用

1. **先读这三处：** memory-baseline.md §0（结论与未决），再读 §5（疑点）和 §4（可用性）。需要依据时按 ID 查 facts-and-dependencies.yaml。
2. **复核：** 在仓库根目录运行 `PYTHONDONTWRITEBYTECODE=1 python3 scripts/mm_facts_check.py facts-and-dependencies.yaml <新目录> <out.json> integration-03-disposition.md memory-baseline.md`（相对本目录），它只读 git 对象，会重新核对引文、检索、结构和 M00 算术。`scripts/mm_scope.py start|stage|docs` 用来核对写区边界。
3. **决定下一步：** 如果要验证，NV-1…NV-3 只需宿主夹具授权，NV-4…NV-6 需要完整内核构建与 QEMU 授权，并且会触及调度前置条件。在获得授权之前，这些都只是规格。
