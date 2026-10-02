---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-01
phase: core_recheck_01
record_type: verifier_recheck_manifest
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）；不认证具体后端"
execution_surface: "claude.ai/code 托管云端会话容器（x86_64；python3 3.11.15、PyYAML 6.0.1、gcc 13.3.0、git 2.43.0、curl 8.5.0，均为容器既有工具）；与 pilot/core 同一会话、同一执行分支"
lead_attribution:
  source: "12 号任务书与 core-review 回执的 YAML 头"
  model_per_owner: gpt6
  effort_per_owner: pro
  scope: originating_lead_only_not_executor
date: "2026-09-27"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；agent/MYOS2-LEAD-002：57a7c3e0eebf（六份技术输入）、f79b3a281616（pilot-review）、ec62453e76d2（12 号任务书与 core-review，开工时固定）；冻结 core a1e7c2277705；旧 core results 7e2fa84a9823；旧 pilot 0851af4fc08b。均为短 SHA，自 git rev-parse --short=12 复制"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
batch1_commit_short12: f9ae2bce9a64
results_commit_short12: efb9846b88ec
read_channel: mixed
read_channel_detail: "12 号任务书与 core-review 经 raw URL 读取，并与 git 对象逐字节比较；其余输入按固定提交 git show；open PR 经 GitHub MCP；results.yaml 回读经 raw.githubusercontent.com 与 api.github.com"
inputs_read:
  - "agent/MYOS2-LEAD-002 @ ec62453e76d2：12-core-check-recheck-contract.md（全文）"
  - "agent/MYOS2-LEAD-002 @ ec62453e76d2：reviews/CORE-CHECK-01-core-review.md（全文）"
  - "11、09 与 pilot-review：沿用本会话前两轮已完整读取的内容；六份技术输入与 pilot-review 由 identity2 机械核对，与冻结点逐字节相同"
  - "claude/dazzling-cori-q0dnyt @ a1e7c2277705：core/ 三份正文与 core/fixtures/ 二十个文件（旧验证器从该对象逐字节解出，不读工作树）"
  - "time @ a039d9803ade：夹具抽取所用源文件、mykernel/CMakeLists.txt（A46-C/ASM）、mykernel/kactive/swait/swait.c（V03 守卫边界）"
  - "master @ de3bb1df906a：agent-workspace/conventions.md（startup_selfcheck_quote）"
status: final_for_recheck_01
recheck_conclusion: VERIFIER_REPAIRED_M01_M12_MET_SAME_SOURCE_RERUN_COMPLETE_WITH_FINDINGS
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_acceptance_verdict: NOT_ISSUED
reusable_harness_self_certified: false
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "写入 core/recheck-01/、推进本执行分支、更新 PR #17，由 Owner 本轮指令、12 号任务书与 core-review 回执授权（allowed_write_prefix 由 identity2 机械核对）。未写 agent/MYOS2-LEAD-002、master、time；pilot 与旧 core 原件未改。"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本批不新增源码断言标签。data_summary、delivered_files 与正文的 V/M/CA 表由 fixtures/final_check2.py 对照 results.yaml 机械核对（evidence.md §9.5）。"
data_summary:
  run_status: COMPLETE
  run_exit_code: 0
  execution_complete: true
  verification_findings: [V00, V08]
  infrastructure_errors: 0
  dynamic_calls: {h00: 1, fixtures: 1}
  identity_gate_ok: true
  h00_dynamic_safety_ok: true
  top_level_cases: 15
  evidence_status: {VALID: 15}
  results_for_valid_evidence: {COUNTEREVIDENCE: 2, NO_FAILURE_IN_SCOPE: 2, OBSERVED_AS_PREDICTED: 11}
  layers_not_executed: [V04.smp_or_real_scheduler, V05.irq_or_signal_context, V06.concurrent_wakeups, V07.ap_migration,
    V08.global_reachability, V09.real_timer_expiry, V10.real_timer_or_scheduler, V11.real_context_switch,
    V12.multicore_atomicity, V13.smp_stress, V14.existing_elf]
  case_results_differing_from_frozen_core: []
  ca_rulings_differing_from_frozen_core: [CA-02, CA-05, CA-07]
  a46_correction: [VALID, OBSERVED_AS_PREDICTED, [A46-C, A46-ASM]]
  m_items_met: {M01: true, M02: true, M03: true, M04: true, M05: true, M06: true, M07: true, M08: true, M09: true,
    M10: true, M11: true, M12: true}
  generator_errors: 0
delivered_files:
  - MANIFEST.md
  - evidence.md
  - fixtures/a46_check.py
  - fixtures/build2.py
  - fixtures/build_evidence2.py
  - fixtures/compare_runs2.py
  - fixtures/evaluate2.py
  - fixtures/final_check2.py
  - fixtures/frozen.py
  - fixtures/h00_2.py
  - fixtures/harness2.py
  - fixtures/identity2.py
  - fixtures/make_results2.py
  - fixtures/meta_tests.py
  - fixtures/old_counterexamples.py
  - fixtures/readback2.py
  - fixtures/run_recheck.py
  - observations/a46.json
  - observations/after_batch1/identity_after_push.json
  - observations/after_batch1/readback_results.branch.json
  - observations/after_batch1/readback_results.object.json
  - observations/after_batch2/identity_after_push.json
  - observations/after_batch2/readback_results.branch.json
  - observations/after_batch2/readback_results.object.json
  - observations/meta_tests.json
  - observations/old_counterexamples.json
  - observations/rerun_vs_batch1.json
  - observations/run.json
  - observations/static.json
  - observations/v00.json
  - observations/v01.json
  - results.yaml
kernel_modified: false
repo_scripts_run: false
full_kernel_build: false
qemu_run: false
tools_installed: false
privilege_escalation: false
credentials_output: false
merge_or_branch_delete: false
open_questions:
  - "CA-02 全局可达性：idle 能否以非 RUNNING 状态被切出。kernel_thread/copy_process 等 idle 上下文路径没有做全链证明。"
  - "V08 额外观察：pick_next_task_myos 按 vruntime 回插时，实测 [3,4,5] 变为 [4,5,2]，按 vruntime 排序应为 [4,2,5]。作为后续候选保留，本轮不修内核。"
  - "真实 ELF、时基、IRQ、SMP 与上下文切换仍未执行（data_summary.layers_not_executed 所列 11 层）。"
---

# CORE-CHECK-01 RECHECK-01：验证器按 R01–R04 修好，M01–M12 全部满足，同源复跑结论不变

**主线列出的四项验证器问题，先在冻结旧程序上实跑，全部复现。修订版在 M01–M12 中逐项满足。同源 V00–V14 复跑中，15 个顶层条目的证据全部有效，结论与冻结 core 相同。** 仍有两项发现需要主线处理：旧 A46 引文越界（V00），以及 CA-02 在有限模型下的可达性反证（V08）。这不是对 MyOS2 内核的任何验收，上限仍为 PASS_PENDING_LOCAL。

## 1. 本轮做了什么

1. **Phase A**：从 `a1e7c2277705` 逐字节解出冻结旧验证器，在全新目录中实跑主线 R01–R04 的反例（evidence.md §2）。
2. **修订**：修订代码只新增在 `recheck-01/fixtures/`。旧 pilot/core 原件、内核源码与 07/09 预测都没有改动。
3. **同源复跑**：`run_recheck.py` 完整运行一次（exit 0，COMPLETE）；`meta_tests.py` 覆盖 M01–M12，全部满足。
4. **发布后修正**：第一批 `f9ae2bce9a64` 推送后，整理本清单时发现 static_checks 阶段缺失时，V08 仍被判为 VALID，并给出函数层结论。第二批 `efb9846b88ec` 修正了这一点，加严 M09，并重跑同源运行与 M01–M12。`compare_runs2.py` 显示第二批的同源结果与第一批相同：evaluation 相同，每个夹具案例的 stdout 与退出码逐字节相同。

## 2. R01–R04 的处理

| 问题 | 旧程序实跑（Phase A） | 修订 | 验证 |
|---|---|---|---|
| R01 空、坏或失败的证据被判为符合预测 | `evaluate.v05({"cases": {}})` 返回 OBSERVED_AS_PREDICTED。同一空输入经 `run_all.evaluate` 调度时，其余评估器给出 COUNTEREVIDENCE，V03 为 COMPARATOR_INVALID | evaluate2 为每个案例固定证据合同：退出码类（0/42/43）、完整事件序列、字段类型、运行状态（RAN、无超时、无收集超时、已确认回收、stderr 为空、两次运行一致）。V05 必须覆盖五种固定 label 及对应 q 事件。只有 VALID 才与原预测比较 | M01–M05 |
| R02 准入与安全只记录、不控制执行 | gate=false、identity 抛异常、H00 失败三种情形下，旧 `run_all.main` 仍调用 2 个动态入口；旧重放中 h00 阶段退出 1 后夹具照常运行 | run_recheck 的顺序为：解出冻结文件 → 身份门 → H00 → 只读阶段 → 动态夹具。门关或出错时动态调用为 0，退出码 3。H00 未确立动态安全时夹具为 BLOCKED，只读阶段继续。记录分列 execution_complete、verification_findings、infrastructure_errors。harness2.run_pg 只杀本试验创建的进程组，终态分为 exited、timeout_group_killed、timeout_group_already_gone、timeout_kill_error 及 _collect_timeout 后缀；只有实际拿到返回码才记 reaped_confirmed | M06、M07；H00 七个探针 |
| R03 结果生成含固定结论 | V01 引用路径不存在且解析失败时，旧 make_results 仍写 NO_FAILURE_IN_SCOPE。缺 static 阶段时报 FileNotFoundError，夹具编译受阻时报 KeyError 'run'，整份报告都不生成 | make_results2 的结果、层次、计数、CA 裁定都按明示判据由数据计算；固定说明只放在 *_template 键。缺阶段、BLOCKED、异常、部分运行都生成完整而诚实的记录。阶段输出只在该阶段完成时使用 | M08、M09 |
| R04 冻结源版本与移动的执行分支混用 | 执行分支前进后，旧 identity 记录了 pilot 头不匹配，gate 却仍为 true。旧 probe_readback 两通道字节相同，却判失败。旧 readback.py 无 field 而带 --out 时报 ValueError | identity2 分开核对：回执与冻结输入；pilot/core 原件逐字节；当前头是否为被审 core 的后继；远端是否同线且仍含被审 core。任何异常都关门。readback2 改用 argparse；object 模式按提交对象读取，branch 模式只接受分支头等于目标，或该文件 blob 相同的后继 | M10、M11；§8 两次推送后的实例 |

## 3. M01–M12

除标为 HARNESS_META_TEST 的变造输入外，其余都是真实记录。每行的旧入口结果都是冻结旧程序的实际返回。

| M | 输入 | 旧入口真实结果 | 修订版结果 | 满足 |
|---|---|---|---|---|
| M01 | 冻结旧 v05 与调度入口，cases 为空 | v05 返回 OBSERVED_AS_PREDICTED；调度入口给出 COUNTEREVIDENCE、COMPARATOR_INVALID 等 | v05 为 INCOMPLETE_EVIDENCE；空输入下行为结论为 0 个 | true |
| M02 | 已交 V05 的五组事件：去掉一组 label、去掉一个 q、截断末两行 | 两个变体仍为 OBSERVED_AS_PREDICTED，一个为 COUNTEREVIDENCE | 三个变体都为 INCOMPLETE_EVIDENCE；完整五组的对照为 VALID | true |
| M03 | 冲突的重复事件、字段类型错误、无法解析的行、重复 q | OBSERVED_AS_PREDICTED、COUNTEREVIDENCE、COMPARATOR_INVALID | 全部为 INVALID_EVIDENCE | true |
| M04 | 输出看似正确，但运行标为 timed_out、collect_timed_out、ERROR、重复不一致或被杀 | 全部为 OBSERVED_AS_PREDICTED | ERROR 或 INVALID_EVIDENCE，没有内核结论 | true |
| M05 | 有效事件配错误退出码；另有 0、42、43 三类正控 | 错配被判为 COUNTEREVIDENCE、OBSERVED_AS_PREDICTED 或 COMPARATOR_INVALID | 错配为 INVALID_EVIDENCE；三类正控为 VALID | true |
| M06 | 入口 identity 返回 gate=false、抛异常、指向不存在的仓库；用计数替身见证调用 | 动态入口各被调用 2 次 | 退出码 3（BLOCKED_IDENTITY 或 ERROR_IDENTITY）；h00、stage、fixtures 的调用都为 0 | true |
| M07 | H00 探针失败；H00 超时抛异常；保留独立的 V01 阶段 | H00 失败后仍调用夹具 | 退出码 2；夹具调用 0；V01 为 VALID，V02 为 BLOCKED；部分报告可生成 | true |
| M08 | 由真实 V00/V01 记录派生：引用路径不存在、解析失败、A46 改为命中、A35 改为未找到 | V01 仍写 NO_FAILURE_IN_SCOPE | V01 变为 COUNTEREVIDENCE；V00、计数、CA-06 随输入变化 | true |
| M09 | 真实部分运行：fx_prims 编译失败、v06 案例异常、跳过 static 阶段。变体 B：static 阶段写出文件后记为失败 | 旧生成器崩溃（FileNotFoundError、KeyError） | 两次都退出 2，报告可生成。部分运行：V06 为 ERROR，V12/V13 为 BLOCKED，V08/V14 为 INCOMPLETE_EVIDENCE。变体 B：V08/V14 为 INCOMPLETE_EVIDENCE，其余与 M12 相同。未执行的层与受影响的 CA 全部列出 | true |
| M10 | 临时仓库：合法后继头、相等头、本地领先；错误分支、非后继头、pilot 原件被改、core 原件被改、远端重置到 master、远端领先 | 旧 identity 没有头、分支、原件输入；把合法前进记为不匹配，运行照常继续 | 三种合法情形开门；六种错误情形各由对应的门关闭 | true |
| M11 | readback2 CLI：有无 field=value 搭配 --out、branch 模式、不存在的路径、错误字段值、畸形参数；注入 curl 超时 | 无 field 带 --out 时报 ValueError；带 field 时因分支头前进判失败 | 参数解析正确；成功读取 exit 0；404 与字段不符 exit 1；畸形参数 exit 2；超时的通道 ok=false | true |
| M12 | 真实同源运行（不是元测试） | 不适用 | 15 项证据全部 VALID；旧 A46 仍失败；A46-C/ASM 单列命中；新旧构建器展开的源码 SHA-256 相同 | true |

## 4. V00–V14 覆盖

先判证据状态，再给结果。“未执行的层”不在本次授权范围内：不运行内核、不用 QEMU、不做 SMP/IRQ、不构建 ELF。

| 项 | 证据状态 | 结果 | 执行的层 | 未执行的层 | 与冻结 core |
|---|---|---|---|---|---|
| V00 | VALID | COUNTEREVIDENCE | source_match | - | 相同；旧 47 条中 A46 仍为 OUTSIDE_OR_PARTIAL_DEFINITION，原文未改 |
| V01 | VALID | NO_FAILURE_IN_SCOPE | structure | - | 相同 |
| V02 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | - | 相同 |
| V03 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | - | 相同；守卫边界见 §5 |
| V04 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | smp_or_real_scheduler | 相同 |
| V05 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | irq_or_signal_context | 相同；五种 label 与 q 事件齐全 |
| V06 | VALID | NO_FAILURE_IN_SCOPE | host_original_slice_serial | concurrent_wakeups | 相同 |
| V07 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | ap_migration | 相同 |
| V08 | VALID | COUNTEREVIDENCE | host_original_slice_function_level, limited_model_reachability | global_reachability | 相同；判据拆为函数层、有限模型、全局可达性 |
| V09 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_stub_timers | real_timer_expiry | 相同 |
| V10 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_scripted_interleaving | real_timer_or_scheduler | 相同 |
| V11 | VALID | OBSERVED_AS_PREDICTED | local_sequence_with_scripted_interleaving | real_context_switch | 相同 |
| V12 | VALID | OBSERVED_AS_PREDICTED | host_original_asm_single_thread | multicore_atomicity | 相同 |
| V13 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_serial | smp_stress | 相同 |
| V14 | VALID | OBSERVED_AS_PREDICTED | source_and_build_reference, host_link_model | existing_elf | 相同；没有可信 ELF，不升级 |

A46 更正单列（`a46_check.py`）：主线的 A46-C 与 A46-ASM 两条都在各自定义体内命中（CMakeLists.txt 第 30、31 行）。旧 47 条的统计与旧 A46 失败结论保持原样。

CA 裁定（results.yaml 计算值）：

| CA | 裁定 | 依据 |
|---|---|---|
| CA-01 | `SUPPORTED_IN_SCOPE` | V04、V05、V07 符合预测，V06 串行范围内无失败 |
| CA-02 | `FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=COUNTEREVIDENCE_IN_LIMITED_MODEL; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK` | V08 函数层符合预测。idle 回插序列成立、四条静态判据都成立时，“阻塞 current + 空队列”在有限模型中不可达。全局可达性不由本检查判定 |
| CA-03 | `SUPPORTED_IN_SCOPE` | V09、V10、V11 |
| CA-04 | `SUPPORTED_IN_SCOPE` | V02、V03、V11 |
| CA-05 | `SUPPORTED_AT_HOST_LINK_MODEL_AND_SOURCE_AND_BUILD_REFERENCE` | V14 的源码/构建引用层与宿主链接模型层；existing_elf 未执行 |
| CA-06 | `SUPPORTED_AT_SOURCE_LEVEL` | A01/A35/A36/A37 机械命中，语义复核为 SUPPORTS*；A35 位于 #ifdef CONFIG_BUG 内。语义复核沿用冻结的 v00_semantic_review.yaml，未重算 |
| CA-07 | `SUPPORTED_IN_SCOPE` | V12、V13 |

与冻结 core 相比，15 项结果全部相同。CA-02、CA-05、CA-07 的裁定名称不同（冻结时分别为 FUNCTION_LEVEL_SUPPORTED_REACHABILITY_COUNTEREVIDENCE、SUPPORTED_AT_SOURCE_AND_MODEL_LEVEL、SUPPORTED_BY_HOST_ORIGINAL_SLICE），因为现在的裁定按统一判据由数据计算，不再是固定文字。

## 5. 对主线推导的反证与补充

- **R01–R04**：四项推导全部由旧程序实跑复现，没有反证。
- **R01 补充（主线未列）**：空观测不只让 V05 假成功。经旧调度入口时，其余评估器会给出假的内核 COUNTEREVIDENCE，V03 为 COMPARATOR_INVALID。也就是说，在旧程序中缺证据既可能变成“符合预测”，也可能变成“内核反证”。
- **进程组（部分不复现）**：core-review 要求“未证实回收不能填已回收”。旧 run_pg 在收集超时路径上的 child_reaped 取自返回码，并没有虚报回收，这一点没有复现为错误。组已消失时，旧 run_pg 抛出 ProcessLookupError，没有终态记录，这一点属实。修订版对两条路径都记录明确终态（H00 探针 group_already_gone_injected、collect_timeout_escaped_grandchild）。
- **R04 细节**：带 field 的旧 CLI 能解析参数，但仍因分支头前进判失败；主线“无可选字段时误解析 --out”的推导准确。旧 identity 记录了不匹配，gate 却仍为 true，与“记录但不阻止”的推导一致。
- **V03 守卫边界**（按 12 号 §4 的要求准确描述）：守卫在夹具替换的 try_to_wake_up 入口。停下之前，原函数 swake_up_locked 已经执行了四步：list_header_is_empty 因 count 仍为 1 判为非空；经 list_headr_first_container（container_of）从 anchor 构造出一个不存在的容器；读取与队列锁字重叠的 curr->task；以该值调用 try_to_wake_up。真实 try_to_wake_up 的解引用与写入，以及其后的 list_del_init，都没有执行。容器构造与越界读取已属 C 未定义行为，守卫不消除它们。
- **发布后自查出的同类缺口**：见 §1 第 4 条。这是本轮修订版自身的问题，不是主线推导错误。

## 6. 文件清单

路径都相对于 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/`，共 32 个文本文件，没有二进制文件。

| 文件 | 内容 | 来源 |
|---|---|---|
| `MANIFEST.md` | 本清单 | 手写；data_summary、文件清单与 V/M/CA 表由 final_check2 核对 |
| `results.yaml` | 分层结果、证据状态、CA 裁定、计数、与冻结 core 的差异、R/M 项汇总 | make_results2.py 由 observations/ 生成；第二批提交后冻结 |
| `evidence.md` | 旧反例、修订、同源复跑、M01–M12、提交与回读、发布前检查的输出 | build_evidence2.py 生成；手工转录处已标注 |
| `fixtures/harness2.py` `frozen.py` `identity2.py` `evaluate2.py` `build2.py` `h00_2.py` `a46_check.py` `run_recheck.py` `make_results2.py` `readback2.py` | 修订后的验证器 | 各文件头部有元数据注释，写明来源与本次变更 |
| `fixtures/old_counterexamples.py` `meta_tests.py` `compare_runs2.py` | Phase A、M01–M12、两次同源运行的比较 | 新写 |
| `fixtures/final_check2.py` `build_evidence2.py` | 发布前检查与 evidence 生成 | 新写；不参与核验运行 |
| `observations/run.json` | 同源运行的完整记录，含每个案例的 stdout | run_recheck.py 输出 |
| `observations/v00.json` `v01.json` `static.json` `a46.json` | 只读阶段输出 | 同一次运行 |
| `observations/old_counterexamples.json` | Phase A | old_counterexamples.py 输出 |
| `observations/meta_tests.json` | M01–M12 | meta_tests.py 输出 |
| `observations/rerun_vs_batch1.json` | 第二批重跑与第一批的比较 | compare_runs2.py 输出 |
| `observations/after_batch1/`、`after_batch2/`（各 3 个） | 两次 results.yaml 的 object/branch 回读与推送后身份门 | readback2、identity2 输出；after_batch1 原样移入，after_batch2 逐字节复制自临时目录 |

冻结旧验证器的 20 个文件不复制进本目录；每次运行都从 `a1e7c2277705` 逐字节解出，文件名、字节数与 SHA-256 记录在 run.json 的 frozen_manifest 中。

## 7. 重建命令

```bash
# 0. 检出执行分支。身份门要求：在该分支上、recheck-01 以外工作树干净、本地头不落后于远端
git clone https://github.com/08822407d/MyOS2 && cd MyOS2
git fetch origin claude/dazzling-cori-q0dnyt agent/MYOS2-LEAD-002 time master
git checkout claude/dazzling-cori-q0dnyt
export MYOS2_REPO=$PWD PYTHONDONTWRITEBYTECODE=1
cd agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/fixtures
P=$(mktemp -d)   # 新建的空父目录；程序在其下再建全新子目录，不复用、不清理
# 1. Phase A：冻结旧验证器实跑反例
RECHECK_ROOT=$P/phaseA python3 old_counterexamples.py $P/old_counterexamples.json
# 2. 同源 V00–V14
RECHECK_ROOT=$P/run python3 run_recheck.py --out $P/run.json; echo "exit=$?"
# 3. M01–M12（用到第 1、2 步的输出）
RECHECK_ROOT=$P/meta python3 meta_tests.py $P/run.json $P/old_counterexamples.json $P/meta_tests.json
# 4. 生成 results.yaml
mkdir $P/obs && cp $P/run.json $P/old_counterexamples.json $P/meta_tests.json $P/obs/ \
  && cp $P/run/run-*/stages/{v00,v01,static,a46}.json $P/obs/
python3 make_results2.py $P/obs $P/results.yaml
# 5. 与已交观测比较；由已交观测逐字节重建已交 results.yaml
python3 compare_runs2.py efb9846b88ec $P/obs $P/compare.json
python3 make_results2.py ../observations $P/delivered.yaml && cmp $P/delivered.yaml ../results.yaml
# 6. 远端回读（object：按提交对象；branch：分支头须等于目标，或为该文件 blob 相同的后继）
mkdir $P/rb && python3 readback2.py efb9846b88ec \
  agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/results.yaml \
  packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out $P/rb/readback.json
```

参数与约定：

- `run_recheck.py [--out FILE]`：需要环境变量 `RECHECK_ROOT`；`MYOS2_REPO` 默认为 `/home/user/MyOS2`。退出码 0 表示完成（可含核验发现），2 部分完成，3 身份或授权阻断，4 内部错误。
- `readback2.py <提交短号> <路径> [NAME=VALUE] [--mode object|branch] [--branch NAME] --out FILE`：退出码 0 满足条件，1 不满足，2 参数错误。它会在 `--out` 所在目录新建 `rbwork-*` 下载目录。
- `final_check2.py results` 或 `final_check2.py docs <results 提交短号>`：在执行分支上、文件已暂存时运行。
- `build_evidence2.py <recheck-01 目录> <out.md> <第一批短号> <results 短号> [results 检查输出] [docs 检查输出]`。
- 限时：单次编译 60 s；单次夹具运行 5 s，每个案例运行两次；有界序列 100 步；只读阶段 600 s；H00 探针 0.2–5 s；回读每个通道 curl `--max-time 25`，进程上限 30 s；M 项中旧程序子进程 60 s（M12 的旧构建器展开为 120 s）。
- 需要 git、Python 3 + PyYAML、gcc、curl。V12/V13 需要 x86-64 主机。M11 与回读需要能访问 raw.githubusercontent.com 和 api.github.com。

## 8. 回读与交付

- 第一批 `f9ae2bce9a64`：results.yaml 39087 字节。object 与 branch 两种模式、raw 与 api 两个通道都满足条件：curl 退出 0、未超时、HTTP 200、与提交 blob 逐字节相同、packet_id 正确。推送后身份门打开。
- 第二批 `efb9846b88ec`：results.yaml 39525 字节，结果同上。执行分支已两次前进（`a1e7c2277705` → `f9ae2bce9a64` → `efb9846b88ec`），旧原件不变，身份门仍然打开。这是 R04 所要求的“合法续提交、旧原件不变、可重放”的真实实例。
- 第三批：本清单、evidence.md、build_evidence2.py 与 after_batch2 的三份记录。提交前运行 `final_check2.py docs efb9846b88ec`，输出嵌入 evidence.md §9.5。推送后的核对写在 PR 正文中。
- results.yaml 不写回读结论，因为回读发生在它的提交之后；回读记录在 `observations/after_batch*/`。
- PR #17 保持 Draft，不合并；执行分支保留到主线审查完成并明确解除。

## 9. 限制

- 所有执行都是宿主上的普通用户态进程，不是 MyOS2 在 CPU 上运行。V04–V13 的真实调度、IRQ/信号、SMP、真实计时器与上下文切换层都没有执行；V14 没有 ELF 层。
- V03 在守卫之前已有 C 未定义行为（见 §5）。
- CA-06 的语义复核沿用冻结的 v00_semantic_review.yaml，是执行者的阅读判断，未重算。CA-02 的全局可达性不由本检查判定。
- M 项中变造的输入都是 HARNESS_META_TEST，不是 MyOS2 原函数的结果。M07 与 M09 变体 B 使用 H00 替身；M09 变体 B 复用 M12 运行的真实夹具记录。
- M10 的临时仓库以只读 alternates 借用本会话浅克隆仓库的对象库。在完整克隆上可以用更简单的方式构建，行为应相同，但没有在完整克隆上运行过。
- 重建依赖远端分支仍然包含钉住的对象（例如 time 的 `a039d9803ade`）。如果这些分支被改写，需要另行取得这些对象。
- 冻结 core 时期的开发期原始日志不存在，本轮不补造。本轮开发期运行（dev、devmeta 等）只在 evidence.md §8 中叙述，没有入库。
- 执行模型未知，不认证具体后端；没有 CI 运行，也没有人工逐行复核。
