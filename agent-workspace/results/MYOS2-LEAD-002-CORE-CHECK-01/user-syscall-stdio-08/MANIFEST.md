---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SYSCALL-STDIO-08
record_type: user_syscall_stdio_manifest
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
execution_surface: "本机专用隔离 clone /home/cheyh/projs/myos2-handoff06-local/MyOS2；只读 git 对象查询与 Python 文本处理（本机既有 git 2.53.0、Python 3.14.4、PyYAML 6.0.3）；Owner 订阅额度（owner_asserted）"
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 ab3b4b86662c；主线 agent/MYOS2-LEAD-002 91753f8155ee；master e8e60cd19540（均为短 SHA，从工具输出复制）"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/22-user-syscall-stdio-contract.md（agent/MYOS2-LEAD-002）"
review_ref: "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOUNDARIES-07-review.md（CORE-USER-BOUNDARIES-07-REVIEW-001）"
findings_policy_ref: "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: "见 PR17 上本批推送后的评论（本文件不自引用其所在提交）"
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_user_syscall_stdio_08
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
user_program_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
build_qemu_install_run: false
tools_installed: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_action_now: none
owner_decisions_requested_now: []
write_authority_note: "只在 user-syscall-stdio-08/ 新增；ab3b4b86662c 中的全部文件（含 06、07 原件）不变；未写主线分支、master、time；PR17 保持 Draft 复用。提交前边界核对见 records/boundary_staged.json"
inputs_read: "见 evidence.md §2"
c00_c04:
  C00: "完成：07 回执 record_id、被审头 ab3b4b86662c、下一任务 08 核对一致；主线对象 91753f8155ee（相对上次所读只新增 22 号任务书、07 回执与 10-28 检查点）；远端 time 仍为 a039d9803ade；ab3b4b86662c 即执行头；先读 options_flags.cmake"
  C01: "完成：用户程序、构建清单、安装脚本、仓库内旧 libc 与历史材料逐项检查，三栏表给出“仓库内已证实 / 仓库外依赖 / 未知”；追踪止于 libc 符号"
  C02: "完成：syscall 入口绑定、建帧、号检查、switch 分派、包装取参、写回 ax 与返回衔接；41 项分派表由源码重算（VT-SYSTAB）"
  C03: "完成：fd 生命周期九个阶段（含 pid 1 初始表、fork、exec、exit）、open/read/write/writev/fcntl/dup 路径与错误返回、console 两张 f_op 表接到系统调用入口"
  C04: "完成：user-syscall-stdio.md、facts-and-boundaries.yaml、deferred-findings.yaml（US-01…US-05，追加 07 UB-01、UB-05 与 05 UV-09、UV-10、UV-11 证据）、evidence.md、核对脚本与记录"
gap_u10_status: "open：仓库源码只到 libc 符号；缺的是构建实际采用的 libc（构建文本指定 /home/cheyh/projs/musl/build 下的 musl-gcc，仓库外、无版本记录）与镜像中的实际二进制"
data_summary: {capabilities: 12, edges: 23, edges_indirect: 4, anchors: 70, anchors_by_theme: {C01: 12, C02: 21, C03: 37}, queries: 38, structure_checks: 23, value_tables: 2, syscall_table_rows: 41, fd_lifecycle: 9, prior_unknowns: 4, gap_status: 1, candidates_not_run: 3, findings: 5, evidence_appends: 5, errata: 0, markdown_verified_blocks: 34}
delivered_files: [MANIFEST.md, user-syscall-stdio.md, facts-and-boundaries.yaml, deferred-findings.yaml, evidence.md, scripts/ss_check.py, records/ss_check.json, records/queries.json, records/run.json, records/boundary_staged.json]
---

# CORE-USER-SYSCALL-STDIO-08 交付清单

## 当前状态

C00–C04 在同一轮完成。用户态文件与标准流请求在本仓库能追到的范围已经写清：用户程序只调用 libc 符号；构建文本把编译器指定为仓库外的 musl-gcc 并静态链接（这一设置写在 project() 之后，实际生效的编译器只凭文本不能确定），无论哪个生效，libc 都在仓库外，所以仓库内的追踪止于 libc 符号，GAP-U10 保持 open；从 syscall 指令进入内核之后，入口、号检查、分派、处理函数、fd 表与 console 回调都有源码证据，并接回 07 的返回路径与两张 f_op 表。全部结论来自 `time a039d9803ade` 的源码与构建文本，没有编译或运行内核、用户程序、C/ASM、构建或安装脚本。本批发现的普通问题都已写入 `deferred-findings.yaml`，当前没有需要 Owner 作出的技术决策。

## 逐文件说明

| 文件 | 内容 | 怎么消费 |
|---|---|---|
| `MANIFEST.md` | 本清单 | 先读 |
| `user-syscall-stdio.md` | 中文源码说明：结论、三栏边界表、syscall 入口与分发、41 项系统调用表、FD 生命周期表、console 衔接表、两层调用路径图、仓库外依赖与 NOT_RUN 边界、与 05/07 的衔接、缺口状态、问题与剩余边界索引 | 讲解与后续任务的入口；依据按编号查 facts |
| `facts-and-boundaries.yaml` | 12 个能力节点（proposed_id，前缀取自共享词汇表）、23 条边（方向同 05/07；4 条间接边带调用点/绑定/接收方）、70 条锚点、38 个检索、23 项结构检查、2 张值表（分派表 41 行、fd 限值）、9 行 FD 生命周期、4 项旧未知项更新、GAP-U10 状态、3 个 NOT_RUN 候选 | 按 ID 引用；核对脚本的输入 |
| `deferred-findings.yaml` | US-01…US-05；对 07 UB-01、UB-05 与 05 UV-09、UV-10、UV-11 的证据追加；勘误为空并说明 | 以后新专项按 qualified_id 选取 |
| `evidence.md` | 绑定核对、实际读取范围、方法、核对结果、自查与更正、写区与回传、限度 | 复核方法与限制 |
| `scripts/ss_check.py` | 本批唯一的新脚本：锚点/正文引文/检索/结构检查/两张值表的重算/记录结构与延期字段（含 05、07 外部 ID 解析、候选覆盖一致）/写区边界的机械核对；例程抄自并精简自 07 的核对脚本（不导入、不运行它），复用 03 包 scan_index 的读取原语 | 见“后续怎么用” |
| `records/ss_check.json` | 核对结果（逐项计数、值表重算结果与失败列表） | 核对数字 |
| `records/queries.json` | 38 个检索的命令、返回码与逐条命中（含注释/活动标记） | 否定结论的范围 |
| `records/run.json` | 核对命令、返回码、时间、工具版本与输入摘要 | 复现核对 |
| `records/boundary_staged.json` | 提交前写区边界：ab3b4b86662c 文件逐个不变、新增只在本前缀、远端头 | 核对写区 |

## 明确没有做的事

- 没有修改内核或用户程序，没有出补丁，没有编译或运行 C/ASM，没有运行 cmake、内核、QEMU、宿主安装/挂盘/分区脚本或仓库原有脚本，没有重跑 H00/V/W/M/N、05、06、07 的核验。
- 没有下载、读取或引入仓库外的 musl 源码，没有读历史主题分支 `libc`，没有把通用 Linux/glibc 行为当作 MyOS2 现状。
- 没有合并，没有删除分支，没有 force-push，没有安装工具，没有提权，也没有索取或输出凭据；没有修改 Owner 的开发工作树。

## 后续怎么用

**恢复顺序：** 18 号入口（主线分支）→ 最新主线检查点与对应回执 → 本包 `user-syscall-stdio.md` 与 `facts-and-boundaries.yaml` → 被选条目的 qualified_id（`deferred-findings.yaml`）→ 原证据与当时最新的 time。

**复核：** 在 clone 根目录运行（检索记录写在输出文件所在目录）：

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B -P <本目录>/scripts/ss_check.py "$PWD" check <本目录> <out.json>
```

它只读 git 对象，复核锚点、正文中的 `[VERIFIED]` 引文、检索、结构检查、两张值表的重算、记录结构与延期字段；只做机械一致性核对，不是语义证明。`boundary <out.json> --staged` 模式核对写区。
