---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-BOUNDARIES-07
record_type: user_boundaries_manifest
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
execution_surface: "本机专用隔离 clone /home/cheyh/projs/myos2-handoff06-local/MyOS2；只读 git 对象查询与 Python 文本处理（本机既有 git 2.53.0、Python 3.14.4、PyYAML 6.0.3）；Owner 订阅额度（owner_asserted）"
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 b12572f14eae；主线 agent/MYOS2-LEAD-002 f5d0423da491；master e8e60cd19540（均为短 SHA，从工具输出复制）"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/21-user-entry-console-contract.md（agent/MYOS2-LEAD-002）"
review_ref: "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-EXECUTION-HANDOFF-06-review.md（CORE-EXECUTION-HANDOFF-06-REVIEW-001）"
findings_policy_ref: "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: "见 PR17 上本批推送后的评论（本文件不自引用其所在提交）"
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_user_boundaries_07
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
build_qemu_install_run: false
tools_installed: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_action_now: none
owner_decisions_requested_now: []
write_authority_note: "只在 user-boundaries-07/ 新增；b12572f14eae 中的全部文件不变；未写主线分支、master、time；PR17 保持 Draft 复用。提交前边界核对见 records/boundary_staged.json"
inputs_read: "见 evidence.md §2"
b00_b04:
  B00: "完成：06 回执 record_id、被审头 b12572f14eae、下一任务 07 核对一致；主线对象 f5d0423da491；远端 time 仍为 a039d9803ade；b12572f14eae 是执行头祖先；先读 options_flags.cmake"
  B01: "完成（GAP-U03 → closed_static，运行部分 NOT_RUN）：根对象与共享 init_fs、devtmpfs 两种类型与建立线程、/dev 挂载与路径跨挂载、各失败输入停在哪里"
  B02: "完成（GAP-U04 → narrowed）：首个程序、fork 子任务、普通系统调用三条路径的写帧/建立/取栈/消费；返回段指令与布局对照。生成偏移数值与 ISA 语义保留未知"
  B03: "完成（GAP-U05 → closed_static，运行部分 NOT_RUN）：console 节点来源、前后两张 f_op 表、读写落点与第一层后端；GAP-U10 维持 open"
  B04: "完成：user-entry-console.md（三问答案、两层图、六条链衔接、缺口状态、剩余边界索引）与 deferred-findings.yaml（UB-01…UB-14；文档勘误 ER-01；追加 CORE-USER-VFS-BASELINE-05::UV-01、UV-04 证据）"
data_summary: {capabilities: 14, edges: 37, edges_indirect: 8, anchors: 70, anchors_by_theme: {B01: 29, B02: 16, B03: 25}, queries: 70, structure_checks: 20, value_tables: 3, frame_paths: 3, chain_links: 6, gap_status: 6, candidates_not_run: 4, findings: 14, findings_needs_evidence: 3, evidence_appends: 2, errata: 1, markdown_verified_blocks: 22}
delivered_files: [MANIFEST.md, user-entry-console.md, facts-and-boundaries.yaml, deferred-findings.yaml, evidence.md, scripts/ub_check.py, records/ub_check.json, records/queries.json, records/run.json, records/boundary_staged.json]
---

# CORE-USER-BOUNDARIES-07 交付清单

## 当前状态

B00–B04 在同一轮完成。05 留下的三处断点都已补证：初始根与 /dev（GAP-U03）、用户返回入口与寄存器帧来源（GAP-U04）、/dev/console 回调（GAP-U05），并接回 05 的六条链。全部结论来自 `time a039d9803ade` 的源码文本与规则推导，没有编译或运行内核、C/ASM、构建或安装脚本。本批发现的普通问题都已写入 `deferred-findings.yaml`（另含一项对 05 表述的勘误 ER-01，05 原文件不改），当前没有需要 Owner 作出的技术决策。

## 逐文件说明

| 文件 | 内容 | 怎么消费 |
|---|---|---|
| `MANIFEST.md` | 本清单 | 先读 |
| `user-entry-console.md` | 中文源码说明：三个问题的答案、三条返回路径对照、根与 /dev 对象表和时序、console 前后两张 f_op 表与读写落点、正常/失败两层图、与 05 六条链的衔接、缺口状态、问题索引、剩余边界索引 | 讲解与后续任务的入口；依据按编号查 facts |
| `facts-and-boundaries.yaml` | 14 个能力节点（proposed_id，前缀取自共享词汇表）、37 条依赖/绑定边（方向同 05；8 条间接边带调用点/绑定/接收方）、70 条锚点、70 个检索、20 项结构检查、3 张值表、3 条帧路径、6 条链衔接、6 项缺口状态、4 个 NOT_RUN 候选 | 按 ID 引用；核对脚本的输入 |
| `deferred-findings.yaml` | UB-01…UB-14；文档勘误 ER-01（05 DRU-06 中 fs 的一半）；对 05 UV-01、UV-04 的证据追加 | 以后新专项按 qualified_id 选取 |
| `evidence.md` | 绑定核对、实际读取范围、方法、核对结果、自查与更正、提交回读与限度 | 复核方法与限制 |
| `scripts/ub_check.py` | 本批唯一的新脚本：锚点/正文引文/检索/结构检查/值表/结构与延期字段（含候选覆盖一致、勘误原文在 b12572f14eae 中逐字存在）/写区边界的机械核对；复用 03 包 scan_index 的读取原语并重绑其 REPO | 见“后续怎么用” |
| `records/ub_check.json` | 核对结果（逐项计数与失败列表） | 核对数字 |
| `records/queries.json` | 70 个检索的命令、返回码与逐条命中（含注释/活动标记） | 否定结论的范围 |
| `records/run.json` | 核对命令、返回码、时间、工具版本与输入摘要 | 复现核对 |
| `records/boundary_staged.json` | 提交前写区边界：b12572f14eae 文件逐个不变、新增只在本前缀、远端头 | 核对写区 |

## 明确没有做的事

- 没有编译或运行 C/ASM，没有运行 cmake、内核、QEMU、宿主安装/挂盘脚本或仓库原有脚本，没有重跑 H00/V/W/M/N、05 或 06 的核验，没有生成 asm-offsets。
- 没有修改内核，没有出补丁，候选 A、B、键下限都没有采用；没有读取仓库外的 musl 源码。
- 没有合并，没有删除分支，没有 force-push，没有安装工具，没有提权，也没有索取或输出凭据；没有修改 Owner 的开发工作树。

## 后续怎么用

**恢复顺序：** 18 号入口（主线分支）→ 最新主线检查点与对应回执 → 本包 `user-entry-console.md` 与 `facts-and-boundaries.yaml` → 被选条目的 qualified_id（`deferred-findings.yaml`）→ 原证据与当时最新的 time。

**复核：** 在仓库根目录运行（检索记录写在输出文件所在目录）：

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B -P <本目录>/scripts/ub_check.py "$PWD" check <本目录> <out.json>
```

它只读 git 对象，复核锚点、正文中的 `[VERIFIED]` 引文、检索、结构检查、值表、记录结构与延期字段；只做机械一致性核对，不是语义证明。`boundary <out.json> --staged` 模式核对写区。
