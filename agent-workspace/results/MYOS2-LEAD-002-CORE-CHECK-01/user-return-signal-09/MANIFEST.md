---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-RETURN-SIGNAL-09
record_type: user_return_signal_manifest
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
execution_surface: "本机专用隔离 clone /home/cheyh/projs/myos2-handoff06-local/MyOS2；只读 git 对象查询与 Python 文本处理（本机既有 git 2.53.0、Python 3.14.4、PyYAML 6.0.3）；Owner 订阅额度（owner_asserted）"
execution_model_note: "任务书默认“次一档”（receiving_model_tier: next_tier_default），08 回执 §7 也写明“Deep Research和额外并行评审当前不需要”。Owner 在本地会话 f464c1b9 中明确开启 ultracode，并要求子任务以 Opus 5.5、max 推理强度运行（执行方转述，主线不能直接核验）。本批据此偏离上述默认，用七个工作流共 130 个完成的只读子代理：追踪组 41 个（追踪 7、合并 1、两视角核验 32、D00 复核 1），写作组 8 个，成稿后的独立审查 55 个（审查 5、逐条复核 50），按审查结果修正三轮共 23 个（5、6、12），提交前的终审 3 个；另有首次启动的 5 个审查代理因订阅周限额失败、没有产出，额度重置后续跑。规模、结果与合规审计见 evidence.md §3、§5。子代理的模型都只是运行时声明，实际后端 unknown_or_not_attestable。全部在 Owner 订阅额度内（owner_asserted），没有转 API 或额外付费，没有调用外部研究。开放冲突只形成候选交回主线。"
read_channel: "本地 git 对象（git show / git grep / git ls-tree / git diff）；开工时一次 git fetch，只更新四个远端跟踪引用；gh 对 PR17 元数据的只读查询"
date: "2026-10-29"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 8983a9235c8a；主线 agent/MYOS2-LEAD-002 14ecef9fc596；master e8e60cd19540（均为短 SHA，从工具输出复制）"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/23-user-return-signal-contract.md（agent/MYOS2-LEAD-002）"
review_ref: "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SYSCALL-STDIO-08-review.md（CORE-USER-SYSCALL-STDIO-08-REVIEW-001）"
findings_policy_ref: "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: "见 PR17 上本批推送后的评论（本文件不自引用其所在提交）"
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_user_return_signal_09
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
user_program_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
preprocessing_run: false
build_qemu_install_run: false
external_libc_read: false
tools_installed: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_action_now: none
owner_decisions_requested_now: []
write_authority_note: "只在 user-return-signal-09/ 新增；8983a9235c8a 中的全部文件（含 05、06、07、08 原件）不变；未写主线分支、master、time；PR17 保持 Draft 复用。写区以完整对象比较与明确布尔条件核对，任一条件不满足即以非零退出码停止（RC08-01）；提交前的写区记录见 records/boundary_staged.json，推送后的写区复核（boundary --commit）与远端读回写在 PR17 评论中，不放进本提交，以免记录自引用所在提交"
inputs_read: "见 evidence.md §2"
d00_d04:
  D00: "完成：08 回执 record_id、被审头 8983a9235c8a 与本任务一致；主线对象 14ecef9fc596（新增 C02→C03 交接件、23 号任务书、08 回执与检查点，属正常追加）；18 号与上次所读对象相同；time 仍为 a039d9803ade；PR17 open、Draft；先读 options_flags.cmake。SR08-01、SR08-02 都接受，处置写在 stdio-08-disposition.md，对应勘误 ER-01、ER-02；没有改 08 原件，没有重跑 08 的检查器"
  D01: "完成：分派之后的返回尾、注释与活动语句的区分、ti_work、调度门与信号门、SYSRET 判断不被使用、ret_from_fork 进入同一尾段；返回阶段表 RT-01…RT-36"
  D02: "完成：rt_sigaction、rt_sigprocmask 的保存与校验，get_signal 的选取与默认动作，建帧的地址、字段、寄存器改写与失败消费，rt_sigreturn 的恢复与错误分支；帧字段表 FF-01…FF-19"
  D03: "完成：重启条件表 RR-01…RR-19、重启码定义与取值、219 号与活动分派表，以及与 07 返回帧、08 分派局部表的衔接（含普通未改写返回的充分条件与充要条件）；GAP-U04 只追加静态证据，GAP-U10 保持 open"
  D04: "完成：本清单、stdio-08-disposition.md、user-return-signal.md、facts-and-boundaries.yaml、return-paths.dot、deferred-findings.yaml、evidence.md、核对脚本与记录"
gap_status: {GAP-U04: "narrowed（不变；只追加静态证据）", GAP-U10: "open（不变）"}
mainline_open_points:
  - "RS-12 与 CORE-USER-VFS-BASELINE-05::UV-14 同源，是否并入 UV-14。"
  - "CORE-USER-BOUNDARIES-07::UB-09 的原句：本批以勘误 ER-03 收窄、另以两条证据追加处理，也可改为新条目。"
  - "FN-01、FN-02 是否改登为潜伏条目。"
  - "RS-01 由头文件包含次序推出，需构建侧核实（NVR-2）。"
  - "任务切换的 FPU 缺口在本批只记在 RS-10 中（SD-91、QD-131），CORE-CHECK-01 的已有包都没有登记（MYOS2-DR-005 有速查结论，不是延期条目）；是否在调度侧另立条目。"
not_covered: "发送端与唤醒的完整语义（RS-03、RS-14 只为前提定点读了 sys_kill → __send_signal_locked 一条链；RS-04 只核了 complete_signal、signal_wake_up_state、calculate_sigpending 的活动调用状态；读取边界见 evidence.md §2）、sigaltstack（131 号不在活动表）、FPU/XSAVE 内部、myos_handle_mm_fault 的内部与缺页处理成功后的去向、copy_user_generic 的汇编后端、信号相关的调度与并发、libc 与镜像、硬件语义；均未运行"
data_summary: {capabilities: 10, edges: 21, edges_indirect: 2, anchors: 75, anchors_not_contained: 69, anchor_line_ranges_merged: 64, anchors_by_theme: {D00: 22, D01: 11, D02: 22, D03: 20}, queries: 262, line_hit_queries: 41, line_hit_patterns: 182, hit_refs: 353, hit_refs_with_expect: 353, structure_checks: 110, value_tables: 1, return_frame_transitions: 36, frame_fields: 19, restart_conditions: 19, fact_notes: 2, sr08_dispositions: 2, gap_status: 2, candidates_not_run: 4, findings: 14, findings_needs_evidence: 1, evidence_appends: 2, errata: 3, markdown_verified_blocks: 16, duplicate_queries_and_checks: "计数含同义重复：QD-178 同 QD-87，QD-165 同 QD-91（都只差一个 -e）；SD-95 同 SD-69，SD-81 同 SD-68，SD-85 同 SD-71，SD-97 同 SD-72，SD-109 同 SD-66（同一函数体，查同一件事）；另有 6 个锚点被其他锚点完全包含（见 facts 的 anchor_policy）"}
delivered_files: [MANIFEST.md, stdio-08-disposition.md, user-return-signal.md, facts-and-boundaries.yaml, return-paths.dot, deferred-findings.yaml, evidence.md, scripts/rs_check.py, records/rs_check.json, records/queries.json, records/run.json, records/boundary_staged.json]
---

# CORE-USER-RETURN-SIGNAL-09 交付清单

## 当前状态

D00–D04 在同一轮完成。

**D00：08 的两项退回。** SR08-01（fdput 引用计数措辞的条件）与 SR08-02（分派局部的 ax 与最终返回值）都接受；处置与勘误只追加，08 原件未改。

**D01–D03：系统调用完成后、回到用户程序前的一段。**
- 返回尾每次都无条件进入信号与重启处理。
- 信号的设置、选取、建帧与恢复。
- 重启条件，以及与 07 返回帧、08 分派表的衔接。

全部结论来自 `time a039d9803ade` 的源码文本，没有构建、预处理，也没有运行内核、用户程序或 QEMU（只运行了只读 git 检索与文本核对脚本）。

**审查与更正。** 成稿后做了五个视角的独立审查：共 90 个问题，中、高级 50 个逐条复核，确认 48 个。确认的问题经三轮修正全部落实，修正中没有采纳的低级与可选建议在 evidence.md §5 逐条说明理由。提交前又做一次终审，3 个复核者找出跨轮修正留下的 27 处矛盾、过时数字与非原文引语，逐条回源核对后全部改正。核对脚本因此新增两项检查：
- 每处命中引用都带所在行必须含有的 `expect` 文字；
- 锚点 role 的格式检查。

**入库与开放点。** 本批发现的普通问题都已写入 `deferred-findings.yaml`，当前没有需要 Owner 作出的技术决策。几处是否合并、是否改登的开放点作为候选交回主线。

**执行方式的偏离。** 执行方式偏离了任务书默认的“次一档”，也偏离了 08 回执 §7“额外并行评审当前不需要”的预期：按 Owner 在本地会话中的明确指示，用七个工作流共 130 个只读子代理完成追踪、写作、审查、修正与提交前的终审。详见 frontmatter 的 execution_model_note 与 evidence.md §3、§5。

## 消费顺序

1. 本清单
2. `stdio-08-disposition.md`（D00）
3. `user-return-signal.md`（D01–D03 与衔接）
4. `facts-and-boundaries.yaml` 中被引用的条目
5. `deferred-findings.yaml` 中被选条目的 qualified_id
6. 原证据所指的 time 源码

## 逐文件说明

| 文件 | 内容 | 怎么消费 |
|---|---|---|
| `MANIFEST.md` | 本清单 | 先读 |
| `stdio-08-disposition.md` | SR08-01、SR08-02 的定点处置：原位置、接受与否、短引文、条件表、附注全文、受影响的结构化 ID | 主线复核 08 处置的入口 |
| `user-return-signal.md` | 中文源码说明：结论、返回尾（D01）、信号设置/选取/建帧/恢复（D02）、重启条件表（D03）、与 07/08 的衔接、返回阶段表与帧字段表、缺口状态、问题索引、主线开放点、剩余边界 | 讲解与后续任务的入口 |
| `facts-and-boundaries.yaml` | 10 个能力节点、21 条边、75 条锚点；262 个检索，其中 41 个是整行锚定的引文定位检索，共 182 条模式；110 项结构检查、值表 VT-RESTART；返回阶段 36 行、帧字段 19 行、重启条件 19 行；事实 2 条、SR08 处置 2 条、缺口 2 项、NOT_RUN 候选 4 个。353 处命中引用都带 expect 文字 | 按 ID 引用；核对脚本的输入 |
| `return-paths.dot` | 能力与边的图，节点与边 ID 与事实文件相同 | 看链路结构 |
| `deferred-findings.yaml` | RS-01…RS-14；对 07 UB-09 的两条证据追加；勘误 ER-01、ER-02（08）与 ER-03（07） | 以后新专项按 qualified_id 选取 |
| `evidence.md` | 绑定核对、实际读取范围、方法、核对结果、审查与更正（含子代理与命令合规审计）、写区与回传、限度 | 复核方法与限制 |
| `scripts/rs_check.py` | 本批唯一交付的新脚本，详见下文 | 见“后续怎么用” |
| `records/rs_check.json` | 核对结果 | 核对数字 |
| `records/queries.json` | 262 个检索的命令、返回码与逐条命中（含命中行文字） | 否定结论与引文定位的范围 |
| `records/run.json` | 核对命令、返回码、时间、工具版本与输入摘要 | 复现核对 |
| `records/boundary_staged.json` | 提交前写区边界（暂存区对 8983a9235c8a） | 核对写区 |

`scripts/rs_check.py` 的核对内容：
- 锚点；
- 锚点 role 的格式；
- 正文引文；
- 检索，以及命中行的 `expect` 文字；
- 结构检查；
- 值表重算；
- 记录结构；
- DOT 与事实的 ID 一致；
- 延期字段与勘误原文；
- 按完整对象比较的写区边界，违规即以非零退出码结束。

它的来源与依赖：
- 例程抄自并精简自 08 的 ss_check.py，不导入、不运行它。
- 与 07、08 相同，以 importlib 载入 03 包 scan_index.py，只调用其 show、analyse、enclosing，并把其 REPO 重绑到本 clone 根；不调用其 main，不运行 03 的检索。
- 另有未交付的一次性组装与生成程序，见 evidence.md §3。

提交后的写区复核（`boundary --commit`）与远端读回写在 PR17 的评论中，不放进本提交，以免记录自引用所在提交。

## 明确没有做的事

- 没有修改内核或用户程序，没有出补丁。
- 没有编译、预处理或运行 C/ASM，没有运行 cmake、内核、QEMU、宿主安装/挂盘/分区脚本或仓库原有脚本，没有重跑 05/06/07/08 的检查器。唯一的例外是核对脚本以 import 方式复用 03 包 scan_index.py 的读取函数：不调用其 main，不运行 03 的检索或检查，见 evidence.md §3。
- 没有读取仓库外 libc、历史主题分支 libc、构建产物或磁盘镜像，没有联网下载外部源码。
- 没有合并，没有删除分支，没有 force-push，没有安装工具，没有提权，也没有索取或输出凭据。
- 没有修改 Owner 的开发工作树。

## 后续怎么用

**复核：** 在 clone 根目录运行下面的命令；检索记录写在输出文件所在的目录：

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B -P <本目录>/scripts/rs_check.py "$PWD" check <本目录> <out.json>
```

这个命令只读 git 对象，复核以下各项：
- 锚点与 role 格式；
- 正文中的 `[VERIFIED]` 引文；
- 检索，以及引文定位命中与其 expect 文字；
- 结构检查与值表；
- 记录结构；
- DOT 与事实的 ID 一致；
- 延期字段与勘误原文。

它只做机械一致性核对，不是语义证明。

写区用另外两个命令核对，任何条件不满足即以退出码 2 结束：
- `boundary <out.json> --staged`
- `boundary <out.json> --commit <rev>`
