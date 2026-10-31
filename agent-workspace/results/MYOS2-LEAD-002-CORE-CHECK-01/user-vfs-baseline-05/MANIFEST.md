---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-BASELINE-05
record_type: user_vfs_baseline_manifest
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
execution_model_selection: unknown_or_not_attestable
execution_surface: "claude.ai/code 托管云端会话容器；只读 git 对象查询与 Python 文本处理（容器既有 git 与 Python）；与此前 pilot/core/recheck/scheduler-order-02/integration-03/mm-baseline-04 是同一会话、同一执行分支"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a（开工与交付时均为事实，未固定）；主线 agent/MYOS2-LEAD-002 0d066801371d（开工时）；被审执行头 de7c96ede546；MM 结果冻结 eb75b3100601。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: "22934124fe92"
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_user_vfs_baseline_05
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_action_now: none
owner_decisions_requested_now: []
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "只在 user-vfs-baseline-05/ 新增，依据 Owner 本轮指令与 19 号任务书 allowed_write_prefix（scripts/uv_scope.py 机械核对）；de7c96ede546 的全部文件不变；未写 agent/MYOS2-LEAD-002、master、time；PR17 保持 Draft 复用"
inputs_read: "见 user-vfs-baseline.md 头部与 evidence.md §2"
u00_u04:
  U00: "完成：MR-01 按大于/等于/小于队首与无非 idle 队首四类改写，值表 7 行规则算术核对通过；MR-02 收窄 33 处旧文字、复查保留 7 处，均回到 de7c96ede546 逐处核对"
  U01: "完成：两主对象各 9 个代表能力，另 2 个后端交界端点；六条主链的入口、正常路径、失败/清理边界、证据与覆盖限制"
  U02: "完成：五轴证据；43 条边（call/data/init_order/config/build），间接调用均给出调用点、绑定、接收方；否定断言均带范围"
  U03: "完成：deferred-findings.yaml（16 项新条目、2 项文档勘误、10 项旧条目状态变更、13 项交叉链接）"
  U04: "完成：两层可用性表；6 项未来核验候选，全部 NOT_RUN"
data_summary: {capabilities: 20, capabilities_per_object: {sched.forkexec: 9, fs.vfs: 9, boundary: 2}, chains: 6, edges: 43, anchors: 90, queries: 44, structure_checks: 7, value_tables: 2, link_breaks: 10, dependency_risks: 6, candidates_not_run: 6, gaps_open: 10, findings: 16, findings_needs_evidence: 4, errata: 2, status_changes: 10, cross_links: 13, u00_superseded_locations: 33, u00_reviewed_retained: 7}
delivered_files: [MANIFEST.md, mm-04-disposition.md, user-vfs-baseline.md, facts-and-dependencies.yaml, deferred-findings.yaml, evidence.md, scripts/uv_scope.py, scripts/uv_facts_check.py, records/scope_start.json, records/scope_stage.json, records/facts_check.json, records/queries.json, records/checker_probes.json, records/readback_results.json]
---

# CORE-USER-VFS-BASELINE-05 交付清单

## 当前状态

U00–U04 一次完成。先追加了 MM-BASELINE-04 两处文档表述的校正，随后完成用户程序与文件访问的静态基线。所有结论都来自源码文本与规则算术，没有编译，也没有运行任何内核、程序或脚本。本批发现的问题都已按 18 号约定写入 `deferred-findings.yaml`，当前没有需要 Owner 作出的技术决策。

## 逐文件说明

| 文件 | 内容 | 覆盖状态 | 怎么消费 |
|---|---|---|---|
| `MANIFEST.md` | 本清单：状态、逐文件说明、未做事项、恢复顺序 | 完整 | 先读 |
| `mm-04-disposition.md` | U00：MR-01 四类改写与 VT-MR01 值表；MR-02 收窄 33 处旧文字、复查保留 7 处（front matter 逐处定位） | 完整 | 消费 mm-baseline-04 原件时，落在 supersedes_scope 中的旧句以本文件为准 |
| `user-vfs-baseline.md` | U01–U04 正文：20 个能力、六条主链、依赖与断点、两层可用性表、入库条目索引、候选、缺口 | 完整（静态，NOT_RUN） | 讲解与后续任务的入口；依据按 ID 查 facts |
| `facts-and-dependencies.yaml` | 结构化事实：能力、链、边（含间接调用三项证据）、断点、依赖风险、U04、候选、旧 ID 映射、结构检查、值表、90 条锚点、44 个检索、缺口 | 完整 | 按 ID 引用；检查器的输入 |
| `deferred-findings.yaml` | U03：16 项新条目、2 项文档勘误、10 项旧条目状态变更、13 项交叉链接；恢复顺序；空集合声明 | 完整 | 以后新专项按 qualified_id 选取 |
| `evidence.md` | 命令、核对、独立审查、提交与回读、范围检查 | 完整 | 复核方法与限制 |
| `scripts/uv_scope.py` | 开工、暂存、文档批的写区与绑定核对（只读 git） | 完整 | start|stage|docs |
| `scripts/uv_facts_check.py` | 机械一致性核对（import 复用 MM 包的读取原语） | 完整 | 见“后续怎么用” |
| `records/scope_start.json` | 开工绑定记录 | 完整 | 核对开工时的对象与字段 |
| `records/scope_stage.json` | 结果提交前的暂存边界记录 | 完整 | 核对新增文件与卫生 |
| `records/facts_check.json` | 检查器结果（all_ok: True） | 完整 | 核对数字 |
| `records/queries.json` | 44 个检索的命令、返回码与命中 | 完整 | 否定断言的范围 |
| `records/checker_probes.json` | 检查器负面探针（12 个，加 1 个未修改副本） | 完整 | 说明检查器不是空转 |
| `records/readback_results.json` | 结果提交 22934124fe92 的远端回读（6 个文件 × 对象/分支） | 完整 | 核对远端内容与提交一致 |

## 明确没有做的事

- 没有编译或运行 C/ASM，没有运行 cmake、内核、QEMU、宿主安装脚本或仓库原有脚本，没有重跑旧测试（H00/V/W/M/N）。
- 没有修改内核，没有出补丁，A、B、键下限都没有采用；de7c96ede546 的旧文件、主线分支、master、time 都没有修改。
- 没有合并，没有删除分支，没有 force-push，没有安装工具，没有挂盘，没有提权，也没有索取或输出凭据。

## 后续怎么用

**恢复顺序**（给以后新开的任务）：18 号入口（`agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md`，agent/MYOS2-LEAD-002）→ 最新主线回执（同分支 `reviews/`）→ 本包基线（`user-vfs-baseline.md`、`facts-and-dependencies.yaml`）→ 被选条目的 qualified_id（`deferred-findings.yaml`）→ 原证据与当时最新的 time。未来修复前，先重核快照与权限。本包不生成实施代码，也不激活修复任务。

**复核：** 在仓库根目录运行 `PYTHONDONTWRITEBYTECODE=1 python3 <本目录>/scripts/uv_facts_check.py <本目录> <out.json> <本目录>/mm-04-disposition.md <本目录>/user-vfs-baseline.md`。它只读 git 对象，会重新核对引文、检索、结构、值表、旧文字定位与延期记录字段；检索记录 queries.json 写在 <out.json> 所在目录。`scripts/uv_scope.py start|stage|docs` 用来核对写区边界。检查器只做机械一致性核对，不充当语义证明。
